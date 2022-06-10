//********************************//
//Copyright(C),
//FileName:
//Author:		Puzzy Gong
//Version:
//Date:			2022/6/10
//Description:
//Others:
//********************************//

#ifndef _INFRARED_H_
#define _INFRARED_H_

#include <reg52.h>

// nvic: interrupt0 EX0
// gpio: P3^2
// t:    t0
//------------------------------------------------------------------------------------------//

//来自红外遥控器的一个字符
void InitInfrared(void(* ir_)(unsigned char));

#endif