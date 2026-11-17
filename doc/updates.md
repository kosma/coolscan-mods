# Firmware updates

## Firmware update format

For our purposes, I distinguish three firmware file formats:

1. Full FLASH dumps. Those should NOT be considered "firmware updates"
   since the dump also contains things like calibration data (reference
   white balance values) and logs (yes, the scanner keeps a log of all
   operations). Those dumps, however, are best for disaster recovery
   purposes - they let you pretty much undo anything weird you might have
   done to the firmware.

2. The `FIRMDATA` resources, as included in Nikon updaters. They use
   a length header and a custom variant of LZSS (dictionary initialized
   with 0 instead of spaces). They can be uncompressed with `pylzss`:

   ```python
   # pip install pylzss
   uncompressed_size, = struct.unpack("<I", firmdata[:4])
   uncompressed_data = lzss.decompress(firmdata[4:], 0)
   ```

   This format is useless in most cases, except if we ever wanted to
   repackage the original Nikon flashers with our custom firmware.

4. The binary files used with Nikon service software - just the binary data,
   starting at 0x10000. This is the format that the scanner expects,
   and the format we use for our custom flashers and patching tools.
   Use `scripts/create_fwupdate_file.py` to convert a full flash dump
   into this format.

A good clue that your firmware update file is correct is a scanner model
string (e.g. `DF17811`) located at offset 0x11.

## Firmware update procedure

NOTE: This section is pure speculation. I will edit this as I gain more
information.

1. Send `WRITE BUFFER` command with `MODE=5`, `BUFFER_ID=7`.
   The offset and payload should be... I'm not sure what, that's TBD.
2. The scanner verifies the validity of update (correct model string),
   deinitializes all peripherals and jumps to Recovery.
3. Recovery recognizes this and jumps straight to handling that SCSI
   command again.
4. The markers at `0x4000` are set to indicate firmware is not valid.
5. The firmware update file is received in chunks and flashed starting
   at offset `0x20000`.
6. The markers at `0x4000` are set to indicate firmware is valid
   but Recovery is not valid.
7. When the main firmware starts, it realizes that Recovery is missing
   and writes a copy of Recovery from the end of the Main firmware into
   sector `0x10000`.
8. The markers at `0x4000` are set to indicate recovery is valid.

## Firmware update protocol

Firmware updates are handled through SCSI `WRITE BUFFER` command. The sequence
performed by the original Nikon updater is:

1. Issue `IDENTIFY` to confirm eligible scanner model.
2. Issue `RESERVE UNIT`.
3. Issue `SEND DIAGNOSTICS.`
4. Issue `WRITE BUFFER` with `MODE=5`, send 4096 bytes of `FIRMDATA`.
5. Continue 4) until you're out of data.
6. Issue `SET PARAMETERS` (0xE0).
7. Issue `EXECUTE`. Wait for it to complete.
8. Issue `RELEASE UNIT`.

The exact flow of an LS-40 update can be found [here](update_ls_40.md).

## Reinstalling the same version

In order to be able to reinstall the same firmware version as one that's
already in the scanner FLASH, open the updater in ResEdit/ResHacker and
change the VERSIONVALUE resource to e.g. `00 01 09 09`.
