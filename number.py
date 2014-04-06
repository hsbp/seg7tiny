#!/usr/bin/env python

from __future__ import print_function
import re

SEG_DP = 1 << 3
SEG_A = 1 << 7
SEG_B = 1 << 0
SEG_C = 1 << 2
SEG_D = 1 << 4
SEG_E = 1 << 5
SEG_F = 1 << 6
SEG_G = 1 << 1

SEGMAP = {
        '': 0,
        '.': SEG_DP,
        '0': SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
        '1': SEG_B | SEG_C,
        '2': SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,
        '3': SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,
        '4': SEG_F | SEG_B | SEG_C | SEG_G,
        '5': SEG_A | SEG_F | SEG_D | SEG_C | SEG_G,
        '6': SEG_A | SEG_F | SEG_D | SEG_C | SEG_G | SEG_E,
        '7': SEG_A | SEG_B | SEG_C,
        '8': SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
        '9': SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,
        }

def print_number(numstr, writer):
    for digit, match in enumerate(re.finditer(r'(\d)(\.?)', numstr)):
        segments = SEGMAP[match.group(1)] | SEGMAP[match.group(2)]
        writer(chr(digit) + chr(0xFF ^ segments))

if __name__ == '__main__':
    from simulator import DEFAULT_FILENAME
    from sys import argv, stderr
    if len(argv) < 2:
        print('Usage: {0} <number>'.format(argv[0]), file=stderr)
    else:
        with file(DEFAULT_FILENAME, 'w') as fifo:
            print_number(argv[1], fifo.write)
