#include "coolscan_transport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#define QUERY_PHASE 0xd0
#define PHASE_NONE 0
#define PHASE_STATUS 1
#define PHASE_OUT 2
#define PHASE_IN 3
#define PHASE_BUSY 4

#define SENSE_KEY_NOT_READY 0x02
#define SENSE_ASC_NO_DOCS   0x3a

// Endpoints based on common Nikon Coolscan USB topologies
#define EP_OUT 0x01
#define EP_IN  0x82
#define USB_TIMEOUT_MS 5000

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

static libusb_context *ctx = NULL;

coolscan_connection coolscan_scanner_open(void)
{
    if (libusb_init(&ctx) < 0) {
        fprintf(stderr, "Error: Failed to initialize libusb\n");
        exit(1);
    }

    libusb_device_handle *handle = NULL;
    const struct coolscan_model *matched_model = NULL;

    // Iterate through supported scanner models to find a connected device
    for (const struct coolscan_model *model = coolscan_models; model->codename; model++) {
        handle = libusb_open_device_with_vid_pid(ctx, model->vendor, model->product);
        if (handle) {
            matched_model = model;
            break;
        }
    }

    if (!handle) {
        fprintf(stderr, "Error: No matching Nikon Coolscan scanner found via USB.\n");
        libusb_exit(ctx);
        exit(1);
    }

    printf("Connecting to scanner (Vendor: 0x%04x, Product: 0x%04x)...\n", matched_model->vendor, matched_model->product);

    // Detach kernel driver (like usbfs or sg) if Linux auto-claimed it
    if (libusb_kernel_driver_active(handle, 0) == 1) {
        if (libusb_detach_kernel_driver(handle, 0) < 0) {
            fprintf(stderr, "Warning: Could not detach kernel driver\n");
        }
    }

    if (libusb_claim_interface(handle, 0) < 0) {
        fprintf(stderr, "Error: Cannot claim USB interface 0\n");
        libusb_close(handle);
        libusb_exit(ctx);
        exit(1);
    }

    printf("...connected.\n");
    return (coolscan_connection)handle;
}

const char *coolscan_scanner_get_model(coolscan_connection scanner)
{
    libusb_device_handle *handle = (libusb_device_handle *)scanner;
    struct libusb_device_descriptor desc;
    libusb_device *dev = libusb_get_device(handle);

    if (libusb_get_device_descriptor(dev, &desc) < 0) {
        fprintf(stderr, "Error: cannot read device descriptor\n");
        exit(1);
    }

    int major = (desc.bcdDevice >> 8) & 0xf;
    int minor = (desc.bcdDevice >> 4) & 0xf;
    int patch = (desc.bcdDevice >> 0) & 0xf;

    printf("Checking model: 0x%04x, product: 0x%04x, version: %d.%d%d\n",
           desc.idVendor, desc.idProduct, major, minor, patch);

    for (const struct coolscan_model *model = coolscan_models; model->codename; model++) {
        if (desc.idVendor == model->vendor && desc.idProduct == model->product) {
            printf("...model: %s\n", model->codename);
            return model->codename;
        }
    }

    fprintf(stderr, "Error: unrecognized scanner model.\n");
    exit(1);
}

void coolscan_scanner_close(coolscan_connection scanner)
{
    libusb_device_handle *handle = (libusb_device_handle *)scanner;
    if (handle) {
        libusb_release_interface(handle, 0);
        libusb_close(handle);
    }
    if (ctx) {
        libusb_exit(ctx);
        ctx = NULL;
    }
}

void coolscan_scanner_exchange(coolscan_connection scanner,
                               const struct coolscan_buffer *command,
                               const struct coolscan_buffer *data_out,
                               const struct coolscan_buffer *data_in,
                               const struct coolscan_buffer *sense)
{
    libusb_device_handle *handle = (libusb_device_handle *)scanner;
    int transferred = 0;
    int r;

    coolscan_buffer_dump("cmd", command);

    // 1. Send command packet
    r = libusb_bulk_transfer(handle, EP_OUT, (unsigned char *)command->data, command->length, &transferred, USB_TIMEOUT_MS);
    if (r < 0 || transferred != command->length) {
        fprintf(stderr, "Error: Command WriteFile failed: %s\n", libusb_error_name(r));
        exit(1);
    }

    // 2. Query Phase flag
    uint8_t q_phase = QUERY_PHASE;
    r = libusb_bulk_transfer(handle, EP_OUT, &q_phase, sizeof(q_phase), &transferred, USB_TIMEOUT_MS);
    if (r < 0) {
        fprintf(stderr, "Error: Phase query failed: %s\n", libusb_error_name(r));
        exit(1);
    }

    // 3. Read back execution Phase state
    uint8_t phase;
    r = libusb_bulk_transfer(handle, EP_IN, &phase, sizeof(phase), &transferred, USB_TIMEOUT_MS);
    if (r < 0) {
        fprintf(stderr, "Error: Phase read failed: %s\n", libusb_error_name(r));
        exit(1);
    }

    // 4. Act depending on the phase
    switch (phase) {
        case PHASE_OUT:
            if (data_out == NULL) {
                printf("Error: scanner wants data but we have none\n");
                exit(1);
            }
            r = libusb_bulk_transfer(handle, EP_OUT, (unsigned char *)data_out->data, data_out->length, &transferred, USB_TIMEOUT_MS);
            if (r < 0) { fprintf(stderr, "Error: Data phase out failed: %s\n", libusb_error_name(r)); exit(1); }
            break;

        case PHASE_IN:
            if (data_in == NULL) {
                printf("Error: scanner has data but we want none\n");
                exit(1);
            }
            r = libusb_bulk_transfer(handle, EP_IN, data_in->data, data_in->length, &transferred, USB_TIMEOUT_MS);
            if (r < 0) { fprintf(stderr, "Error: Data phase in failed: %s\n", libusb_error_name(r)); exit(1); }
            break;

        case PHASE_STATUS:
            // Process completed successfully without payload
            break;

        default:
            printf("Error: unexpected phase 0x%02x\n", phase);
            exit(1);
            break;
    }

    // 5. Handle Sense Status Buffer
    bool check_sense = false;
    uint8_t sense_buffer[8];
    struct coolscan_buffer local_sense = { sizeof(sense_buffer), sense_buffer };
    if (sense == NULL) {
        sense = &local_sense;
        check_sense = true;
    }

    r = libusb_bulk_transfer(handle, EP_IN, sense->data, sense->length, &transferred, USB_TIMEOUT_MS);
    if (r < 0) {
        fprintf(stderr, "Error: Sense buffer read failed: %s\n", libusb_error_name(r));
        exit(1);
    }

    // Windows hack port: clear pseudo errors when scanner returns NOT_READY/NO_DOCS during updates
    uint8_t sense_key = sense->data[1] & 0x0f;
    uint8_t sense_asc = sense->data[2];
    if (sense_key == SENSE_KEY_NOT_READY && sense_asc == SENSE_ASC_NO_DOCS) {
        memset(sense->data, 0x00, sense->length);
    }

    uint8_t expected[8] = {0};
    if (memcmp(sense->data, expected, sense->length) != 0) {
        coolscan_buffer_dump("sense", sense);
        if (check_sense) {
            printf("Error: sense_buffer data indicates failure\n");
            exit(1);
        }
    }
}
