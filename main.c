#include"stm32f10x.h"
#include"def.h"

extern u8 buf[ROW][COLUMN];
extern u16 row;
extern u8 processReady;
extern u8 wantProcess;
extern u8 sampleReady;

u32 total_distance=0;			 //行驶距离
u32 total_time=0;				 //行驶时间
u8 read_ultrasonic_flag=0;		 //读超声波标志
u8 line_no=0;					 //车所在边的编号
u8 carry_count=0;				 //搬运的方块数
shape_type shape;				 //图形


int main()
{
 motor_init();		   
 trace_gpio_init();
 search_gpio_init();
 find_gpio_init();	   	
 sevor_init();
 usart_init();
 camera_init(); 
 init_12864();
 write_address(0,0);
 write_word("行驶距离：");
 write_address(2,0);
 write_word("行驶时间：");
 tim2_init();

 /*recover();
 delay_ms(200);
 sevor1_modify(1930);
 delay_ms(200);
 pick_up();
 while(1)
 {
 	print_image();
 } */

  while(carry_count<4)
  {
   while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7) == Bit_SET)
   {
    trace_search();
   }

   forward_count(1000);
   motor_stop();

   exti_disable();
   
   up();
   sevor1_modify(1930);
   delay_ms(200);
   right_count(850);  
   recover();
  
   while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6) != 0)
   {
    forward();
   }
   motor_stop();

   forward_count(300);
   sevor1_modify(1000);
   delay_ms(800);
   sevor1_modify(1500);
   delay_ms(700);
   forward_count(550);
   pick_up();
   shape=distinguish_shape();

   exti_init();

   switch(line_no)
   {
    case 0:
	{
	  switch(shape)
	  {
	   case TRI:
	   {
	    right_count(850);
	    goto_store();
		line_no=0;
		carry_count++;
	    break;
	   }
	   case REC:
	   {
	    left_count(850);
	    goto_store();
		line_no=2;
		carry_count++;
	    break;
	   }
	   case CROSS:
	   {
	    left_count(1700);
	    goto_store();
		line_no=1;
		carry_count++;
	    break;
	   }
	   case CIRC:
	   {
	    goto_store();
		carry_count++;
		line_no=3;
	    break;
	   }
	 }
	 break;
	}	
	case 1:
	{
	 switch(shape)
	 {
	  case TRI:
	  {
		goto_store();
		line_no=0;
		carry_count++;
	    break;
	  }
	  case REC:
	  {
	   left_count(1700);
	   goto_store();
	   line_no=2;
	   carry_count++;
	   break;
	  }
	  case CROSS:
	  {
	   right_count(850);
	   goto_store();
	   line_no=1;
	   carry_count++;
	   break;
	  }
	  case CIRC:
	  {
	   left_count(850);
	   goto_store();
	   line_no=3;
	   carry_count++;
	   break;
	  }
	 }
	 break;
	}
	case 2:
    {
	switch(shape)
	{
	 case TRI:
	 {
	  left_count(850);
	  goto_store();
	  line_no=0;
	  carry_count++;
	  break;
	 }
	 case REC:
	 {
	  right_count(850);
	  goto_store();
	  line_no=2;
	  carry_count++;
	  break;
	 }
	 case CROSS:
	 {
	  goto_store();
	  line_no=1;
	  carry_count++;
	  break;
	 }
	 case CIRC:
	 {
	  left_count(1700);
	  goto_store();
	  line_no=3;
	  carry_count++;
	  break;
	 }
	}
    break;
    }
	case 3:
	{
	 switch(shape)
	 {
	  case TRI:
	  {
	   left_count(1700);
	   goto_store();
	   line_no=0;
	   carry_count++;
	   break;
	  }
	  case REC:
	  {
	   goto_store();
	   line_no=2;
	   carry_count++;
	   break;
	  }
	  case CROSS:
	  {
	   left_count(850);
	   goto_store();
	   line_no=1;
	   carry_count++;
	   break;
	  }
	  case CIRC:
	  {
	   right_count(850);
	   goto_store();
	   line_no=3;
	   carry_count++;
	   break;
	  }
	 }
	 break;
	}  
   }
  }
  tim2_stop();
  while(1);     
} 



