//********************************//
//Copyright(C),
//FileName:
//Author:		Puzzy Gong
//Version:
//Date:			2022/6/10
//Description:
//Others:
//********************************//

#include "deal.h"

//***************************************************//
//接收和发送信息需要用到的AT指令
unsigned char code rx1[] = "AT+CNMI=2,1\r\n";
unsigned char code rx2[] = "AT+CMGR=1\r\n";
unsigned char code rx3[] = "AT+CMGD=1,4\r\n";

unsigned char code tx1[] = "AT+CSCS=\"GSM\"\r\n";
unsigned char code tx2[] = "AT+CMGF=1\r\n";

unsigned char code tx4[] = "AT+CMGS=\"";
unsigned char code tx5[] = "\"\r\n";
//***************************************************//

static unsigned char strin[15] = {0};
static unsigned char page = 1 ,pagenow = 1;
//***************************************************//

static void Delay1ms(unsigned int x)
{
    unsigned char a;
    while((x--) > 0)
		for(a = 113; a > 0; a--);
}

//用对应的pagenow显示strout
static void show_deal(unsigned char cmd)
{
	signed char index = 0;

	if(cmd == '+')
		pagenow = (pagenow + 1 <= page) ? pagenow + 1 : pagenow;
	else if(cmd == '-')
		pagenow = (pagenow - 1 > 0) ? pagenow - 1 : pagenow;
	else
		pagenow = 1;
	
	index = -1;
	while(index < 9 && strout[++index][0]);
	page = index;
	
	if(strout[0][0] == 'E')
		LcdShowStr(0, 0, "invalid input");
	else if(strout[0][0] == 'N')
		LcdShowStr(0, 0, "no answer found");		
	else
	{
		LcdShowStr(0, 0, strout[pagenow - 1]);
		LcdShowChar(1, 1, pagenow + '0');
		LcdShowStr(2, 1, "/");
		LcdShowChar(3, 1, page + '0');	
	}
}

void infrared_deal(unsigned char ir_getchar)
{
	static signed char index = 0;

	if(ir_getchar >= '0' && ir_getchar <= '9')
	{
		strin[index++] = ir_getchar;
		LcdFullClear();
		LcdShowStr(0, 0, strin);
	}
	else if(ir_getchar == '.')
	{
		strin[index++] = ',';
		LcdFullClear();
		LcdShowStr(0, 0, strin);
	}
	else if(ir_getchar == '/')
	{
		strin[--index] = '\0';
		LcdFullClear();
		LcdShowStr(0, 0, strin);
	}
	else if(ir_getchar == '=')
	{
		strin[index] = '\0';
		_24_points_calculator(strin);
		
		for(index = 0; index < 15; index++)
			strin[index] = '\0';
		index = 0;
		
		show_deal('0');
	}
	else if(ir_getchar == '+' || ir_getchar == '-')
		show_deal(ir_getchar);
}

void uart_deal(unsigned char* str)
{
	signed char index = 0;
	static unsigned char flag = 0;
	
	static unsigned char cnt = 0;
	static unsigned char phonenum[18] = {0};
	static unsigned char j = 0;
	
	if(str[0] == '+' && str[1] == 'C' && str[2] == 'M' && str[3] == 'T' && str[4] == 'I')
		Delay1ms(200), send_str(rx2);
	else if(flag == 0 && str[0] == '+' && str[1] == 'C' && str[2] == 'M' && str[3] == 'G' && str[4] == 'R')
		flag = 1;
	else if(flag == 2 && str[19] == 0) 
		flag = 3;
	else if(flag == 3)
	{
		flag = 0;
		while(str[index] != '\r')
		{
			strin[index++] = str[index];
		}
		strin[index] = '\0';
		LcdFullClear();
		LcdShowStr(0, 0, strin);
		
		_24_points_calculator(strin);
		for(index = 0; index < 15; index++)
			strin[index] = '\0';
		
		show_deal('0');
		//SIM模块是新的那么需要初始化这两句
		//Delay1ms(200), send_str(tx1);
		//Delay1ms(200), send_str(tx2);
		Delay1ms(200), send_str(tx4), send_str(phonenum), send_str(tx5);
		if(strout[0][0] == 'E')
			Delay1ms(200), send_str("invalid input");
		else if(strout[0][0] == 'N')
			Delay1ms(200), send_str("no answer found");	
		else
		{
			Delay1ms(200);
			index = page;
			while(index--)	
			{
				send_str(strout[index]);
				send_char('\n');
			}
		}
		Delay1ms(200), send_char(0x1A);
		Delay1ms(3000);
		uart_init();
	}
	if(flag == 1)
	{
		for(index = 0; index < 19; index++)
		{
			if(str[index] == '\"')
			{
				if(cnt == 3)
				{
					phonenum[j] = 0;
					j = 0;
					cnt = 0;
					flag = 2;
					break;
				}
				cnt++;
			}
			else if(cnt == 3)
				phonenum[j++] = str[index];
		}
	}
}

void uart_init(void)
{
	Delay1ms(1000);
	send_str(rx3);
	Delay1ms(1000);
	send_str(rx1);
}