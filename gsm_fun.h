#define CR 0x0D
#define SUB 0x1A

#define DELL _delay_ms (100)

#define SEND (UCSRB |= (1<<UDRIE))
#define RESEN (UCSRB |= (1<<RXEN))

#define BUF_SIZE 128
#define BUF_MASK (BUF_SIZE-1)
#define IN_BUF_SIZE 64
#define IN_BUF_MASK (IN_BUF_SIZE-1)

#define RING ('R'^'I'^'N'^'G')

volatile char buffer[BUF_SIZE]="";
volatile uint8_t ind_in=0, ind_out=0, rxind_out=0, rxind_in=0, mess = 0;
volatile uint8_t com_detect=0;

volatile char inbuf[IN_BUF_SIZE]="$"; //inner buffer of USART

#define TIMEOUT 40

//sending RS232 with interupt
void SendByte(char byte)
	{
//	while(!(UCSRA & (1<<UDRE))); //Stop interrupt for correct position
//	UDR = byte;
	buffer[ind_in++] = byte;
	ind_in &= BUF_MASK;
	}

void SendStr(char *string)
	{
	while (*string!='\n')  //check if End
		{
		SendByte(*string);
		string++;
		}
	}

void Ready_Snd (void)
	{
	SND_OFF;
	RDY_ON;
	LCD_CLR;
	lcd_string ("Ready\n");
	}

void ErrMes (void)
	{
	ERR_ON;
	SND_OFF;
	RDY_ON;
	LCD_CLR;
	lcd_string ("Error\n");
	}


void code_com (uint8_t count)
	{
	switch (com_detect)
		{
		case (0x12): if (count == 4) com_detect = 2; break; //R^I^N^G
		case (0x58): if (count == 5) com_detect = 3; break; //ERROR
		case (0x04): if (count == 2) com_detect = 1; break; //OK
		case (0x5C): if (count == 3) com_detect = 4; break; //ATI
		default: com_detect = 0;
		}
	}

void rx_check_in (void)
	{
	uint8_t count=0;
	com_detect = 0;  //zero command scaner
	LCD_CLR;
	lcd_string ("Mess in BUF: \n");
	lcd_data (mess+0x30);   //how many mess in buf (ASCII)
	lcd_com (LCD_LINE2);
	while (1)
		{
		if (inbuf[rxind_out] != '$') //if mess separator detected
			{
			com_detect ^=  inbuf[rxind_out];
			lcd_data (inbuf[rxind_out++]);
			rxind_out &= IN_BUF_MASK;
			count++;
			}
		else 
			{
			rxind_out++;
			rxind_out &= IN_BUF_MASK;
			code_com (count);
			break;
			}
		}
	if (com_detect != 0)
		{
		lcd_data (' ');
		lcd_data (com_detect + 0x30);
		}
	}

int send_sms (uint8_t fun, char *number)
	{
	int clock = 0;
	SND_ON;	//turn on LED
	RDY_OFF;
	ERR_OFF;
	SendStr("AT+CMGF=1\n");
	SendByte(CR);
	UCSRB &= ~(1<<RXEN);   //disable recieve
	SEND;
	LCD_CLR;
	lcd_string ("Sending to:\n");
	lcd_com (LCD_LINE2);
	while (com_detect != 1)
		{
		if (++clock == TIMEOUT)	return 0;
		if (mess != 0) //if we have mess in buffer
			{
			// code 
			mess--;   //minus one
			rx_check_in ();
			}
		DELL;
		}
	com_detect = 0;
	clock = 0;
	SendStr("AT+CMGS=\n");
	while (*number != '\n')
		{
		SendByte (*number);
		lcd_data (*number);
		number++;
		}
	SendByte(CR);  //send ENTER
	UCSRB &= ~(1<<RXEN);
	SEND;
	DELL;
	if (fun == 1) SendStr(STR_MESS);
	else SendStr(REG_MESS);
	SendByte(SUB);
	UCSRB &= ~(1<<RXEN);
	SEND;
	while (com_detect != 1)
		{
		if (++clock == TIMEOUT || com_detect == 3)	return 0;
		if (mess != 0) //if we have mess in buffer
			{
			// code 
			mess--;   //minus one
			rx_check_in ();
			}
		DELL;
		}
	LCD_CLR;
	lcd_string ("Sended\n");
	return 1;
	}

//Sending data from buffer
ISR (USART_UDRE_vect)		
	{
	UDR = buffer[ind_out++];
	ind_out &= BUF_MASK;
	if (ind_in == ind_out)  //if last byte
		{
		UCSRB &= ~(1<<UDRIE); //disable instrupt UDR empty
		RESEN;
		}
	sei ();
	}

//recieving Data from RS232 and LCD out
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
		if (tmp != 0x0A) //clear bad simble
			{
			inbuf[rxind_in++] = tmp;
			rxind_in &= IN_BUF_MASK;
			}
		}
	sei ();
}

