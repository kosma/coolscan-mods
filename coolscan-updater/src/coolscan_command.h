#ifndef COOLSCAN_UPDATER_COOLSCAN_COMMAND_H
#define COOLSCAN_UPDATER_COOLSCAN_COMMAND_H

#include "coolscan_buffer.h"
#include "coolscan_transport.h"

/**
 * Standard SCSI command to test scanner status.
 */
bool coolscan_command_test_unit_ready(coolscan_connection scanner);

/**
 * Standard SCSI command to ask the scanner about itself.
 */
const char *coolscan_command_inquiry(coolscan_connection scanner);

/**
 * Standard SCSI command to ask the scanner how it feels. ???
 */
void coolscan_command_self_test(coolscan_connection scanner);

/**
 * Standard SCSI command to upload new firmware.
 */
void coolscan_command_write_firmware_chunk(coolscan_connection scanner, struct coolscan_buffer *buffer);

/**
 * Unknown/proprietary command needed to update the firmware.
 */
void coolscan_command_firmware_update_set(coolscan_connection scanner);

/**
 * Unknown/proprietary command needed to update the firmware.
 */
void coolscan_command_firmware_update_execute(coolscan_connection scanner);

/**
 * Standard SCSI command to read diagnostic data.
 * NOTE: This can be dangerous, depending on the exact
 */
void coolscan_command_read_buffer(coolscan_connection scanner, uint8_t mode, uint8_t buffer_id, uint32_t address, struct coolscan_buffer *buffer);

#endif // COOLSCAN_UPDATER_COOLSCAN_COMMAND_H
