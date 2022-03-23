#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import struct


def main():
    dump = open(sys.argv[1], "rb").read()
    assert len(dump) == 512*1024

    ptr_recovery_start = struct.unpack(">L", dump[0x10008:0x10008+4])[0]
    ptr_firmware_end   = struct.unpack(">L", dump[0x10004:0x10004+4])[0]

    up = dump[ptr_recovery_start:ptr_firmware_end]
    open(sys.argv[2], "wb").write(up)

    print("%s -> 0x%x..0x%x -> %s" % (sys.argv[1], ptr_recovery_start, ptr_firmware_end, sys.argv[2]))

if __name__ == '__main__':
    main()
