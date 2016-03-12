#include"stm32f10x.h"

extern u32 total_time;
extern u32 total_distance;
extern u8 read_ultrasonic_flag;
extern u8 circle_direction;
u8 get_distance_flag=0;
s16 ultrasonic_distance=1000;


void tim2_init()
{
 NVIC_InitTypeDef NVIC_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
 TIM_TimeBaseStructure.TIM_Period=5000;  //定时器初始值
 TIM_TimeBaseStructure.TIM_Prescaler=7200-1;         //时钟预分频
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  // 时钟分割
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; //向下计数模式
 TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);   //初始化定时器的值
 TIM_ClearFlag(TIM2,TIM_FLAG_Update);               //清除定时器中断标志 
 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  //使能中断
 TIM_Cmd(TIM2,ENABLE);            //开启时钟 
}

void tim2_stop()
{
 TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);  //使能中断
 TIM_Cmd(TIM2,DISABLE); 
}

void TIM2_IRQHandler()
{
 if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
 {
  total_time++;
  write_address(1,0);
  write_data((u32)(total_distance*0.127)/10000+48);
  write_data((u32)(total_distance*0.127)%10000/1000+48);
  write_data((u32)(total_distance*0.127)%1000/100+48);
  write_data((u32)(total_distance*0.127)%100/10+48);
  write_data('.');
  write_data((u32)(total_distance*0.127)%10+48);
  write_word("cm");

  write_address(3,0);
  write_data(total_time*5/1000+48);
  write_data(total_time*5%1000/100+48);
  write_data(total_time*5%100/10+48);
  write_data('.');
  write_data(total_time*5%10+48);
  write_word("s"); 
 }
 TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//必须清除中断标志位否则一直中断
}