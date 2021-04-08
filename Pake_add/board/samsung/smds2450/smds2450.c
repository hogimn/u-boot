/*
 * (C) Copyright 2014
 * Gemini IT <gemini525@nate.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <regs.h>

/* ------------------------------------------------------------------------- */
static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

static void smsc9220_pre_init(void)
{
        SMBIDCYR5_REG = 0;                      //Bank5 Idle cycle ctrl.
        SMBWSTWRR5_REG = 14;                    //Bank5 Write Wait State ctrl.
        SMBWSTOENR5_REG = 2;                    //Bank5 Output Enable Assertion Delay ctrl.     Tcho?
        SMBWSTWENR5_REG = 2;                    //Bank5 Write Enable Assertion Delay ctrl.
        SMBWSTRDR5_REG = 14;                    //Bank5 Read Wait State cont. = 14 clk      Tacc?

        SMBCR5_REG |=  ((1<<15)|(1<<7));                //dsf
        SMBCR5_REG |=  ((1<<2)|(1<<0));         //SMWAIT active High, Read Byte Lane Enabl          WS1?
        SMBCR5_REG &= ~((3<<20)|(3<<12));       //SMADDRVALID = always High when Read/Write
        SMBCR5_REG &= ~(3<<4);                  //Clear Memory Width
        SMBCR5_REG |=  (1<<4);                  //Memory Width = 16bit
}

// Gemini 2014.01.10
static void lcd_pre_init( void )
{
    unsigned int gpgcon_regs, gpgdat_regs;

    gpgcon_regs = GPGCON_REG;
	gpgcon_regs &= ~(0x3 << (15 * 2));
	gpgcon_regs |= (0x1 << (15 * 2));
	GPGCON_REG = gpgcon_regs;

    gpgdat_regs = GPGDAT_REG;
	gpgdat_regs &= ~(0x1 << 15);
	GPGDAT_REG = gpgdat_regs;

}
/*
 * Miscellaneous platform dependent initialisations
 */
static void usb_pre_init (void)
{
	CLKDIV1CON_REG |= 1<<4;

	USB_RSTCON_REG = 0x1;
	delay(500);
	USB_RSTCON_REG = 0x2;
	delay(500);
	USB_RSTCON_REG = 0x0;
	delay(500);

//	USB_PHYCTRL_REG &= ~0x2;
	USB_CLKCON_REG |= 0x2;
}

// Gemini 2014.02.05
int led_pre_init(void)
{
    unsigned int regs;

    regs = GPGCON_REG;
    regs &= ~((0x3 << (4*2))|(0x3 << (5*2))|(0x3 << (6*2))|(0x3 << (7*2)));
    regs |= ((0x1 << (4*2))|(0x1 << (5*2))|(0x1 << (6*2))|(0x1 << (7*2)));

    GPGCON_REG = regs;

    regs = GPGPU_REG;
    regs &= ~((0x3 << (4*2))|(0x3 << (5*2))|(0x3 << (6*2))|(0x3 << (7*2)));
    regs |= ((0x2 << (4*2))|(0x2 << (5*2))|(0x2 << (6*2))|(0x2 << (7*2)));

    GPGPU_REG = regs;

    regs = GPGDAT_REG;
    //regs &= ~((0x3 << (4*2))|(0x3 << (5*2))|(0x3 << (6*2))|(0x3 << (7*2)));
    regs |= ((0x1 << 4)|(0x1 << 5)|(0x1 << 6)|(0x1 << 7));

    GPGDAT_REG = regs;
	
	return 0;
}

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	led_pre_init();
	lcd_pre_init();
	usb_pre_init();
	smsc9220_pre_init();

	gd->bd->bi_arch_number = MACH_TYPE_SMDS2450;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

    if (CONFIG_NR_DRAM_BANKS == 2) {
	    gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	    gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
    }

	return 0;
}



#ifdef BOARD_LATE_INIT

/*
 * Marvell WiFi/BT SD8787 module should have 32.768 kHz RTC Clock
 * GPH14 (CLKOUT1) is connected with RTC Clock
 *      GPH14 [29:28] 00 = Input 01 = Output
 *      10 = CLKOUT1 11 = Reserved
 * MISCCR reg CLKSEL1 set with 010 = RTC clock output
 *      CLKSEL1 * [10:8] Select source clock with CLKOUT1 pad
 *      000 = RESERVED
 *      001 = Gated EPLL output
 *      010 = RTC clock output
 *      011 = HCLK
 *      100 = PCLK
 *      101 = DCLK1(Divided PCLK)
 *      11x = Reserved
 */
void rtc_clock_enable_for_wifi_bt(void)
{
    printf("%s called\n", __func__);

    unsigned int gphcon_regs, misccr_regs;

    gphcon_regs = GPHCON_REG;
    misccr_regs = MISCCR_REG;
    
    printf("before GPHCON: %08X, MISCCR: %08X\n", gphcon_regs, misccr_regs);

    gphcon_regs &= 0xCFFFFFFF;
    gphcon_regs |= 0x2 << 28;

    misccr_regs &= 0xFFFFF8FF;
    misccr_regs |= 0x2 << 8;

    GPHCON_REG = gphcon_regs;
    MISCCR_REG = misccr_regs;
    gphcon_regs = GPHCON_REG;
    misccr_regs = MISCCR_REG;

    printf("after GPHCON: %08X, MISCCR: %08X\n", gphcon_regs, misccr_regs);
}
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];

	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "nand erase 0 40000;nand write %08x 0 40000", PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
        setenv("bootdelay", "1");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	vu_long *mem_reg = (vu_long*) 0x48000000;

	printf("Board: MDS2450 ");
	switch ((*mem_reg>>1) & 0x7) {
	case 0:
		puts("SDRAM\n");
		break;
	case 1:
		puts("DDR2\n");
		break;
	case 2:
		puts("Mobile SDRAM\n");
		break;
	case 4:
		puts("DDR\n");
		break;
	case 6:
		puts("Mobile DDR\n");
		break;
	default:
		puts("unknown Memory Type\n");
	}
	return (0);
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_smdk2416(ulong addr)
{
	if ((0xC0000000 <= addr) && (addr < 0xC4000000))
		return (addr - 0xC0000000 + 0x30000000);
//	else if ((0xC4000000 <= addr) && (addr < 0xC8000000))
//        return (addr - 0xC4000000 +0x38000000);
	else if ((0x30000000 <= addr) && (addr < 0x34000000))
        return addr;
//   	else if ((0x38000000 <= addr) && (addr < 0x3c000000))
//		return addr;
    else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif

#if defined(CONFIG_CMD_NAND) && defined(CFG_NAND_LEGACY)
#include <linux/mtd/nand.h>
extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];
void nand_init(void)
{
	nand_probe(CFG_NAND_BASE);
	if (nand_dev_desc[0].ChipID != NAND_ChipID_UNKNOWN) {
		print_size(nand_dev_desc[0].totlen, "\n");
	}
}
#endif
