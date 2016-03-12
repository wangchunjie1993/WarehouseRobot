#ifndef __DEF_H__
#define __DEF_H__


/*****************摄像头*******************/
#define ROW 60
#define COLUMN	180
#define THRESHOLD  130

typedef enum{REC,CROSS,TRI,CIRC}shape_type;

/******************12864LCD控制引脚********************/
#define CS GPIO_WriteBit(GPIOE,GPIO_Pin_0,SET)             //PORTG |= (1<<2)			//片选信号
#define SID_UP GPIO_WriteBit(GPIOE,GPIO_Pin_1,SET)         //PORTG|= (1<<3)		//高电平数据信号
#define SID_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_1,RESET)     //PORTG&= ~(1<<3)	//低电平数据信号
#define SCLK_UP GPIO_WriteBit(GPIOE,GPIO_Pin_2,SET)        //PORTG|=(1<<4)		//同步时钟高电平信号
#define SCLK_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_2,RESET)    //PORTG &= ~(1<<4)	//同步时钟低电平信号
#define RST_UP GPIO_WriteBit(GPIOE,GPIO_Pin_4,SET)         //PORTA|=(1<<5)		//复位信号无效***************
#define RST_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_4,RESET)     //PORTA &= ~(1<<5)	//复位信号有效******************
#define PSB GPIO_WriteBit(GPIOE,GPIO_Pin_3,RESET)          //PORTG&=~(1<<1)			//并行、串行选择端

/**********************步进电机***********************/
#define STEP  1


#endif