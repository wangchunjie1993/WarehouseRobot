#include"stm32f10x.h"
#include"def.h"

extern u8 buf[ROW][COLUMN];
extern u8 processReady;
extern u8 wantProcess;


u8 average(u8 arr[],u8 n)
{
 u8 i;
 u16 sum=0;
 for(i=0;i<n;i++)
 {
  sum+=arr[i];
 }

 return sum/n;
}

u8 max_no(u8 arr[],u8 n)
{
 u8 i,max_ele_no;
 max_ele_no=0;
 for(i=1;i<n;i++)
 {
  if(arr[max_ele_no]<arr[i]){max_ele_no=i;}
 }
 return max_ele_no;
}

u8 is_black(u8 i1,u8 j1)
{
 if((buf[i1][j1]<THRESHOLD)&&(buf[i1][j1+1]<THRESHOLD))
 return 1;
 return 0;
}

u8 white_black(u8 i1,u8 j1)
{
 if((is_black(i1,j1)==0)&&(is_black(i1,j1+2)==1))
 return 1;
 return 0;
}

u8 black_white(u8 i1,u8 j1)
{
 if((is_black(i1,j1)==1)&&(is_black(i1,j1+2)==0))
 return 1;
 return 0;
}

u8 _is_circle()
{
 u8 i=0,j=COLUMN/2;
 u8 n=0;
 u8 left[3]={0},right[3]={0},width[3]={0};
 s8 xielv1=0,xielv2=0;
 
 while(is_black(n,COLUMN/2)==0)
 {
  n++;
 }
 n+=5;

 for(i=n;i<n+15;i+=7)
 {
  for(j=COLUMN/2;j>0;j--)
  {
   if(white_black(i,j)==1)
   {
    left[(i-n)/7]=j;
	break;
   }
  }

  for(j=COLUMN/2;j<COLUMN-6;j++)
  {
   if(black_white(i,j)==1)
   {
    right[(i-n)/7]=j;
	break;
   }
  }

  width[(i-n)/7]=right[(i-n)/7]-left[(i-n)/7];
 }

 xielv1=width[1]-width[0];
 xielv2=width[2]-width[1];

 if((xielv1-xielv2>13)&&(xielv1>3)&&(xielv2>3))
 return 1;
 return 0;
 

}

u8 _is_rectangle()
{
 u8 i=0,j=COLUMN/2;
 u8 n=0;
 u8 left[20]={0},right[20]={0},width[20]={0};
 u8 width_average=0;
 while(is_black(n,COLUMN/2)==0)
 {
  n++;
 }
 n+=5;

 for(i=n;i<n+20;i++)
 {
  for(j=COLUMN/2;j>0;j--)
  {
   if(white_black(i,j)==1)
   {
    left[i-n]=j;
	break;
   }
  }

  for(j=COLUMN/2;j<COLUMN-6;j++)
  {
   if(black_white(i,j)==1)
   {
    right[i-n]=j;
	break;
   }
  }

  width[i-n]=right[i-n]-left[i-n];
 }

 width_average=average(width,20);

 for(i=0;i<20;i++)
 {
  if((width[i]-width_average>15)||(width_average-width[i])>15)
  {
   return 0;
  }
 }

 return 1;
}

u8 _is_triangle()
{
 u8 i=0,j=COLUMN/2;
 u8 n=0;
 u8 left[3]={0},right[3]={0},width[3]={0};
 s8 xielv1=0,xielv2=0;
 
 while(is_black(n,COLUMN/2)==0)
 {
  n++;
 }
 n+=5;

 for(i=n;i<n+11;i+=5)
 {
  for(j=COLUMN/2;j>0;j--)
  {
   if(white_black(i,j)==1)
   {
    left[(i-n)/5]=j;
	break;
   }
  }

  for(j=COLUMN/2;j<COLUMN-6;j++)
  {
   if(black_white(i,j)==1)
   {
    right[(i-n)/5]=j;
	break;
   }
  }

  width[(i-n)/5]=right[(i-n)/5]-left[(i-n)/5];
 }

 xielv1=width[1]-width[0];
 xielv2=width[2]-width[1];

 if((xielv1-xielv2<8)&&(xielv2-xielv1<8)&&((xielv1>10)||(xielv1<-10))&&((xielv2>10)||(xielv2<-10)))
 return 1;
 return 0;
 
}

u8 _is_cross()
{
 u8 i,j;
 u8 left[5]={0},right[5]={0},width[5]={0};
 for(i=15;i<40;i+=5)
 {
  for(j=25;j<COLUMN-10;j++)
  {
   if(white_black(i,j)==1)
   {
    left[(i-15)/5]=j;
   }
   else if(black_white(i,j)==1)
   {
    right[(i-15)/5]=j;
   }
  }
 }
 
 for(i=0;i<5;i++)
 {
  width[i]=right[i]-left[i];
 }

 for(i=0;i<4;i++)
 {
  if((width[i]-width[i+1]>60)||(width[i+1]-width[i]>60))
  return 1;
 }
 return 0;
}

u8 is_cross()
{
 u8 left[2],right[2],width[2]={0,0};
 u8 i=12,j=COLUMN/2;
 if(is_black(i,j)==1)
 return 0;
 while((is_black(i,j)==0)&&(j<COLUMN))
 {
  j++;
 }
 while((is_black(i,j)==1)&&(j<COLUMN))
 {
  j++;
  width[0]++;
 }
 j=COLUMN/2;
 i+=7;
 while((is_black(i,j)==0)&&(j<COLUMN))
 {
  j++;
 }
 while((is_black(i,j)==1)&&(j<COLUMN))
 {
  j++;
  width[1]++;
 }

 if((width[0]>11)&&(width[0]<21)&&(width[0]>11)&&(width[0]<21)&&(width[0]-width[1]<5)&&(width[1]-width[0]<5))
 return 1;
 return 0;
}

u8 is_triangle()
{

}

u8 is_rectangle()
{
 u8 i,j=COLUMN/2+30;
 u8 width[5]={0,0,0,0,0};
 for(i=20;i<41;i+=5)
 {
  j=COLUMN/2+30;
  if(is_black(i,j)==0)
  return 0;
  while((is_black(i,j)==1)&&(j<COLUMN-4))
  {
   width[(i-20)/5]++;
   j++;
  }
  j=COLUMN/2+30;
  while((is_black(i,j)==1)&&(j>0))
  {
   width[(i-20)/5]++;
   j--;
  }
 }
 for(i=0;i<5;i++)
 {
  if((width[i]<75)||(width[i]>95))
  return 0;
 }
 return 1;
}

u8 is_circle()
{
 u8 i=15,j=COLUMN/2+30;
 u8 width[3]={0,0,0};
 
 for(i=15;i<42;i+=13)
 {
  j=COLUMN/2+30; 
  if(is_black(i,j)==0)
  return 0;
  while((is_black(i,j)==1)&&(j<COLUMN-4))
  {
   width[(i-15)/13]++;
   j++;
  }
  j=COLUMN/2+30;
  while((is_black(i,j)==1)&&(j>0))
  {
   width[(i-15)/13]++;
   j--;
  }
 }
 
 if((width[1]-width[0]>10)&&(width[1]-width[2])>10)
 return 1;
 return 0;
}


shape_type distinguish_shape()
{
 u8 counter=0;
 delay_ms(1000);
 wantProcess=1;
 while(processReady!=1);
 processReady=0;
 while(processReady!=1);
 processReady=0;
 while(processReady!=1);
 wantProcess=0;
 processReady=0;
 while(1)
 {
  counter++;
  if(is_cross()==1)
  {
   return CROSS;
  }
  else if(is_circle()==1)
  {
   return CIRC;
  }
  else if(is_rectangle()==1)
  {
   return REC;
  }
  else //if(is_triangle()==1)
  {
   return TRI;
  }
  wantProcess=1;
  while(processReady!=1);
  wantProcess=0;
  processReady=0;
  if(counter>=3)
  {
   adjust();
  }  
 } 
} 