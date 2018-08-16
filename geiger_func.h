volatile uint32_t gcount;

#define CPM_USV_CF	57  //0.0057

void beeper (uint8_t timer)
	{
	while (timer--) 
		{
		ZUM_ON;
		_delay_ms (1);
		ZUM_OFF;
		_delay_ms (1);
		}
	}

//interupt procedure
ISR (INT0_vect)		
	{
	gcount++;
	gc_ar[t_sec]++;
	beeper (2);
	sei ();
	}

void cpm_usv_lcd (uint32_t ucpm)
	{
	ucpm *= CPM_USV_CF;
	data_int_lcd (ucpm,0,1);
	lcd_string (" uSv/Hr");
	}

int geiger_cpm (void)
	{
	uint32_t cpm=0;
	uint8_t i=60;
	while (i>>0) cpm += gc_ar[i--]; //sum all counts last minute
	return cpm;
	}


char convert_32to_l8 (uint32_t data)
	{
	uint8_t num = 0;
	if (data>=255) num = 255;
	else num = data;
	return num;
	}
