#ifndef _24POINTS_H_
#define _24POINTS_H_

#include "lcd.h"//可以不加入"lcd.h"，删除"lcd.c" :line 215, 216, 217

void _24_points_calculator(unsigned char* strin); 
//输入数据格式如下例"12 13 11 10"，每个数都为1 ~ 19的整数

extern unsigned char xdata strout[9][16];
//表达式非法则：strout[0][0] = 'E'
//表达式无解则：strout[0][0] = 'N'
//表达式合法则每一行对应一个表达式如：
//strout[0] = "((1*2)*3)*4"

#endif