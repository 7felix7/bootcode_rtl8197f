#include <asm/asm.h>
#include <regdef.h>
#include <asm/mipsregs.h>
#include "start.h"
#include "../autoconf.h"

#define CONFIG_CACHE_INIT

#define CONFIG_SYS_ROM_CACHE_BASE_ADDR      0x9FD00000
#define CONFIG_SYS_SRAM_CACHE_BASE_ADDR     0x9FE00000
#define CONFIG_SYS_ROM_NONCACHE_BASE_ADDR   0xBFD00000
#define CONFIG_SYS_SRAM_NONCACHE_BASE_ADDR  0xBFE00000
#define CONFIG_SYS_ROM_PHY_BASE_ADDR        0x1FD00000
#define CONFIG_SYS_SRAM_PHY_BASE_ADDR       0x1FE00000

#define CONFIG_SYS_INIT_SP_OFFSET	        0x4000


		.text
		.set noreorder
		.globl __start
__start:

		j 	reset
		nop     
		nop
		
//	.word 0x20120607		//ROM code version tag, 
//	.word 0x00000005
//--------------------------------------------------------------------------------              


reset:
	nop
	REG32_W(0xb8000014, ((1<<8)|(1<<4))); //enable uart
	nop
	
	#if 1
		jal uart_show    //show boot msg
		nop
	#endif
	
	/* Clear watch registers */
	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* WP(Watch Pending), SW0/1 should be cleared */
	mtc0	zero, CP0_CAUSE

	mfc0	t0, CP0_STATUS
	or	t0, ST0_CU0 | 0x1f  
	xor	t0, 0x1f 
	mtc0	t0, CP0_STATUS
	.set	noreorder
	sll	zero, 3				# ehb

	/* Init Timer */
	mtc0	zero, CP0_COUNT
	mtc0	zero, CP0_COMPARE

#ifdef CONFIG_CACHE_INIT
	nop
	li	t7, 0xb8000008
	lw	t6, 0(t7)
	and	t6,t6,0x0F
	li	t7, 0x01
	beq	t6,t7, cache_init_end
	nop

	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG

	/* processor ID */
	mfc0		k1, C0_PRId
	li      t0, M_PRIdImp | M_PRIdCoID
	and     k1, t0

	/* Initialize caches... */
	la	t9, sys_init_cache
	jalr	t9
	 nop

	/* ... and enable them */
	li	t0, CONF_CM_CACHABLE_NONCOHERENT
	mtc0	t0, CP0_CONFIG
	
cache_init_end:
#endif

//------------------------------------------------	 	
#if 0  //0xbfc00000->0x9fc00000
	la	t0, cache_start
	and	t0,0xdfffffff
	jr	t0
	nop
	nop
cache_start:
#endif

//============================================================================
	//UART_WRITE('G');
#ifndef CONFIG_NAND_FLASH_BOOTING
	/* Set up temporary stack */
	/* spi flash do ddr init here */
	li  sp, CONFIG_SYS_SRAM_CACHE_BASE_ADDR + CONFIG_SYS_INIT_SP_OFFSET
	nop
	jal start_c
#endif
	nop
    nop

//--------------------------------------------------------------------------------
flash2ram:
// Load to 80100000 for compress
// Load to 80000000 for non-compress

		la		k0, __boot_start
		la		k1, (__boot_end + 4)
		la		t1, BOOT_ADDR
1:
		#if 0
		jal SPI_show    //show SPI msg
		nop
		#endif
		
		lw		t0, 0(k0)
		nop
		sw		t0, 0(t1)	//*(t1)=*(t0)   t1+=4;  t0+=4;   t1=BOOT_ADDR, t0=__boot_start, run loop
		nop
		addu	t1, 4
		addu	k0, 4
		bne		k1, k0, 1b
		nop

//		UART_PRINT_DELAY(SPI_done_msg);
		//		Jump to booting
		li		k0, BOOT_ADDR
		jr		k0
		nop 
              //all END
	
//--------------------------------------------------------------------------------	
#if 0
		la a0,__boot_start
		la a1,(__boot_end+4) 
		la a2, BOOT_ADDR
		jal cmp_flash2mem
		nop
		nop
#endif		
//--------------------------------------------------------------------------------

//==========================================================================
uart_show:	

	//uart_init:		
  	REG32_W(BSP_UART0_LCR,0x05);
  		
  	REG32_W(BSP_UART0_FCR,0xc7);
  	REG32_W(BSP_UART0_IER,0x00);

  	REG32_W(BSP_UART0_LCR,0x85);
  	REG32_W(BSP_UART0_DLL, (BSP_UART0_BAUD_DIVISOR & 0x00ff));
   	REG32_W(BSP_UART0_DLM, ((BSP_UART0_BAUD_DIVISOR & 0xff00) >> 8)); 
//REG32_W(BSP_UART0_SCR, 0xA0030); // 57600, 115200
//REG32_W(BSP_UART0_STSR, 0xC0); // 57600, 115200
	REG32_W(BSP_UART0_LCR, 0x05);

       //uart show
		UART_PRINT(boot_msg);		
	j ra 
 	nop

 SPI_show:	

	//uart_init:		
	REG32_W(BSP_UART0_LCR,0x05);
  		
  	REG32_W(BSP_UART0_FCR,0xc7);
  	REG32_W(BSP_UART0_IER,0x00);
  	
  	REG32_W(BSP_UART0_LCR,0x85);
  	REG32_W(BSP_UART0_DLL, (BSP_UART0_BAUD_DIVISOR & 0x00ff));
   	REG32_W(BSP_UART0_DLM, ((BSP_UART0_BAUD_DIVISOR & 0xff00) >> 8)); 
//REG32_W(BSP_UART0_SCR, 0xA0030); // 57600, 115200
//REG32_W(BSP_UART0_STSR, 0xC0); // 57600, 115200
	REG32_W(BSP_UART0_LCR, 0x05);
	
//	UART_PRINT_DELAY(SPI_msg);		
	
	j ra 
 	nop


boot_msg:	.ascii "\r\nBooting...\r\n\0"
SPI_done_msg:  .ascii "\r\nSPI image move done ->Jump to DRAM\r\n\0"
SPI_msg:  .ascii "\r\n.\0"

//==========================================================================

