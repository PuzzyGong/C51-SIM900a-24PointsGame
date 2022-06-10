//********************************//
//Copyright(C),
//FileName:
//Author:		Puzzy Gong
//Version:
//Date:			2022/6/10
//Description:
//Others:
//********************************//

#include "main.h"

typedef enum{
	UART,
	INFRARED
}MODE;

MODE mode = INFRARED;

void ir(unsigned char ir_getchar)
{
	if(ir_getchar == '(')
	{
		mode = INFRARED; 
		P1 = P1 & (~( 0x01 << 2 ));
		P1 = P1 | ( 0x01 << 6 );
	}
	else if(ir_getchar == ')')
	{
		mode = UART;
		P1 = P1 & (~( 0x01 << 6 ));
		P1 = P1 | ( 0x01 << 2 );
	}
	if(mode == INFRARED)
		infrared_deal(ir_getchar);
}

void i1 (unsigned char* str)
{
	if(mode == UART)
		uart_deal(str);
}

void main()
{
	EA = 1;
	InitLcd1602();
	InitInfrared(ir);
	ConfigUART(9600, i1);
	uart_init();
	
	mode = INFRARED;
	P1 = P1 & (~( 0x01 << 2 ));
	P1 = P1 | ( 0x01 << 6 );
	
	while(1)
	{ 
		;
	}
}
