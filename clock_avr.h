volatile uint8_t t_sec=0, t_min=0, t_hr=0;

//declarate geiger array

#define GE_BUF_SIZE 60

volatile uint32_t gc_ar[GE_BUF_SIZE];
//geiger function seat in INT0 pin

void clock_conf ()
	{

	TIMSK &=~(1<<OCIE2 | 1<< TOIE2);	// if first init
	ASSR  = 1<<AS2;				// set ASR
	TCNT2 = 0;
	TCCR2 = 5<<CS20;
	TIFR  |= 1<<OCF2 | 1<< TOV2;
	TIMSK |= 1<< TOIE2;
	}

void lcd_clock (uint8_t line, uint8_t sym)
	{
		lcd_line_place (line,sym);
		lcd_dec_out (t_hr,1);
		lcd_data (':');
		lcd_dec_out (t_min,1);
		lcd_data (':');
		lcd_dec_out (t_sec,1);
	}

ISR(TIMER2_OVF_vect)			// tTimer 2 clock programm
{
	//zero geiger count next box
	gc_ar[++t_sec]=0;
	if (t_sec==60)
		{
		t_sec=0;
		if (++t_min==60)
			{
			t_min=0;
			if (++t_hr==24)
				{
				t_hr=0;
				}
			}
		}
	sei ();
}
