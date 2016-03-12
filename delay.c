#include"stm32f10x.h"

void delay_us(s16 time)
{
 u16 i=0;
 while(time--)
 {
  i=10;
  while(i--);
 }
}

void delay_ms(s16 time)
{
 u16 i=0;
 while(time--)
 {
  i=12000;
  while(i--);
 }
}