#ifndef COOLSCAN_TRANSPORT_H
#define COOLSCAN_TRANSPORT_H

#include <stdbool.h>
#include <stdint.h>

#include "coolscan_buffer.h"

/**
 * Opaque type used to represent a transport instance.
 */
typedef uintptr_t coolscan_connection;

/**
 * Create a connection to the scanner.
 */
coolscan_connection coolscan_scanner_open(void);

/**
 * Close the connection to the scanner.
 */
void coolscan_scanner_close(coolscan_connection instance);

/**
 * Read scanner model. Returns a string matching the firmware header model codename.
 */
const char *coolscan_scanner_get_model(coolscan_connection scanner);

/**
 * Perform a SCSI exchange with the scanner.
 */
void coolscan_scanner_exchange(coolscan_connection scanner,
                               const struct coolscan_buffer *command,
                               const struct coolscan_buffer *data_out,
                               const struct coolscan_buffer *data_in,
                               const struct coolscan_buffer *sense);

#endif // COOLSCAN_TRANSPORT_H
