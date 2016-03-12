#include"stm32f10x.h"

extern u8 dis_H,dis_L;
extern s8 receive_count;

s16 read_ultrasonic_1()
{
 receive_count=0;
 dis_H=dis_L=0;
 usart1_transmit(0x55);
 while(receive_count<=1);
 return dis_H*256+dis_L;
}

s16 read_ultrasonic_2()
{
 receive_count=0;
 dis_H=dis_L=0;
 usart3_transmit(0x55);
 while(receive_count<=1);
 return dis_H*256+dis_L;
}
