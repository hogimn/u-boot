#include <s5pc210.h>
#include "./fpga_io.h"

#define mdelay(n) ({unsigned long msec=(n); while (msec--) udelay(1000);})

int fullcolorled_test(void)
{

	unsigned short * full_led1_addr[3];
	unsigned short * full_led2_addr[3];
	unsigned short * full_led3_addr[3];
	unsigned short * full_led4_addr[3];
	int iRed=100, iBlue=0, iGreen=100,i;

	full_led1_addr[0] = (unsigned short *)ADDR_OF_FULL_LED1_RED;
	full_led1_addr[1] = (unsigned short *)ADDR_OF_FULL_LED1_GREEN;
	full_led1_addr[2] = (unsigned short *)ADDR_OF_FULL_LED1_BLUE;

	full_led2_addr[0] = (unsigned short *)ADDR_OF_FULL_LED2_RED;
	full_led2_addr[1] = (unsigned short *)ADDR_OF_FULL_LED2_GREEN;
	full_led2_addr[2] = (unsigned short *)ADDR_OF_FULL_LED2_BLUE;

	full_led3_addr[0] = (unsigned short *)ADDR_OF_FULL_LED3_RED;
	full_led3_addr[1] = (unsigned short *)ADDR_OF_FULL_LED3_GREEN;
	full_led3_addr[2] = (unsigned short *)ADDR_OF_FULL_LED3_BLUE;

	full_led4_addr[0] = (unsigned short *)ADDR_OF_FULL_LED4_RED;
	full_led4_addr[1] = (unsigned short *)ADDR_OF_FULL_LED4_GREEN;
	full_led4_addr[2] = (unsigned short *)ADDR_OF_FULL_LED4_BLUE;

/*
	*full_led1_addr[0]=0x00;
	*full_led2_addr[0]=0x50;
	*full_led3_addr[0]=0x00;
	*full_led3_addr[0]=0x50;

	*full_led1_addr[1]=0x50;
	*full_led2_addr[1]=0x00;
	*full_led3_addr[1]=0x00;
	*full_led3_addr[1]=0x50;

	*full_led1_addr[2]=0x00;
	*full_led2_addr[2]=0x00;
	*full_led3_addr[2]=0x50;
	*full_led3_addr[2]=0x50;
	*/
	for(i =0; i <100; i++)
	{

		*full_led1_addr[0]=iRed;
		*full_led2_addr[0]=iRed;
		*full_led3_addr[0]=iRed;
		*full_led4_addr[0]=iRed;

		*full_led1_addr[1]=iBlue;
		*full_led2_addr[1]=iBlue;
		*full_led3_addr[1]=iBlue;
		*full_led4_addr[1]=iBlue;

		*full_led1_addr[2]=iGreen;
		*full_led2_addr[2]=iGreen;
		*full_led3_addr[2]=iGreen;
		*full_led4_addr[2]=iGreen;

		iRed--;
		iBlue++;
		iGreen--;
		mdelay(100);

	}
	
	return 0;
}
