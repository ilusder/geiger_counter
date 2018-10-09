#define CR 0x0D
#define SUB 0x1A

#define DELL _delay_ms (100)

#define SEND (UCSRB |= (1<<UDRIE))
#define RESEN (UCSRB |= (1<<RXEN))

#define BUF_SIZE 16
#define BUF_MASK (BUF_SIZE-1)
#define IN_BUF_SIZE 16
#define IN_BUF_MASK (IN_BUF_SIZE-1)

volatile char buffer[BUF_SIZE]="";
volatile uint8_t ind_in=0, ind_out=0, rxind_out=0, rxind_in=0, mess = 0;
volatile uint8_t com_detect;
volatile char inbuf[IN_BUF_SIZE]="$"; //inner buffer of USART

//sending RS232 with interupt
void SendByte(char byte)
	{
	buffer[ind_in++] = byte;
	ind_in &= BUF_MASK;
	}

void SendStr(char *string)
	{
	while (*string!=0x00)  //check if End
		{
		SendByte(*string);
		string++;
		}
	SendByte (CR);
	UCSRB &= ~(1<<UDRE);  //DISABLE RECIVE
	SND_ON;
	SEND;
	}



//Sending data from buffer
ISR (USART_UDRE_vect)		
	{
	UDR = buffer[ind_out++];
	ind_out &= BUF_MASK;
	if (ind_in == ind_out)  //if last byte
		{
		UCSRB &= ~(1<<UDRIE); //disable instrupt UDR empty
//		RESEN;
		SND_OFF;
		}
	sei ();
	}

//recieving Data from RS232
ISR (USART_RXC_vect)		
	{
	uint8_t tmp;
	tmp = UDR;
	if (tmp == 0x0D) 	
		{
		mess++; //one more message
		inbuf[rxind_in++] = '$'; //set separate between mess
		rxind_in &= IN_BUF_MASK;
		}
    else 
		{
		if (tmp != ' ') inbuf[rxind_in++] = tmp;
		rxind_in &= IN_BUF_MASK;
		}
	sei ();
	}

void data_int_com(uint32_t count, uint8_t no_zero, uint8_t zeros)
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
			SendByte (sym + 0x30);
			no_zero = 1;
			}
		par /= 10;
		if (par == 10000 && zeros && count) no_zero=1;
		if (par == 1000 && zeros && count) 	SendByte ('.');
		if (par == 1) no_zero=1;
		}
	SendStr(" CPM");
	SendByte ('\n');
	SEND;
	}

