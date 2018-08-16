#define LCD_CLR lcd_clr ();
#define LCD_TOST 0x02 //upper line start
#define LCD_START 0x30 //LCD START
#define LCD_NO_CURS 0x0C  //display on, no cursor
#define LCD_DISP_OFF 0x08  //display off, no cursor
#define LCD_MODE_4 0x28 //4 bit data, 2 lines, 5x8
#define LCD_LINE1 0x80
#define LCD_LINE2 0xA8  //bottom line start
#define LCD_LIGHT_OFF (OCR2 = 0;)
#define LCD_LIGHT_ON (OCR2 = 15)

//LCD functions------------------

void lcd_com (uint8_t cmd1)
	{
	LCD_PORT = ((cmd1 & 0xF0)); //RS=0 - it's command
	LCD_PORT |= (1<<E);
	asm ("nop");
	LCD_PORT  &=~(1<<E);
	LCD_PORT = ((cmd1 & 0x0F)<<4); //Write low tetrade, RS, E to port
	LCD_PORT |= (1<<E); //Write low tetrade to LCD
	asm ("nop");
	LCD_PORT &=(~(1<<E));
	LCD_DELAY;
	}

void lcd_data (uint8_t cmd1)
	{
	LCD_PORT=((cmd1 & 0xF0));
	LCD_PORT|= ((1<<E)|(1<<RS)); //RS=1 - it's data Write higth tetrade to LCD
	asm ("nop");
	LCD_PORT&=~(1<<E);
	LCD_PORT = ((cmd1 & 0x0F)<<4); //Write low tetrade, RS, E to port 
	LCD_PORT|= ((1<<E)|(1<<RS)); //RS=1 - it's dataWrite low tetrade to LCD
	asm ("nop");
	LCD_PORT&=~(1<<E);
	LCD_DELAY;
	}

void init_lcd (void)
	{
	lcd_com (LCD_START);
	lcd_com (LCD_MODE_4);
	lcd_com (LCD_NO_CURS);
	LCD_LIGHT_ON;	
	}

void lcd_clr ()
	{
	lcd_com (0x01);
	LCD_DELAY;
	}

void lcd_string (char *s)
	{
	while  (*s!=0x00) 
		lcd_data (*s++);
	}

void lcd_hex_out (char sim)
	{
	uint8_t tmp;
	tmp = ((sim & 0xF0) >> 4); //first sim
	if (tmp >= 0x0A) tmp += 7; //if simbol (ascii)
	tmp += 0x30;    //add ascii
	lcd_data (tmp);
	tmp = sim & 0x0F; //second sim
	if (tmp >= 0x0A) tmp += 7; //if simbol (ascii)
	tmp += 0x30;    //add ascii
	lcd_data (tmp);
	}

void lcd_dec_out (char sim, uint8_t no_zero)
	{
	uint8_t tmp = 0;
	while (sim >= 100)
		{
		tmp++;
		sim -= 100;
		}
	if (tmp) 
		{
		lcd_data (tmp + 0x30);
		no_zero = 1;
		}
	tmp = 0;
	while (sim >= 10)
		{
		tmp++;
		sim -= 10;
		}
	if (tmp || no_zero) lcd_data (tmp + 0x30);
	tmp = 0;
	while (sim >= 1)
		{
		tmp++;
		sim -= 1;
		}
	lcd_data (tmp + 0x30);
	}

void lcd_line_place (uint8_t line, uint8_t place) //2 lines
	{
	switch (line)
		{
		case 1: lcd_com (LCD_LINE1 + place); break;
		case 2: lcd_com (LCD_LINE2 + place); break;
		}
	}

void data_int_lcd (uint32_t count, uint8_t no_zero, uint8_t zeros)
	{
	uint32_t par;
	uint8_t sym;
	par = 10000000;
	while (par >= 1)
		{
		sym = 0;
		while (count >= par)
			{
			sym++;
			count -= par;
			}
		if (sym || no_zero || par == 1) 
			{
			lcd_data (sym + 0x30);
			no_zero = 1;
			}
		par /= 10;
		if (par == 10000 && zeros && count) no_zero=1;
		if (par == 1000 && zeros && count) 	lcd_data ('.');
		if (par == 1) no_zero=1;
		}
	}

//END LCD functions--------------
