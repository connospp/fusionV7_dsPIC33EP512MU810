/* 
 * Author: constantinos.pavlide
 *
 * Created on 16 August 2023, 13:25
 */

uint16_t SP2_transmitArray(uint8_t buffer[3]);
void __attribute__((__interrupt__, no_auto_psv)) _SPI2Interrupt(void);
uint16_t convertValue (float value);
double reverseConvertValue(uint16_t value);
float getUserReq(int CommandSel);
void handleRequest(void);