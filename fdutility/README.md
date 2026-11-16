# Patcher for Nikon FDUtility 2.2

Allows the use of FDUtility for slide feeder atachments SF-200/SF-210 on LS-40 and LS-50.

The utility offers manual motor control for the slide feeder to get it unstuck.

## Usage

Run `python patch_fdutility.py FDUtility.exe -m LS50` with the path to `FDUtility.exe` and the desired model name to create `FDUtility_LS50.exe`. Options for model names are LS40 and LS50.

## Compatibility Chart 

|           | **SF-200**  | **SF-210**  |
|-----------|-------------|-------------|
| **LS-40** |      ❓     |      ❓     |
| **LS-50** |      ❓     |      ✅     |

Only LS-50 with SF-210 was tested.

## Patch Details

FDUtility checks for LS-5000 via USB Vendor ID and Product ID at the beginning and exits with a Message Box if it is not found. This check is patched with the supplied Product ID.
That means the utility will show LS-5000 in the info field (only cosmetic).
