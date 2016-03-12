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
 TIM_TimeBaseStructure.TIM_Period=5000;  //��ʱ����ʼֵ
 TIM_TimeBaseStructure.TIM_Prescaler=7200-1;         //ʱ��Ԥ��Ƶ
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  // ʱ�ӷָ�
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; //���¼���ģʽ
 TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);   //��ʼ����ʱ����ֵ
 TIM_ClearFlag(TIM2,TIM_FLAG_Update);               //�����ʱ���жϱ�־ 
 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  //ʹ���ж�
 TIM_Cmd(TIM2,ENABLE);            //����ʱ�� 
}

void tim2_stop()
{
 TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);  //ʹ���ж�
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
 TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
}