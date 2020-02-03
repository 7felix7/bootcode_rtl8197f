#include <asm/asm.h>
#include <regdef.h>
#include <asm/mipsregs.h>
#include "start.h"
#include "../autoconf.h"

#define CONFIG_SYS_SRAM_CACHE_BASE_ADDR     0x9FE00000
#define CONFIG_SYS_INIT_SP_OFFSET	        0x1540
#define CONFIG_NAND_IRAM_IMAGE_COPY_INROM
#define NAND_LOADADDR						0xa0500010

		.text
		.set noreorder
		.globl __start
#ifndef CONFIG_CUSTOMER_BOOT_SIGNATURE_SUPPORT
#ifdef CONFIG_NAND_IRAM_IMAGE_COPY_INROM
__start:
		/* init ddr */
		addiu   sp,sp,-24
		sw      ra,20(sp)
		nop
		nop
		nop
		nop
		//li  sp, CONFIG_SYS_SRAM_CACHE_BASE_ADDR + CONFIG_SYS_INIT_SP_OFFSET
		nop
		nop
		jal start_c
		nop
		nop
		jal iram_jump_to_load_image
		nop
		nop
		li		k0, NAND_LOADADDR
		jr		k0
		nop 
		nop
		nop
		lw      ra,20(sp)
		nop
		jr ra
		addiu   sp,sp,24
		nop
		nop
#else
__start:
		/* init ddr */
		addiu   sp,sp,-24
		sw      ra,20(sp)
		nop
		nop
		//li  sp, CONFIG_SYS_SRAM_CACHE_BASE_ADDR + CONFIG_SYS_INIT_SP_OFFSET
		nop
		nop
		jal start_c
		nop
		nop

		/* store sp */
		//li	t7, 0xa0400000
		//sw	sp, 0(t7)
		//nop
		//nop

		/* do page read in sram */
		//li  sp, 0xa0300000
		nop
		nop
		jal iram_nflash_read
 		nop
 		nop
 		li		k0, NAND_LOADADDR
		jr		k0

		/* restore sp in rom-code */
		//li	t7, 0xa0400000
		//lw	sp, 0(t7)
		//nop
		nop
		nop
		lw      ra,20(sp)
		nop
		jr ra
		addiu   sp,sp,24
		nop
		nop
#endif
#else
__start:
		/* init ddr */
		addiu   sp,sp,-24
		sw      ra,20(sp)
		nop
		nop
		nop
		jal iram_do_ddr_init
		nop
		nop
		nop
		jal iram_nflash_read
 		nop
 		nop
		nop
		lw      ra,20(sp)
		nop
		jr ra
		addiu   sp,sp,24
		nop
#endif
//==========================================================================

