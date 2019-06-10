/*
 * new_main.c
 *
 *  Created on: Apr 2, 2019
 *      Author: zizo rashad
 */

#include "types.h"
#include "KEYPAD.h"
#include "LCD.h"
#include "UART.h"


#define F_CPU 8000000
#include<util/delay.h>

#define no_button_pressed 'N'
typedef enum
{
	false = 0,
	true,
	initial
}a;

/*
typedef enum
{
	operand1,
	op_code,
	operand2
};
*/
void operands_result (s32 * operands);
void print( s32 operands[]);
f32 result=0;	// result of any calculations
u8 divide_zero_error = false;	// zero error indicator

int main(void)
{
	/* declaring all variables
	 * ______________________*/

	// u8 keypad_mapp[16]={1,2,3,'+' ,4,5,6,'-' ,7,8,9,'*' ,'c',0,'=','/'};
	// the last for imt kits and the next map is for proetus map
	u8 keypad_mapp[16]={7,8,9,'/', 4,5,6,'*', 1,2,3,'-', 'c',0,'=','+'};
	u8 read_value=0;	// reading value from keypad mapp
	s32 operands[16]={0};	//all operands in one array
	s8 position = -1;	// position of operands array
	u8 lcd_position=0;	// lcd position indication in the first row
	u8 char_indicator = initial;	// character or operator indicator
	u8 equal_indicator = false;	//indicator of pressed equal or not

	// initializing lcd and keypad
	LCD_vidInit();
	KEYPAD_vidInit();
	UART_vidInit();

	// the glopal while loob
	while (1)
	{
		/*// getting value from keyboard on lab top via UART
		 // u need to change writenumber to writeChar and some changes
		if ( UART_u8Recieve( (u8*) &read_value)==1) {}
		else
			{
			read_value = no_button_pressed;	// this is indicator of no press
			}
			*/
		// getting value from keypad
		for (u8 i=0;i<16;i++)
		{
			if ( KEYPAD_u8GetButtonStatus(i) == pressed)
			{
				read_value = keypad_mapp[i];
				// this while for no action till unpress the keypad
				// for preventing continous display in one press
				while (KEYPAD_u8GetButtonStatus(i) == pressed) {}
				break;
			}
			else
				{
				read_value = no_button_pressed;	// this is indicator of no press

				}
		}

//reset after pressing '='
		if ( (equal_indicator == true) && (read_value != no_button_pressed) )
		{
			for (u8 i=0; i<16; i++)
				operands[i] = 0;
			position = -1;
			lcd_position=0;
			LCD_vidDisplayClr();
			divide_zero_error = false;
			equal_indicator = false;

			if ((read_value >=0) && (read_value <=9))
				{
					char_indicator = initial;
					result =0;
				}
			else
				{
					char_indicator = false ;
					LCD_vidSetPosition(2,1);
					LCD_vidWriteFloatNumber(result);
				}

		}

// entering numbers from 0 to 9
		if ( (read_value >=0) && (read_value <=9) )
		{
			if (char_indicator == true || char_indicator == initial)
			{
				position++;
				char_indicator = false;
			}
			else{}
			lcd_position++;
			LCD_vidSetPosition(1,lcd_position);
			LCD_vidWriteNumber(read_value);
			operands[position] = operands[position] * 10 + (u32) read_value;
			//number_indicator++;
		}
		else{}

		// entiring characters or operators
		switch (read_value)
		{
		case '+':
			if (char_indicator == true)	// for never make action if '+' is the second operator
			{
				break;
			}
			else if (char_indicator == false)	// means this is the first operator after numbers
			{
				char_indicator = true;
				position++;
				operands[position] = read_value;
			}
			else {}
			lcd_position++;
			LCD_vidSetPosition(1, lcd_position);
			LCD_vidWriteChar(read_value);
			break;

		case '-':
			if (char_indicator == true)
			{
				if (operands[position] == '+')
				{
					operands[position] = read_value;

				}
				else if (operands[position] == '-')
				{
					operands[position] = '+' ;
				}
				else if ( ( operands[position] == '*' || operands[position] == '/') )
				{
						position ++;
						operands[position] = read_value;
				}
				else {}
				lcd_position++;
				LCD_vidSetPosition(1, lcd_position);
				LCD_vidWriteChar(read_value);
			}
			else
			{
				char_indicator = true;
				position++;
				operands[position] = read_value;
				lcd_position++;
				LCD_vidSetPosition(1, lcd_position);
				LCD_vidWriteChar(read_value);
			}
			break;

		case '*':
			if (char_indicator == true)
				{
				break;
				}
			else if(char_indicator == false)
			{
				position++;
				operands[position] = read_value;
				lcd_position++;
				LCD_vidSetPosition(1, lcd_position);
				LCD_vidWriteChar(read_value);
				char_indicator = true;
			}
			else {}
			break;

		case '/':
			if (char_indicator == true)
				{
					break;
				}
			else if(char_indicator == false)
			{
				position++;
				operands[position] = read_value;
				lcd_position++;
				LCD_vidSetPosition(1,lcd_position);
				LCD_vidWriteChar(read_value);
				char_indicator = true;
			}
			else {}
			break;

		case 'c':
			char_indicator = initial;
			for (u8 i=0; i<16; i++)
				operands[i] = 0;
			position = -1;
			lcd_position=0;
			LCD_vidDisplayClr();
			divide_zero_error = false;
			result = 0;
			break;

		case '=':
			equal_indicator = true;
			operands_result (operands); // result function
			LCD_vidSetPosition(2,1);
			if (divide_zero_error == true)
				{
				LCD_vidWriteString( (u8*) "ERROR Divide by zero");
				}
			else
				{
				LCD_vidWriteFloatNumber(result);
				}
			break;

		default:
			break;
		}	// switch case


	} // while (1)
	return 0;
}	// main

void operands_result (s32 operands[])
{
	for (u8 i=0; i<16; i++ )	// looping in all restores data
	{
		if (operands[i]=='+')	// case we found +
		{

			result +=(f32) operands[i+1];
			operands[i+1]=0;
			i++;	// for escibing invain loobs
		}
		else if (operands[i]=='-')	//case we found -
		{
			result -= (f32) operands[i+1];
			operands[i+1]=0;
			i++;	// for escibing invain loobs
		}
		else if (operands[i]=='*')	// multiplying case have many cases
		{
			if (operands[i+1] == '-')
				// if there is - next to * ,,,
				//then multiply the next number by -1 and by the old result
			{
				result = result * -1 * operands[i+2];
				// multiplication gives error if casting as f32
				operands[i+2]=0;
				i +=2;	// for escibing invain loobs
			}
			else
				// if there is no minus next to *
				// then mutiply the next number position which must be number by result
			{
				result *=  operands[i+1];
				operands[i+1]=0;
				i++;	// for escibing invain loobs
			}
		}
		else if (operands[i]=='/')
		{
			if (operands[i+1]==0)	// if dividing by zero then error
				divide_zero_error = true;
			else if (operands[i+1] == '-')
				// the same as * previously
			{
				if (operands[i+2] == 0)
				{
					divide_zero_error = true;
				}
				else
				{
					result = ( result * -1 ) / (f32) operands[i+2];
					operands[i+2]=0;
					i +=2;	// for escibing invain loobs
				}

			}
			else
			{
				result /= (f32) operands[i+1];
				operands[i+1]=0;
				i++;
			}
		}
		else
			// for numbers in case comes first of all
		{
			if (i==0)
			{
				result = operands[i];
			}
		}


	} // for loop
} // function

// used for testing the first line and print it to the second line
void print( s32 operands[])
{
	LCD_vidSetPosition(2,1);
	for (u8 i=0;i<16;i++)
	{
		//if (operands[i] == 0 )
		//break;
		if ( (operands[i] == '+') || (operands[i] == '-') || (operands[i] == '*') || (operands[i] == '/') )
			LCD_vidWriteChar(operands[i]);
		else
			LCD_vidWriteNumber(operands[i]);
	}
}

