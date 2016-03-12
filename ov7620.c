#include"stm32f10x.h"
#include"def.h"

u8 buf[ROW][COLUMN];
u16 row=0;
u8 processReady=0;
u8 wantProcess=0;
u8 sampleReady=0;

void camera_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;

 //Camre_GPIO
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
 GPIO_Init(GPIOC,&GPIO_InitStructure);

 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOC,&GPIO_InitStructure);

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
 
 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;     //选择中断通道2
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占式中断优先级设置为0
 //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应式中断优先级设置为0
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
 NVIC_Init(&NVIC_InitStructure);

 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;            //选择中断通道3
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占式中断优先级设置为1
 //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应式中断优先级设置为1
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
 NVIC_Init(&NVIC_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);						 //key1
 EXTI_InitStructure.EXTI_Line = EXTI_Line4;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);						 //key2
 EXTI_InitStructure.EXTI_Line = EXTI_Line5;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

}

void print_image()
{
u8 i,j;
 //发送处理指令
		wantProcess=1;
		while(processReady!=1);
		//if(processReady==1)
		{
			wantProcess=0;
			processReady=0;	 

			for(i=0;i<ROW;i++)
			{
				for(j=0;j<COLUMN;j++)
				{        			
					if(buf[i][j]<THRESHOLD&&buf[i][j+1]<THRESHOLD)
					USART_SendData(USART1, '0');
					else 
					USART_SendData(USART1, '1');

					while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				}
				USART_SendData(USART1, 0x0D);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0A);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
			}
			    
				USART_SendData(USART1, 0x0D);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0A);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0D);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0A);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0D);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, 0x0A);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		   		
		}
}


void EXTI4_IRQHandler()
{
	u16 line1=0;
	EXTI->PR|=1<<4;								   //清除外部中断标志位
	row++;
    if(row%4!=0) {return;}
	if(row>240) {EXTI->IMR&=~(1<<4);return;}
    line1=(row-4)/4;  
    
	buf[line1][0]=(u8)GPIOA->IDR;__NOP;
	buf[line1][1]=(u8)GPIOA->IDR;__NOP;
	buf[line1][2]=(u8)GPIOA->IDR;__NOP;
	buf[line1][3]=(u8)GPIOA->IDR;__NOP;
	buf[line1][4]=(u8)GPIOA->IDR;__NOP;
	buf[line1][5]=(u8)GPIOA->IDR;__NOP;
	buf[line1][6]=(u8)GPIOA->IDR;__NOP;
	buf[line1][7]=(u8)GPIOA->IDR;__NOP;
	buf[line1][8]=(u8)GPIOA->IDR;__NOP;
	buf[line1][9]=(u8)GPIOA->IDR;__NOP;
	buf[line1][10]=(u8)GPIOA->IDR;__NOP;
	buf[line1][11]=(u8)GPIOA->IDR;__NOP;
	buf[line1][12]=(u8)GPIOA->IDR;__NOP;
	buf[line1][13]=(u8)GPIOA->IDR;__NOP;
	buf[line1][14]=(u8)GPIOA->IDR;__NOP;
	buf[line1][15]=(u8)GPIOA->IDR;__NOP;
	buf[line1][16]=(u8)GPIOA->IDR;__NOP;
	buf[line1][17]=(u8)GPIOA->IDR;__NOP;
	buf[line1][18]=(u8)GPIOA->IDR;__NOP;
	buf[line1][19]=(u8)GPIOA->IDR;__NOP;
	buf[line1][20]=(u8)GPIOA->IDR;__NOP;
	buf[line1][21]=(u8)GPIOA->IDR;__NOP;
	buf[line1][22]=(u8)GPIOA->IDR;__NOP;
	buf[line1][23]=(u8)GPIOA->IDR;__NOP;
	buf[line1][24]=(u8)GPIOA->IDR;__NOP;
	buf[line1][25]=(u8)GPIOA->IDR;__NOP;
	buf[line1][26]=(u8)GPIOA->IDR;__NOP;
	buf[line1][27]=(u8)GPIOA->IDR;__NOP;
	buf[line1][28]=(u8)GPIOA->IDR;__NOP;
	buf[line1][29]=(u8)GPIOA->IDR;__NOP;
	buf[line1][30]=(u8)GPIOA->IDR;__NOP;
	buf[line1][31]=(u8)GPIOA->IDR;__NOP;
	buf[line1][32]=(u8)GPIOA->IDR;__NOP;
	buf[line1][33]=(u8)GPIOA->IDR;__NOP;
	buf[line1][34]=(u8)GPIOA->IDR;__NOP;
	buf[line1][35]=(u8)GPIOA->IDR;__NOP;
	buf[line1][36]=(u8)GPIOA->IDR;__NOP;
	buf[line1][37]=(u8)GPIOA->IDR;__NOP;
	buf[line1][38]=(u8)GPIOA->IDR;__NOP;
	buf[line1][39]=(u8)GPIOA->IDR;__NOP;
	buf[line1][40]=(u8)GPIOA->IDR;__NOP;
	buf[line1][41]=(u8)GPIOA->IDR;__NOP;
	buf[line1][42]=(u8)GPIOA->IDR;__NOP;
	buf[line1][43]=(u8)GPIOA->IDR;__NOP;
	buf[line1][44]=(u8)GPIOA->IDR;__NOP;
	buf[line1][45]=(u8)GPIOA->IDR;__NOP;
	buf[line1][46]=(u8)GPIOA->IDR;__NOP;
	buf[line1][47]=(u8)GPIOA->IDR;__NOP;
	buf[line1][48]=(u8)GPIOA->IDR;__NOP;
	buf[line1][49]=(u8)GPIOA->IDR;__NOP;
	buf[line1][50]=(u8)GPIOA->IDR;__NOP;
	buf[line1][51]=(u8)GPIOA->IDR;__NOP;
	buf[line1][52]=(u8)GPIOA->IDR;__NOP;
	buf[line1][53]=(u8)GPIOA->IDR;__NOP;
	buf[line1][54]=(u8)GPIOA->IDR;__NOP;
	buf[line1][55]=(u8)GPIOA->IDR;__NOP;
	buf[line1][56]=(u8)GPIOA->IDR;__NOP;
	buf[line1][57]=(u8)GPIOA->IDR;__NOP;
	buf[line1][58]=(u8)GPIOA->IDR;__NOP;
	buf[line1][59]=(u8)GPIOA->IDR;__NOP;
	buf[line1][60]=(u8)GPIOA->IDR;__NOP;
	buf[line1][61]=(u8)GPIOA->IDR;__NOP;
	buf[line1][62]=(u8)GPIOA->IDR;__NOP;
	buf[line1][63]=(u8)GPIOA->IDR;__NOP;
	buf[line1][64]=(u8)GPIOA->IDR;__NOP;
	buf[line1][65]=(u8)GPIOA->IDR;__NOP;
	buf[line1][66]=(u8)GPIOA->IDR;__NOP;
	buf[line1][67]=(u8)GPIOA->IDR;__NOP;
	buf[line1][68]=(u8)GPIOA->IDR;__NOP;
	buf[line1][69]=(u8)GPIOA->IDR;__NOP;
	buf[line1][70]=(u8)GPIOA->IDR;__NOP;
	buf[line1][71]=(u8)GPIOA->IDR;__NOP;
	buf[line1][72]=(u8)GPIOA->IDR;__NOP;
	buf[line1][73]=(u8)GPIOA->IDR;__NOP;
	buf[line1][74]=(u8)GPIOA->IDR;__NOP;
	buf[line1][75]=(u8)GPIOA->IDR;__NOP;
	buf[line1][76]=(u8)GPIOA->IDR;__NOP;
	buf[line1][77]=(u8)GPIOA->IDR;__NOP;
	buf[line1][78]=(u8)GPIOA->IDR;__NOP;
	buf[line1][79]=(u8)GPIOA->IDR;__NOP;
	buf[line1][80]=(u8)GPIOA->IDR;__NOP;
	buf[line1][81]=(u8)GPIOA->IDR;__NOP;
	buf[line1][82]=(u8)GPIOA->IDR;__NOP;
	buf[line1][83]=(u8)GPIOA->IDR;__NOP;
	buf[line1][84]=(u8)GPIOA->IDR;__NOP;
	buf[line1][85]=(u8)GPIOA->IDR;__NOP;
	buf[line1][86]=(u8)GPIOA->IDR;__NOP;
	buf[line1][87]=(u8)GPIOA->IDR;__NOP;
	buf[line1][88]=(u8)GPIOA->IDR;__NOP;
	buf[line1][89]=(u8)GPIOA->IDR;__NOP;
	buf[line1][90]=(u8)GPIOA->IDR;__NOP;
	buf[line1][91]=(u8)GPIOA->IDR;__NOP;
	buf[line1][92]=(u8)GPIOA->IDR;__NOP;
	buf[line1][93]=(u8)GPIOA->IDR;__NOP;
	buf[line1][94]=(u8)GPIOA->IDR;__NOP;
	buf[line1][95]=(u8)GPIOA->IDR;__NOP;
	buf[line1][96]=(u8)GPIOA->IDR;__NOP;
	buf[line1][97]=(u8)GPIOA->IDR;__NOP;
	buf[line1][98]=(u8)GPIOA->IDR;__NOP;
	buf[line1][99]=(u8)GPIOA->IDR;__NOP;
	buf[line1][100]=(u8)GPIOA->IDR;__NOP;
	buf[line1][101]=(u8)GPIOA->IDR;__NOP;
	buf[line1][102]=(u8)GPIOA->IDR;__NOP;
	buf[line1][103]=(u8)GPIOA->IDR;__NOP;
	buf[line1][104]=(u8)GPIOA->IDR;__NOP;
	buf[line1][105]=(u8)GPIOA->IDR;__NOP;
	buf[line1][106]=(u8)GPIOA->IDR;__NOP;
	buf[line1][107]=(u8)GPIOA->IDR;__NOP;
	buf[line1][108]=(u8)GPIOA->IDR;__NOP;
	buf[line1][109]=(u8)GPIOA->IDR;__NOP;
	buf[line1][110]=(u8)GPIOA->IDR;__NOP;
	buf[line1][111]=(u8)GPIOA->IDR;__NOP;
	buf[line1][112]=(u8)GPIOA->IDR;__NOP;
	buf[line1][113]=(u8)GPIOA->IDR;__NOP;
	buf[line1][114]=(u8)GPIOA->IDR;__NOP;
	buf[line1][115]=(u8)GPIOA->IDR;__NOP;
	buf[line1][116]=(u8)GPIOA->IDR;__NOP;
	buf[line1][117]=(u8)GPIOA->IDR;__NOP;
	buf[line1][118]=(u8)GPIOA->IDR;__NOP;
	buf[line1][119]=(u8)GPIOA->IDR;__NOP;
	buf[line1][120]=(u8)GPIOA->IDR;__NOP;
	buf[line1][121]=(u8)GPIOA->IDR;__NOP;
	buf[line1][122]=(u8)GPIOA->IDR;__NOP;
	buf[line1][123]=(u8)GPIOA->IDR;__NOP;
	buf[line1][124]=(u8)GPIOA->IDR;__NOP;
	buf[line1][125]=(u8)GPIOA->IDR;__NOP;
	buf[line1][126]=(u8)GPIOA->IDR;__NOP;
	buf[line1][127]=(u8)GPIOA->IDR;__NOP;
	buf[line1][128]=(u8)GPIOA->IDR;__NOP;
	buf[line1][129]=(u8)GPIOA->IDR;__NOP;
	buf[line1][130]=(u8)GPIOA->IDR;__NOP;
	buf[line1][131]=(u8)GPIOA->IDR;__NOP;
	buf[line1][132]=(u8)GPIOA->IDR;__NOP;
	buf[line1][133]=(u8)GPIOA->IDR;__NOP;
	buf[line1][134]=(u8)GPIOA->IDR;__NOP;
	buf[line1][135]=(u8)GPIOA->IDR;__NOP;
	buf[line1][136]=(u8)GPIOA->IDR;__NOP;
	buf[line1][137]=(u8)GPIOA->IDR;__NOP;
	buf[line1][138]=(u8)GPIOA->IDR;__NOP;
	buf[line1][139]=(u8)GPIOA->IDR;__NOP;
	buf[line1][140]=(u8)GPIOA->IDR;__NOP;
	buf[line1][141]=(u8)GPIOA->IDR;__NOP;
	buf[line1][142]=(u8)GPIOA->IDR;__NOP;
	buf[line1][143]=(u8)GPIOA->IDR;__NOP;
	buf[line1][144]=(u8)GPIOA->IDR;__NOP;
	buf[line1][145]=(u8)GPIOA->IDR;__NOP;
	buf[line1][146]=(u8)GPIOA->IDR;__NOP;
	buf[line1][147]=(u8)GPIOA->IDR;__NOP;
	buf[line1][148]=(u8)GPIOA->IDR;__NOP;
	buf[line1][149]=(u8)GPIOA->IDR;__NOP;
	buf[line1][150]=(u8)GPIOA->IDR;__NOP;
	buf[line1][151]=(u8)GPIOA->IDR;__NOP;
	buf[line1][152]=(u8)GPIOA->IDR;__NOP;
	buf[line1][153]=(u8)GPIOA->IDR;__NOP;
	buf[line1][154]=(u8)GPIOA->IDR;__NOP;
	buf[line1][155]=(u8)GPIOA->IDR;__NOP;
	buf[line1][156]=(u8)GPIOA->IDR;__NOP;
	buf[line1][157]=(u8)GPIOA->IDR;__NOP;
	buf[line1][158]=(u8)GPIOA->IDR;__NOP;
	buf[line1][159]=(u8)GPIOA->IDR;__NOP;
	buf[line1][160]=(u8)GPIOA->IDR;__NOP;
	buf[line1][161]=(u8)GPIOA->IDR;__NOP;
	buf[line1][162]=(u8)GPIOA->IDR;__NOP;
	buf[line1][163]=(u8)GPIOA->IDR;__NOP;
	buf[line1][164]=(u8)GPIOA->IDR;__NOP;
	buf[line1][165]=(u8)GPIOA->IDR;__NOP;
	buf[line1][166]=(u8)GPIOA->IDR;__NOP;
	buf[line1][167]=(u8)GPIOA->IDR;__NOP;
	buf[line1][168]=(u8)GPIOA->IDR;__NOP;
	buf[line1][169]=(u8)GPIOA->IDR;__NOP;
	buf[line1][170]=(u8)GPIOA->IDR;__NOP;
	buf[line1][171]=(u8)GPIOA->IDR;__NOP;
	buf[line1][172]=(u8)GPIOA->IDR;__NOP;
	buf[line1][173]=(u8)GPIOA->IDR;__NOP;
	buf[line1][174]=(u8)GPIOA->IDR;__NOP;
	buf[line1][175]=(u8)GPIOA->IDR;__NOP;
	buf[line1][176]=(u8)GPIOA->IDR;__NOP;
	buf[line1][177]=(u8)GPIOA->IDR;__NOP;
	buf[line1][178]=(u8)GPIOA->IDR;__NOP;
	buf[line1][179]=(u8)GPIOA->IDR;__NOP;	

	if(line1==ROW-1)
	{
		sampleReady=1;
	}
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
    //写入中断函数
	EXTI->PR|=1<<5;							   //清除外部中断标志位
	if(wantProcess==1)
	{
		if(sampleReady==0)
		{
			row=0;
			EXTI->IMR|=1<<4;
		}
		else
		{
			EXTI->IMR&=~(1<<4);
			sampleReady=0;
			processReady=1;
		}
	}    
    /* Clear the EXTI Line 13 */  
    EXTI_ClearITPendingBit(EXTI_Line13);
  }

}




