# Nikon Coolscan modification project

## Adapter compatibility chart

|           | **SA-30**  | **SF-200**  | **SF-210**  |
|-----------|------------|-------------|-------------|
| **LS-40** |     ❌\*   |      ✅     |      ✅     |
| **LS-50** |     ✅\*\* |      ✅     |      ✅     |

\* Not currently working, requires investigation.

\*\* Requires cutting a hole in the back - see [attached stencil](hole/).

## Background

This project aims to extend old and unsupported Nikon Coolscan scanners.
Currently it allows owners of those scanners to unlock the ability to scan
full film rolls (using SA-30) and batch scan slides (using SF-210) on
Nikon Coolscan LS-40 and LS-50 (known as Coolscan IV and Coolscan V in
some markets).

Why? Because Nikon artificially crippled the LS-40 and LS-50 by removing
the ability to batch-scan, for no reason other than corporate greed and
market segmentation. All the logic needed to support SA-30 and SF-210 on
those scanners is already in place - it is just artificially disabled to
placate the finance and marketing.

Now, twenty years later, we can finally lift those artificial limitations
and enjoy the full power of these scanners.

## Requirements

* At the moment, this firmware is only available for LS-40 and LS-50.
* You need a 64-bit system to run the firmware updater.
* You need Vuescan drivers for the updater to work. I have not tested them
  on any other drivers. It *might* work with Nikon drivers.
* You must not have any other scanners connected during the update process.
* You must not have any scanning software running during the update process.
* For scanning entire rolls of film you need a hole cut in the back of
  the scanner. Trying to load an entire roll without the hole cut will
  cause physical damage to the film and/or scanner. The hole is not needed
  for bulk slide scanning with SF-200/SF-210.

## Instructions

1. Download the latest firmware package from the [Releases](https://github.com/kosma/coolscan-mods/releases) section and unpack it.
2. Connect your LS-40 or LS-50 over USB. Turn on the scanner.
3. Double-click `coolscan-updater.exe` and wait for the process to complete.
   As with all firmware updates, do NOT interrupt it until it says it's done.
   You might get a SmartScreen warning - it's a false positive. Check the file
   on VirusTotal if you're suspicious.
4. Turn the scanner off and on again.
5. Congratulations! Your scanner can now accept SA-30 and SF-210.

There is a bit more of mechanical work involved if you want to scan long
strips of film (longer than six frames, and up to 40 frames):

1. Cut a hole in the back of the scanner. This requires extensive disassembly
   of the scanner, as all electronic and mechanical components inside have to
   be removed before you can safelly cut/drill an opening.
   Do **NOT** attempt to cut the hole with the components still inside - getting
   *any* metal shavings inside the scanner pretty much dooms it to fail sooner
   than later.

   For cutting the opening, use [these guides](hole/) - make sure to print
   at 100% scale! The shape of the opening accommodates a standard
   LS-4000/LS-5000 back door and a spool holder; if you don't want to install
   them (you really don't need it), you can make the shape of the opening much
   simpler. On my scanner, I used a drill and a hacksaw - the results are
   not pretty, but it works like a charm.
   
   See [Gleb's website](http://www.shtengel.com/gleb/nikon_coolscan_4000_disassembly_and_scanhead_gear_repair.htm)
   for important disassembly notes.

2. Modify your SA-21 to be an SA-30 - the conversion process is described in
   many places on the internet, e.g. [Gleb's website](http://www.shtengel.com/gleb/converting_nikon_sa21_to_sa30.htm).
   It involves nothing more than installing/soldering one jumper.

**Warning:** At the moment the SA-30 support on LS-40 is somewhat broken -
the adapter is correctly identified but you can't scan more than six frames.

In the future I will also offer 3D-printable designs for the film door and
front and rear takeup spools - but those projects are still in development.
In the meantime, consider the absolutely stunning [full roll conversion kit](https://www.etsy.com/pl/listing/949217792/nikon-4000ed-and-5000ed-full-roll)
that should also fit your modded LS-40 or LS-50.

## Risks

There are a few risks involved with installing and using modified firmware:

1. If the firmware update process goes wrong, your scanner might be left in
   a non-operational (bricked) state. In most cases you can recover from this
   by simply restarting the update procedure - but in rare cases it might be
   required to desolder the FLASH chip from the mainboard and reprogram it in
   an external programmer (e.g. TL866II). Such services are commonly offered
   by respectable electronics repair shops for a modest fee. Full firmware
   dumps are provided for this purpose in the [dumps](dumps/) directory.

2. Using the unsupported LS-30 and SF-210 adapters might cause some components
   to wear out faster. I have not identified any potential components that are
   not fit to handle a bulk adapter, but again - I might have missed something.

3. Running SA-30 with long strips of film without cutting a hole in the back
   of the scanner will cause physical damage... I don't think I have to explain
   why. As a precaution, do *not* perform the SA-21 to SA-30 conversion until
   you have the hole cut.

## Technical documentation

This information is only relevant for people working on scanners - be it modders
or repairmen. It is of not much use to general public.

* [Technical information about the mod](doc/mod.md)
* [Hardware overview](doc/hardware.md)
* [Firmware overview](doc/firmware.md)
* [Communication protocol](doc/protocol.md)
* [Firmware updates](doc/updates.md)
* [Service Manuals](manuals/)
* [Datasheets](datasheets/)
* [Firmware dumps](dumps/)
* [Firmware reflashers](reflashers/)

## Acknowledgements

This project has been made possible thanks to some great people:

* [Gleb Shtengel](http://www.shtengel.com/gleb/index.htm) - provided knowledge, firmware dumps, USB dumps, and parts
* Victor Gerasimov - provided knowledge, ideas, and firmware dumps
* Paulo Meireles - provided inspiration and smiles
* [Nikon Coolscan Users](https://www.facebook.com/groups/1514948298527146) - provided a fantastic captive audience for my shenanigans
* [SANE](https://gitlab.com/sane-project/backends/-/blob/master/backend/coolscan3.c) - provided bad but working open driver implementation
* [Служба НТИ](https://www.snti.ru/) - provided cheap ISO/IEC standards
* Frank Phillips - said he's skeptical, giving me the best motivation possible: spite
