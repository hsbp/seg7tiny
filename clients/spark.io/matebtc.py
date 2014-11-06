#!/usr/bin/python

from urllib2 import urlopen
from simulator import DEFAULT_FILENAME
from number import print_number
from subprocess import call
from StringIO import StringIO
from os import path
from binascii import hexlify
import json

prices = json.load(urlopen("https://api.bitcoinaverage.com/ticker/EUR"))
eur24 = 2 / float(prices["24h_avg"])
sio = StringIO()
print_number(str(eur24 * 1000)[:4], sio.write)
call(['spark', 'call', 'YOUR DEVID COMES HERE', 'txserial', hexlify(sio.getvalue())])
