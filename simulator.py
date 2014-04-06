#!/usr/bin/env python

from __future__ import with_statement
from os import mkfifo, remove
from blessings import Terminal

DEFAULT_FILENAME = 'simulator.fifo'
SEGMENTS = { # change numbers before ':' according to your wiring
        7: [(6, 0, '______')], # a
        6: [(5, 1, '/'), (4, 2, '/'), (3, 3, '/')], # f
        0: [(11, 1, '/'), (10, 2, '/'), (9, 3, '/')], # b
        1: [(4, 3, '_____')], # g
        5: [(2, 4, '/'), (1, 5, '/'), (0, 6, '/')], # e
        2: [(8, 4, '/'), (7, 5, '/'), (6, 6, '/')], # c
        4: [(1, 6, '_____')], # d
        3: [(8, 6, 'o')], # DP
        }
DIGIT_WIDTH = 10
NUM_DIGITS = 3
SERIAL_COMMAND = 0xFF

T = Terminal()

def simulator(filename):
    serial_digit = SERIAL_COMMAND
    framebuf = [0xFF for _ in xrange(NUM_DIGITS)]
    mkfifo(filename)
    try:
        dump(framebuf)
        while True:
            with file(filename) as fifo:
                while True:
                    chunk = fifo.read(1)
                    if not chunk:
                        break
                    byte = ord(chunk)
                    if serial_digit == SERIAL_COMMAND:
                        serial_digit = byte & 0x0F
                        if serial_digit >= NUM_DIGITS:
                            serial_digit = 0
                    else:
                        framebuf[serial_digit] = byte
                        serial_digit = SERIAL_COMMAND
                        dump(framebuf)
    finally:
        remove(filename)

def dump(framebuf):
    for n, digit in enumerate(framebuf):
        for offset, elements in SEGMENTS.iteritems():
            mapper = T.red if (1 << offset) & digit else T.bold_yellow
            for x, y, element in elements:
                with T.location(x + n * DIGIT_WIDTH, y):
                    print mapper(element)

if __name__ == '__main__':
    simulator(DEFAULT_FILENAME)
