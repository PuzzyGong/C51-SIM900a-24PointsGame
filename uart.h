//********************************//
//Copyright(C),
//FileName:
//Author:		Puzzy Gong
//Version:
//Date:			2022/6/10
//Description:
//Others:
//********************************//

#include <reg52.h>

// nvic: interrupt4 ES
// gpio: P3^0 P3^1
// t:    t1
//------------------------------------------------------------------------------------------//

//接收'\n'结尾的字符串（包括'\n'），储存在unsigned char* 所指的空间中，
//如果长度超过19则：[19] = 1; 否则[19] = 0;
void ConfigUART(unsigned int baud, void(* i1_)(unsigned char*));

//发送字符
void send_char(unsigned char char_tmp);

//发送字符串
void send_str(unsigned char* str_tmp);