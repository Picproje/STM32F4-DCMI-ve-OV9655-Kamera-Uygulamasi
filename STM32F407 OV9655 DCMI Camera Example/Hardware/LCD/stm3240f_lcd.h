
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3240F_EVAL_LCD_H
#define __STM3240F_EVAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "fonts.h"

typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;   

#ifdef USE_Delay
#include "main.h" 
  #define _delay_     Delay  /* !< User can provide more timing precise _delay_ function
                                   (with 10ms time base), using SysTick for example */
#else
  #define _delay_     delay      /* !< Default _delay_ function with less precise timing */
#endif
 
/** 
  * @brief  LCD Registers  
  */ 
#define LCD_REG_0             0x00
#define LCD_REG_1             0x01
#define LCD_REG_2             0x02
#define LCD_REG_3             0x03
#define LCD_REG_4             0x04
#define LCD_REG_5             0x05
#define LCD_REG_6             0x06
#define LCD_REG_7             0x07
#define LCD_REG_8             0x08
#define LCD_REG_9             0x09
#define LCD_REG_10            0x0A
#define LCD_REG_12            0x0C
#define LCD_REG_13            0x0D
#define LCD_REG_14            0x0E
#define LCD_REG_15            0x0F
#define LCD_REG_16            0x10
#define LCD_REG_17            0x11
#define LCD_REG_18            0x12
#define LCD_REG_19            0x13
#define LCD_REG_20            0x14
#define LCD_REG_21            0x15
#define LCD_REG_22            0x16
#define LCD_REG_23            0x17
#define LCD_REG_24            0x18
#define LCD_REG_25            0x19
#define LCD_REG_26            0x1A
#define LCD_REG_27            0x1B
#define LCD_REG_28            0x1C
#define LCD_REG_29            0x1D
#define LCD_REG_30            0x1E
#define LCD_REG_31            0x1F
#define LCD_REG_32            0x20
#define LCD_REG_33            0x21
#define LCD_REG_34            0x22
#define LCD_REG_36            0x24
#define LCD_REG_37            0x25
#define LCD_REG_40            0x28
#define LCD_REG_41            0x29
#define LCD_REG_43            0x2B
#define LCD_REG_45            0x2D
#define LCD_REG_48            0x30
#define LCD_REG_49            0x31
#define LCD_REG_50            0x32
#define LCD_REG_51            0x33
#define LCD_REG_52            0x34
#define LCD_REG_53            0x35
#define LCD_REG_54            0x36
#define LCD_REG_55            0x37
#define LCD_REG_56            0x38
#define LCD_REG_57            0x39
#define LCD_REG_58            0x3A
#define LCD_REG_59            0x3B
#define LCD_REG_60            0x3C
#define LCD_REG_61            0x3D
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_80            0x50
#define LCD_REG_81            0x51
#define LCD_REG_82            0x52
#define LCD_REG_83            0x53
#define LCD_REG_96            0x60
#define LCD_REG_97            0x61
#define LCD_REG_106           0x6A
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_229           0xE5

/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

/** 
  * @brief  LCD Lines depending on the chosen fonts.  
  */
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT         Font16x24

/** 
  * @brief  LCD Direction  
  */ 
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/** 
  * @brief  LCD Size (Width and Height)  
  */ 
#define LCD_PIXEL_WIDTH          0x0140
#define LCD_PIXEL_HEIGHT         0x00F0


#define LCD_RSNWR_GPIO_CLK  LCD_NWR_GPIO_CLK
#define LCD_SPI_GPIO_PORT   LCD_SPI_SCK_GPIO_PORT
#define LCD_SPI_GPIO_CLK    LCD_SPI_SCK_GPIO_CLK
#define R0                  LCD_REG_0
#define R1                  LCD_REG_1
#define R2                  LCD_REG_2
#define R3                  LCD_REG_3
#define R4                  LCD_REG_4
#define R5                  LCD_REG_5
#define R6                  LCD_REG_6
#define R7                  LCD_REG_7
#define R8                  LCD_REG_8
#define R9                  LCD_REG_9
#define R10                 LCD_REG_10
#define R12                 LCD_REG_12
#define R13                 LCD_REG_13
#define R14                 LCD_REG_14
#define R15                 LCD_REG_15
#define R16                 LCD_REG_16
#define R17                 LCD_REG_17
#define R18                 LCD_REG_18
#define R19                 LCD_REG_19
#define R20                 LCD_REG_20
#define R21                 LCD_REG_21
#define R22                 LCD_REG_22
#define R23                 LCD_REG_23
#define R24                 LCD_REG_24
#define R25                 LCD_REG_25
#define R26                 LCD_REG_26
#define R27                 LCD_REG_27
#define R28                 LCD_REG_28
#define R29                 LCD_REG_29
#define R30                 LCD_REG_30
#define R31                 LCD_REG_31
#define R32                 LCD_REG_32
#define R33                 LCD_REG_33
#define R34                 LCD_REG_34
#define R36                 LCD_REG_36
#define R37                 LCD_REG_37
#define R40                 LCD_REG_40
#define R41                 LCD_REG_41
#define R43                 LCD_REG_43
#define R45                 LCD_REG_45
#define R48                 LCD_REG_48
#define R49                 LCD_REG_49
#define R50                 LCD_REG_50
#define R51                 LCD_REG_51
#define R52                 LCD_REG_52
#define R53                 LCD_REG_53
#define R54                 LCD_REG_54
#define R55                 LCD_REG_55
#define R56                 LCD_REG_56
#define R57                 LCD_REG_57
#define R59                 LCD_REG_59
#define R60                 LCD_REG_60
#define R61                 LCD_REG_61
#define R62                 LCD_REG_62
#define R63                 LCD_REG_63
#define R64                 LCD_REG_64
#define R65                 LCD_REG_65
#define R66                 LCD_REG_66
#define R67                 LCD_REG_67
#define R68                 LCD_REG_68
#define R69                 LCD_REG_69
#define R70                 LCD_REG_70
#define R71                 LCD_REG_71
#define R72                 LCD_REG_72
#define R73                 LCD_REG_73
#define R74                 LCD_REG_74
#define R75                 LCD_REG_75
#define R76                 LCD_REG_76
#define R77                 LCD_REG_77
#define R78                 LCD_REG_78
#define R79                 LCD_REG_79
#define R80                 LCD_REG_80
#define R81                 LCD_REG_81
#define R82                 LCD_REG_82
#define R83                 LCD_REG_83
#define R96                 LCD_REG_96
#define R97                 LCD_REG_97
#define R106                LCD_REG_106
#define R118                LCD_REG_118
#define R128                LCD_REG_128
#define R129                LCD_REG_129
#define R130                LCD_REG_130
#define R131                LCD_REG_131
#define R132                LCD_REG_132
#define R133                LCD_REG_133
#define R134                LCD_REG_134
#define R135                LCD_REG_135
#define R136                LCD_REG_136
#define R137                LCD_REG_137
#define R139                LCD_REG_139
#define R140                LCD_REG_140
#define R141                LCD_REG_141
#define R143                LCD_REG_143
#define R144                LCD_REG_144
#define R145                LCD_REG_145
#define R146                LCD_REG_146
#define R147                LCD_REG_147
#define R148                LCD_REG_148
#define R149                LCD_REG_149
#define R150                LCD_REG_150
#define R151                LCD_REG_151
#define R152                LCD_REG_152
#define R153                LCD_REG_153
#define R154                LCD_REG_154
#define R157                LCD_REG_157
#define R192                LCD_REG_192
#define R193                LCD_REG_193
#define R227                LCD_REG_227
#define R229                LCD_REG_229
#define R231                LCD_REG_231
#define R239                LCD_REG_239
#define White               LCD_COLOR_WHITE
#define Black               LCD_COLOR_BLACK
#define Grey                LCD_COLOR_GREY
#define Blue                LCD_COLOR_BLUE
#define Blue2               LCD_COLOR_BLUE2
#define Red                 LCD_COLOR_RED
#define Magenta             LCD_COLOR_MAGENTA
#define Green               LCD_COLOR_GREEN
#define Cyan                LCD_COLOR_CYAN
#define Yellow              LCD_COLOR_YELLOW
#define Line0               LCD_LINE_0
#define Line1               LCD_LINE_1
#define Line2               LCD_LINE_2
#define Line3               LCD_LINE_3
#define Line4               LCD_LINE_4
#define Line5               LCD_LINE_5
#define Line6               LCD_LINE_6
#define Line7               LCD_LINE_7
#define Line8               LCD_LINE_8
#define Line9               LCD_LINE_9
#define Horizontal          LCD_DIR_HORIZONTAL
#define Vertical            LCD_DIR_VERTICAL


#define ASSEMBLE_RGB(R ,G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3)) 

    
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
void LCD_WriteBMP2(uint16_t Xpos, uint8_t Ypos, uint16_t Width, uint16_t Height, unsigned char *bitmap);
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
   
#ifdef __cplusplus
}
#endif

#endif 
