#ifndef __DEF_H__
#define __DEF_H__


/*****************����ͷ*******************/
#define ROW 60
#define COLUMN	180
#define THRESHOLD  130

typedef enum{REC,CROSS,TRI,CIRC}shape_type;

/******************12864LCD��������********************/
#define CS GPIO_WriteBit(GPIOE,GPIO_Pin_0,SET)             //PORTG |= (1<<2)			//Ƭѡ�ź�
#define SID_UP GPIO_WriteBit(GPIOE,GPIO_Pin_1,SET)         //PORTG|= (1<<3)		//�ߵ�ƽ�����ź�
#define SID_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_1,RESET)     //PORTG&= ~(1<<3)	//�͵�ƽ�����ź�
#define SCLK_UP GPIO_WriteBit(GPIOE,GPIO_Pin_2,SET)        //PORTG|=(1<<4)		//ͬ��ʱ�Ӹߵ�ƽ�ź�
#define SCLK_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_2,RESET)    //PORTG &= ~(1<<4)	//ͬ��ʱ�ӵ͵�ƽ�ź�
#define RST_UP GPIO_WriteBit(GPIOE,GPIO_Pin_4,SET)         //PORTA|=(1<<5)		//��λ�ź���Ч***************
#define RST_DOWN GPIO_WriteBit(GPIOE,GPIO_Pin_4,RESET)     //PORTA &= ~(1<<5)	//��λ�ź���Ч******************
#define PSB GPIO_WriteBit(GPIOE,GPIO_Pin_3,RESET)          //PORTG&=~(1<<1)			//���С�����ѡ���

/**********************�������***********************/
#define STEP  1


#endif