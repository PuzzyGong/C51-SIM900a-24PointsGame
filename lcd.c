//********************************//
//Copyright(C),
//FileName:
//Author:		Puzzy Gong
//Version:
//Date:			2022/6/10
//Description:
//Others:
//********************************//

#include "lcd.h"

#define LCD1602_DB P0

sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

/* 等待液晶准备好 */
static void LcdWaitReady()
{
     unsigned char sta;
     
     LCD1602_DB = 0xFF;
     LCD1602_RS = 0;
     LCD1602_RW = 1;
     do 
     {
         LCD1602_E = 1;
         sta = LCD1602_DB; //读取状态字
         LCD1602_E = 0;
     } while (sta & 0x80); //bit7 等于 1 表示液晶正忙，重复检测直到其等于 0 为止
}

/* 向 LCD1602 液晶写入一字节命令，cmd-待写入命令值 */
static void LcdWriteCmd(unsigned char cmd)
{
     LcdWaitReady();
     LCD1602_RS = 0;
     LCD1602_RW = 0;
     LCD1602_DB = cmd;
     LCD1602_E = 1;
     LCD1602_E = 0;
}

/* 向 LCD1602 液晶写入一字节数据，dat-待写入数据值 */
static void LcdWriteDat(unsigned char dat)
{
     LcdWaitReady();
     LCD1602_RS = 1;
     LCD1602_RW = 0;
     LCD1602_DB = dat;
     LCD1602_E = 1;
     LCD1602_E = 0;
}

/* 设置显示 RAM 起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
static void LcdSetCursor(unsigned char x, unsigned char y)
{
     unsigned char addr;
     
     if (y == 0) //由输入的屏幕坐标计算显示 RAM 的地址
     addr = 0x00 + x; //第一行字符地址从 0x00 起始
     else
     addr = 0x40 + x; //第二行字符地址从 0x40 起始
     LcdWriteCmd(addr | 0x80); //设置 RAM 地址
}

/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char* str )
{
    if(x >= 16) x-=16, y+=1;
	LcdSetCursor(x, y); //设置起始地址
    while( *str != '\0' )
		LcdWriteDat( *str++ );
}

void LcdShowUChar(unsigned char x, unsigned char y, unsigned char char_tmp)
{
	if(x >= 16) x -= 16, y += 1;
	LcdSetCursor(x, y);
	if (char_tmp / 100) LcdWriteDat( char_tmp / 100 + '0' );
	if (char_tmp / 10) LcdWriteDat( (char_tmp / 10) % 10 + '0' );
	LcdWriteDat( char_tmp % 10 + '0' );
}

void LcdShowChar(unsigned char x, unsigned char y, unsigned char char_tmp)
{
	if(x >= 16) x -= 16, y += 1;
	LcdSetCursor(x, y);
	LcdWriteDat( char_tmp );
}

/* 整屏清除 */
void LcdFullClear()
{
    LcdWriteCmd(0x01);
}

/* 初始化 1602 液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38); //16*2 显示，5*7 点阵，8 位数据接口
    LcdWriteCmd(0x0C); //显示器开，光标关闭
    LcdWriteCmd(0x06); //文字不动，地址自动+1
    LcdWriteCmd(0x01); //清屏
}