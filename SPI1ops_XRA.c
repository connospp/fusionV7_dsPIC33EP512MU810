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

#include "SPI1ops_XRA.h"            /* variables/params used by user.c             */
#include "system.h"        /* System funct/params, like osc/peripheral config */

#define SIZE 6

volatile uint16_t SPInewData;


uint16_t SP1_transmitArray(void) {     
    
    uint16_t SPInewData;
    //uint32_t buffer[4] = {0x13800D, 0x12800D, 0x013200, 0x900100}; //PLL 28
    uint16_t buffer[SIZE] = {0x0600, 0x0400, 0x0F00, 0x0D00,0x042A,0x060B};   //INITialization 
    
    
    
    for(int p=0; p<SIZE; p++)
    {
        LATDbits.LATD1 = 0;
        SPI1BUF = buffer[p]; 
        delay1s();
        SPInewData = SPI1BUF;        
    }    
    return SPInewData;
}

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{
    LATDbits.LATD1 = 1;
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
}
