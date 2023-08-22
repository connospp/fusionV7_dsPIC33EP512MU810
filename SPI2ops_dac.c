/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
// This code in included only when you are using the XC16 compiler
#include <xc.h>
#endif
#define _OPEN_SYS_ITOA_EXT

#include <stdio.h>        /* Includes uint16_t definition                    */
#include <stdlib.h>        /* Includes uint16_t definition                    */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>


#include "uartOps.h"            /* variables/params used by user.c             */
#include "SPI2ops_dac.h"            /* variables/params used by user.c             */
#include "system.h"        /* System funct/params, like osc/peripheral config */


#define Max_V 5
#define Max_16b 0xFFFF

volatile uint16_t SPI2newData;
extern volatile unsigned int UdataPointer;
extern volatile char newData[];

uint16_t SP2_transmitArray(uint8_t buffer[3]) {

    uint16_t SPInewData = 0x0000;
    uint8_t discM[3];
    LATGbits.LATG0 = 0; // SS low

    for (int p = 0; p < 3; p++) {
        SPI2BUF = buffer[p];
    }

    while (SPI2STATbits.SRXMPT);
    //while(SPI2STATbits.SPIRBF);   

    int s = 0;
    while (!SPI2STATbits.SRXMPT) // loads data from Rx bud
    {
        delay1ms(3); //Allows buffer to fill up
        discM[s++] = SPI2BUF;
    }

    SPInewData = SPInewData | discM[1] << 8; // 0 is irrelevant/ 1 is MSB
    SPInewData = SPInewData | discM[2]; // 2 is LSB

    return SPInewData;
}

uint16_t convertValue(float value) {
    unsigned short NEW_VALUE = (Max_16b / (Max_V / value));
    return NEW_VALUE;
}

double reverseConvertValue(uint16_t value) {
    double NEW_VALUE = (long) value * Max_V / (double) Max_16b;
    return NEW_VALUE;
}

void __attribute__((__interrupt__, no_auto_psv)) _SPI2Interrupt(void) {
    //transmitArray("IN", 3); //echoes      
    LATGbits.LATG0 = 1; // SS High 
    IFS2bits.SPI2IF = 0; // Clear the Interrupt flag
}

void handleRequest(void) {
    uint16_t received;
    int16_t selectChanel;
    float enterValue;

    char instr1[] = "->Select:\n0 - To cancel request\n"
            "1 - For Set all channels\n"
            "2 - For Set 1 channel\n"
            "3 - For Read channel back\n"
            "4 - For Increment channel\n"
            "5 - For Decrement channel";

    transmitArray(instr1, sizeof (instr1)); //echoes  

    while (UdataPointer == 0 || newData[UdataPointer - 1] != '\n');    
    UdataPointer = 0; //Clear RX  pointer
    delay1ms(3); //Allows buffer to fill up

    uint8_t SendBuffer[3];

    if (strstr((char *) newData, "1")) { //Set all channels
        transmitArray("1", 2); //echoes       
        enterValue = getUserReq(0);
        SendBuffer[0] = 0xB0; //0xB0 is Select all channels
        SendBuffer[1] = 0x00 | (convertValue(enterValue) >> 8);
        SendBuffer[2] = 0x00 | convertValue(enterValue);
        SP2_transmitArray(SendBuffer);
        return;
    } else if (strstr((char *) newData, "2")) { // Set for 1 channel
        transmitArray("2", 2); //echoes
        selectChanel = getUserReq(1);
        enterValue = getUserReq(0);
        SendBuffer[0] = 0x30 | selectChanel; //0x30 is Select channel
        SendBuffer[1] = 0x00 | (convertValue(enterValue) >> 8);
        SendBuffer[2] = 0x00 | convertValue(enterValue);
        SP2_transmitArray(SendBuffer);
        return;
    } else if (strstr((char *) newData, "3")) { //Readback channel      
        transmitArray("3", 2); //echoes 
        selectChanel = getUserReq(1);
        SendBuffer[0] = 0x90 | selectChanel; //0x90 is readback command
        SendBuffer[1] = 0x00;
        SendBuffer[2] = 0x00;
        SP2_transmitArray(SendBuffer); //Setup request for channel

        SendBuffer[0] = 0x00; //0x000000 is executing request
        received = SP2_transmitArray(SendBuffer); //Execute request for channel 

        char LCDBuffer[4];
        snprintf(LCDBuffer, 5, "%.3f", reverseConvertValue(received));
        transmitArray((char*) LCDBuffer, 5);
        return;
    } else if (strstr((char *) newData, "4")) { //Incremeant
        transmitArray("4", 2); //echoes 
        selectChanel = getUserReq(1);
        enterValue = getUserReq(2);
        return;
    } else if (strstr((char *) newData, "5")) { //Decreaments
        transmitArray("5", 2); //echoes 
        selectChanel = getUserReq(1);
        enterValue = getUserReq(3);
        return;
    } else {
        char strR[] = "Cancelled";
        transmitArray(strR, sizeof (strR)); //echoes 
        return;
    }

}

float getUserReq(int CommandSel) {
    const char valueStr[4] [256] = {"Enter set value 0-MAX(5)",
        "Select channel 0-7", "Enter increment value", "Enter decrement value"};

    transmitArray((char *) valueStr[CommandSel], strlen(valueStr[CommandSel]) + 1);

    while (UdataPointer == 0 || newData[UdataPointer - 1] != '\n');
    delay1ms(3); //Allows buffer to fill up
    transmitArray((char *) newData, UdataPointer); //echoes  
    UdataPointer = 0; //Clear RX buffer pointer

    return atof((char *) newData);
}

