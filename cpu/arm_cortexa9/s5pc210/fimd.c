/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PC110 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <s5pc210.h> //JNJ

#define LCD_BGCOLOR		0x1428A0

static unsigned int gFgColor = 0xFF;
static unsigned int gLeftColor = LCD_BGCOLOR;

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)


#if defined(CFG_LCD_LMS700) || defined(CFG_LCD_LTE480WV)
#if defined(CFG_LCD_LMS700)
#define LCD_WIDTH   1024
#define LCD_HEIGHT  600
#else
#define LCD_WIDTH   800
#define LCD_HEIGHT  480
#endif

void LCD_Initialize(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;
//	u32 uFbAddr = 0x42000000;

	u32 i;
	u32* pBuffer = (u32*)uFbAddr;

printf("GPF0CON = 0x%x \r\n",GPF0CON);

	Outp32(GPF0CON, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(GPF0PUD,0x0);		//set pull-up,down disable
	Outp32(GPF1CON, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(GPF1PUD,0x0);		//set pull-up,down disable
	Outp32(GPF2CON, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(GPF2PUD,0x0);		//set pull-up,down disable
	Outp32(GPF3CON, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(GPF3PUD,0x0);		//set pull-up,down disable

	//--------- S5PC210 EVT1 drive strength	---------//
	Outp32(GPF0DRV,0xffffffff);	//set GPF0 drive strength 
	Outp32(GPF1DRV,0xffffffff);	//set GPF1 drive strength 
	Outp32(GPF2DRV,0xffffffff);	//set GPF2 drive strength 
	Outp32(GPF3DRV,0x3ff);		//set GPF3 drive strength 

	Outp32(GPD0CON, 0x11111111);	//set PWM(0-3) as output
	Outp32(GPD0PUD,0x0);		//set pull-up,down disable
	Outp32(GPD0DAT,(1<<0)); 	// PWM 0 high // backlight on

#if 0 //JNJ  
//SPI
	Outp32(GPBCON, 0x11111111);	//set Output
	Outp32(GPBPUD,0x0);		//set pull-up,down disable
	Outp32(GPBDAT,0);		//set Data 0

//I2C
	Outp32(GPD1CON, 0x11111111);	//set Output
	Outp32(GPD1PUD,0x0);		//set pull-up,down disable
	Outp32(GPD1DAT,0);		//set Data 0

// XEINT[0]~XEINT[7]
	Outp32(GPX0CON, 0x11111111);	//set EINT disable
	Outp32(GPX0PUD,0x0);		//set pull-up,down disable
	Outp32(GPX0DAT,0);
	Delay(50);
	Outp32(GPX0DAT,0xffffffff);	//set EINT enable
#endif

printf("ELFIN_LCD_BASE = 0x%x \r\n",ELFIN_LCD_BASE);
	Outp32(ELFIN_LCD_BASE+0x0004, 0x01d3c060);	//VIDCON1
//JNJ	Outp32(ELFIN_LCD_BASE+0x0010, 0x00060400);	//VIDTCON0
//JNJ	Outp32(ELFIN_LCD_BASE+0x0014, 0x000c0702);	//VIDTCON1
	Outp32(ELFIN_LCD_BASE+0x0010, 0x010B0A0A);	//VIDTCON0
	Outp32(ELFIN_LCD_BASE+0x0014, 0x013C241E);	//VIDTCON1

	Outp32(ELFIN_LCD_BASE+0x0170, 0x00000000);	//DITHMODE
//JNJ	Outp32(ELFIN_LCD_BASE+0x0018, 0x000efb1f);	//VIDTCON2
	Outp32(ELFIN_LCD_BASE+0x0018, 0x00095fff);	//VIDTCON2

	Outp32(ELFIN_LCD_BASE+0x0000, 0x0);	//VIDCON0
//JNJ	Outp32(ELFIN_LCD_BASE+0x0000, 0x00000157);	//VIDCON0
	Outp32(ELFIN_LCD_BASE+0x0000, 0x00000143);	//VIDCON0
#if 0
	Outp32(ELFIN_LCD_BASE+0x0130, 0x00001001);	//VIDINTCON0
#endif

	Outp32(ELFIN_LCD_BASE+0x0020, 0x0);	//WINCON0
	Outp32(ELFIN_LCD_BASE+0x0024, 0x0);	//WINCON1
	Outp32(ELFIN_LCD_BASE+0x0028, 0x0);	//WINCON2
	Outp32(ELFIN_LCD_BASE+0x002C, 0x0);	//WINCON3
	Outp32(ELFIN_LCD_BASE+0x0030, 0x0);	//WINCON4
	Outp32(ELFIN_LCD_BASE+0x0034, 0x0);	//SHADOWCON
	Outp32(ELFIN_LCD_BASE+0x0180, 0x0);	//WIN0MAP
	Outp32(ELFIN_LCD_BASE+0x0184, 0x0);	//WIN1MAP
	Outp32(ELFIN_LCD_BASE+0x0188, 0x0);	//WIN2MAP
	Outp32(ELFIN_LCD_BASE+0x018C, 0x0);	//WIN3MAP
	Outp32(ELFIN_LCD_BASE+0x0190, 0x0);	//WIN4MAP
	Outp32(ELFIN_LCD_BASE+0x0140, 0x0);	//W1KEYCON0
	Outp32(ELFIN_LCD_BASE+0x0148, 0x0);	//W2KEYCON0
	Outp32(ELFIN_LCD_BASE+0x0150, 0x0);	//W3KEYCON0
	Outp32(ELFIN_LCD_BASE+0x0158, 0x0);	//W4KEYCON0
	Outp32(ELFIN_LCD_BASE+0x0058, 0x0);	//VIDOSD1C
	Outp32(ELFIN_LCD_BASE+0x0224, 0x0);	//VIDW1ALPHA0
	Outp32(ELFIN_LCD_BASE+0x0228, 0x0);	//VIDW1ALPHA1
	Outp32(ELFIN_LCD_BASE+0x0068, 0x0);	//VIDOSD2C
	Outp32(ELFIN_LCD_BASE+0x022C, 0x0);	//VIDW2ALPHA0
	Outp32(ELFIN_LCD_BASE+0x0230, 0x0);	//VIDW2ALPHA1
	Outp32(ELFIN_LCD_BASE+0x0078, 0x0);	//VIDOSD3C
	Outp32(ELFIN_LCD_BASE+0x0234, 0x0);	//VIDW3ALPHA0
	Outp32(ELFIN_LCD_BASE+0x0238, 0x0);	//VIDW3ALPHA1
	Outp32(ELFIN_LCD_BASE+0x0088, 0x0);	//VIDOSD4C
	Outp32(ELFIN_LCD_BASE+0x023C, 0x0);	//VIDW4ALPHA0
	Outp32(ELFIN_LCD_BASE+0x0240, 0x0);	//VIDW4ALPHA1
	Outp32(ELFIN_LCD_BASE+0x0260, 0x1);	//BLENDCON

	Outp32(ELFIN_LCD_BASE+0x0048, 0x100200);	//VIDOSD0C
	Outp32(ELFIN_LCD_BASE+0x021C, 0xffffff);	//VIDW0ALPHA0
	Outp32(ELFIN_LCD_BASE+0x0220, 0xffffff);	//VIDW0ALPHA1
	Outp32(ELFIN_LCD_BASE+0x0034, 0x0);	//SHADOWCON
	Outp32(ELFIN_LCD_BASE+0x0020, 0x802c);	//WINCON0
	Outp32(ELFIN_LCD_BASE+0x00A0, uFbAddr + 0x00000000);	//VIDW00ADD0B0, window 0 start addr, buffer 0
	Outp32(ELFIN_LCD_BASE+0x00D0, uFbAddr + 0x00400800);	//VIDW00ADD1B0, window 0 end addr, buffer 0
	Outp32(ELFIN_LCD_BASE+0x00A4, uFbAddr + 0x00000000);	//VIDW00ADD0B1, window 0 start addr, buffer 1
	Outp32(ELFIN_LCD_BASE+0x00D4, uFbAddr + 0x00400800);	//VIDW00ADD1B1, window 0 end addr, buffer 1
	Outp32(ELFIN_LCD_BASE+0x20A0, uFbAddr + 0x00000000);	//VIDW00ADD0B2, window 0 start addr, buffer 2
	Outp32(ELFIN_LCD_BASE+0x20D0, uFbAddr + 0x00400800);	//VIDW00ADD1B2, window 0 end addr, buffer 2
	Outp32(ELFIN_LCD_BASE+0x0100, 0x1558);	//VIDW00ADD2, window0 buffer size
	Outp32(ELFIN_LCD_BASE+0x0040, 0x0);	//VIDOSD0A
	Outp32(ELFIN_LCD_BASE+0x0044, 0x2aaaff);	//VIDOSD0B
	Outp32(ELFIN_LCD_BASE+0x0020, 0x802d);	//WINCON0
	Outp32(ELFIN_LCD_BASE+0x0034, 0x1);	//SHADOWCON
	Outp32(ELFIN_LCD_BASE+0x0020, 0x802d);	//WINCON0
	Outp32(ELFIN_LCD_BASE+0x0034, 0x1);	//SHADOWCON
//JNJ	Outp32(ELFIN_LCD_BASE+0x0000, 0x153); 	//VIDCON0 
	Outp32(ELFIN_LCD_BASE+0x0000, 0x143); 	//VIDCON0 
//JNJ	Outp32(ELFIN_LCD_BASE+0x0010, 0x60400);	//VIDTCON0
	Outp32(ELFIN_LCD_BASE+0x0010, 0x010B0A0A);	//VIDTCON0
	Outp32(ELFIN_LCD_BASE+0x01A4, 0x3);	//TRIGCON

	Outp32(0x10010210,0x0C800200); 	//DISPLAY_CONTROL, reset value,i80 
	Outp32(0x1003C234,0x00000006); //CLK_SRC_LCD0, syscon fimdclk = mpll
}

#endif

/*
 */
void LCD_turnon(void)
{
#if defined(CFG_LCD_LMS700) || defined(CFG_LCD_LTE480WV)
	LCD_Initialize();
#endif
}

void LCD_turnoff(void)
{
	u32 gpio;

// Backlight off
	// GPD0(0) output, PWM0
	gpio = Inp32(GPD0CON);
	gpio &= 0xFFF0;
	gpio |= 0x1;
	Outp32(GPD0CON,gpio); 

	// GPD0(0) DAT
	gpio = 0x0;
	Outp32(GPD0DAT,gpio); 
}

void LCD_setfgcolor(unsigned int color)
{
	gFgColor = color;
}

void LCD_setleftcolor(unsigned int color)
{
	gLeftColor = color;
}


#define BMP_LOGO_RAM_ADDR   0xC4000000
#define BMP_LOGO_LENGTH     0x200000

int LCD_ReadBmpLogo(void)
{
	movi_read_logo(BMP_LOGO_RAM_ADDR);	// bmp image load ddram from sd/mmc memory
}


#define BMP_ROW 800
#define BMP_COL 480

void ConvertBmpToRgb24bpp(u32 fromAddr, u32 toAddr, u32 xSize, u32 ySize)
{
  u32 i,x,y;
  u32 *lcdFramePtr,*tempPtr;
  u32 temp[800*480*4];
  u32 tempBuffer = fromAddr + 0x200000;
  u32 *tempBuffer1;
  u8 b,g,r;
  u8 *srcPtr=(u8 *)(fromAddr+54);

  lcdFramePtr=(u32 *)(toAddr);
  tempBuffer1=(u32 *)(fromAddr + 0x400000);
  for(y=0;y<xSize*ySize;y++) {
#if 0 //JNJ
    b=*srcPtr++;
    g=*srcPtr++;
    r=*srcPtr++;
#else
    b=0;
    g=0;
    r=0xff;
#endif

    *tempBuffer1++=(r<<16)|(g<<8)|(b<<0);
  }

  tempBuffer1=(u32 *)((fromAddr + 0x400000)+xSize*ySize*4-4);
  tempPtr=(u32 *)(tempBuffer);
  for(y=0;y<xSize*ySize;y++) {
    *tempPtr++=*tempBuffer1--;
  }

  lcdFramePtr=(u32 *)(toAddr);
  tempPtr=(u32 *)(tempBuffer);
  for(y=0;y<ySize;y++) {
    for(x=0;x<xSize;x++) {
      temp[(xSize-1)-x]=*tempPtr++;
    }
    if(x==xSize) {        
      for(i=0;i<xSize;i++)
        *lcdFramePtr++=temp[i];
    }
  } 
}

void InitializeDisplay(void)
{
#if 0 //JNJ
  LCD_ReadBmpLogo();
  ConvertBmpToRgb24bpp(BMP_LOGO_RAM_ADDR, CFG_LCD_FBUFFER, 800, 480);
  LCD_turnon();
#else
  LCD_turnoff();
#endif
}
