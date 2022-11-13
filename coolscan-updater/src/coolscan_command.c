#include <string.h>
#include "coolscan_command.h"


bool coolscan_command_test_unit_ready(coolscan_connection scanner)
{
    struct coolscan_buffer cmd = {
            6, (uint8_t []){
                    0x00, // TEST UNIT READY
                    0x00, 0x00, 0x00, 0x00, 0x00
            },
    };
    uint8_t sense_buf[8];
    struct coolscan_buffer sense = { sizeof(sense_buf), sense_buf };
    coolscan_scanner_exchange(scanner, &cmd, NULL, NULL, &sense);

    uint8_t expected[8] = { 0 };
    return (memcmp(sense_buf, expected, sizeof(sense_buf)) == 0);
}

const char *coolscan_command_inquiry(coolscan_connection scanner)
{
    size_t response_length = 36;

    struct coolscan_buffer cmd = {
            6, (uint8_t []){
                    0x12, // INQUIRY
                    0x00,
                    0x00,
                    0x00,
                    response_length,
                    0x00,
            },
    };
    uint8_t data_in_buf[response_length];
    struct coolscan_buffer data_in = { response_length, data_in_buf };

    coolscan_scanner_exchange(scanner, &cmd, NULL, &data_in, NULL);

    // extract scanner name from inquiry data
    size_t name_length = 28;
    char *result = malloc(name_length+1);
    memcpy(result, data_in_buf+8, name_length);
    result[name_length] = '\0';

    return result;
}

void coolscan_command_self_test(coolscan_connection scanner)
{
    struct coolscan_buffer cmd = {
            6, (uint8_t []){
                0x1d, // SEND DIAGNOSTIC
                0x04, // SelfTest
                0x00, 0x00, 0x00, 0x00
            },
    };
    coolscan_scanner_exchange(scanner, &cmd, NULL, NULL, NULL);
}

void coolscan_command_write_firmware_chunk(coolscan_connection scanner, struct coolscan_buffer *buffer)
{
    struct coolscan_buffer cmd = {
            10, (uint8_t []){
                0x3B, // WRITE BUFFER
                0x05, // Download microcode and save
                0x00, // Buffer ID = 0 (default)
                0x00, // Buffer offset (ignored)
                0x00, // Buffer offset (ignored)
                0x00, // Buffer offset (ignored)
                (buffer->length >> 16) & 0xFF,
                (buffer->length >>  8) & 0xFF,
                (buffer->length >>  0) & 0xFF,
                0x00, // Control
            },
    };
    coolscan_scanner_exchange(scanner, &cmd, buffer, NULL, NULL);
}

void coolscan_command_firmware_update_set(coolscan_connection scanner)
{
    struct coolscan_buffer cmd = {
            10, (uint8_t []) {
                0xE0, // Proprietary SET command
                0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00,
            },
    };
    struct coolscan_buffer data_out = {
            9, (uint8_t []) {
                // Unknown payload - copy as-is from original updater
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            },
    };
    coolscan_scanner_exchange(scanner, &cmd, &data_out, NULL, NULL);
}

void coolscan_command_firmware_update_execute(coolscan_connection scanner)
{
    struct coolscan_buffer cmd = {
            6, (uint8_t []) {
                    0xC1, // Proprietary EXECUTE command
                    0x00, 0x00, 0x00, 0x00, 0x00,
            },
    };
    coolscan_scanner_exchange(scanner, &cmd, NULL, NULL, NULL);
}

void coolscan_command_read_buffer(coolscan_connection scanner, uint8_t mode, uint8_t buffer_id, uint32_t address, struct coolscan_buffer *buffer)
{
    struct coolscan_buffer cmd = {
            10, (uint8_t []){
                    0x3C, // READ BUFFER
                    mode,
                    buffer_id,
                    (address >> 16) & 0xFF,
                    (address >>  8) & 0xFF,
                    (address >>  0) & 0xFF,
                    (buffer->length >> 16) & 0xFF,
                    (buffer->length >>  8) & 0xFF,
                    (buffer->length >>  0) & 0xFF,
                    0x00, // Control
            },
    };
    coolscan_scanner_exchange(scanner, &cmd, NULL, buffer, NULL);
}