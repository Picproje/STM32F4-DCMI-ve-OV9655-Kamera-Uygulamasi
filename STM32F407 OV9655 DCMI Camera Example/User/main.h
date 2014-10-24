/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lis302dl.h"
#include "stm3240f_lcd.h"
#include "OV9655.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
extern void Delay(__IO uint32_t nTime);
#define     __IO    volatile             /*!< defines 'read / write' permissions              */
//static __IO uint32_t TimingDelay;

#endif   /* __MAIN*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
