/**
  ******************************************************************************
  * @file    DCMI/OV9655_Camera/dcmi_ov9655.c
  * @author  MCD Application Team
  * @version V0.1.4
  * @date    10/15/2010
  * @brief   This file includes the driver for OV9655 Camera module mounted on 
  *          STM3220F-EVAL board RevA.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "OV9655.h"
#include "stm32f4xx.h"
#include "I2C.h"
#include "main.h"

/** @addtogroup DCMI_OV9655_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
   RCC_ClocksTypeDef RCC_Clocks;
/* Private define ------------------------------------------------------------*/
   #define  TIMEOUT  2
   #define  DCMI_DR_ADDRESS     0x50050028
   #define  FSMC_LCD_ADDRESS    0x60020000
/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Pin Map -------------------------------------------------------------------*/ 
/*
      DCMI D0    = PC6
			DCMI D1    = PC7
			DCMI D2    = PE0
			DCMI D3    = PE1
      DCMI D4    = PE4
			DCMI D5    = PB6
			DCMI D6    = PE5
			DCMI D7    = PE6
      DCMI HSYNC = PA4
			DCMI VSYNC = PB7
			DCMI XCLK  = PC9
			DCMI PXCLK = PA6
			I2C_SCL    = PB8
			I2C_SDA    = PB9
*/

/**
  * @brief  Configures the OV9655 DCMI
  * @param  None
  * @retval None
  */
uint8_t OV9655_Configuration(void){
	int i=0;
	OV9655_IDTypeDef OV9655_ID;
	/*OV9655 MCO Configuration*/
	OV9655_MCO_Configuration();
	/* I2C configuration */
  Soft_I2C_Configuration();

  for(i=0;i<10000;i++){}
  /* Reset and check the presence of the OV9655 camera module */
  if (DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x80)==0xFF)
  {
     return (0xFF); //Camera Failed!
  }
	/* Camera Ok!*/
  for(i=0;i<10000;i++){}
   
  DCMI_OV9655_ReadID(&OV9655_ID);	
		
  /* OV9655 Camera size setup */    
  #if defined (QQVGA_SIZE)
    DCMI_OV9655_QQVGASizeSetup();
  #elif defined (QVGA_SIZE)
    DCMI_OV9655_QVGASizeSetup();
  #endif 
  /* Set the RGB565 mode */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0xd0);

  /* Invert the HRef signal*/
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08);
	/* OV9655 Camera DCMI setup */
  OV9655_DCMI_Configuration();	

  return (0x00);
}

/**
  * @brief  Configures the MCO Module
  * @param  None
  * @retval None
  */
void OV9655_MCO_Configuration(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_MCO);
  
  RCC_PLLI2SCmd(DISABLE);
  
  RCC_PLLI2SConfig(240, 5); // 192..432, 2..7, ie range of 27.429 Mhz to 192 MHz
  /* PLLI2SSCLK = 240 / 5 = 48Mhz */
	/* MCO2 Output Freq = 48 / 2 = 24Mhz*/
  RCC_MCO2Config(RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_2); // 24 MHz with default PLL fComp
  
  RCC_PLLI2SCmd(ENABLE);
  
  while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == RESET);
}

/**
  * @brief  Configures the DCMI to interface with the OV9655 camera module.
  * @param  None
  * @retval None
  */
void OV9655_DCMI_Configuration(void)
{
  DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable DCMI GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE); 

  /* Enable DCMI clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* Connect DCMI pins to AF13 ************************************************/
  /* PCLK */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);
  /* D0-D7 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);
  /* VSYNC */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);
  /* HSYNC */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);
  
  /* DCMI GPIO configuration **************************************************/
  /* D0 D1(PC6/7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* D2..D4(PE0/1/4) D6/D7(PE5/6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 
	                              | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* D5(PB6), VSYNC(PB7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PCLK(PA6) HSYNC(PA4)*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DCMI configuration *******************************************************/ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  
  DCMI_Init(&DCMI_InitStructure);

  /* DCMI Interrupts config ***************************************************/
  //DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);
  //DCMI_ITConfig(DCMI_IT_LINE, ENABLE);
  DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
  //DCMI_ITConfig(DCMI_IT_ERR, ENABLE);
      
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
	
  /* Configures the DMA2 to transfer Data from DCMI to the LCD ****************/
  /* Enable DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 Configuration */  
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr = FSMC_LCD_ADDRESS;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

/**
  * @brief  Reset the OV9655 SCCB registers.
  * @param  None
  * @retval None
  */
void DCMI_OV9655_Reset(void)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, SCCB_REG_RESET);    
}
/**
  * @brief  Set the QVGA size(240*320).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QVGASizeSetup(void)
{  
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x12);//
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80);//78
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80);//48
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x05, 0x39);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x06, 0x39);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x07, 0x39);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x08, 0x38);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x03);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0a, 0x96); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0c, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0d, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x00);//old val 0xc0
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x10, 0x7b);//old val 7b
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x00);//old val 0x80
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x63);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xEF);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x2A);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x15, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1b, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1c, 0x7F);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1d, 0xA2);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1E, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1F, 0x00); 
  Delay(TIMEOUT); 
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x20, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x21, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x22, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x23, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x60);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
  Delay(TIMEOUT);						   
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
  Delay(TIMEOUT);							   
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2d, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2e, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2f, 0x38);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x30, 0x08);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x31, 0x30);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0x12);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x37, 0x04);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0xca);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3c, 0x0c);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x02); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xd0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x49, 0x48);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xec);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x55, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x56, 0x48);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x57, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x67, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x68, 0x80);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);//
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6a, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x0a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x11);// 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x78, 0x28);//Test
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x79, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8b, 0x04);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
//  Delay(TIMEOUT);
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8d, 0x00);
//  Delay(TIMEOUT);	
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8e, 0x00);
//  Delay(TIMEOUT);	
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8f, 0x00);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x71);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x6f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x6D);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);//
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa2, 0x9D);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);//50
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb7, 0xee);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb8, 0xee);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb9, 0x0c);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7c);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7d);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc5, 0x2e);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x81);//81
  Delay(TIMEOUT);
	DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc8, 0x06);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xce, 0xe3);
  Delay(TIMEOUT);	
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcf, 0x74);
  Delay(TIMEOUT);	
}

/**
  * @brief  Set the QQVGA size(120*160).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QQVGASizeSetup(void)
{
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x61);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x40);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x62);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xc7);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x3a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x15, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1e, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x3c);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0xa4);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0xcc);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x0e); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xc0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xfc);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x1a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x22); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x7d);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x7b);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x79);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x4a);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x82);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
  Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
  Delay(TIMEOUT);
}

/**
  * @brief  Read the OV9655 Manufacturer identifier.
  * @param  OV9655ID: pointer to the OV9655 Manufacturer identifier. 
  * @retval None
  */
void DCMI_OV9655_ReadID(OV9655_IDTypeDef* OV9655ID)
{
  OV9655ID->Manufacturer_ID1 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDH);
  OV9655ID->Manufacturer_ID2 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDL);
  OV9655ID->Version = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_VER);
  OV9655ID->PID = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_PID);
}

/**
  * @brief  Set the Internal Clock Prescaler.
  * @param  OV9655_Prescaler: the new value of the prescaler. 
  *   This parameter can be a value between 0x0 and 0x1F
  * @retval None
  */
void DCMI_OV9655_SetPrescaler(uint8_t OV9655_Prescaler)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_CLKRC, OV9655_Prescaler);
}

/**
  * @brief  Select the Output Format.
  * @param  OV9655_OuputFormat: the Format of the ouput Data.  
  *   This parameter can be a value between 0x0 and 0x1F
  *     @arg OUTPUT_FORMAT_RAWRGB_DATA 
  *     @arg OUTPUT_FORMAT_RAWRGB_INTERP    
  *     @arg OUTPUT_FORMAT_YUV              
  *     @arg OUTPUT_FORMAT_RGB    
  * @retval None
  */
void DCMI_OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_OuputFormat);
}

/**
  * @brief  Select the Output Format Resolution.
  * @param  OV9655_FormatResolution: the Resolution of the ouput Data. 
  *   This parameter can FORMAT_CTRL_15fpsVGA a value between 0x0 and 0x1F
  *     @arg FORMAT_CTRL_15fpsVGA 
  *     @arg FORMAT_CTRL_30fpsVGA_NoVArioPixel    
  *     @arg FORMAT_CTRL_30fpsVGA_VArioPixel     
  * @retval None
  */
void DCMI_OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_FormatResolution);
}

/**
  * @brief  Set the new value of OV9655 registers
  * @param  OV9655_Register: the OV9655 Register to be configured. 
  * @param  Register_Val: The new value to be set 
  * @retval None
  */
void DCMI_OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_Register, Register_Val);
}

/**
  * @brief  Select the HREF Control signal option
  * @param  OV9665_HREFControl: the HREF Control signal option.
  *   This parameter can FORMAT_CTRL_15fpsVGA a value between 0x0 and 0x1F
  *     @arg OV9665_HREFControl_Opt1: HREF edge offset to data output. 
  *     @arg OV9665_HREFControl_Opt2: HREF end 3 LSB    
  *     @arg OV9665_HREFControl_Opt3: HREF start 3 LSB      
  * @retval None
  */
void DCMI_OV9655_HREFControl(uint8_t OV9665_HREFControl)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_HREF, OV9665_HREFControl);
}

/**
  * @brief  Select the RGB format option
  * @param  OV9665_RGBOption: the RGB Format option.
  *   This parameter can FORMAT_CTRL_15fpsVGA a value between 0x0 and 0x1F
  *     @arg RGB_NORMAL
  *     @arg RGB_565  
  *     @arg RGB_555    
  * @retval None
  */
void DCMI_OV9655_SelectRGBOption(uint8_t OV9665_RGBOption)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, OV9665_RGBOption);
}

/**
  * @brief  Writes a byte at a specific Camera register
  * @param  Device: OV9655 write address.
  * @param  Addr: OV9655 register address. 
  * @param  Data: data to be written to the specific register 
  * @retval None
  */
uint8_t DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data)
{
	I2C_Delay();
	
	I2C_Start();
	I2C_SendByte(Device);
	if(I2C_WaitAck()==0xFF) return(0xFF);
	I2C_SendByte(Addr);
	if(I2C_WaitAck()==0xFF) return(0xFF);
	I2C_SendByte(Data);
	if(I2C_WaitAck()==0xFF) return(0xFF);	
	I2C_Stop();
	
  return(0x00);
}
/**
  * @brief  Reads a byte from a specific Camera register 
  * @param  Device: OV9655 write address.
  * @param  Addr: OV9655 register address. 
  * @retval data read from the specific register 
  */
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr)
{
	unsigned char Recieved_Data=0;
  I2C_Delay();
	I2C_Start();
	I2C_SendByte(Device);
	if(I2C_WaitAck()==0xFF) return(0xFF);
	I2C_SendByte(Addr);
	if(I2C_WaitAck()==0xFF) return(0xFF);
	I2C_Stop();
	I2C_Start();
	I2C_SendByte(Device+1);
  if(I2C_WaitAck()==0xFF) return(0xFF);
	Recieved_Data=I2C_ReceiveByte();
	I2C_Stop();
	return Recieved_Data;
}


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
