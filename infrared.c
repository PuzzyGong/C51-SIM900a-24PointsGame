#include "infrared.h"

sbit  IR_INPUT = P3^2;

//还剩余三个按键未定义符号
unsigned char code byt_To_getchar[21][2] = {
 0x44, '=',
 0x40, '(',
 0x43, ')',
 0x15, '-',
 0x09, '+',
 0x19, '*',
 0x0D, '/',
 0x0C, '1',
 0x18, '2',
 0x5E, '3',
 0x08, '4',
 0x1C, '5',
 0x5A, '6',
 0x42, '7',
 0x52, '8',
 0x4A, '9',
 0x16, '0',
 0x07, '.',
 0x0C, '1',
 0x0C, '1',
 0x0C, '1'};

static void(* ir)(unsigned char) = 0;

/*初始化红外接收功能*/
void InitInfrared(void(* ir_)(unsigned char))
{
    IR_INPUT = 1;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TR0 = 0;
	ET0 = 0;
	IT0 = 1;  //设置INT1为负边沿触发
	EX0 = 1;  //使能INT1中断
	PX0 = 1;
	ir = ir_;
}

/*获取当前高电平的持续时间*/
static unsigned int GetHighTime(){
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;
	while(IR_INPUT){
	    if(TH0 >= 40){
		    break;
		}
	}
	TR0=0;
	
	return (TH0*256+TL0);
}

static unsigned int GetLowTime(){
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;
	while(!IR_INPUT){
	    if(TH0 >= 40){
		    break;
		}
	}
	TR0=0;
	
	return (TH0*256+TL0);
}

void EXINT1_ISR()interrupt 0{
    unsigned char i,j;
	unsigned char byt;
	unsigned int time;
	unsigned char ir_getchar = 0;
	
	//接收并判定引导码的9ms低电平
	time = GetLowTime();
	if(time < 7833 || time > 8755){
	    IE0 = 0;
		return;
	}
    //接收并判定引导码的4.5ms高电平
	time = GetHighTime();
	if(time < 3686 || time > 4608){
	    IE0 = 0;
		return;
	}
	//接收并判定后续的四字节数据
	for(i = 0;i < 3;i++){
	    for(j = 0;j < 8;j++){
			time = GetLowTime();
            if(time < 313 || time > 718){
	            IE0 = 0;
		        return;
	        }
			time = GetHighTime();
            if(time > 313 && time < 718){
	            byt >>= 1;
	        }
            else if(time > 1345 && time < 1751){
	            byt >>= 1;
				byt |= 0x80;
	        }
            else{
			    IE0 = 0;
		        return;
			}			
	    }
		if( i == 2 )
		{
		    for( j = 0; j < 21; j++ )
			{
			    if( byt == byt_To_getchar[j][0] )
                {
				    ir_getchar = byt_To_getchar[j][1];
					break;
				}
			}
		}
	}
	
	IE0 = 0;
	ir(ir_getchar);
}