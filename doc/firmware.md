# Firmware overview

## Flash sector map

| Start   | Size    | Content                                    |
|---------|---------|--------------------------------------------|
|       0 |  0x4000 | Vector Table + Main/Recovery startup logic |
|  0x4000 |  0x2000 | Firmware flashing status flags             |
|  0x6000 |  0x2000 | Some kind of settings page?                |
|  0x8000 |  0x8000 | Some kind of settings page?                |
| 0x10000 | 0x10000 | Recovery                                   |
| 0x20000 | 0x10000 | Main firmware                              |
| 0x30000 | 0x10000 | Main firmware                              |
| 0x40000 | 0x10000 | Main firmware                              |
| 0x50000 | 0x10000 | Main firmware                              |
| 0x60000 | 0x10000 | Logs?                                      |
| 0x70000 | 0x10000 | Logs?                                      |

To the best of my knowledge, the following applies:

- The first sector never gets altered - consider it a 1st stage bootloader.
- The second sector (0x4000) is used a a flag/marker for the bootloader
  to select either the Main or Recovery firmware.
- The sectors at 0x6000 and 0x8000 can be read/written and are used for
  various purposes I haven't determined yet.
- Sector 0x10000 contains a bit of software I've dubbed "recovery".
  It contains a ton of library functions used for communication, as well
  as a dumbed-down minimal SCSI command handling stack that can pretty
  much only reply to INQUIRY and receive WRITE BUFFER requests to write
  new firmware. The firmware updates are handled by Recovery.
- Area 0x20000 through 0x60000 is where the "main" firmware lives.
- Whenever firmware update is requested, after a simple verification
  (the model string inside the firmware update file has to match) the
  Main firmware deinitializes all peripherals, sets a special flag, and
  jumps to Recovery - which proceeds to directly handle the SCSI command
  received by the Main firmware. As a firmware engineer I find this
  mechanism both abhorrent and amusing.

## Firmware mechanics

The firmware makes *ample* use of data-driven development. This makes
disassembly a bit annoying as you have to manually create relevant structures
in IDA and then declare array-of-struct in order to have offsets resolve
properly. I have spent a good deal of time scratching my head before
discovering things like Main/Recovery function pointers, or SCSI dispatch
tables.
