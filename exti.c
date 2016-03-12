#include"stm32f10x.h"

void exti_init()			//PC0,PC1
{
 GPIO_InitTypeDef GPIO_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
 
 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     //选择中断通道0
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //抢占式中断优先级设置为0
 //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应式中断优先级设置为0
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
 NVIC_Init(&NVIC_InitStructure);

 NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;            //选择中断通道3
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //抢占式中断优先级设置为1
 //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应式中断优先级设置为1
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
 NVIC_Init(&NVIC_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);						 //key1
 EXTI_InitStructure.EXTI_Line = EXTI_Line0;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);						 //key2
 EXTI_InitStructure.EXTI_Line = EXTI_Line1;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

 EXTI_ClearITPendingBit(EXTI_Line0);
 EXTI_ClearITPendingBit(EXTI_Line1);
}

void exti_disable()
{
 EXTI_ClearITPendingBit(EXTI_Line0);
 EXTI_ClearITPendingBit(EXTI_Line1);

 EXTI->IMR &= (~(1<<0))&(~(1<<1));

 EXTI_ClearITPendingBit(EXTI_Line0);
 EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI0_IRQHandler()
{
 if(EXTI_GetITStatus(EXTI_Line0) != RESET)
 {
  motor_stop();
  left_count(850);
  forward_count(1250);
  right_count(850);
  forward();
 }
 EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler()
{
 if(EXTI_GetITStatus(EXTI_Line1) != RESET)
 {
  motor_stop();
  right_count(850);
  forward_count(1250);
  left_count(850);
  forward();
 }
 EXTI_ClearITPendingBit(EXTI_Line1);
} 