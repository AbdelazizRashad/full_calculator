/*
 * KEYPAD.h
 *
 *  Created on: Mar 30, 2019
 *      Author: zizo rashad
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

typedef enum
{
	pressed,
	unpressed
}b;
void KEYPAD_vidInit (void);

u8 KEYPAD_u8GetButtonStatus (u8 button);


#endif /* KEYPAD_H_ */
