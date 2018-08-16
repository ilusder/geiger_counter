void init_all (void)
	{
//	MCUCSR |= (1<<JTD); //Cancel JTAG
//	MCUCSR |= (1<<JTD);
	//Timers
	TCCR0 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR2 = 0;
//	DDRA = 0xFF;  //Comment on ATmega8
	DDRB = 0xFF;   //Config out port LCD PORT
	DDRD = 0x00;   //
	DDRC = 0x00;   //Button port
//	PORTA = 0;    //Comment on ATmega8
	PORTC = 0;
	PORTD = 0;
	PORTB = 0;
	//Enable interupts
	GICR = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge

	//InitUSART

	#define baudrate 9600L
	#define bauddivider (F_CPU/(16*baudrate)-1)
	#define HI(x) ((x)>>8)
	#define LO(x) ((x)& 0xFF)
 
	UBRRL = LO(bauddivider);
	UBRRH = HI(bauddivider);
	UCSRA = 0;
	UCSRB = 1<<RXEN|1<<TXEN|0<<RXCIE|0<<TXCIE;
	UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;
	sei(); 		//Enable inerupts
	}
