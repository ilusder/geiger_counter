#define VERSION "v0.5 demo"


#define NEXT_BUTT_DELAY 7     //Next button adjust here!!

//BUTTONS names
#define BUT1_F 3
#define BUT2_L 5
#define BUT3_R 6


#define BUT_PORT PINC
//LED PORT config
#define LED_PORT PORTD
#define ZUM_PORT PORTD
#define ZUM_PIN 7
#define RDY 4
#define SND 5
#define ERR 7
#define LCD_LIGHT 3

//LED config - '0' - off '1' - on
#define RDY_OFF (LED_PORT &= ~ (1<<RDY))
#define RDY_ON (LED_PORT |= (1<<RDY))

#define SND_OFF (LED_PORT &= ~ (1<<SND))
#define SND_ON (LED_PORT |= (1<<SND))


#define ERR_OFF (LED_PORT &= ~ (1<<ERR))
#define ERR_ON (LED_PORT |= (1<<ERR))

#define LED_LGT (LED_PORT = (1<<RDY) | (1<<SND) | (1<<ERR))
#define LED_CLR (LED_PORT = (0<<RDY) | (0<<SND) | (0<<ERR))

#define ZUM_ON (ZUM_PORT |= (1<<ZUM_PIN))
#define ZUM_OFF (ZUM_PORT  &= ~(1<<ZUM_PIN))

#define LONG_DELAY _delay_ms (3100)



//LCD_config
#define RS 0    //ATmega8 for Tower - 2, Pinboard - 0
#define E 2   //Always PB - 2
#define LCD_PORT PORTB   //pinboard - PORTB, ATmega8 for Tower PORTD
#define LCD_DELAY _delay_us (780)

void init_all (void)
	{
	MCUCSR |= (1<<JTD); //Cancel JTAG
	MCUCSR |= (1<<JTD);
	//Timers
	TCCR0 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR2 = 0;
	DDRA = 0xFF;  //Comment on ATmega8
	DDRB = 0xFF;   //Config out port LCD PORT
	DDRD = 0xDF;   //
	DDRC = 0x00;   //Button port
	PORTA = 0;    //Comment on ATmega8
	PORTC = 0xFF; //pull UP
	PORTD = 0xFF;
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
	UCSRB = 1<<RXEN|1<<TXEN|1<<RXCIE|0<<TXCIE;
	UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;
	UCSRA &= ~(1<< UDRE);
	sei(); 		//Enable inerupts
	}
