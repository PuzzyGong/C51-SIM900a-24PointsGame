#ifndef _LCD_H_
#define _LCD_H_

#include <reg52.h>

// nvic: no
// gpio: P0 P1^0 P1^1 P1^5
// t:    no
//------------------------------------------------------------------------------------------//

void InitLcd1602(void);

//显示字符串
void LcdShowStr(unsigned char x, unsigned char y, unsigned char* str);

//显示三位数
void LcdShowUChar(unsigned char x, unsigned char y, unsigned char char_tmp);

//显示字符
void LcdShowChar(unsigned char x, unsigned char y, unsigned char char_tmp);

//清屏
void LcdFullClear(void);
	
#endif