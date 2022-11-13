#include "coolscan_buffer.h"

#include <stdio.h>


#define COLUMN_SIZE 8


void coolscan_buffer_dump(const char *label, const struct coolscan_buffer *buffer) {
    printf("%s:", label);

    for (int i=0; i<buffer->length; i++) {
        printf(" %02x", buffer->data[i]);
    }

    printf("\n");
}

void coolscan_buffer_dump_long(const char *label, const struct coolscan_buffer *buffer)
{
    printf("%s:\n", label);

    for (int i=0; i<buffer->length; i++) {
        if ((i % COLUMN_SIZE) == 0) {
            printf("%04x:", i);
        }
        printf(" %02x", buffer->data[i]);
        if (((i+1) % COLUMN_SIZE) == 0) {
            printf("\n");
        }
    }

    printf("\n");
}