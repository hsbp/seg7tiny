7-segment display driver over RS-232 using ATtiny2313
=====================================================

Building and burning the firmware
---------------------------------

Dependencies: `make` `gcc-avr` `avrdude`

	make program

If you've already changed the clock settings from the factory default,
you'll also need to run `make burn-fuse`

Compiling Raspberry Pi GPIO software serial port on Raspbian
------------------------------------------------------------

Two variables can be configured

 - `PIN` is the number of the pin used for serial data (UART)
 - `BITLEN` is the length of a bit in microseconds (733 works for 1200 bps)

These variables should be put into the appropriate `-D` parameters, and the
commands below produce an executable called `rpi` that can be ran as an
unprivileged user (the permissions are set to SUID root).

	gcc rpi.c -o rpi -O3 -DPIN=... -DBITLEN=... -funroll-loops
	sudo chown root:root rpi
	sudo chmod +s rpi

The software serial port reads bytes from the standard input and emits them
on the pin defined in `PIN`. `BITLEN` values under 1000 should be hand-tuned
since the jitter on the Raspberry Pi without realtime mode can be high
enough to make UART signals unreadable.
