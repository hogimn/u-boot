#include <common.h>
#include <command.h>
//#include <regs.h>  // KSJ
#include <s5pc210.h>



#ifdef CONFIG_CMD_BDT
void main_menu_print(void)
{

	printf("\n-------------- HBE-SMV-S4210 Board Test --------------\n");
	printf(" [1] Dip Switch Input Testing..\n");
	printf(" [2] KeyPad Input Testing..\n");
	printf(" [3] Character LCD Output Testing..\n");
	printf(" [4] LED Output Testing..\n");
	printf(" [5] 7-Segment LED Output Testing..\n");
	printf(" [6] Dot LED Testing...\n");
	printf(" [7] OLED Testing...\n");
	printf(" [8] Full Color LED Testing...\n");
	printf(" [Q or q] Quit\n\n");
	printf("---------------------------------------------------\n");
	printf("Select Menu ? ");
}

int do_bdtest(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char ch;
	int rtn;

	main_menu_print();
	ch = getc();
	while(1) {
		switch(ch) {
			case '1':
				printf("\nDip switch Input Testing...\n");
				dipsw_test();
				//return_home();
				break;

			case '2':
				printf("\nKeyPad Input Testing...\n");
				key_test();
				//BootMessage();
				break;

			case '3':
				printf("\nCharacter LCD Testing...\n");
				lcd_test();
				break;

			case '4':
				printf("\nLED Output Testing...\n");
				led_test();
				break;

			case '5':
				printf("\n7-Segment LED Testing...\n");
				seg_c_main();
				break;

			case '6':
				printf("\nDot-LED Testing...\n");
				dotled_test();
				break;
				
			case '7':
				printf("\nOLED Testing...\n");
				initOLED();
				break;
				
			case '8':
				printf("\nFull Color LED Testing...\n");
				fullcolorled_test();
				break;

/*
			case '9':
				printf("\nGPIO LED Testing...\n");
				gpio_led_test();
				break;
*/

			case 'q': case 'Q':
				return 0;
		}
			main_menu_print();
			ch = getc();
	}
	return rtn;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	bdtest,	1,	1,	do_bdtest,
	"bdtest  - Board test\n",
	""
);
#endif	/* CONFIG_CMD_BDT */
