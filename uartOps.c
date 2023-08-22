/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
// This code in included only when you are using the XC16 compiler
#include <xc.h>
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "uartOps.h"            /* variables/params used by user.c               */
#include "system.h"        /* System funct/params, like osc/peripheral config */

#define MaxSize 4096

volatile char newData[MaxSize];
volatile unsigned int UdataPointer = 0;

void transmitArray(char buf[], unsigned int size) { //Send UART known size
    for (int p = 0; p < size - 1; p++) {
        U1TXREG = buf[p]; // Transmit one character
        delay1ms(1);
    }
    U1TXREG = '\r';
}


void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {

    while (U1STAbits.URXDA == 1) {
        bool errB = false;
        /* check for receive errors */
        if (U1STAbits.FERR == 1) {
            U1STAbits.FERR = 0;
            errB = true;
        }
        if (U1STAbits.PERR == 1) {
            U1STAbits.PERR = 0;
            errB = true;
        }        
        if (errB == true) return;
        
        newData[UdataPointer++] = U1RXREG; //read Buffer
        
        if (U1STAbits.OERR == 1) {
            U1STAbits.OERR = 0;
        }         
    }
     IFS0bits.U1RXIF = 0; // Clear RX Interrupt flag
}