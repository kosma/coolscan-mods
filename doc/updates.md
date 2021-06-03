# Firmware updates

## Firmware update format

For our purposes, I distinguish three firmware file formats:

1. Full FLASH dumps. Those should NOT be considered "firmware updates"
   since the dump also contains things like calibration data (reference
   white balance values) and logs (yes, the scanner keeps a log of all
   operations). Those dumps, however, are best for disaster recovery
   purposes - they let you pretty much undo anything weird you might have
   done to the firmware.

2. The `FIRMDATA` files, as included in Nikon updaters. They use an unknown
   compression/packing format that is an enormous pain in the ass to reverse
   engineer.

3. The binary files used with Nikon service software. Don't ask how I know
   of their existence. They seem to have a simpler format - just the binary
   data, starting at 0x10000 or possibly 0x20000. There's a good chance that
   this is the format that the scanner expects. We need to run an LS-40 USB
   traffic dump in order to confirm this theory.

A good clue that your firmware update file is correct is a scanner model
string (e.g. `DF17811`) located at offset 0x11.

## Firmware update procedure

NOTE: This section is pure speculation. I will edit this as I gain more
information.

## Firmware update protocol

Firmware updates are handled through SCSI `WRITE BUFFER` command. Exact values
for the `MODE`/`BUFFER_ID` are to be determined based on disassembly and/or
LS-40 USB traffic dumps. Theoretically I can determine the IDs and protocol
experimentally but it's a potentially huge pain in case something goes wrong
and I have to desolder and reprogram the FLASH chip to recover from it.

## Reinstalling the same version

In order to be able to reinstall the same firmware version as one that's
already in the scanner FLASH, open the updater in ResEdit/ResHacker and
change the VERSIONVALUE resource to e.g. `00 01 09 09`.
