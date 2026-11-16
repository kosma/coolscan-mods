#!/usr/bin/env python3
"""Patch the scanner USB product ID in the Nikon FDUtility 2.2.

Only the exact original executable identified by ORIGINAL_SHA256 is accepted.
The patch changes the 16-bit little-endian product ID at file offset 0x7F8F.
The input is never modified; a separate output executable is created.

Examples:
    python patch_fdutility.py FDUtility.exe -m LS50
    python patch_fdutility.py FDUtility.exe -m LS40 -o FDUtility_LS40.exe
"""

from __future__ import annotations

import argparse
import hashlib
import sys
from pathlib import Path

ORIGINAL_SHA256 = "d8905ad4247b4d870672afe58ec799e5dc60e74a018f1a2ceff1481bd0542a38"
PRODUCT_ID_FILE_OFFSET = 0x7F8F

MODEL_MAP = {
    "LS50": 0x4001,
    "LS40": 0x4000,
}

def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Patch the product ID in the exact, SHA-256-verified original "
            "FDUtility.exe using a model name."
        )
    )
    parser.add_argument("input", type=Path, help="original FDUtility.exe")
    parser.add_argument(
        "-m", "--model", 
        choices=list(MODEL_MAP.keys()), 
        required=True,
        help="Scanner model name (LS50 or LS40)"
    )
    parser.add_argument(
        "-o", "--output", type=Path,
        help="output executable (default: <input>_<model>.exe)"
    )
    parser.add_argument(
        "--force", action="store_true",
        help="overwrite the output file if it already exists"
    )
    args = parser.parse_args()

    target_product_id = MODEL_MAP[args.model]

    input_path = args.input.expanduser()
    if not input_path.is_file():
        print(f"Error: input file not found: {input_path}", file=sys.stderr)
        return 2

    try:
        original = input_path.read_bytes()
    except OSError as exc:
        print(f"Error reading input: {exc}", file=sys.stderr)
        return 2

    actual_hash = sha256(original)
    if actual_hash != ORIGINAL_SHA256:
        print(
            "Error: input SHA-256 does not match the verified original FDUtility.exe.\n"
            f"Expected: {ORIGINAL_SHA256}\n"
            f"Found:    {actual_hash}\n"
            "No changes were made. This patcher deliberately accepts only the "
            "exact original binary.",
            file=sys.stderr,
        )
        return 2

    output_path = args.output.expanduser() if args.output else input_path.with_name(
        f"{input_path.stem}_{args.model}{input_path.suffix}"
    )

    try:
        if output_path.resolve() == input_path.resolve():
            print(
                "Error: output must be a different file; the input is never overwritten.",
                file=sys.stderr,
            )
            return 2
        if output_path.exists() and not args.force:
            print(
                f"Error: output already exists: {output_path}\n"
                "Use --force to overwrite it, or choose another -o path.",
                file=sys.stderr,
            )
            return 2

        patched = bytearray(original)
        previous_product_id = int.from_bytes(
            patched[PRODUCT_ID_FILE_OFFSET:PRODUCT_ID_FILE_OFFSET + 2], "little"
        )
        patched[PRODUCT_ID_FILE_OFFSET:PRODUCT_ID_FILE_OFFSET + 2] = (
            target_product_id.to_bytes(2, "little")
        )
        output_path.parent.mkdir(parents=True, exist_ok=True)
        output_path.write_bytes(patched)
    except OSError as exc:
        print(f"Error writing output: {exc}", file=sys.stderr)
        return 2

    print(f"Input:              {input_path}")
    print(f"Verified SHA-256:   {actual_hash}")
    print(f"Output:             {output_path}")
    print(f"Previous product:   0x{previous_product_id:04X}")
    print(f"Selected model:     {args.model} (0x{target_product_id:04X})")
    print(f"Patched file offset: 0x{PRODUCT_ID_FILE_OFFSET:X}")
    print("Done.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
