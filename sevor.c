#include"stm32f10x.h"

void sevor_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TimOCInitStructure;


 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);


 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  TIM_DeInit(TIM1);
  TIM_InternalClockConfig(TIM1);
  //预分频系数为72，这样计数器时钟为72MHz/72 = 1MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 72;
  //设置时钟分割
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  //设置计数器模式为向上计数模式
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //设置计数溢出大小，每计1000个数就产生一个更新事件
  TIM_TimeBaseStructure.TIM_Period = 20000-1;
  //将配置应用到TIM1中
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
  //禁止ARR预装载缓冲器
  TIM_ARRPreloadConfig(TIM1, DISABLE);
 
  TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设



  //设置缺省值
       TIM_OCStructInit(&TimOCInitStructure);
       //PWM模式1输出
       TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
       //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
       TimOCInitStructure.TIM_Pulse = 1500-1;
       //TIM输出比较极性高
       TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
       //使能输出状态
       TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   //TimOCInitStructure.TIM_Channel = TIM_Channel_4;
       //TIM1的CH4输出
       TIM_OC1Init(TIM1, &TimOCInitStructure);
	   TIM_OC4Init(TIM1, &TimOCInitStructure);
       //设置TIM2的PWM输出为使能
       TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


void sevor1_modify(u16 duty)
{
 TIM_SetCompare1(TIM1,duty);
}

void sevor2_modify(u16 duty)
{
 TIM_SetCompare4(TIM1,duty);
}

void pick_up()
{
 sevor2_modify(1315); 
 sevor1_modify(1000);
 delay_ms(1000);
 sevor2_modify(1700);
 delay_ms(500);
}

void put_down()
{
 sevor2_modify(1380);
 delay_ms(200); 
 sevor1_modify(1930);
 delay_ms(200);

}

void up()
{
 sevor2_modify(1900);
}

void recover()
{
 sevor2_modify(1380);
}

void adjust()
{
 sevor2_modify(1870);
}