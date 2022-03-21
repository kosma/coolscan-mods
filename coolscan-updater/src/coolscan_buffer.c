#include "coolscan_buffer.h"

#include <stdio.h>


void coolscan_buffer_dump(const char *label, const struct coolscan_buffer *buffer) {
    printf("%s:", label);

    for (int i=0; i<buffer->length; i++) {
        printf(" %02x", buffer->data[i]);
    }

    printf("\n");
}