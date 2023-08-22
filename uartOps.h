/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */
void transmitArray(char buffer[],unsigned int size);
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void);