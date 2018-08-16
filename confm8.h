#define VERSION "v0.9f 07.09.2014"

#define NEXT_BUTT_DELAY 8     //Next button adjust here!!

//BUTTONS names
//MY DEVICE
//#define BUT1_F 3
//#define BUT2_L 5
//#define BUT3_R 6

//MY TOWER DEVICES
#define BUT1_F 6
#define BUT2_L 3
#define BUT3_R 5


#define BUT_PORT PORTC
//LED PORT config
#define LED_PORT PORTB
#define RDY 0
#define SND 1
#define ERR 2
#define LCD_LIGHT 3

//LED config - '1' - off '0' - on
#define RDY_ON (LED_PORT &= ~ (1<<RDY))
#define RDY_OFF (LED_PORT |= (1<<RDY))


#define SND_ON (LED_PORT &= ~ (1<<SND))
#define SND_OFF (LED_PORT |= (1<<SND))


#define ERR_ON (LED_PORT &= ~ (1<<ERR))
#define ERR_OFF (LED_PORT |= (1<<ERR))

#define LED_CLR (LED_PORT = (1<<LCD_LIGHT | 1<<RDY) | (1<<SND) | (1<<ERR))
#define LED_LGT (LED_PORT = (1<<LCD_LIGHT | 0<<RDY) | (0<<SND) | (0<<ERR))

#define LONG_DELAY _delay_ms (3100)
//--


//LCD_config
#define RS 2    //ATmega8 for Tower - 2, Pinboard - 1
#define E 3   //Alwais 3
#define LCD_PORT PORTD   //pinboard - PORTB, ATmega8 for Tower PORTD
#define LCD_DELAY _delay_ms (4)

void init_all (void)
	{
//	MCUCSR |= (1<<JTD); //Cancel JTAG
//	MCUCSR |= (1<<JTD);
	//Timers
	TCCR0 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR2 =  0x6E; //Fast PWM counter 2
					//256 CLK
	OCR2 = 0;		//Start from 0
//	DDRA = 0xFF;  //Comment on ATmega8
	DDRB = 0xFF;   //Config out port LCD PORT
	DDRD = 0xFF;   //
	DDRC = 0x00;   //Button port
	PORTC = 0xFF;
//	PORTA = 0;    //Comment on ATmega8
	PORTC = 0xFF;
	PORTD = 0;
	PORTB = 0;
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
	sei(); 		//Enable inerupts
	}
