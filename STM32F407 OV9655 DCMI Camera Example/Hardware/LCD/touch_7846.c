#include "touch_7846.h"
#include "stm3240f_lcd.h"
#include "stm32f4xx_it.h"

Pen_Holder Pen_Point;//�����ʵ��

unsigned int xxx,yyy;
unsigned char flag=0;

//SPIд����
//��7846д��1byte����   
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

//��ȡһ��X,Yֵ
//������X,Y����ֵ���붼����100
//�ɹ�����1,���ɹ�����0
//����������100~3800֮��.			   
u8 read_once(void)
{
   Pen_Point.X=TPReadX();	  //  4096-
   Pen_Point.Y=TPReadY(); //4096-
   return 1;
}	 

//�ж����ȼ�����/����	   
void NVIC_TOUCHConfiguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//�洢��ӳ��,������    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ��ֵ���2�� �ܹ�5��		 
  //	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;  //ʹ���ⲿ�ж�10~15
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//�׼�2,���Ա�1����.
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //�ײ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 	 									 
}
	    
//�ⲿ�жϳ�ʼ������
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
	
	//Read_Ads7846();        //��һ�ζ�ȡ��ʼ��			  
}

void LCD_ShowNum(uint8_t x,uint16_t y,uint16_t data)
{
LCD_DisplayChar(x,319-y,data/10000+48); 
LCD_DisplayChar(x,319-(y+25),data%10000/1000+48);   // %10000
LCD_DisplayChar(x,319-(y+50),data%1000/100+48); 
LCD_DisplayChar(x,319-(y+75),data%100/10+48);	 
LCD_DisplayChar(x,319-(y+100),data%10+48);
}

//��ȡADS7846	  
//������ȡ10������,��ʮ����������,Ȼ���
//�м�����ȡƽ��ֵ,�õ����յ�X,Yֵ  	   
u8 Read_Ads7846(void)
{
	u8 t,t1,count=0;
	u16 databuffer[2][10];//������
	u16 temp=0;	 
			  //NPEN
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==0)	
	  	read_once();  //����û����
	 else return 0;
	
	while(count<10)   					  //ѭ������10��
	{	    		   
	   {	if(read_once())//�����ɹ�
		   {	  
			databuffer[0][count]=Pen_Point.X;
			databuffer[1][count]=Pen_Point.Y;
			count++;  
		    }
		}
	}

	if(count==10)//һ��Ҫ����10������,������
	{  
	    do//������X��������
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[0][t]>databuffer[0][t+1])//��������
				{
					temp=databuffer[0][t+1];
					databuffer[0][t+1]=databuffer[0][t];
					databuffer[0][t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	  
		do//������Y��������
		{	
			t1=0;		 
			for(t=0;t<count-1;t++)
			{
				if(databuffer[1][t]>databuffer[1][t+1])//��������
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
	return 0;//����ʧ��
}	   

void EXTI9_5_IRQHandler(void)		
{
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {   	  
    EXTI_ClearITPendingBit(EXTI_Line6);
    Read_Ads7846();
  }  
}
	  
