# Hardware overview

## CPU

The CPU used in all Coolscans is H8/3003. The CPU operates in Advanced Mode
(24-bit address bus). It is supported in IDA as "Hitachi H8/300H advanced".

## Peripheral connections

The scanner contains multiple address/data buses, with ASIC sitting in the
middle of it all. Despite of this complexity, all peripherals (including
LUT RAM, USB controller, etc.) can be directly accessed from the CPU.
The ASIC takes care of asserting the correct Chip Select lines and access
arbitration.

For exact connection diagrams see LS-50 and LS-4000 Repair Manuals. Most
information in those manuals can be extrapolated to other models.

## Memory map

This is the memory map on the LS-50. Different scanners will have
subtle differences - things like smaller/dual-chip memory (LS-2000),
different communication controllers (LS-4000), etc.

| Start    | End      | Mapped Device            | Actual Size |
|----------|----------|--------------------------|-------------|
| 0x000000 | 0x1fffff | NOR Flash (MBM29F400BC)  | 0x80000     |
| 0x200000 | 0x3fffff | ASIC?                    | 0x488       |
| 0x400000 | 0x5fffff | RAM                      | 0x20000     |
| 0x600000 | 0x7fffff | USB Controller (ISP1581) | 0x100       |
| 0x800000 | 0x83ffff | ASIC-connected RAM (DSL) | 0x40000     |
| 0xc00000 | 0xc0ffff | ASIC-connected RAM (BUF) | 0x10000     |
| 0xfffd10 | 0xffff0f | Internal RAM             | 0x200       |
| 0xffff1c | 0xffffff | Peripheral Registers     | 0xe4        |

The exact purpose of ASIC-connected RAM has not been determined but it's
not very important either - that RAM is used as 1. data buffer and 2. CCD
compensation. The above memory ranges were taken from `memtest_table`
(Coolscans run a memory test at boot) and they might be slightly wrong.

LS-2000 has two separate RAM chips mapped at 0x400000 and 0x600000.
How ASIC is mapped on this scanner is TBD. Note that I haven't yet done
extensive analysis on LS-2000 firmware so this information may be wrong.

## GPIO connections

Not all signals are routed on all scanners - e.g. LS-50 lacks rear sensors
but the signals are still physically present.

| GPIO        | Signal Name | Peripheral                             |
|-------------|-------------|----------------------------------------|
| P77         | MSW19       | Output photointerrupter (active high)  |
| PC7         | MSW12       | Adapter detection switch (active high) |
| PC6         | MSW18       | Rear door sensor (active low)          |
| PC5         | MSW11       | Adapter ID bit #2 (active high)        |
| PC4         | MSW10       | Adapter ID bit #1 (active high)        |
| PC3         | MSW9        | Adapter ID bit #0 (active high)        |
| PB4..PB7    | MT3_\*      | Film advance motor                     |

## Adapter IDs

Adapter ID is a three-bit value. The bits are inverted from the signals
on the GPIO lines - a line shorted to ground is 1, a line left open is 0.
The "Adapter ID" column refers to the adapter as seen by firmware.

| Adapter type              | PC7 | PC5 | PC4 | PC3 | Adapter ID |
|---------------------------|-----|-----|-----|-----|------------|
| No adapter                | 0   | *   | *   | *   |            |
| "For inspection purposes" | 1   | 0   | 0   | 0   | 7          |
| "Not defined"             | 1   | 0   | 0   | 1   | 6          |
| SA-30                     | 1   | 0   | 1   | 0   | 5          |
| SA-21                     | 1   | 0   | 1   | 1   | 4          |
| SF-210                    | 1   | 1   | 0   | 0   | 3          |
| IA-20                     | 1   | 1   | 0   | 1   | 2          |
| SA-20                     | 1   | 1   | 1   | 0   | 1          |
| MA-21                     | 1   | 1   | 1   | 1   | 0          |

## "For inspection purposes"

There is code that suggests that it's possible to enter recovery mode
by inserting an adapter with the ID of 7 (all ID lines pulled low).
Such an adapter probably doesn't exist "in the wild" but can be
fabricated by e.g. modifying an SA-30.
