#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "confm16.h" //pinboard
//#include "confm8.h" //device

volatile uint8_t prog0 = 0, prog1 = 0, prog2 = 0, prog3 = 0;
volatile uint8_t sc_key = 0;   //last key after scan
#define MAX_PROG 15

#include "lcd.h"
#include "com_func.h"

int main (void)
	{
	uint8_t range,temp;
	init_all ();
	LED_LGT;
	init_lcd ();
	LCD_CLR;
	lcd_string ("U-Sonic meter");
	LED_CLR;
	RDY_ON;
	DDRA = 0x02;
	PORTA = 02;
 	while (1)
		{
		lcd_com (LCD_LINE2);
		lcd_string ("Range X:");
			while (1)
				{
				range = 0;
				PORTA |= (1<<1);
				_delay_ms (10);
				PORTA &= ~(1<<1); 
				while ((temp = PINA && 1) ==0)
				PORTA &= ~(1<<2);
				while ((temp = PINA && 1) ==1) 
					{
					range++;
					_delay_us (25);
					if (range >= 254) break;
					}
				lcd_line_place (2,9);
				lcd_dec_out (range);
//					 SendByte (range);
//					 SEND;
//				com_dec_out (range);
				}
		LONG_DELAY;
		}
	return 0;
	}
