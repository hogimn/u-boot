/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
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
#include <s5pc210.h>
#include <asm/io.h>


/* ------------------------------------------------------------------------- */
#if 1
#define SMC9115_Tacs	(0x2)	// 0clk		address set-up
#define SMC9115_Tcos	(0x4)	// 4clk		chip selection set-up
#define SMC9115_Tacc	(0xe)	// 14clk	access cycle
#define SMC9115_Tcoh	(0x2)	// 1clk		chip selection hold
#define SMC9115_Tah	(0x4)	// 4clk		address holding time
#define SMC9115_Tacp	(0x6)	// 6clk		page mode access cycle
#define SMC9115_PMC	(0x0)	// normal(1data)page mode configuration

#define SROM_DATA16_WIDTH(x)	(1<<((x*4)+0))
#define SROM_WAIT_ENABLE(x)	(1<<((x*4)+1))
#define SROM_BYTE_ENABLE(x)	(1<<((x*4)+2))
#endif

#if 0
/* nCS1 - FPGA SROM_BC Resister Value */
#define FPGA_Tacs       (0x0)   // 0clk         address set-up
#define FPGA_Tcos       (0x4)   // 4clk         chip selection set-up
#define FPGA_Tacc       (0xe)   // 14clk        access cycle
#define FPGA_Tcoh       (0x1)   // 1clk         chip selection hold
#define FPGA_Tah          (0x4) // 4clk         address holding time
#define FPGA_Tacp       (0x6)   // 6clk         page mode access cycle
#define FPGA_PMC          (0x0) // normal(1data)page mode configuration
#else
/* nCS1 - FPGA SROM_BC Resister Value */
#define FPGA_Tacs       (0x3)   // 0clk         address set-up
#define FPGA_Tcos       (0x2)   // 4clk         chip selection set-up
#define FPGA_Tacc       (0x4)   // 14clk        access cycle
#define FPGA_Tcoh       (0x2)   // 1clk         chip selection hold
#define FPGA_Tah        (0x2) // 4clk         address holding time
#define FPGA_Tacp       (0x2)   // 6clk         page mode access cycle
#define FPGA_PMC        (0x0) // normal(1data)page mode configuration
#endif

#if 0
#define SMC9115_Tacs	(0x2)	// 0clk		address set-up
#define SMC9115_Tcos	(0x2)	// 4clk		chip selection set-up
#define SMC9115_Tacc	(0xe)	// 14clk	access cycle
#define SMC9115_Tcoh	(0x2)	// 1clk		chip selection hold
#define SMC9115_Tah	(0x2)	// 4clk		address holding time
#define SMC9115_Tacp	(0x2)	// 6clk		page mode access cycle
#define SMC9115_PMC	(0x0)	// normal(1data)page mode configuration

#define SROM_DATA16_WIDTH(x)	(1<<((x*4)+0))
#define SROM_WAIT_ENABLE(x)	(1<<((x*4)+1))
#define SROM_BYTE_ENABLE(x)	(1<<((x*4)+2))
#endif

/* nCS1 - FPGA SROM_BC Resister Value */
#define FPGA_Tacs       (0x4)   // 0clk         address set-up
#define FPGA_Tcos       (0x2)   // 4clk         chip selection set-up
#define FPGA_Tacc       (0x5)   // 14clk        access cycle
#define FPGA_Tcoh       (0x2)   // 1clk         chip selection hold
#define FPGA_Tah        (0x2) // 4clk         address holding time
#define FPGA_Tacp       (0x2)   // 6clk         page mode access cycle
#define FPGA_PMC        (0x0) // normal(1data)page mode configuration

/* ------------------------------------------------------------------------- */
#define DM9000_Tacs	(0x0)	// 0clk		address set-up
#define DM9000_Tcos	(0x4)	// 4clk		chip selection set-up
#define DM9000_Tacc	(0xE)	// 14clk	access cycle
#define DM9000_Tcoh	(0x1)	// 1clk		chip selection hold
#define DM9000_Tah	(0x4)	// 4clk		address holding time
#define DM9000_Tacp	(0x6)	// 6clk		page mode access cycle
#define DM9000_PMC	(0x0)	// normal(1data)page mode configuration

int check_bootmode(void);
unsigned int OmPin;
int boot_mode;


DECLARE_GLOBAL_DATA_PTR;
extern int nr_dram_banks;
#ifdef CONFIG_EVT1
unsigned int dmc_density = 0xFFFFFFFF;
#endif

static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

/*
 * Miscellaneous platform dependent initialisations
 */


static void dm9000_pre_init(void)
{
	unsigned int tmp;

#if defined(DM9000_16BIT_DATA)
	SROM_BW_REG &= ~(0xf << 20);
	SROM_BW_REG |= (0<<23) | (0<<22) | (0<<21) | (1<<20);

#else	
	SROM_BW_REG &= ~(0xf << 20);
	SROM_BW_REG |= (0<<19) | (0<<18) | (0<<16);
#endif
	SROM_BC5_REG = ((0<<28)|(1<<24)|(5<<16)|(1<<12)|(4<<8)|(6<<4)|(0<<0));

	tmp = MP01CON_REG;
	tmp &=~(0xf<<20);
	tmp |=(2<<20);
	MP01CON_REG = tmp;
}

#if defined(CONFIG_SM5S4210)
// nCS0
static void smc9115_pre_init(void)
{
	unsigned int tmp;

	SROM_BW_REG &= ~(0xf << 0);
	SROM_BW_REG |= (0<<3) | (0<<2) | (0<<1) | (1<<0);
//	SROM_BW_REG |= (1<<3) | (0<<2) | (0<<1) | (1<<0);
	SROM_BC0_REG = ((SMC9115_Tacs<<28)|(SMC9115_Tcos<<24)|(SMC9115_Tacc<<16)|(SMC9115_Tcoh<<12)|(SMC9115_Tah<<8)|(SMC9115_Tacp<<4)|(SMC9115_PMC<<0));
	tmp = MP01CON_REG;
	tmp &=~(0xf<<0);
	tmp |=(2<<0);
	MP01CON_REG = tmp;
}
/* nCS1, Resister init.. */
static void fpga_pre_init(void)
{
  unsigned int tmp;

  SROM_BW_REG &= ~(0xf << 4);
  SROM_BW_REG |= (0<<7) | (0<<6) | (1<<5) | (1<<4);
  SROM_BC1_REG = ((FPGA_Tacs<<28)|(FPGA_Tcos<<24)|(FPGA_Tacc<<16)|(FPGA_Tcoh<<12)|(FPGA_Tah<<8)|(FPGA_Tacp<<4)|(FPGA_PMC<<0));

  tmp = MP01CON_REG;
  tmp &=~(0xf<<4);
  tmp |=(2<<4);
  MP01CON_REG = tmp;
}

void backlight_off(void)
{
	u32 gpio;
        // GPD0(0) output, PWM0
        gpio = __REG(GPD0CON);
        gpio &= 0xFFF0;
        gpio |= 0x1;
        __REG(GPD0CON) = gpio;

        // GPD0(0) DAT
        gpio = 0x0;
        __REG(GPD0DAT) = gpio;

        // GPX1(1) output, LCD enable
        gpio = __REG(GPX2CON);
        gpio &= 0xFFFFFF0F;
        gpio |= 0x10;
        __REG(GPX2CON) = gpio;

        // GPX2(1) DAT
        gpio = 0x0;
        __REG(GPX2DAT) = gpio;
}
#endif


int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
#if CONFIG_SM5S4210
	backlight_off();
	smc9115_pre_init();
	fpga_pre_init();
#endif

#ifdef CONFIG_DRIVER_DM9000
//	dm9000_pre_init();
#endif

	gd->bd->bi_arch_number = MACH_TYPE;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	//if(((PRO_ID & 0x300) >> 8) == 2){
	if(((*((volatile unsigned long *)CHIP_ID_BASE) & 0x300) >> 8) == 2){
#ifdef CONFIG_EVT1
	 	printf("POP type: ");
		if(dmc_density == 6){
			printf("POP_B\n");
			nr_dram_banks = 4;
			gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
			gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
			gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
			gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
			gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
			gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
			gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
			gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;
		}
		else if(dmc_density == 5){
			printf("POP_A\n");
			nr_dram_banks = 2;
			gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
			gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
			gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
			gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
		}
		else{
			printf("unknown POP type\n");
			nr_dram_banks = 2;
			gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
			gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
			gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
			gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
		}
#else
		nr_dram_banks = 2;
		gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
		gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
		gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
		gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
#endif
	}
	else{
		nr_dram_banks = 8;
		gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
		gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
		gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
		gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
		gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
		gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
		gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
		gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;
		gd->bd->bi_dram[4].start = PHYS_SDRAM_5;
		gd->bd->bi_dram[4].size = PHYS_SDRAM_5_SIZE;
		gd->bd->bi_dram[5].start = PHYS_SDRAM_6;
		gd->bd->bi_dram[5].size = PHYS_SDRAM_6_SIZE;
		gd->bd->bi_dram[6].start = PHYS_SDRAM_7;
		gd->bd->bi_dram[6].size = PHYS_SDRAM_7_SIZE;
		gd->bd->bi_dram[7].start = PHYS_SDRAM_8;
		gd->bd->bi_dram[7].size = PHYS_SDRAM_8_SIZE;
	}

/*
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

#if defined(CONFIG_SPARSEMEM)
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
#endif
*/

	return 0;
}

#ifdef BOARD_LATE_INIT
/*
#if defined(CONFIG_BOOT_NAND)
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];

	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "nand erase 0 40000;nand write %08x 0 40000", PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
		setenv("bootdelay", "0");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#elif defined(CONFIG_BOOT_MOVINAND)
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];
	int hc;

	hc = (magic[2] & 0x1) ? 1 : 0;

	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "movi init %d %d;movi write u-boot %08x", magic[3], hc, PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
		setenv("bootdelay", "0");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#else
*/
int board_late_init (void)
{
	int ret = check_bootmode();
	printf("board_late_init\n");
	if ((ret == BOOT_MMCSD || ret == BOOT_EMMC441 || ret == BOOT_EMMC43 )
				&& (getenv("bootcmd") == NULL)
				&& boot_mode == 0) {
		char boot_cmd[100];
		int hc;
		sprintf(boot_cmd, CONFIG_BOOTCOMMAND);
		setenv("bootcmd", boot_cmd);
		}

	return 0;
}
//#endif
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:	%s\n", CONFIG_DEVICE_STRING);
	return (0);
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_smdkc210(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return (addr - 0xc0000000 + 0x40000000);
	else;
		//printf("The input address don't need "\
		//	"a virtual-to-physical translation : %08lx\n", addr);

	return addr;
}
#endif

int check_bootmode(void)
{
	OmPin = INF_REG3_REG;

	printf("\n\nChecking Boot Mode ...");

	if(OmPin == BOOT_ONENAND) {
		printf(" OneNand\n");
	} else if (OmPin == BOOT_NAND) {
		printf(" NAND\n");
	} else if (OmPin == BOOT_MMCSD) {
		printf(" SDMMC\n");
	} else if (OmPin == BOOT_EMMC43) {
		printf(" EMMC4.3\n");
	} else if (OmPin == BOOT_EMMC441) {
		printf(" EMMC4.41\n");
	}
	return OmPin;
}

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


