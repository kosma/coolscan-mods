#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Coolscan firmware patcher. Just a bunch of helper functions really,
# to make the patches easy to understand and apply.


import os


class Patch(object):

    def __init__(self, model, offset, length, original, patched):

        self.model = model
        self.offset = offset
        self.length = length
        self.original = original
        self.patched = patched

    def __repr__(self):

        return "Patch[%s]<0x%x: %s => %s>" % (self.model, self.offset, self.original.hex(), self.patched.hex())

    def apply(self, model, target):

        if model != self.model:
            return
        print(self)
        value = bytes(target[self.offset:self.offset+self.length])
        assert value == self.original
        target[self.offset:self.offset+self.length] = self.patched

    @classmethod
    def parse(cls, line):

        # remove trailing comment
        line = line.partition('#')[0]
        # remove whitespace
        line = line.strip()
        # bail out if the line was empty
        if not line:
            return None
        # split into components
        model, offset, original, patched = line.split()
        # parse/validate them
        assert offset.startswith("0x")
        offset = int(offset, 0) - 0x10000 # update files start at 0x10000
        assert len(original) == len(patched)
        if original.startswith("0x"):
            length = len(original)//2 - 1
            original = int(original, 0).to_bytes(length, byteorder='big')
            patched = int(patched, 0).to_bytes(length, byteorder='big')
        elif original.startswith("\""):
            length = len(original)-2
            original = original[1:-1].encode('ascii')
            patched = patched[1:-1].encode('ascii')
        else:
            raise ValueError("Unknown format for original/patched")
        return cls(model, offset, length, original, patched)


class Patchset(object):
    
    def __init__(self, path):

        self.patches = []
        lines = open(path).readlines()
        for line in lines:
            patch = Patch.parse(line)
            if patch is not None:
                self.patches.append(patch)

    def apply(self, model, target):
        for patch in self.patches:
            patch.apply(model, target)

def main():
    source_dir = "firmware/original"
    target_dir = "firmware/patched"
    files = [ "DF17811.bin", "PT17035.bin" ]
    patchset = Patchset("firmware/patches.txt")

    for file in files:
        model = file.removesuffix('.bin')
        source_path = os.path.join(source_dir, file)
        target_path = os.path.join(target_dir, file)
        data = bytearray(open(source_path, 'rb').read())
        patchset.apply(model, data)
        open(target_path, 'wb').write(data)

if __name__ == '__main__':
    main()
