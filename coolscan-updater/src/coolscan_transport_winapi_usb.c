#include "coolscan_transport.h"

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <usbscan.h>

// defines below... thanks to https://gitlab.com/sane-project/backends/-/blob/master/backend/coolscan3.c

#define QUERY_PHASE 0xd0
#define PHASE_NONE 0
#define PHASE_STATUS 1
#define PHASE_OUT 2
#define PHASE_IN 3
#define PHASE_BUSY 4

#define SENSE_KEY_NOT_READY 0x02
#define SENSE_ASC_NO_DOCS   0x3a


struct coolscan_model {
    const char *codename;
    uint16_t vendor;
    uint16_t product;
};

static const struct coolscan_model coolscan_models[] = {
    { "PT17035", 0x04b0, 0x4000 }, // LS-40
    { "DF17811", 0x04b0, 0x4001 }, // LS-50
    { "DF17810", 0x04b0, 0x4002 }, // LS-5000
    { NULL }
};


/**
 * Helper: Windows equivalent of strerror(errno). Gee, I love WinAPI.
 */
static const char *format_last_error()
{
    // https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror

    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return NULL;

    char *messageBuffer = NULL;
    DWORD size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

    char *message = malloc(size+1);
    memcpy(message, messageBuffer, size);
    message[size] = '\0';

    LocalFree(messageBuffer);

    return message;
}

coolscan_connection coolscan_scanner_open(void)
{
    const char *scanner_path = "\\\\.\\usbscan0";

    printf("Connecting to scanner %s...\n", scanner_path);
    HANDLE scanner = CreateFile(scanner_path,
                               GENERIC_READ|GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                0,
                                NULL);

    if (scanner == INVALID_HANDLE_VALUE) {
        printf("Error: cannot open %s: %s\n", scanner_path, format_last_error());
        exit(1);
    }
    printf("...connected.\n");

    return (coolscan_connection) scanner;
}

const char *coolscan_scanner_get_model(coolscan_connection scanner)
{
    DEVICE_DESCRIPTOR deviceDescriptor;
    bool result = DeviceIoControl((HANDLE) scanner,
                                  IOCTL_GET_DEVICE_DESCRIPTOR,
                                  &deviceDescriptor, sizeof(deviceDescriptor),
                                  &deviceDescriptor, sizeof(deviceDescriptor),
                                  NULL, NULL);
    if (result == false) {
        printf("Error: cannot read device descriptor: %s\n", format_last_error());
        exit(1);
    }

    int major = (deviceDescriptor.usBcdDevice >> 8) & 0xf;
    int minor = (deviceDescriptor.usBcdDevice >> 4) & 0xf;
    int patch = (deviceDescriptor.usBcdDevice >> 0) & 0xf;

    printf("Checking model: 0x%04x, product: 0x%04x, version: %d.%d%d\n",
           deviceDescriptor.usVendorId,
           deviceDescriptor.usProductId,
           major, minor, patch);

    for (const struct coolscan_model *model = coolscan_models; model->codename; model++) {
        if (deviceDescriptor.usVendorId == model->vendor &&
            deviceDescriptor.usProductId == model->product) {
            printf("...model: %s\n", model->codename);
            return model->codename;
        }
    }

    printf("Error: unrecognized scanner model.\n");
    exit(1);
}

void coolscan_scanner_close(coolscan_connection scanner)
{
    CloseHandle((HANDLE) scanner);
}

static void query_phase(coolscan_connection scanner)
{
}

void coolscan_scanner_exchange(coolscan_connection scanner,
                               const struct coolscan_buffer *command,
                               const struct coolscan_buffer *data_out,
                               const struct coolscan_buffer *data_in,
                               const struct coolscan_buffer *sense)
{
    coolscan_buffer_dump("cmd", command);

    // send cmd
    WriteFile((HANDLE) scanner, command->data, command->length, NULL, NULL);

    // query phase
    uint8_t query_phase = QUERY_PHASE;
    WriteFile((HANDLE) scanner, &query_phase, sizeof(query_phase), NULL, NULL);

    // read phase
    uint8_t phase;
    ReadFile((HANDLE) scanner, &phase, sizeof(phase), NULL, NULL);

    // act depending on the phase
    switch (phase) {
        case PHASE_OUT:
            if (data_out == NULL) {
                printf("Error: scanner wants data but we have none\n");
                exit(1);
            }
            WriteFile((HANDLE) scanner, data_out->data, data_out->length, NULL, NULL);
            break;

        case PHASE_IN:
            if (data_in == NULL) {
                printf("Error: scanner has data but we want none\n");
                exit(1);
            }
            ReadFile((HANDLE) scanner, data_in->data, data_in->length, NULL, NULL);
            break;

        case PHASE_STATUS:
            // all good
            break;

        default:
            printf("Error: unexpected phase 0x%02x\n", phase);
            exit(1);
            break;
    }

    // if caller doesn't want to read sense data, allocate our buffer and check the result
    bool check_sense = false;
    uint8_t sense_buffer[8];
    struct coolscan_buffer local_sense = { sizeof(sense_buffer), sense_buffer };
    if (sense == NULL) {
        sense = &local_sense;
        check_sense = true;
    }

    ReadFile((HANDLE) scanner, sense->data, sense->length, NULL, NULL);

    // hack: the scanner can return a non-zero SENSE even when it's perfectly
    // fit for receiving a firmware update - if that's the case, fake the
    // "correct" all-zero response so the caller doesn't have to check for it.
    uint8_t sense_key = sense->data[1] & 0x0f;
    uint8_t sense_asc = sense->data[2];
    if (sense_key == SENSE_KEY_NOT_READY && sense_asc == SENSE_ASC_NO_DOCS) {
        memset(sense->data, 0x00, sense->length);
    }

    uint8_t expected[8] = {0};
    if (memcmp(sense->data, expected, sense->length) != 0) {
        coolscan_buffer_dump("sense", sense);
        // check sense if caller doesn't want to
        if (check_sense) {
            printf("Error: sense_buffer data indicates failure\n");
            exit(1);
        }
    }
}
