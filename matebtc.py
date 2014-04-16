#!/usr/bin/python

from urllib2 import urlopen
from simulator import DEFAULT_FILENAME
from number import print_number
from serial import Serial
import json

prices = json.load(urlopen("https://api.bitcoinaverage.com/ticker/EUR"))
eur24 = 2 / float(prices["24h_avg"])
port = Serial('/dev/ttyUSB0', 9600)
print_number(str(eur24 * 1000)[:4], port.write)
