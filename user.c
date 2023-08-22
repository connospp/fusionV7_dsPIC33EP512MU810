/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
// This code in included only when you are using the XC16 compiler
#include <xc.h>
#endif

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

#define FP 4000000 
#define BAUDRATE 38400
#define BRGVAL ((FP/BAUDRATE)/16)-1
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
void InitApp(void) {
    /* TODO Initialize User Ports/Peripherals/Project here */
    PortSetup();
    UartSetup();
    SPI_1_Setup();
    SPI_2_Setup();
}

void SPI_2_Setup() {
    TRISGbits.TRISG0 = 0; //RG0 as output     
    LATGbits.LATG0 = 1; // SS high        

    SPI2STATbits.SPIEN = 0;   //Disable until condif
    SPI2STATbits.SPISIDL = 0; //Dont stop in idle mode
    SPI2STATbits.SISEL = 0;//was 5   //Interrupt when Tx finished
    
    SPI2CON1bits.MSTEN = 1; //Master enable    
    SPI2STATbits.SPIROV = 0; 
    
    SPI2CON1bits.DISSCK = 0; //Enable CLOCK
    SPI2CON1bits.DISSDO = 0; // Enable DO    
    SPI2CON1bits.MODE16 = 0; // Operate in 8bit/16bit   

    SPI2CON1bits.SMP = 0; //Data sampled middle of data output
    SPI2CON1bits.CKE = 0; //
    SPI2CON1bits.CKP = 0; //Polarity normal (idle low)            
    SPI2CON1bits.SPRE = 0; //Secondary Prescaler 8:1
    SPI2CON1bits.PPRE = 3; //Primary prescaler 16:1   

    
    SPI2CON2bits.SPIBEN = 1; //enchanced mode

    SPI2STATbits.SPIEN = 1;
    
    // Interrupt Controller Settings
    IFS2bits.SPI2IF = 0; // Clear the Interrupt flag
    IEC2bits.SPI2IE = 1; // Enable the interrupt

}

void SPI_1_Setup() {
    TRISDbits.TRISD1 = 0; //RD1 as output     
    LATDbits.LATD1 = 1; // SS high    
    
    RPOR1bits.RP66R = 0x05; //SDO RP66 RD2
    RPOR1bits.RP67R = 0x06; //SCK1 RP67 RD3        

    TRISDbits.TRISD0 = 1; //RD0 as input
    RPINR20bits.SDI1R = 0x40; //SDI1 RP64 RD0
    
    SPI1STATbits.SPIEN = 0;   //Disable until condif
    SPI1STATbits.SPISIDL = 0; //Dont stop in idle mode
    SPI1STATbits.SISEL = 5;   //Interrupt when Tx finished
    
    SPI1CON1bits.MSTEN = 1; //Master enable    
    SPI1STATbits.SPIROV = 0; 
    
    SPI1CON1bits.DISSCK = 0; //Enable CLOCK
    SPI1CON1bits.DISSDO = 0; // Enable DO    
    SPI1CON1bits.MODE16 = 1; // Operate in 8bit/16bit   

    SPI1CON1bits.SMP = 0; //Data sampled middle of data output
    SPI1CON1bits.CKE = 1; //
    SPI1CON1bits.CKP = 0; //Polarity normal (idle low)            
    SPI1CON1bits.SPRE = 0; //Secondary Prescaler 8:1
    SPI1CON1bits.PPRE = 3; //Primary prescaler 16:1   

    
    SPI1CON2bits.SPIBEN = 1; //enchanced mode

    SPI1STATbits.SPIEN = 1;
    
    // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 1; // Enable the interrupt

}

void UartSetup(void) {
    U1MODEbits.UARTEN = 0; // Bit15 TX, RX DISABLED, ENABLE at end 

    /*TX SETUP*/
    RPOR9bits.RP101R = 1; //Tx on RF5

    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = BRGVAL; // Baud Rate setting for 38400

    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;

    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt

    /*RX SETUP*/
    TRISFbits.TRISF4 = 1; //F4 as input
    RPINR18bits.U1RXR = 100; //Rx on RF4

    U1STAbits.URXISEL0 = 0; // Interrupt after one character is received
    U1STAbits.URXISEL1 = 0;

    /* Generic*/
    U1MODEbits.UARTEN = 1; // And turn the peripheral on
    U1STAbits.UTXEN = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1; // Enable UART RX interrupt
}

void PortSetup(void) {  
    
    ANSELA = 0x00; // Convert all I/O pins to digital
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    ANSELG = 0x00;    
}