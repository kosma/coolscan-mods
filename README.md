# Nikon Coolscan modification project

Nikon artificially crippled the LS-40 and LS-50 by removing the ability
to batch-scan or multisample, for no reason other than corporate greed and
market segmentation. All the logic needed to support SA-30 and SF-210 on
those scanners is already in place - it is just artificially disabled.
This project restores them.

## Feature chart

| **Feature**            | **LS-40**          | **LS-50**          | **LS-4000** | **LS-5000** |
|------------------------|--------------------|--------------------|-------------|-------------|
| **SA-21**              | ✅ works as SA-30* | ✅ works as SA-30* | ✔ works**   | ✔ works**   |
| **SA-30**              | ✅ works           | ✅ works           | ✔ works     | ✔ works     |
| **SF-200, SF-210**     | ✅ works           | ✅ works           | ✔ works     | ✔ works     |
| **Multisampling**      | ✅ works***        | ✅ works***        | ✔ works     | ✔ works     |
| **Full roll scanning** | 🪚 cut a hole      | 🪚 cut a hole      | ✔ works     | ✔ works     |

✔ - Feature supported by Nikon. \
✅ - Feature added by this firmware. \
🪚 - You have to cut a hole in the back of the scanner so the film can pass through. We have stencils.

\* Your SA-21 magically becomes an SA-30, no need to solder anything. \
\** Firmware-based SA-30 upgrade coming soon. \
\*** Not all software has been confirmed compatible, please report your success or failure.

## Instructions

1. Download the latest firmware package from the [Releases](https://github.com/kosma/coolscan-mods/releases) section and unpack it.
2. Disconnect all USB scanners. Connect your LS-40 or LS-50 over USB. Turn on the scanner.
3. Double-click `coolscan-updater.exe` and wait for the process to complete.
   As with all firmware updates, do NOT interrupt it until it says it's done.
   You might get a SmartScreen warning - it's a false positive. Check the file
   on VirusTotal if you're suspicious.
4. Turn the scanner off and on again.
5. Congratulations! Your scanner can now do things it always could (but Nikon locked it down).

If you want to scan full film rolls on LS-40/LS-50, you'll need to cut a hole in the back.
This requires extensive disassembly of the scanner, as all electronic and mechanical components
inside have to be removed before you can safely cut/drill an opening.
Do **NOT** attempt to cut the hole with the components still inside - getting
*any* metal shavings inside the scanner pretty much dooms it to fail sooner
than later.

1. See [Gleb's website](http://www.shtengel.com/gleb/nikon_coolscan_4000_disassembly_and_scanhead_gear_repair.htm)
   for disassembly instructions.

2. For cutting the opening, use [these guides](hole/) - make sure to print it 100% scale.
   The shape of the opening accommodates a standard LS-4000/LS-5000 back door and a spool holder.

3. You can 3D print a simple [film door](3dmodels/) to cover the rear hole but it's not
   strictly necessary.

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
* [SA-21/SA-30 pinout](doc/sa-21-30.md)
* [Firmware overview](doc/firmware.md)
* [Communication protocol](doc/protocol.md)
* [Firmware updates](doc/updates.md)
* [Service Manuals](manuals/)
* [Datasheets](datasheets/)
* [Firmware dumps](dumps/)
* [Firmware reflashers](reflashers/)
* [Nikon SDKs](sdks/)

## Acknowledgements

This project has been made possible thanks to some great people:

* [Gleb Shtengel](http://www.shtengel.com/gleb/index.htm) - provided knowledge, firmware dumps, USB dumps, and parts
* [Kilian Dolan](https://github.com/arahowerya) - solved the six frame mystery ([issue #4](https://github.com/kosma/coolscan-mods/issues/4))
* Victor Gerasimov - provided knowledge, ideas, and firmware dumps
* Paulo Meireles - provided hardware, inspiration, and smiles
* Graeme Hardie - provided wisdom on the value of shipping 32-bit executables
* [Nikon Coolscan Users](https://www.facebook.com/groups/1514948298527146) - provided a fantastic captive audience for my shenanigans
* [SANE](https://gitlab.com/sane-project/backends/-/blob/master/backend/coolscan3.c) - provided bad but working open driver implementation
* [Служба НТИ](https://www.snti.ru/) - provided cheap ISO/IEC standards
* Frank Phillips - said he's skeptical, giving me the best motivation possible: spite
