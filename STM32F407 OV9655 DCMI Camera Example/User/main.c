/**
  ******************************************************************************
  * @file    STM32F407 Test Yazilimi/main.c 
  * @author  Ferhat YOL
  * @version V1.0.0
  * @date    18-Kasim-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
  #include "main.h"
	#include "I2C.h"
/** @addtogroup OV9655_Camera_Test
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
char data[20];
unsigned char temp=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main()
{
	SystemInit();
	if (SysTick_Config(SystemCoreClock / 3119))//1ms 
  { 
    /* Capture error */ 
    while (1);
  }	

  STM_EVAL_LEDInit(LED3);

  STM3240F_LCD_Init();  	
	LCD_Clear(Black);
	LCD_SetColors(Green,Black);
	LCD_SetFont(&Font8x8);

    if(OV9655_Configuration()==0xFF) 
		{
	     LCD_DisplayStringLine(0,0,"Camera Failed");
    }else{
       LCD_DisplayStringLine(0,0,"Camera OK!");
    }
		/* Start Image capture */ 

	LCD_SetDisplayWindow(0, 0, 319, 239);
  LCD_WriteRAM_Prepare();
  DCMI_CaptureCmd(ENABLE); 
  /* Enable DMA transfer */
  DMA_Cmd(DMA2_Stream1, ENABLE);
    /* Enable DCMI interface */
  DCMI_Cmd(ENABLE); 
    /* Start Image capture */ 
  DCMI_CaptureCmd(ENABLE);   
//  Delay(2);
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8d, 0x10);//Camera Color Test Mode Enabled
//    temp=DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS,OV9655_COM15);
//		sprintf(data,"Okunan Data=0x%02X",temp);
//		LCD_DisplayStringLine(0,0,data);
	while(1)
	{ 	
		 
 		 
 	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
