/* 
 * File:   SPIops.h
 * Author: constantinos.pavlide
 *
 * Created on 16 August 2023, 13:25
 */

uint16_t SP1_transmitArray(void);
void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void);
float getUserReq(int CommandSel);