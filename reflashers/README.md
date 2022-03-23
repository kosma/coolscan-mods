# Firmware Reflashers

These are original Nikon firmware updater programs modified to allow reflashing -
that is, installing the same version of firmware that is already present. This can
help recover from corrupted firmware - a failure mode found in many scanners out
in the wild. Symptoms of this failure are:

* The scanner turns on and the LED lights up green immediately.
* The scanner is visible in the operating system.
* The scanner, nevertheless, does not *scan*.

The idea for those reflashers came from Gleb Shtengel, and he confirmed their
utility in fixing at least *some* of the faulty boards.
