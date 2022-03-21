#include "coolscan_updatefile.h"

#include <stdio.h>
#include <stdlib.h>


struct coolscan_buffer *coolscan_read_firmware_file(const char *model)
{
    char filename[PATH_MAX];
    snprintf(filename, sizeof(filename), "%s.bin", model);

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Error: cannot load firmware from %s\n", filename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *data = malloc(file_size);
    fread(data, 1, file_size, f);

    struct coolscan_buffer *buffer = malloc(sizeof(*buffer));
    buffer->data = data;
    buffer->length = file_size;
    return buffer;
}
