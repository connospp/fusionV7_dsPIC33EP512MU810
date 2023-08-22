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

#include "main.h"          /* User funct/params, such as InitApp   */
#include "uartOps.h"        /* UartOperations          */
#include "SPI2ops_dac.h"          /* User funct/params, such as InitApp   */
#include "SPI1ops_XRA.h"          /* User funct/params, such as InitApp   */
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp   */
#include <string.h>



/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/

int16_t main(void) {

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    extern unsigned int UdataPointer;
    extern char newData[];

    while (1) {
        if (UdataPointer > 0 && newData[UdataPointer - 1] == '\n') {
            transmitArray(newData, UdataPointer); //echoes       
            UdataPointer = 0; // So it only print once

            ExecuteOperation(newData);
        }
    }
}

void ExecuteOperation(char stringInp[]) {

    if (strstr(stringInp, "XRA")) {
        SP1_transmitArray();
    } else if (strstr(stringInp, "DAC")) {       
       handleRequest();
    }
}