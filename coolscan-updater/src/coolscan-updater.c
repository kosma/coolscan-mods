#include <stdio.h>
#include <unistd.h>
#include <conio.h>

#include "coolscan_command.h"
#include "coolscan_transport.h"
#include "coolscan_updatefile.h"

void press_any_key_to_continue(void)
{
    printf("Press any key to continue...\n");
    _getch();
}

int main()
{
    atexit(press_any_key_to_continue);

    printf("******************************************\n");
    printf("* Nikon Coolscan Updater by Kosma Moczek *\n");
    printf("******************************************\n");
    printf("\n");

    // connect to scanner
    coolscan_connection scanner = coolscan_scanner_open();
    const char *model = coolscan_scanner_get_model(scanner);

    // load firmware from file
    printf("Loading firmware...\n");
    struct coolscan_buffer *firmware = coolscan_read_firmware_file(model);
    printf("...firmware %s loaded successfully, size: %d\n", model, firmware->length);

    // ask the scanner how it feels :3
    while (true) {
        printf("Waiting for scanner to be ready...\n");
        if (coolscan_command_test_unit_ready(scanner))
            break;
        sleep(1);
    }
    printf("Checking scanner...\n");
    printf("...scanner says it's a %s\n", coolscan_command_inquiry(scanner));
    coolscan_command_self_test(scanner);

    // upload firmware chunks
    size_t chunk_size = 4096;
    uint8_t *data = firmware->data;
    size_t left = firmware->length;
    size_t done = 0;
    while (left > 0) {
        size_t curr = (left < chunk_size) ? left : chunk_size;

        struct coolscan_buffer chunk = { curr, data };
        coolscan_command_write_firmware_chunk(scanner, &chunk);
        printf("...written %d/%d bytes\n", done, firmware->length);

        data += curr;
        left -= curr;
        done += curr;
    }

    // execute firmware update
    printf("Installing firmware...\n");
    coolscan_command_firmware_update_set(scanner);
    coolscan_command_firmware_update_execute(scanner);

    // wait for scanner to come back online
    int time = 0;
    while (true) {
        printf("Waiting for the scanner to boot (%ds)...\n", ++time);
        if (coolscan_command_test_unit_ready(scanner))
            break;
        else
            sleep(1);
    }

    printf("Asking the scanner about its new firmware...\n");
    printf("...scanner says it's a %s\n", coolscan_command_inquiry(scanner));

    printf("Firmware update done.\n");
    coolscan_scanner_close(scanner);

    return 0;
}
