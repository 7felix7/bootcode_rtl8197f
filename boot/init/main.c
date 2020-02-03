/*
 *  linux/init/main.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  GK 2/5/95  -  Changed to support mounting root fs via NFS
 *  Added initrd & change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Moan early if gcc is old, avoiding bogus kernels - Paul Gortmaker, May '96
 *  Simplified starting of init:  Michael A. Griffith <grif@acm.org>
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <linux/string.h>

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>

#include <rtk_lan_wan_isolation.h>
#include <rtl8196x/asicregs.h>

#include "main.h"

#define REG32(reg)	(*(volatile unsigned int *)(reg))	

// __attribute__ ((section (".text.wei"))) 

#if 0
#define REG(reg)                      (*((volatile unsigned int *)(reg)))
#define D3ZQCCR (0xB8001080)
#define MCR_DRAMTYPE_DDR3 	(0x20000000)
#define MCR_DRAMTYPE_MASK	(0xF0000000)
#define MCR			(0xB8001000)
#define D3ZQCCR (0xB8001080)

/* Function Name: 
 * 	memctlc_is_SDR
 * Descripton:
 *	Determine whether the DRAM type is SDR SDRAM.
 * Input:
 *	None
 * Output:
 * 	None
 * Return:
 *  	1  -DRAM type is SDR SDRAM
 *	0  -DRAM type isn't SDR SDRAM
 */
unsigned int memctlc_is_DDR3(void)
{
	if(MCR_DRAMTYPE_DDR3 == (REG(MCR) & MCR_DRAMTYPE_MASK))
		return 1;
	else
		return 0;
}


unsigned int DDR_short_ZQ()
{
		printf("\nEnable short ZQ for DDR3\n");
			volatile unsigned int *d3zqccr;
			d3zqccr = (volatile unsigned int *)D3ZQCCR;

			/* 4. Enable short ZQ */
			if(memctlc_is_DDR3()){
				*d3zqccr |= (1<<30);
			}else{
				*d3zqccr &= (~(1<<30));
			}		

}
		
#endif

extern unsigned int gCHKKEY_HIT;
extern unsigned int gCHKKEY_CNT;

#if defined(CONFIG_POST_ENABLE)
int POSTRW_API(void)
{
	// boot code: from 0x80000000 ~ ...,
	// stack:     from ... ~ 0x80700000 (please see head.S)
	unsigned int Test_Start_offset=0x00700000;  
	unsigned int Test_Size;

	unsigned short test_pattern_mode=0;//"0"=0x5a,"1"=0xa5,
	unsigned int test_cnt=0;    
	//unsigned int test_result=1;  //"1":Pass,"0":fail

	unsigned int DRAM_ADR = 0x80000000; //cache address
	unsigned int DRAM_Start_Test_ADR,DRAM_End_Test_ADR; 	  
	unsigned int DRAM_pattern1 = 0xA5A5A5A5;
	unsigned int DRAM_pattern0 = 0x5A5A5A5A;
	unsigned int DRAM_pattern;

	DRAM_Start_Test_ADR= DRAM_ADR + Test_Start_offset; 
	DRAM_End_Test_ADR=  DRAM_Start_Test_ADR+Test_Size; 

	Test_Size = REG32(0xb8000f00); // total dram size from start_c.c
	if (Test_Size <= Test_Start_offset) // skip test
		return 1;

	#if CONFIG_ESD_SUPPORT//patch for ESD
	REG32(0xb800311c) = 0xa5000000;
	#endif
				 
	Test_Size -= Test_Start_offset;
	
	prom_printf("\nPOSTing ...");		
	
	for(test_pattern_mode=1 ;test_pattern_mode<=2;test_pattern_mode++)
	{		
		if(test_pattern_mode%2==0)
		{
			DRAM_pattern=DRAM_pattern0;
		}
		else
		{
			DRAM_pattern=DRAM_pattern1;
		}
		//prom_printf("\nPOST(%d),Pattern:0x%x=> ",test_pattern_mode,DRAM_pattern);		

		/* Set Data Loop*/
		/* 	Test from 1~16MB ,except 7~8MB*/
		for (test_cnt= 0; test_cnt < Test_Size;test_cnt+=0x00100000 )
		{	 	
//			if ((test_cnt==0x0 )||(test_cnt==0x00400000 ))//skip DRAM size from 0~1MB and 4~5MB
//				continue;

			memset((unsigned int *) (DRAM_Start_Test_ADR+ test_cnt),DRAM_pattern,(unsigned int)0x00100000 );
		}	 

		/*Verify Data Loop*/
		for(test_cnt=0;test_cnt<Test_Size;test_cnt+=4)
		{
//			if(((test_cnt >= 0x0 ) && (test_cnt <=0x00100000))||((test_cnt >= 0x00400000 ) && (test_cnt <=0x00500000)))
//				continue;

			if (REG32(DRAM_Start_Test_ADR+test_cnt) != DRAM_pattern)//Compare FAIL
			{											
				prom_printf("\nDRAM POST Fail at addr:0x%x!!!\n\n",(DRAM_Start_Test_ADR+test_cnt) );
				//test_result=0;

				#if CONFIG_ESD_SUPPORT
				REG32(0xb800311c) = 0x00600000;
				#endif
				return 0;					
			}
		}
	}

//	if (test_result)
		prom_printf("\ntest OK\n");
//	else
//		prom_printf("Fail\n");
		 	 
	#if CONFIG_ESD_SUPPORT
	REG32(0xb800311c) = 0x00600000;
	#endif

	return 1;
}
#endif

void start_kernel(void)
{

	int ret;

	gCHKKEY_HIT = 0;
	gCHKKEY_CNT = 0;

	IMG_HEADER_T header;
	SETTING_HEADER_T setting_header;

//-------------------------------------------------------
	setClkInitConsole();

	initHeap();
		
	initInterrupt();

	initFlash();

	//Init_GPIO();				
	//MxSpdupThanLexra();  //wei add

#if defined(CONFIG_POST_ENABLE)
	ret = POSTRW_API();
#endif

	showBoardInfo();

#if defined(CONFIG_POST_ENABLE)
	if(ret == 0)
		return;
#endif

#ifdef CONFIG_BOOT_RESET_ENABLE	
	#if defined(CONFIG_SW_8367R)
	// 8198C+8367RB, reset button: PIN_P0_RXD3, GPIO_E7, 0xb8000108 bit 29~27 = 0b011
	REG32(BSP_PIN_MUX_SEL3) = (REG32(BSP_PIN_MUX_SEL3) & ~(7<<27)) | RTL_GPIO_RESET_BTN_MUX;
	#elif defined(CONFIG_SW_83XX)
	// 8198C+8367RB, reset button: PIN_P0_RXD3, GPIO_E7, 0xb8000108 bit 29~27 = 0b011
	REG32(BSP_PIN_MUX_SEL3) = (REG32(BSP_PIN_MUX_SEL3) & ~(7<<27)) | RTL_GPIO_RESET_BTN_MUX;
	#else
	/* set pin mux as gpio */
	RTL_W32(BSP_RESET_BTN_PIN_MUX_SEL, 
			(RTL_R32(BSP_RESET_BTN_PIN_MUX_SEL) 
			& ~(BSP_RESET_BTN_PIN_MASK) | (BSP_RESET_BTN_PIN_MUX))); 
	#endif
	/* turn on gpio pin */
	RTL_W32(BSP_GPIO_CNR_REG(BSP_RESET_BTN_PIN), 
			(RTL_R32(BSP_GPIO_CNR_REG(BSP_RESET_BTN_PIN)) 
			& (~(1 << BSP_GPIO_BIT(BSP_RESET_BTN_PIN)))));
	/* set gpio as input */
	RTL_W32(BSP_GPIO_DIR_REG(BSP_RESET_BTN_PIN), 
			(RTL_R32(BSP_GPIO_DIR_REG(BSP_RESET_BTN_PIN)) 
			& (~(1 << BSP_GPIO_BIT(BSP_RESET_BTN_PIN)))));
#endif

	#if 0
	DDR_short_ZQ();
	#endif

	#if 0
	#define REG32(reg)		(*(volatile unsigned int   *)(reg))
	REG32(0xb8010000)|=(3<<28); //set b8010000 bit 29-28=0b11 (burst size = 128 bytes)//0xf4000000
	#endif

#if defined(CONFIG_RTL8197F) && (defined (CONFIG_SW_8367R) || defined (CONFIG_SW_83XX)) && defined(CONFIG_LAN_WAN_ISOLATION)
	//init_97f_8367r();
	if (swCore_init()) {  	
		dprintf("\nSwitch core initialization failed!\n");        
		return;
	}
#elif defined(CONFIG_RTL8197F) && (defined (CONFIG_SW_8367R) || defined (CONFIG_SW_83XX))
	//pull low for 8367R
	#define GPIO_RESET				26		// GPIO_H2
	REG32(PIN_MUX_SEL14) = (REG32(PIN_MUX_SEL14) & ~(0xF<<28)) | (2<<28);
	REG32(PEFGHCNR) &= ~(1<<GPIO_RESET);
	REG32(PEFGHDIR) |= (1<<GPIO_RESET);
	// for 8367r h/w reset pin	
	REG32(PEFGHDAT) &= ~(1<<GPIO_RESET);	
#endif	

#ifdef SUPPORT_TFTP_CLIENT
	extern volatile unsigned int last_sent_time;
	extern unsigned int tftp_from_command;
	extern int retry_cnt;
	retry_cnt = 0;
	tftp_from_command  = 0;
	last_sent_time  = 0;
	eth_startup(0); 
	sti();			
	tftpd_entry(1);
#endif

	return_addr=0;
	ret=check_image	(&header,&setting_header);

#if defined(CONFIG_NAND_FLASH_BOOTING)
 	#define MAX_MOUNT_ROOTFS_TIMES	5
	if(REG32(0xb8019004) > MAX_MOUNT_ROOTFS_TIMES){
		REG32(0xb8019004) = 0;
		ret = 0;
	}
#endif
	doBooting(ret, return_addr, &header);
}

//-------------------------------------------------------
//show board info
void showBoardInfo(void)
{
	volatile int cpu_speed = 0;

#if 1//patch for ESD
        REG32(0xb800311c)=0xa5000000;	
#endif
#if 1
	#define SYS_INT_STATUS 0xb8000004
	REG32(SYS_INT_STATUS)=(1<<1);  //clear wakeup interrupt	
#ifdef CONFIG_CPU_800M
	SettingCPUClk(0x7,0);	//chg from 400 -> 800
#endif
#ifdef CONFIG_CPU_1G
	SettingCPUClk(0xB, 0);	//chg from 400 -> 1000
#endif

	REG32(SYS_INT_STATUS)=(1<<1);  //clear wakeup interrupt	
#endif

	//REG32(0xb8000010)|=0xffff800;
#if 0//def CONFIG_NAND_FLASH_BOOTING 
	cpu_speed = check_cpu_speed();	
	cpu_speed = 450 + ((REG32(SYS_HW_STRAP) & 0x78000) >> 15) * 50;
#else
	cpu_speed = check_cpu_speed();	
#endif
#if 0//defined(CONFIG_NAND_FLASH_BOOTING)
	#if defined(CONFIG_DDR2_SDRAM)
		prom_printf("DDR2:");
	#elif defined(CONFIG_DDR3_SDRAM)
		prom_printf("DDR3:");
	#elif defined(CONFIG_DDR1_SDRAM)
		prom_printf("DDR1:");
	#elif defined(CONFIG_SDRAM)
		prom_printf("SDRAM:");
	#endif
	#if defined(CONFIG_DDR2_32MB_16bit)
		prom_printf("32 MB\n");
	#elif defined(CONFIG_DDR2_64MB_16bit)
		prom_printf("64 MB\n");
	#elif defined(CONFIG_DDR2_128MB_16bit)
		prom_printf("128 MB\n");
	#elif defined(CONFIG_DDR3_128MB_16bit)
		prom_printf("128 MB\n");
	#elif defined(CONFIG_DDR3_256MB_16bit)
		prom_printf("256 MB\n");
#endif
#endif


	prom_printf("%s",((*(volatile unsigned int *)(0xb8000008)) & (0x1<<22))?"Reboot Result from Watchdog Timeout!\n":" ");
	
	/* clean watchdog bit */
	(*(volatile unsigned int *)(0xb8000008)) &= (~(0x1<<22));

	
	prom_printf("\n---Realtek RTL8197F boot code at %s %s (%dMHz)\n", BOOT_CODE_TIME,B_VERSION,  cpu_speed);

}



