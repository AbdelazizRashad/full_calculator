/*
 * KEYPAD.c
 *
 *  Created on: Mar 30, 2019
 *      Author: zizo rashad
 */
#include"types.h"
#include"KEYPAD_cfg.h"
#include"KEYPAD.h"
#include"DIO.h"
#define F_CPU 8000000
#include<util/delay.h>

void KEYPAD_vidInit(void)
{
	DIO_vidSetPinDirection(KeypadPort,R0,0); // R0 i/p
	DIO_vidSetPinValue(KeypadPort,R0,1);	// R0 Pull up
	DIO_vidSetPinDirection(KeypadPort,R1,0); // R1 i/p
	DIO_vidSetPinValue(KeypadPort,R1,1);	// R1 Pull up
	DIO_vidSetPinDirection(KeypadPort,R2,0); // R2 i/p
	DIO_vidSetPinValue(KeypadPort,R2,1);	// R2 Pull up
	DIO_vidSetPinDirection(KeypadPort,R3,0); // R3 i/p
	DIO_vidSetPinValue(KeypadPort,R3,1);	// R3 Pull up

	DIO_vidSetPinDirection(KeypadPort,C0,1);	// C0 o/p
	DIO_vidSetPinDirection(KeypadPort,C1,1);	// C1 o/p
	DIO_vidSetPinDirection(KeypadPort,C2,1);	// C2 o/p
	DIO_vidSetPinDirection(KeypadPort,C3,1);	// C3 o/p
}

u8 KEYPAD_u8GetButtonStatus(u8 button)
{
	u8 real_col[]={C0,C1,C2,C3};
	u8 real_row[]={R0,R1,R2,R3};
	u8 row = button /4;
	u8 col = button %4;
	u8 value=0 ;
	u8 status = unpressed;

	DIO_vidSetPinValue(KeypadPort,C0,1);
	DIO_vidSetPinValue(KeypadPort,C1,1);
	DIO_vidSetPinValue(KeypadPort,C2,1);
	DIO_vidSetPinValue(KeypadPort,C3,1);

	DIO_vidSetPinValue(KeypadPort,real_col[col],0);

	value = DIO_u8ReadPinValue(KeypadPort,real_row[row]);
	if (value == 0)
		_delay_ms(10);
	else {}
	value = DIO_u8ReadPinValue(KeypadPort,real_row[row]);
	if (value == 0)
		status = pressed;
	else {}
	return status;
}
