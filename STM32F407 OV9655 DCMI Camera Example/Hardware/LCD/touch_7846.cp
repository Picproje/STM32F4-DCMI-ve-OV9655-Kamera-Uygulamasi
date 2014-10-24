#line 1 "C:/Users/My_PC/Desktop/Son Projeler/STM32F407 Test Yazýlýmý/Hardware/LCD/touch_7846.c"
#line 1 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/touch_7846.h"
#line 12 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/touch_7846.h"
typedef struct
{
 u16 X0;
 u16 Y0;
 u16 X;
 u16 Y;
 u8 Key_Sta;
 u8 Key_LSta;

 float xfac;
 float yfac;
 short xoff;
 short yoff;
}Pen_Holder;
#line 42 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/touch_7846.h"
unsigned char SPI_WriteByte(u8 num);
void SpiDelay(unsigned int DelayCnt);
u16 TPReadX(void);
u16 TPReadY(void);
u8 read_once(void);
u8 Read_Ads7846(void);

void EXTI9_5_IRQHandler(void);
void NVIC_TOUCHConfiguration(void);
void touch_init(void);
void LCD_ShowNum(uint8_t x,uint16_t y,uint16_t data);
#line 1 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/stm3240f_lcd.h"
#line 1 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/fonts.h"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for arm/include/stdint.h"





typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;
typedef signed long long int64_t;


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef signed int int_least16_t;
typedef signed long int int_least32_t;
typedef signed long long int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned int uint_least16_t;
typedef unsigned long int uint_least32_t;
typedef unsigned long long uint_least64_t;



typedef signed long int int_fast8_t;
typedef signed long int int_fast16_t;
typedef signed long int int_fast32_t;
typedef signed long long int_fast64_t;


typedef unsigned long int uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long long uint_fast64_t;


typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;


typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
#line 51 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/fonts.h"
typedef struct _tFont
{
 const uint16_t *table;
 uint16_t Width;
 uint16_t Height;

} sFONT;

extern sFONT Font16x24;
extern sFONT Font12x12;
extern sFONT Font8x12;
extern sFONT Font8x8;
#line 14 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/stm3240f_lcd.h"
typedef struct
{
 int16_t X;
 int16_t Y;
} Point, * pPoint;
#line 348 "c:/users/my_pc/desktop/son projeler/stm32f407 test yazýlýmý/hardware/lcd/stm3240f_lcd.h"
void STM3240F_LCD_Init(void);
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor);
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor);
void LCD_SetTextColor(__IO uint16_t Color);
void LCD_SetBackColor(__IO uint16_t Color);
void LCD_ClearLine(uint8_t Line);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint8_t Ypos);
void LCD_DrawChar(uint16_t Xpos, uint8_t Ypos, const uint16_t *c);
void LCD_DisplayChar(uint16_t Column, uint8_t Line, uint8_t Ascii);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_DisplayStringLine(uint16_t Xpos, uint8_t Ypos, uint8_t *ptr);
void LCD_SetDisplayWindow(uint16_t Xpos, uint8_t Ypos, uint16_t Width, uint8_t Height);
void LCD_PutPixel(uint16_t Xpos, uint8_t Ypos, uint16_t rgb_data);
uint16_t LCD_GetPixel(uint16_t Xpos, uint8_t Ypos);
void LCD_WindowModeDisable(void);
void LCD_DrawRect(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void LCD_DrawCircle(uint8_t Xpos, uint16_t Ypos, uint16_t Radius);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawMonoPict(const uint32_t *Pict);
void LCD_WriteBMP(uint32_t BmpAddress);
void LCD_WriteBMP2(uint16_t Xpos, uint8_t Ypos, uint16_t Height, uint8_t Width, uint16_t *bitmap);
void LCD_DrawFullRect(uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2);
void LCD_DrawFullCircle(uint16_t Xpos, uint8_t Ypos, uint16_t Radius);
void LCD_PolyLine(pPoint Points, uint16_t PointCount);
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount);
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount);
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount);
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount);

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t LCD_ReadRAM(void);
void LCD_PowerOn(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);
#line 5 "C:/Users/My_PC/Desktop/Son Projeler/STM32F407 Test Yazýlýmý/Hardware/LCD/touch_7846.c"
Pen_Holder Pen_Point;

unsigned int xxx,yyy;
unsigned char flag=0;



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
  GPIO_ResetBits( GPIOD , GPIO_Pin_3 ); ;
 SpiDelay(10);
 SPI_WriteByte(0x90);
 SpiDelay(10);
 x=SPI_WriteByte(0xFF);
 x<<=8;
 x+=SPI_WriteByte(0x0);
  GPIO_SetBits( GPIOD , GPIO_Pin_3 ); ;
 SpiDelay(10);
 x=x>>4;
 return x;
}


u16 TPReadY(void)
{
 u16 y=0;
  GPIO_ResetBits( GPIOD , GPIO_Pin_3 ); ;
 SpiDelay(10);
 SPI_WriteByte(0xd0);
 SpiDelay(10);
 y=SPI_WriteByte(0x0);
 y<<=8;
 y+=SPI_WriteByte(0x0);
  GPIO_SetBits( GPIOD , GPIO_Pin_3 ); ;
 y = y>>4;
 return (y);
}





u8 read_once(void)
{
 Pen_Point.X=TPReadX();
 Pen_Point.Y=TPReadY();
 return 1;
}


void NVIC_TOUCHConfiguration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;





 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);


 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
}


void touch_init(void)
{
 SPI_InitTypeDef SPI_InitStructure;
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
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 SPI_InitStructure.SPI_CRCPolynomial = 7;
 SPI_Init(SPI3,&SPI_InitStructure);
 SPI_Cmd(SPI3,ENABLE);



 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
 GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
 GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;

 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
 GPIO_Init(GPIOD,&GPIO_InitStruct);
  GPIO_SetBits( GPIOD , GPIO_Pin_3 ); ;


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


}

void LCD_ShowNum(uint8_t x,uint16_t y,uint16_t data)
{
LCD_DisplayChar(x,319-y,data/10000+48);
LCD_DisplayChar(x,319-(y+25),data%10000/1000+48);
LCD_DisplayChar(x,319-(y+50),data%1000/100+48);
LCD_DisplayChar(x,319-(y+75),data%100/10+48);
LCD_DisplayChar(x,319-(y+100),data%10+48);
}




u8 Read_Ads7846(void)
{
 u8 t,t1,count=0;
 u16 databuffer[2][10];
 u16 temp=0;

 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==0)
 read_once();
 else return 0;

 while(count<10)
 {
 { if(read_once())
 {
 databuffer[0][count]=Pen_Point.X;
 databuffer[1][count]=Pen_Point.Y;
 count++;
 }
 }
 }

 if(count==10)
 {
 do
 {
 t1=0;
 for(t=0;t<count-1;t++)
 {
 if(databuffer[0][t]>databuffer[0][t+1])
 {
 temp=databuffer[0][t+1];
 databuffer[0][t+1]=databuffer[0][t];
 databuffer[0][t]=temp;
 t1=1;
 }
 }
 }while(t1);
 do
 {
 t1=0;
 for(t=0;t<count-1;t++)
 {
 if(databuffer[1][t]>databuffer[1][t+1])
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
 return 0;
}

void EXTI9_5_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line6) != RESET)
 {
 EXTI_ClearITPendingBit(EXTI_Line6);
 Read_Ads7846();
 }
}
