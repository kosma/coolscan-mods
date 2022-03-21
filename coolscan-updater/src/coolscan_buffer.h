#ifndef COOLSCAN_UPDATER_COOLSCAN_BUFFER_H
#define COOLSCAN_UPDATER_COOLSCAN_BUFFER_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Simplified buffer structure.
 */
struct coolscan_buffer
{
    size_t length;
    uint8_t *data;
};

void coolscan_buffer_dump(const char *label, const struct coolscan_buffer *buffer);

#endif // COOLSCAN_UPDATER_COOLSCAN_BUFFER_H
