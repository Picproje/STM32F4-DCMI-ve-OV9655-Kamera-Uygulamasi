#include "touch_7846.h"
#include "stm3240f_lcd.h"
#include "stm32f4xx_it.h"

Pen_Holder Pen_Point;//定义笔实体

unsigned int xxx,yyy;
unsigned char flag=0;

//SPI写数据
//向7846写入1byte数据   
unsigned char SPI_WriteByte(u8 num)    
{  
  unsigned char Data = 0;
  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET); 
  SPI_I2S_SendData(SPI3,num);   
  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET); 
  Data = SPI_I2S_ReceiveData(SPI3);

  return Data; 	 				   
} 	

void SpiDelay(unsigned int DelayCnt)
{
 unsigned int i;
 for(i=0;i<DelayCnt;i++);
}

u16 TPReadX(void)
{ 
   u16 x=0;
   T_CS();
   SpiDelay(10);
   SPI_WriteByte(0x90);
   SpiDelay(10);  
   x=SPI_WriteByte(0xFF);
   x<<=8;
   x+=SPI_WriteByte(0x0);	
   T_DCS(); 
   SpiDelay(10);    
   x=x>>4;
   return x;
}


u16 TPReadY(void)
{
  u16 y=0;
  T_CS();
  SpiDelay(10);
  SPI_WriteByte(0xd0);
  SpiDelay(10);	 
  y=SPI_WriteByte(0x0);
  y<<=8;
  y+=SPI_WriteByte(0x0); 
  T_DCS(); 
  y = y>>4; 
  return (y);
}

//读取一次X,Y值
//读到的X,Y坐标值必须都大于100
//成功返回1,不成功返回0
//读数限制在100~3800之间.			   
u8 read_once(void)
{
   Pen_Point.X=TPReadX();	  //  4096-
   Pen_Point.Y=TPReadY(); //4096-
   return 1;
}	 

//中断优先级管理/开启	   
void NVIC_TOUCHConfiguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//存储器映射,不用理    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分到第2组 总共5组		 
  //	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;  //使用外部中断10~15
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//阶级2,可以被1抢断.
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //阶层0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 	 									 
}
	    
//外部中断初始化函数
void touch_init(void)
{	
    SPI_InitTypeDef    SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStruct;  
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
  
  	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  

   GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_11;
   GPIO_Init(GPIOC,&GPIO_InitStruct);
   	  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);	
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);	 
												   
  SPI_I2S_DeInit(SPI3);	    
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//;Low//; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI3,&SPI_InitStructure); 
  SPI_Cmd(SPI3,ENABLE); 

  
  	//CS
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  

   GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
   GPIO_Init(GPIOD,&GPIO_InitStruct);     	    			
   T_DCS(); 				 
	
	//T_PEN				
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  

   GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
   GPIO_Init(GPIOD,&GPIO_InitStruct);     
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);  	 
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	    			
	
	//Read_Ads7846();        //第一次读取初始化			  
}

void LCD_ShowNum(uint8_t x,uint16_t y,uint16_t data)
{
LCD_DisplayChar(x,319-y,data/10000+48); 
LCD_DisplayChar(x,319-(y+25),data%10000/1000+48);   // %10000
LCD_DisplayChar(x,319-(y+50),data%1000/100+48); 
LCD_DisplayChar(x,319-(y+75),data%100/10+48);	 
LCD_DisplayChar(x,319-(y+100),data%10+48);
}

//读取ADS7846	  
//连续读取10次数据,对十次数据排序,然后对
//中间三次取平均值,得到最终的X,Y值  	   
u8 Read_Ads7846(void)
{
	u8 t,t1,count=0;
	u16 databuffer[2][10];//数据组
	u16 temp=0;	 
			  //NPEN
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==0)	
	  	read_once();  //触摸没按下
	 else return 0;
	
	while(count<10)   					  //循环读数10次
	{	    		   
	   {	if(read_once())//读数成功
		   {	  
			databuffer[0][count]=Pen_Point.X;
			databuffer[1][count]=Pen_Point.Y;
			count++;  
		    }
		}
	}

	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[0][t]>databuffer[0][t+1])//升序排列
				{
					temp=databuffer[0][t+1];
					databuffer[0][t+1]=databuffer[0][t];
					databuffer[0][t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	  
		do//将数据Y升序排列
		{	
			t1=0;		 
			for(t=0;t<count-1;t++)
			{
				if(databuffer[1][t]>databuffer[1][t+1])//升序排列
				{
					temp=databuffer[1][t+1];
					databuffer[1][t+1]=databuffer[1][t];
					databuffer[1][t]=temp;
					t1=1;	 
				}  
			}
		}while(t1);		   
		 	 		  
		Pen_Point.X=(databuffer[0][3]+databuffer[0][4]+databuffer[0][5])/3;
		Pen_Point.Y=(databuffer[1][3]+databuffer[1][4]+databuffer[1][5])/3;	 
		 
		xxx=Pen_Point.X;
		yyy=Pen_Point.Y;
		flag=1;
		return 1;									   
	}
	flag=0;
	return 0;//读数失败
}	   

void EXTI9_5_IRQHandler(void)		
{
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {   	  
    EXTI_ClearITPendingBit(EXTI_Line6);
    Read_Ads7846();
  }  
}
	  
