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

int main() {
    atexit(press_any_key_to_continue);

    printf("***************************************\n");
    printf("* Nikon Memory Dumper by Kosma Moczek *\n");
    printf("***************************************\n");

    coolscan_connection scanner = coolscan_scanner_open();
    const char *model = coolscan_scanner_get_model(scanner);

    printf("Checking scanner...\n");
    printf("...scanner says it's a %s\n", coolscan_command_inquiry(scanner));

    const size_t memory_dump_addr = 0xFFFFC0;
    const size_t memory_dump_last = 0xFFFFFF;
    const size_t memory_dump_size = memory_dump_last - memory_dump_addr + 1;
    struct coolscan_buffer memory_dump_buffer = {
        .length = memory_dump_size,
        .data = malloc(memory_dump_size),
    };
    printf("Reading memory at 0x%06x to 0x%06x...\n", memory_dump_addr, memory_dump_last);
    coolscan_command_read_buffer(scanner, 2, 0, memory_dump_addr, &memory_dump_buffer);
    coolscan_buffer_dump_long("I/O Registers", &memory_dump_buffer);

    coolscan_scanner_close(scanner);
    printf("Finished.\n");

    return 0;
}