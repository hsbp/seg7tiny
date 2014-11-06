#!/usr/bin/python

from urllib2 import urlopen
from simulator import DEFAULT_FILENAME
from number import print_number
from subprocess import Popen, PIPE
from os import path
import json

prices = json.load(urlopen("https://api.bitcoinaverage.com/ticker/EUR"))
eur24 = 2 / float(prices["24h_avg"])
rpi = Popen([path.join(path.dirname(__file__), 'rpi')],
        stdin=PIPE, stdout=PIPE, stderr=PIPE)
print_number(str(eur24 * 1000)[:4], rpi.stdin.write)
print ''.join(rpi.communicate(''))
