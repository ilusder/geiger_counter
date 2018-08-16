#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "confm16.h" //pinboard
//#include "confm8.h" //device



#include "lcd.h"
#include "clock_avr.h"
#include "geiger_func.h"
#include "com_func.h"



int main (void)
	{
	uint8_t tmp;
	init_all ();
	LED_LGT;
	init_lcd ();
	LCD_CLR;
	lcd_string ("Geiger Counter");
	LED_CLR;
	RDY_ON;
	clock_conf ();
	DDRA = 0x02;
	PORTA = 02;
	LCD_CLR;
	//format display
//	lcd_line_place (2,0);
//	lcd_string ("Cnt:");
	lcd_line_place (1,9);
	lcd_string ("CPM:");
 	while (1)
		{
		lcd_clock (1,0);
//	Out total counts
//		lcd_line_place (2,4);     //out counts
//		data_int_lcd (gcount,0);
		//out CPM
		lcd_line_place (1,13);
		tmp = geiger_cpm ();
		data_int_lcd (tmp,0,0);
		lcd_data (' ');
		lcd_data (' ');
		lcd_line_place (2,0);
		cpm_usv_lcd (tmp);
		UDR = tmp;
//		SendByte (tmp);
//		SEND;
		DELL;
		}
	return 0;
	}

