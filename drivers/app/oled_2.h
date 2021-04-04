/*******************************************************************************
  OLED Library

Converter : Jee Hwn SONG (2008/07/26)
Target :  STM32F10x
Compiler: IAR ARM 5.xx (EWARM)
*******************************************************************************/

#ifndef OLED_H
#define OLED_H

#include "stm32f10x_lib.h"
#include"user_delay.h"
#include<math.h>

//--------------------------------------------------------------------------//
// General definitions
//--------------------------------------------------------------------------//
#ifndef uint
  #define	uint  unsigned int
#endif

#ifndef uchar
  #define uchar unsigned char
#endif


//------------------------------------/*--------------------------------------//
// OLED Interface
//--------------------------------------------------------------------------//
#define	CS_OLED			GPIO_Pin_0		// OLED CS
#define	RESETPIN_OLED	        GPIO_Pin_1
#define	D_C			GPIO_Pin_2
#define WN_OLED			GPIO_Pin_3		// OLED WN : R/W#(WR#)
#define	RN_OLED			GPIO_Pin_4		// OLED RN : E(RD#)


#define OLED_CTRL_PORT	GPIOC
#define	OLED_DATABUS	GPIOA

#define OLED_CTRL_DDR	RCC_APB2Periph_GPIOC
#define	OLED_DATA_DDR	RCC_APB2Periph_GPIOA


//--------------------------------------------------------------------------//
// SSD1355 Registers
//--------------------------------------------------------------------------//
#define	SET_COLUMN_ADDRESS		0x2A
#define	SET_ROW_ADDRESS			0x2B
#define	WRITE_GRAM			0x2C
#define MEMORY_ACCSEE_CNTL		0x36
#define	INTERFACE_PIXEL_FORMAT		0x3A
#define EN_T_EFFECT			0x35
#define	SET_MUX_RATIO			0xCA
#define	SET_DISPLAY_OFFSET		0xC8
#define FUNC_SEL			0xB3
#define	CLOCK_FREQUENCY			0xD2

#define	CONTRAST_RGB			0xBA
#define WRITE_LUMINANCE			0x51

#define	DISPLAY_ALL_ONOFF		0x29
#define	DISPLAY_INVERSE_OFF		0x20
#define	DISPLAY_NORMAL			0x13

#define	GAMMA_LUT			0xBE

#define	SET_PHASE_LENGTH		0xCD
#define	FIRST_PRECHARGE			0xBD
#define	SECOND_PRECHARGE_PERIOD		0xCE

#define	SET_2TH_PRECHARGE_SPEED		0xCE
#define	SET_VCOMH			0xD3

#define	SLEEP_MODE_ON			0x10
#define	SLEEP_MODE_OFF			0x11

#define COMMAD_LOCK	0xFD
#define SOFT_RESET	0x01

#define	_65K_COLOURS			0x05
#define	_262K_COLOURS			0x06




//--------------------------------------------------------------------------//
// Screen Related definitions
//--------------------------------------------------------------------------//
#define X_RES				128
#define Y_RES				128

#define MAXCOLOUR			0xFFFF
#define	MAX_TEXTROWS		25

#define BLACK				0x0000
#define WHITE				0xFFFF
#define	RED					0xF800
#define	GREEN				0x07E0
#define	BLUE				0x001F
#define	YELLOW				RED | GREEN

#define RGB565(r,g,b) ( (((r)>>3)<<11) | (((g)>>2)<<5) | ((b)>>3) )
/****************************************************************************/
/*          					VARIABLES	                 				*/
/****************************************************************************/




/****************************************************************************/
/*          				FUNCTION PROTOTYPES                				*/
/****************************************************************************/

void initOLED(void);
void SetAddr(unsigned char x, unsigned char y);
void PutPixel(unsigned char x, unsigned char y, unsigned int color);

void Reset_SSD1355(void);
void write_cmd(unsigned char);
void write_data(unsigned char);

void delay_1us(unsigned char u);
void DelayMs(unsigned int);


//3.13 New Fucn
void Draw_Line(u8 sx,u8 sy,u8 ex, u8 ey, u16 color);
void Draw_Rectanle(u8 sx,u8 sy,u8 ex, u8 ey,u16 outColor,u16 inColor);
void Draw_Circle(u8 x,u8 y,u8 rad, u16 inColor,u16 outColor);


#endif
