Firmware for Open Programmer project,
an USB programmer for PIC and ATMEL micros, I2C memories, and other I2C/SPI devices.
Website: http://openprog.altervista.org/
This firmware is based on the USB firmware released by Alexander Enzmann.
All functions are in OPcontrol.c, which is released under GPL2 license.

Changelog:

V 0.3.0	july 2008:		first public release, supports PIC ICSP protocol (PIC10,12,16,18).
V 0.4.0	august 2008: 	added I2C protocol.
V 0.5.0	november 2008: 	added SPI protocol and some ATMEL specific instructions.
V 0.6.0	june 2009:  	switched to GPL USB framework, added instructions for PIC24 and microwire.
V 0.6.1	august 2009:	switched to software SPI due to some bugs in the hardware peripheral.
V 0.7.0	january 2010:	added some PIC24 instructions, hardware ID.
V 0.7.6	july 2010:  	modified TX16 and RX16 with variable clock period; fix for 12 bit ADC.
V 0.8.0	june 2012: 		added one-wire and UNIO protocols; tx buffer sent automatically when rx is 
						elaborated; fixed PROG_C.
