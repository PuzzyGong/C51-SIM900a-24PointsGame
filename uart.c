#include "uart.h"

static void(* i1)(unsigned char*) = 0;

void ConfigUART(unsigned int baud, void(* i1_)(unsigned char*))
{
    SCON  = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 256 - (11059200/12/32)/baud;  
    TL1 = TH1;
    ET1 = 0;
    ES  = 1;
    TR1 = 1;
	
	i1 = i1_;
}

void send_char(unsigned char char_tmp)
{
    SBUF = char_tmp;
	while(!TI);
	TI = 0;
}

void send_str(unsigned char* str_tmp)
{
	while(*str_tmp != '\0')
	{
		SBUF = *str_tmp;
		while(!TI);
		TI = 0;
		str_tmp++;
	}
}

void InterruptUART() interrupt 4
{
    if (RI)
    {
		static unsigned char str_tmp[20] = {0};
		static unsigned char index = 0;
		str_tmp[index++] = SBUF;
		if(SBUF == '\n')
		{
			str_tmp[19] = 0;
			i1(str_tmp);
			index = 0;
		}
		else if(index == 19)
		{
			str_tmp[19] = 1;
			i1(str_tmp);
			index = 0;
		}
		RI = 0;
    }
}