#include "24points.h"

#include "math.h"

unsigned char code SIGN[7] = "+-_*/?"; //'_'：后一个数减去前一个数；'?'：后一个数除以前一个数

//***************************************************//
//类似于树的数据结构
struct ELECMENT_{
	float num;
	unsigned char sign;
	struct ELECMENT_* pa;
	struct ELECMENT_* pb;
	unsigned char tmpa;
	unsigned char tmpb;
};
typedef struct ELECMENT_ ELECMENT;

static ELECMENT xdata elecment[7];
static ELECMENT* xdata head[4];
static unsigned char xdata n = 4;
//***************************************************//
unsigned char xdata strout[9][16] = { 0 };		//输出字符串
static unsigned char xdata strout_index = 0;	

//***************************************************//
//以下三个为数据结构实现函数
static void init_elecment(unsigned char* str)
{
	unsigned char i = 0, j = 0;
	n = 4;
	for (; i < 4; i++)
	{
		elecment[i].num = 0;
		while (*str != ',' && *str != '\0')
			elecment[i].num = 10 * elecment[i].num + *str++ - '0';
		str++;
		elecment[i].sign = 'N';
		elecment[i].pa = 0;
		elecment[i].pb = 0;
		head[i] = &elecment[i];
	}
	for(i = 0; i < 9; i++)
		for(j = 0; j < 16; j++)
			strout[i][j] = '\0';
	strout_index = 0;
}

static void join_elecment(unsigned char a, unsigned char b, unsigned char sign)
{
	unsigned char i = 0;
	switch (sign)
	{
	case '+': elecment[8 - n].num = head[a]->num + head[b]->num; break;
	case '-': elecment[8 - n].num = head[a]->num - head[b]->num; break;
	case '_': elecment[8 - n].num = head[b]->num - head[a]->num; break;
	case '*': elecment[8 - n].num = head[a]->num * head[b]->num; break;
	case '/': elecment[8 - n].num = head[a]->num / head[b]->num; break;
	case '?': elecment[8 - n].num = head[b]->num / head[a]->num; break;
	default: break;
	}
	elecment[8 - n].sign = sign;
	elecment[8 - n].pa = head[a];
	elecment[8 - n].pb = head[b];
	elecment[8 - n].tmpa = a;
	elecment[8 - n].tmpb = b;
	
	for (i = (a > b ? b : a); i > 0; i--)
		head[i] = head[i - 1];
	for (i = (a < b ? b : a); i < 3; i++)
		head[i] = head[i + 1];
	head[0] = &elecment[8 - n];
	n--;
}

static void split_elecment()
{
	unsigned char i = 0;
	ELECMENT* tmp_ptr = head[0];
	for (i = 3; i > (tmp_ptr->tmpb > tmp_ptr->tmpa ? tmp_ptr->tmpb : tmp_ptr->tmpa); i--)
		head[i] = head[i - 1];
	for (i = 0; i < (tmp_ptr->tmpb < tmp_ptr->tmpa ? tmp_ptr->tmpb : tmp_ptr->tmpa); i++)
		head[i] = head[i + 1];
	head[tmp_ptr->tmpa] = tmp_ptr->pa;
	head[tmp_ptr->tmpb] = tmp_ptr->pb;
	n++;
}
//***************************************************//
//将“树”遍历，并将在strout中输出表达式
//1.遍历过程相当于非递归的中序遍历（先左后中再右），每经过一个路径，把路径的方向'左'或者'右'放入栈中
//2.如果到了叶子节点且栈顶为'左'，则栈的最上边部分有多少个'左'，就输出相应量的'('，输出该叶子节点
//3.如果到了叶子节点且栈顶为'右'，则输出该叶子节点，栈的最上边部分有多少个'左右'，就出栈两次，输出相应量的')'
//4.注意如果是非叶子节点，就是符号节点。那么遇到'_','?'，则是（先右后中再左）的左右颠倒
static void output()
{
	ELECMENT* stark[4] = { 0 };	//栈
	unsigned char i = 0;
	
	unsigned char j = 0, k = 0;

	ELECMENT* tmp_ptr = head[0];
	unsigned int left_or_right = 0;

	unsigned char* out_str = strout[strout_index++];
	while (i > 0 || tmp_ptr)
	{
		while (tmp_ptr)
		{
			stark[i++] = tmp_ptr;
			if (tmp_ptr->sign == '_' || tmp_ptr->sign == '?')
				tmp_ptr = tmp_ptr->pb, left_or_right = 4 * left_or_right + 1;//入栈'左'
			else if (tmp_ptr->sign == '+' || tmp_ptr->sign == '-' ||
				tmp_ptr->sign == '*' || tmp_ptr->sign == '/')
				tmp_ptr = tmp_ptr->pa, left_or_right = 4 * left_or_right + 1;
			else
				tmp_ptr = tmp_ptr->pa;
		}
		if (i > 0)
		{
			tmp_ptr = stark[--i];
			if (tmp_ptr->sign != 'N')
			{
				if(tmp_ptr->sign == '_') 
					out_str[j++] = '-';
				else if (tmp_ptr->sign == '?')
					out_str[j++] = '/';
				else
					out_str[j++] = tmp_ptr->sign;
				if (tmp_ptr->sign == '_' || tmp_ptr->sign == '?')
					tmp_ptr = tmp_ptr->pa, left_or_right = 4 * left_or_right + 2;//入栈'右'
				else
					tmp_ptr = tmp_ptr->pb, left_or_right = 4 * left_or_right + 2;
			}
			else if (left_or_right % 4 == 1)
			{
				k = 0;
				while ((left_or_right >> (2 * (k++))) % 4 == 1)//注意点2
					out_str[j++] = '(';

				if ((int)(tmp_ptr->num) / 10 == 1)
					out_str[j++] = '1';
				out_str[j++] = (int)(tmp_ptr->num) % 10 + '0';
				
				tmp_ptr = tmp_ptr->pa;
			}
			else if (left_or_right % 4 == 2)
			{
				if ((int)(tmp_ptr->num) / 10 == 1)
					out_str[j++] = '1';
				out_str[j++] = (int)(tmp_ptr->num) % 10 + '0';

				tmp_ptr = tmp_ptr->pa;

				while (left_or_right % 4 == 2)//注意点3
				{
					left_or_right /= 16;
					if (j < 16) out_str[j++] = ')';//避免超出数组初始化空间
					else j++;
				}
			}
		}
	}
	//去掉两边的'(' ')'
	for (i = 0; i < 16 - 1; i++)
		out_str[i] = out_str[i + 1];
	j = j - 2;
	out_str[j] = '\0';
}

//用这种暴力枚举，不能用递归，因为C51根本不回收变量<._.>
void _24_points_calculator(unsigned char* strin)
{
	unsigned char i4, j4, k4, i3, j3, k3, k2;
	float percent = 0.0;
		
	init_elecment(strin);
	
	if(elecment[0].num >= 1 && elecment[0].num <= 19 &&
		elecment[1].num >= 1 && elecment[1].num <= 19 &&
		elecment[2].num >= 1 && elecment[2].num <= 19 &&
		elecment[3].num >= 1 && elecment[3].num <= 19)
		;
	else
	{
		strout[0][0] = 'E';
		return;
	}
	
	for (i4 = 0; i4 < 4; i4++)
	{
		for (j4 = i4 + 1; j4 < 4; j4++)
		{
			for (k4 = 0; k4 < 6; k4++)
			{
				join_elecment(i4, j4, SIGN[k4]);
				for (i3 = 0; i3 < 3; i3++)
				{
					for (j3 = i3 + 1; j3 < 3; j3++)
					{
						for (k3 = 0; k3 < 6; k3++)
						{
							join_elecment(i3, j3, SIGN[k3]);
							for (k2 = 0; k2 < 6; k2++)
							{
								join_elecment(0, 1, SIGN[k2]);
								if (fabs(head[0]->num - 24.0) < 0.0000001)
								{
									if(strout_index >= 9)
										return;
									else
										output();
								}
								split_elecment();
								//嵌入Lcd.h, 显示完成进度
								percent += 0.02573;                        //100.0 / 3 / 6 / 6 / 6 / 6
								LcdShowUChar(12, 1, (unsigned char)(percent));
								LcdShowChar(15, 1, '%');
								//
							}
							split_elecment();
						}
					}
				}
				split_elecment();
			}
		}
	}
	if(strout[0][0] == '\0') strout[0][0] = 'N';
}