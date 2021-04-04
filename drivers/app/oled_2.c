/*******************************************************************************
  OLED Library

Converter : Jee Hwn SONG (2008/07/26)
Target :  STM32F10x
Compiler: IAR ARM 5.xx (EWARM)
*******************************************************************************/

#include "oled1.h"

const unsigned char gamma[] = {
	0x01,0x04,0x07,0x09,
	0x0B,0x0C,0x0D,0x0E,
	0x0F,0x10,0x11,0x12,
	0x13,0x16,0x1d,0x21,
	0x23,0x24,0x2E,0x31,
	0x32,0x37,0x41,0x43,
	0x4c,0x51,0x54,0x61,
	0x68,0x6f,0x75,0x7f,//
	
	0x01,0x08,0x0E,0x13,
	0x17,0x1a,0x1c,0x1e,
	0x1f,0x20,0x21,0x22,
	0x23,0x25,0x2b,0x30,
	0x31,0x33,0x37,0x39,
	0x41,0x43,0x47,0x50,
	0x52,0x55,0x61,0x63,
	0x64,0x70,0x74,0x7f,//

	0x01,0x04,0x07,0x09,
	0x0B,0x0C,0x0D,0x0E,
	0x0F,0x10,0x11,0x12,
	0x13,0x16,0x1E,0x21,
	0x22,0x25,0x2d,0x30,
	0x32,0x37,0x41,0x43,
	0x4c,0x51,0x56,0x62,
	0x69,0x70,0x77,0x7f
};

void Oled_delay(unsigned int delay){
  while(delay--); 
}

//==================================================
//==================================================
void SetAddr(unsigned char x, unsigned char y)
{
  
	write_cmd(SET_COLUMN_ADDRESS); //colum 주소 
	write_data(x); 
	write_data(0x7f);
	
	
	write_cmd(SET_ROW_ADDRESS);
	write_data(y);
	write_data(0x7f);
	
	write_cmd(WRITE_GRAM);
}

//==================================================
// Delay ms
//==================================================
void DelayMs(unsigned int delay)
{	
    delay_ms(delay);  
}


//==================================================
// Plot one point
// at x,y with pixel color
//==================================================
void PutPixel(unsigned char x, unsigned char y, unsigned int color)
{
	SetAddr(x,y);
	write_data(color >> 8);
	write_data(color);

}

//======================================================================//
void initOLED(void)
{		
      GPIO_InitTypeDef GPIO_InitStructure;
      
      RCC_APB2PeriphClockCmd(OLED_CTRL_DDR | OLED_DATA_DDR, ENABLE);
    
      GPIO_InitStructure.GPIO_Pin = CS_OLED | RESETPIN_OLED | D_C | WN_OLED|RN_OLED;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(OLED_CTRL_PORT, &GPIO_InitStructure);
      
           
      GPIO_InitStructure.GPIO_Pin = 0x00FF;
      GPIO_Init(OLED_DATABUS, &GPIO_InitStructure);
      
      GPIO_SetBits(OLED_CTRL_PORT,D_C);	//데이타/제어 핀
      GPIO_SetBits(OLED_CTRL_PORT,WN_OLED);// MCU interface input
      GPIO_SetBits(OLED_CTRL_PORT,CS_OLED);////chip select -> low => mcu select
      GPIO_SetBits(OLED_CTRL_PORT,RN_OLED);
      GPIO_SetBits(OLED_CTRL_PORT,RESETPIN_OLED);
      
      GPIO_Write(OLED_DATABUS,0x00FF);   
      
	Reset_SSD1355();
	
	write_cmd(SOFT_RESET); 
	
	write_cmd(COMMAD_LOCK); 
	write_data(0x12); 

	write_cmd(COMMAD_LOCK); 
	write_data(0xB3); 
	
	write_cmd(SLEEP_MODE_ON);

	write_cmd(CLOCK_FREQUENCY); 
	write_data(0x10);
	
	write_cmd(SET_MUX_RATIO); 
	write_data(0x7f);  //127

	write_cmd(SET_DISPLAY_OFFSET); 
	write_data(0x00); 

	write_cmd(MEMORY_ACCSEE_CNTL); 
	write_data(0x88); 
	write_data(0x01); 

	write_cmd(INTERFACE_PIXEL_FORMAT); 
	write_data(_65K_COLOURS); 
	
	write_cmd(EN_T_EFFECT); 
	write_data(0x00); 

	write_cmd(FUNC_SEL); 
	write_data(0x03); 

	write_cmd(CONTRAST_RGB); 
	write_data(0xc3); 
	write_data(0x55); 
	write_data(0x87); 
	
	write_cmd(WRITE_LUMINANCE); 
	write_data(0xF0); 
	
	write_cmd(GAMMA_LUT); 		
	
	u8 j;
	for(j=0;j<96;j++)
		write_data(gamma[j]);		
	
	write_cmd(SET_PHASE_LENGTH); 
	write_data(0x32); 

	write_cmd(FIRST_PRECHARGE); 
	write_data(0x09); 

	write_cmd(SECOND_PRECHARGE_PERIOD); 
	write_data(0x0b); 

	write_cmd(SET_2TH_PRECHARGE_SPEED); 
	write_data(0x03); 

	write_cmd(SET_VCOMH); 
	write_data(0x04); 

	write_cmd(DISPLAY_ALL_ONOFF); 

	write_cmd(DISPLAY_INVERSE_OFF); 
	
	write_cmd(DISPLAY_NORMAL); 

	write_cmd(SLEEP_MODE_OFF); 


}

//======================================================================//
//======================================================================//
void Reset_SSD1355(void)
{
	DelayMs(1);
	GPIO_ResetBits(OLED_CTRL_PORT,RESETPIN_OLED);		// IOCLR=bRES;
	DelayMs(2);
	GPIO_SetBits(OLED_CTRL_PORT,RESETPIN_OLED);		// IOSET=bRES;
	DelayMs(1);
}


//======================================================================//
//======================================================================//
void write_cmd(unsigned char cmd)
{
	GPIO_ResetBits(OLED_CTRL_PORT,D_C);				// IOCLR=bD_C;
	GPIO_ResetBits(OLED_CTRL_PORT,CS_OLED);			// IOCLR=bCS;
	GPIO_ResetBits(OLED_CTRL_PORT,WN_OLED);			// IOCLR=bR_W;

							// IOCLR=0x000000ff;
//	OLED_DATABUS = cmd;		// IOSET=cmd;
	GPIO_Write(OLED_DATABUS,cmd);
        asm("nop");

	GPIO_SetBits(OLED_CTRL_PORT,WN_OLED);			// IOSET=bR_W;
	GPIO_SetBits(OLED_CTRL_PORT,CS_OLED);			// IOSET=bCS;
	GPIO_SetBits(OLED_CTRL_PORT,D_C);				// IOSET=bD_C;

}

//======================================================================//
//======================================================================//
void write_data(unsigned char data)
{

	GPIO_SetBits(OLED_CTRL_PORT,D_C);				// IOSET=bD_C;
	GPIO_ResetBits(OLED_CTRL_PORT,CS_OLED);			// IOCLR=bCS;
	GPIO_ResetBits(OLED_CTRL_PORT,WN_OLED);			// IOCLR=bR_W;

							// IOCLR=0x000000ff;
	//OLED_DATABUS = data;	// IOSET=data;
        GPIO_Write(OLED_DATABUS,data);
	asm("nop");
	GPIO_ResetBits(OLED_CTRL_PORT,D_C);				// IOCLR=bD_C;
	GPIO_SetBits(OLED_CTRL_PORT,WN_OLED);			// IOSET=bR_W;
	GPIO_SetBits(OLED_CTRL_PORT,CS_OLED);			// IOSET=bCS;

}



void Draw_Rectanle(u8 sx,u8 sy,u8 ex, u8 ey,u16 outColor,u16 inColor){

	if(sx==ex && sy == ey ) return;
	
	u8 x,y;

	u8 first_x = (sx<ex?sx:ex)&0x7F;
	u8 first_y = (sx<ex?sy:ey)&0x7F;

	u8 end_x		= (sx>ex?sx:ex)&0x7F;
	u8 end_y		= (sx>ex?sy:ey)&0x7F;

	for(y=first_y;y<=end_y;y++) //내부 사각형 그리기
	{
		SetAddr(first_x,y);

		for(x=first_x;x<=end_x;x++)
		{
			write_data(inColor>>8);
			write_data(inColor&0xFF);			
		}	
	}	

	Draw_Line(first_x,first_y,end_x,first_y,outColor);//-
	Draw_Line(end_x,first_y,end_x,end_y,outColor); //   |
	Draw_Line(first_x,first_y,first_x,end_y,outColor); // | 
	Draw_Line(first_x,end_y,end_x,end_y,outColor); // _
}

void Draw_Line(u8 sx,u8 sy,u8 ex, u8 ey, u16 color){

	if(sx==ex && sy == ey ) return;
	
	u8 x;

	u8 first_x = (sx<ex?sx:ex)&0x7F;
	u8 first_y = (sx<ex?sy:ey)&0x7F;

	u8 end_x		= (sx>ex?sx:ex)&0x7F;
	u8 end_y		= (sx>ex?sy:ey)&0x7F;
	
	float gradient = (end_y-first_y)/(end_x-first_x); 
	
	double current_y = 0;
	
	if(sx==ex)
	{
		 first_x = sy;
		 end_x =   ey; 
	
		 for(x=first_x;x<=end_x;x++)
		{			
			PutPixel(sx,x,color);

		}
	 }

	else
	{
		for(x=first_x;x<=end_x;x++)
		{
			current_y = (gradient*(x-first_x)+first_y);
			PutPixel(x,(u8)current_y,color);

		}
	}	
	
}
void Draw_Circle(u8 x,u8 y,u8 rad, u16 inColor,u16 outColor){

	if((x+rad > 127 ) || (y+rad > 127 ) || (x-rad < 0 ) ||(y-rad < 0 ))
		return ;
	
	u8 i=1,j;
	
	char sx = x&0x7F;
	char sy = y&0x7F;	
	
	u16 radius = rad * rad;	

	for(i=0;i<=rad;i++)
	{
		j = (u8)(sqrt(radius-i*i)+0.5);		
	
		Draw_Line(sx+i,sy-j,sx+i,sy+j,inColor);
		Draw_Line(sx-i,sy-j,sx-i,sy+j,inColor);
	
	/*	PutPixel(x+i,y+j,outColor);//1
		PutPixel(x+i,y-j,outColor);//4
		PutPixel(x-i,y+j,outColor);//2
		PutPixel(x-i,y-j,outColor);//3
	*/		
	} 

}

