# coolscan-updater

This is a minimal updater program for use with USB-based Coolscans.
It is based on a LS-40 USB traffic dump kindly provided by Gleb Shtengel.
Not all traffic is replicated perfectly - just enough to install the update.
SCSI (native or over Firewire) is not supported but shouldn't be too hard to add.

To build, use CLion - or CMake with a MinGW toolchain if you're an expert.

There are no user-friendly files here - instead, look in the Releases section
of this repository for a user-friendly package.

## Building 32-bit binaries

To build 32-bit binaries for use with older systems, install MSYS2 and run
the following under MINGW32 shell:

```bash
pacman -S mingw-w64-i686-gcc mingw-w64-i686-ninja
```

Afterwards, select the MINGW32 toolchain in CLion settings (Toolchain and CMake
sections).
