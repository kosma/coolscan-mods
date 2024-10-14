# The Mod

So what does the firmware modification actually do?

It patches out two `if()` checks that prevent SA-30 and SF-210 from working.

That's it. There's no magic, no extra code, no extensive work.

The LS-50 firmware is in reality just a trimmed-down version of LS-5000 firmware.
The work done is so sloppy that there is still a string *LS-5000* present in the
LS-50 firmware.

Remove the checks - and suddenly the scanner accepts the SA-30 happily. It knows
what it is, it knows how it works - it even knows its name if you delve into the
strings section of the firmware.

For posterity, the relevant checks to patch out are:

LS-50 1.02:

* `0x2A213` - replace 0x03 with 0x42 to restore support for SF-210
* `0x2A219` - replace 0x05 with 0x42 to restore support for SA-30
* `0x2A21F` - replace 0x06 with 0x42 to restore support for ????
* `0x2EFA2` - replace 0xFFE6 (register for AN7 result) with 0xFFE0 (Register for AN4 result)
* `0x31C44` - replace 0xFFE6 (register for AN7 result) with 0xFFE0 (Register for AN4 result)
* `0x33080` - replace 0xFFE6 (register for AN7 result) with 0xFFE0 (Register for AN4 result)

LS-40 1.20:

* `0x29EB3` - replace 0x03 with 0x42 to restore support for SF-210
* `0x29EB9` - replace 0x05 with 0x42 to restore support for SA-30
* `0x29EBF` - replace 0x06 with 0x42 to restore support for ????

Offsets are from the start of the FLASH, not from the start of the update file -
for that, subtract 0x10000.
