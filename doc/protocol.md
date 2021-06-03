# Communication protocol

All Coolscans use SCSI internally - even the Firewire/USB ones.
The scanners operate using SGC-2 (SCSI Graphics Commands) with some
proprietary extensions in the form of extra opcodes. Those opcodes
can be inferred from the firmware code (SCSI dispatch tables) and/or
SANE `coolscan3.c` driver (which contains some important nuggets of
knowledge, especially about USB communication protocol).

## SCSI scanners

The flavor of SCSI is SCSI-2. The parallel bus is 8-bit and runs at 10MHz,
giving an effective transfer rate of 10MB/s or 80Mbit/s. The bus is
single-ended and operates at TTL voltage levels, making interfacing
from a simple microcontroller quite possible.

See the [Wikipedia article](https://en.wikipedia.org/wiki/Parallel_SCSI)
for a timeline of Parallel SCSI evolution.

# FireWire scanners

The protocol used is [SBP-2](https://en.wikipedia.org/wiki/Serial_Bus_Protocol_2).
This is a direct SCSI transport for Firewire devices, meaning very little
to no extra translation is needed.

# USB scanners

The USB scanners use extra framing commands to communicate SCSI phase and
sense data over USB. They use a simple bulk interface consisting of one
Bulk-Out endpoint and one Bulk-In endpoint. This makes them suitable to be
used with Windows' `usbscan.sys` driver, since the following is enough to
perform all communication with the scanner - no extra syscalls are needed:

```c
CreateFile('\\\\.\\usbscan0', OPEN_EXISTING...);
WriteFile(hScanner, data, len);
ReadFile(hScanner, data, len);
```

Based on reading the SANE source code (`coolscan3.c`) and sniffing
Vuescan's communication, the following protocol has been established:

1. Send the SCSI command block.
2. Query the current SCSI phase by sending `0xD0`.
3. Receive the status byte that indicates the current phase.
4. Perform read/write depending on the phase.
5. Once phase indicates the command has completed, read
   the SENSE data by sending `0x06` and reading 8 bytes.

The intricacies of this protocol are not yet known to me but the above
information is enough for performing basic communication with success.
