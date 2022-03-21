# Nikon LS-40 Firmware Update Process

This is a description of the exchange used by the original
Nikon updater.

## Conventions used

In order to make the dumps shorter, whenever `< OK` is encountered
it means `00 00 00 00 00 00 00 00` (the default sense data).

## Preliminary scan

This is the first INQUIRY sent to enumerate scanners, before
the actual update starts. Device is closed after this.

### INQUIRY
```
INQUIRY
> 12 00 00 00 24 00
> D0
< 03
< 06 80 02 02 1F 00 00 00 4E 69 6B 6F 6E 20 20 20 4C 53 2D 34 30 20 45 44 20 20 20 20 20 20 20 20 31 2E 31 30
< OK
```

## Setup

This is performed on the actual connection to the scanner
(when the user clicks "update").

### TEST UNIT READY
```
> 00 00 00 00 00 00
> D0
< 01
< OK
```

### RESERVE UNIT
```
> 16 00 00 00 00 00
> D0
< 01
< OK
```

### SEND DIAGNOSTIC
```
> 1D 04 00 00 00 00
> D0
< 01
< OK
```

### TEST UNIT READY
```
> 00 00 00 00 00 00
> D0
< 01
< OK
```

### WRITER BUFFER

This command is repeated to stream the entire update file.
Buffer offset is *always* zero; parameter list length is
the size of the chunk (max 4096, can be less for last chunk).

```
> 3B 05 00 00 00 00 00 10 00 00
> D0
< 02
> [4096 bytes, firmware update]
< OK
```

### SET (proprietary)

```
> E0 00 80 00 00 00 00 00 09 00
> D0
< 02
> 00 00 00 00 00 00 00 00 00
< OK
```

### EXECUTE (proprietary)

```
> C1 00 00 00 00 00
> D0
< 01
< OK
```

### TEST UNIT READY

In the dump I received, this response is received once.

```
> 00 00 00 00 00 00
> D0
< 01
< 02 06 29 00 00 00 00 00
```

### TEST UNIT READY

In the dump I received, this response is received many times.
The device should be polled until sense is zero.

```
> 00 00 00 00 00 00
> D0
< 01
< 02 02 05 00 00 00 00 00
```

### TEST UNIT READY

In the dump I received, this response is received once when the
update process is complete.

```
> 00 00 00 00 00 00
> D0
< 01
< OK
```

### RELEASE UNIT
```
> 17 00 00 00 00 00
> D0
< 01
< OK
```

At this point the update process is complete.
