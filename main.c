/*
 * Firmware framework for USB I/O on PIC 18F2455 (and siblings)
 * Copyright (C) 2005 Alexander Enzmann
 * adapted to MCC18 by Alberto Maccioni on 1/6/09
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 */

#include <p18cxxx.h>
#include <stdio.h>
#include <usart.h>
#include "usb.h"
#include "OPcontrol.h"		// User code

// Define configuration registers (fuses)
#pragma config PLLDIV=3, FOSC=HSPLL_HS		//for 12 MHz crystal
#pragma config CPUDIV=OSC1_PLL2, USBDIV=2
#pragma config IESO=OFF, PWRT=OFF, BOR=ON_ACTIVE, VREGEN=ON, WDT=OFF
#pragma config MCLRE=OFF, PBADEN=OFF, STVREN=ON, LVP=OFF, XINST=OFF, DEBUG=OFF
#if !defined(__18F2450)
#pragma config CCP2MX=OFF, BORV=3
#else
#pragma config BORV=21
#endif
#pragma code low_vector=0x8
void low_interrupt (void)
{
	_asm GOTO timer_isr _endasm
}


#pragma code

// Allocate buffers in RAM for storage of bytes that have either just
// come in from the SIE or are waiting to go out to the SIE.
//char txBuffer[HID_INPUT_REPORT_BYTES];
//char rxBuffer[HID_OUTPUT_REPORT_BYTES];

#if DEBUG_PRINT
static void InitializeUSART()
{
    TRISC &= 0xBF; // Set RC6 as an output
    TRISC |= 0x80; // Set RC7 as an input
    RCSTA   = 0x90; // Enable serial port, enable receiver
    TXSTA   = 0x24; // Asynch, TSR empty, BRGH=1

    // Baud rate formula for BRG16=1, BRGH=1: Baud Rate = Fosc/(4 (n + 1)),
    // or n = (Fosc / (4 * BaudRate)) - 1
    // At 48 MHz, for 115.2K Baud:
    //     SPBRGH:SPBRG = n = Fosc / (4 * 115200) - 1 = 103.17
    BAUDCON = 0x18; // BRG16=1 txd inverted
//    SPBRGH  = 0x00; // At 48MHz, SPBRGH=0, SPBRG=103 gives 115.2K Baud
    SPBRGH  = 0x02; // 0x0270 gives 19200 Baud
#if CLK_48MHZ
//    SPBRG   = 103;  // For 48 MHz clock
    SPBRG   = 0x70;  // For 48 MHz clock
#else
    SPBRG   = 52;   // For 24 MHz clock
#endif
    printf("USB Test Startup\r\n");
}
#endif


// Entry point of the firmware
void main(void)
{
	// Set all I/O pins to digital
    ADCON1 |= 0x0F;
    
	// Initialize USB
    UCFG = 0x14; // Enable pullup resistors; full speed mode

    deviceState = DETACHED;
    remoteWakeup = 0x00;
    currentConfiguration = 0x00;

	// Call user initialization function
	UserInit();
		
	while(1)
    {
        // Ensure USB module is available
		EnableUSBModule();
		
		// As long as we aren't in test mode (UTEYE), process
		// USB transactions.
		if(UCFGbits.UTEYE != 1)
			ProcessUSBTransactions();

        // Application specific tasks
        ProcessIO();
        
    }
}
