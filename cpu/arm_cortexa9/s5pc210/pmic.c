#include <common.h>
#include <common.h>
#include <asm/io.h>

#include "pmic.h"
#include "gpio.h"
#include "i2c.h"

const PMIC_Type_st gVoltage_Type[3] = 
{
#if 0 		
	{"VDD_ARM",	MAX8952_ADDR,	I2C0},	
	{"VDD_INT",	MAX8649_ADDR,	I2C1},
	{"VDD_G3D",	MAX8649A_ADDR,	I2C0},
#else
	{"VDD_ARM",	MAX8997_ADDR,	I2C0},	
	{"VDD_INT",	MAX8997_ADDR,	I2C0},
	{"VDD_G3D",	MAX8997_ADDR,	I2C0},
#endif
};

void GPIO_Reset_A(void)
{
	// RESET
	GPIO_SetFunctionEach(eGPIO_X1, eGPIO_2, eGPO); // XEINT[5]
	GPIO_SetPullUpDownEach(eGPIO_X1, eGPIO_2, eGPUDdis); // XEINT[5]	

	GPIO_SetDataEach(eGPIO_X1, eGPIO_2, 0); // XEINT[5]
	GPIO_SetDataEach(eGPIO_X1, eGPIO_2, 1); // XEINT[5]

}
void GPIO_Reset_B(void)
{
	// RESET
	GPIO_SetFunctionEach(eGPIO_X1, eGPIO_0, eGPO); // XEINT[5]
	GPIO_SetPullUpDownEach(eGPIO_X1, eGPIO_0, eGPUDdis); // XEINT[5]	
	
	GPIO_SetDataEach(eGPIO_X1, eGPIO_0, 0); // XEINT[5] 
	GPIO_SetDataEach(eGPIO_X1, eGPIO_0, 1); // XEINT[5]

}
void PMIC_InitIp(void)
{
	float vdd_arm, vdd_int, vdd_g3d;

	GPIO_Init();
	GPIO_SetFunctionEach(eGPIO_D1, eGPIO_0, 2);
	GPIO_SetFunctionEach(eGPIO_D1, eGPIO_1, 2);

	GPIO_SetDSEach(eGPIO_D1,eGPIO_0,3);
	GPIO_SetDSEach(eGPIO_D1,eGPIO_1,3);

	GPIO_SetPullUpDownEach(eGPIO_D1, eGPIO_0, 0);
	GPIO_SetPullUpDownEach(eGPIO_D1, eGPIO_1, 0);

	I2C_InitIp(I2C0, I2C_TX_CLOCK_125KHZ, I2C_TIMEOUT_INFINITY);

	GPIO_SetFunctionEach(eGPIO_D1, eGPIO_2, 2);	// GPIO I2C1 setting
	GPIO_SetFunctionEach(eGPIO_D1, eGPIO_3, 2);	// GPIO I2C1 setting

	GPIO_SetDSEach(eGPIO_D1,eGPIO_2,3);
	GPIO_SetDSEach(eGPIO_D1,eGPIO_3,3);
	GPIO_SetPullUpDownEach(eGPIO_D1, eGPIO_2, 0);	// Pull-Up/Down Disable
	GPIO_SetPullUpDownEach(eGPIO_D1, eGPIO_3, 0);	// Pull-Up/Down Disable
	if (I2C_InitIp(I2C1, I2C_TX_CLOCK_125KHZ, I2C_TIMEOUT_INFINITY) != 1) {
		printf("PMIC init filed!\n");
	}
	
	GPIO_Reset_A();
	GPIO_Reset_B();


	vdd_arm = CONFIG_PM_VDD_ARM;
	vdd_int = CONFIG_PM_VDD_INT;
	vdd_g3d = CONFIG_PM_VDD_G3D;

	//printf("ARM %01.1fV, INT %01.1fV, G3D %01.1f\n", vdd_arm, vdd_int, vdd_g3d);
	printf("ARM %1d.%2dV, INT %1d.%2dV, G3D %1d.%2dV\n", 
		(unsigned int)vdd_arm, (unsigned int)(vdd_arm * 100) % 100, 
		(unsigned int)vdd_int, (unsigned int)(vdd_int * 100) % 100,
		(unsigned int)vdd_g3d, (unsigned int)(vdd_g3d * 100) % 100);
	
	PMIC_SetVoltage_Buck(eVDD_ARM, eVID_MODE3, vdd_arm);
	PMIC_SetVoltage_Buck(eVDD_INT, eVID_MODE2, vdd_int);
	PMIC_SetVoltage_Buck(eVDD_G3D, eVID_MODE2, vdd_g3d);

	// lcd backlight off
	GPIO_SetFunctionEach(eGPIO_D0, eGPIO_0, eGPO);
	GPIO_SetFunctionEach(eGPIO_D0, eGPIO_1, eGPO);

	GPIO_SetPullUpDownEach(eGPIO_D0, eGPIO_0, eGPUDdis);
	GPIO_SetPullUpDownEach(eGPIO_D0, eGPIO_1, eGPUDdis);

	GPIO_SetDataEach(eGPIO_D0, eGPIO_0, 1);
	GPIO_SetDataEach(eGPIO_D0, eGPIO_1, 1);
#if 0
#if defined(CONFIG_PM_11V)
	printf("ARM 1.1V");
	PMIC_SetVoltage_Buck(eVDD_ARM,	eVID_MODE3,1.1);
	printf(", ");

	printf("INT 1.1V");
	PMIC_SetVoltage_Buck(eVDD_INT,	eVID_MODE2,1.1);

#elif defined(CONFIG_PM_12V)
	printf("ARM 1.2V");
	PMIC_SetVoltage_Buck(eVDD_ARM,	eVID_MODE3,1.2);
	printf(", ");

	printf("INT 1.2V");
	PMIC_SetVoltage_Buck(eVDD_INT,	eVID_MODE2,1.2);
#elif defined(CONFIG_PM_13V_12V)
	printf("ARM 1.3V");
	PMIC_SetVoltage_Buck(eVDD_ARM,  eVID_MODE3,1.3);
	printf(", ");

	printf("INT 1.2V");
	PMIC_SetVoltage_Buck(eVDD_INT,  eVID_MODE2,1.2);
#endif
#else 
	PMIC_MR_ZERO_SEC(I2C0, 0xCC);
//	PMIC_Buck6_Init(I2C0, 0xCC);
//	PMIC_Buck7_Init(I2C0, 0xCC);
//	PMIC_CAM_ENABLE(I2C0, 0xCC);
#endif
	printf("\n");	
}

void PMIC_MR_ZERO_SEC(I2C_CHANNEL eCh, u8 ucSlvAddr)
{
	u8 uSendData[2];
	u8 uRecData[2];
	u8 uRegAddr[2];

	uSendData[0] = 0x14;
	uSendData[1] = 0x00;
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);
}

void PMIC_Buck6_Init(I2C_CHANNEL eCh, u8 ucSlvAddr)
{
	u8 uSendData[2];
	u8 uRecData[2];
	u8 uRegAddr[2];

	uRegAddr[0] = 0x45;
	I2C_RecvEx(eCh, ucSlvAddr, uRegAddr, 1, uRecData, 2);
	printf("\n max8997-1 0x45 : %x \n", uRecData[0]);

	//uSendData[0] = 0x45;
	//uSendData[1] = 0x01 | 0x2 <<6;
	//uSendData[1] = 0x04 | 0x2 <<6;  /* Set3.0v to BUCK6 output */
	//uSendData[1] = 0x06 | 0x3 <<6;  /* Set3.3v to BUCK6 output */
	//uSendData[1] = 0x01 | 0x2 <<6;  /* Set3.0v to BUCK6 output */
	//I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

	uRegAddr[0] = 0x45;
	I2C_RecvEx(eCh, ucSlvAddr, uRegAddr, 1, uRecData, 2);
	printf("\n max8997-2 0x45 : %x \n", uRecData[0]);


	//uSendData[0] = 0x37;
	//uSendData[1] = 0x07;
	//uSendData[1] = 0x02;
	//I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

	uSendData[0] = 0x38;
	uSendData[1] = 0x00;
	//I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

}

void PMIC_Buck7_Init(I2C_CHANNEL eCh, u8 ucSlvAddr)
{
	u8 uSendData[2];
	u8 uRecData[2];
	u8 uRegAddr[2];
	/* Set BUCK7 with 1.85V */ 
	uSendData[0] = 0x3A;
	uSendData[1] = 0x26; 
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);
}

void PMIC_CAM_ENABLE(I2C_CHANNEL eCh, u8 ucSlvAddr)
{
	u8 uSendData[2];
	u8 uRecData[2];
	u8 uRegAddr[2];

	/* Enable LDO13 with 2.8V */ 
	uSendData[0] = 0x47;
	uSendData[1] = 0x28 | 0x2 <<6;
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

	/* Enable LDO14 with 1.8V */ 
	uSendData[0] = 0x48;
	uSendData[1] = 0x14 | 0x2 <<6;
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

	/* Enable LDO15 with 2.8V */ 
	uSendData[0] = 0x49;
	uSendData[1] = 0x28 | 0x2 <<6;
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

	/* Enable LDO21 with 1.5V */ 
	uSendData[0] = 0x45;
	uSendData[1] = 0x0E | 0x2 <<6;
	I2C_SendEx(eCh, ucSlvAddr, NULL, 0, uSendData, 2);

}

void PMIC_SetVoltage_Buck(PMIC_et ePMIC_TYPE ,PMIC_MODE_et uVID, float fVoltage_value )
{

	u8 uSendData[2];
	if((fVoltage_value<1.40)&&(fVoltage_value>0.75))
	{
		uSendData[0] = uVID; //mode 2 register
		
		switch(ePMIC_TYPE)
		{
			case eVDD_ARM:
				uSendData[1] = 0x80|(((u8)(fVoltage_value*100))-77) ; //mode 3 register
				break;
			
			case eVDD_INT:
				uSendData[1] = 0x80|(((u8)(fVoltage_value*100))-75) ; //mode 2 register
				break;
			
			case eVDD_G3D:
				uSendData[1] = 0x80|(((u8)(fVoltage_value*100))-75) ; //mode 2 register
				break;
		
			default:
				printf("PMIC_SetVoltage_Buck : Error, Unknwon PMIC type\n");
				break;
		}
			
		I2C_SendEx(gVoltage_Type[ePMIC_TYPE].ucI2C_Ch, gVoltage_Type[ePMIC_TYPE].ucDIV_Addr, NULL, 0, uSendData, 2);
	}
	
}

