/* $Id: head.S,v 1.1 2009/11/13 13:22:46 jasonwang Exp $
 *
 * arch/mips/kernel/head.S
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995 Waldorf Electronics
 * Written by Ralf Baechle and Andreas Busse
 * Copyright (C) 1995 - 1999 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Modified for DECStation and hence R3000 support by Paul M. Antoine
 * Further modifications by David S. Miller and Harald Koerfgen
 * Copyright (C) 1999 Silicon Graphics, Inc.
 *
 * Head.S contains the MIPS exception handler and startup code.
 */

#include <linux/config.h>
/*Cyrus Tsai*/
#include <linux/init.h>
/*Cyrus Tsai*/

#include <linux/threads.h>

#include <asm/asm.h>
#include <asm/cacheops.h>
#include <asm/current.h>
#include <asm/offset.h>
#include <asm/processor.h>
#include <asm/regdef.h>
#include <asm/cachectl.h>
#include <asm/mipsregs.h>
#include <asm/lexraregs.h>
#include <asm/stackframe.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>
#ifdef CONFIG_PM_SUSPEND_WAKEUP
#include <asm/power.h>
#endif

//#define CONFIG_CACHE_INIT

#define REG32_W(addr,v) 		li t6,v;\
					li t7,addr;\
					sw t6, 0(t7);\
					nop;


__INIT

//-------------------------------------------------------------------------
	.text
	NESTED(start, 16, sp)
	.set	noreorder
	/* The following two symbols are used for kernel profiling. */
	EXPORT(stext)
	EXPORT(_stext)

	
	/*
	 * Reserved space for exception handlers.
	 * Necessary for machines which link their kernels at KSEG0.
	 * FIXME: Use the initcode feature to get rid of unused handler
	 * variants.
	 */

	b check_self_exist
	nop
	nop
	.fill	0x400
check_self_exist:	

reset:

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
	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG

	/* Initialize caches... */
	la	t9, mips_cache_reset
	jalr	t9
	 nop

	/* ... and enable them */
	li	t0, CONF_CM_CACHABLE_NONCOHERENT
	mtc0	t0, CP0_CONFIG
#endif

	//-----------------------------------------------
		li sp,0x80700000
		nop
	
		
	//-----------------------------------------clear bss

    la s0, __bss_start
	la s1, _end
	move t0, s0
1:
	sw zero, 0(t0)
	addi t0, 4
	bne t0, s1, 1b

	//--------------------------------------------
	nop
	nop

	jal	init_arch  	                                          
	nop  
	nop
	
//==========================================================
#if 1
romReserved:
        b       romReserved
        nop

romExcHandle:
        b       romExcHandle
		nop

ramExcHandle:
		b	ramExcHandle
		nop
#endif		

#define REG32_R(addr,v)  	li t7,addr;\
						lw v, 0(t7);\
						nop;

//==============================================================================
#if 1
flush_cache_r:
	#define START_ADDR 0x80000000
	#define _CACHELINE_SIZE 32
	#define _ICACHE_SIZE		(64 * 1024)		
	#define _DCACHE_SIZE		(32 * 1024)	
	#define _SCACHE_SIZE		(512 * 1024)		

	
#define CACHE_OP( code, type )			( ((code) << 2) | (type) )

#define ICACHE_INDEX_INVALIDATE			CACHE_OP(0x0, 0)
#define ICACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 0)
#define ICACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 0)
#define DCACHE_INDEX_WRITEBACK_INVALIDATE	CACHE_OP(0x0, 1)
#define DCACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 1)
#define DCACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 1)
#define SCACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 3)

#define ICACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 0)
#define ICACHE_ADDR_FILL			CACHE_OP(0x5, 0)
#define ICACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 0)
#define DCACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 1)
#define DCACHE_ADDR_HIT_WRITEBACK_INVALIDATE	CACHE_OP(0x5, 1)
#define DCACHE_ADDR_HIT_WRITEBACK		CACHE_OP(0x6, 1)
#define DCACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 1)

#define SCACHE_ADDR_HIT_WRITEBACK_INVALIDATE	CACHE_OP(0x5, 3)


	#define MTC0_SEL_OPCODE(src, dst, sel)\
	  	.##word (0x40800000 | ((src)<<16) | ((dst)<<11) | (sel));\
		sll zero,zero,1;\
		sll zero,zero,1;\
		sll zero,zero,1;\
		sll zero,zero,1
		
	//----------------------------------------
//init_L1_dcahe:

	MTC0_SEL_OPCODE( 0, 29, 2 )   //R_C0_DTagHi
	MTC0_SEL_OPCODE( 0, 28, 2 )

0:	
	li	a2, START_ADDR
	li  a1,_CACHELINE_SIZE
	li  a3,START_ADDR+_DCACHE_SIZE-_CACHELINE_SIZE


	/* Loop through all lines, invalidating each of them */
1:	

	cache	DCACHE_INDEX_STORE_TAG, 0(a2)	/* clear tag */


	bne	a2, a3, 1b
	addu	a2, a1


	//--------------------------------------------
//init_L1_icahe:	
	MTC0_SEL_OPCODE( 0, 29, 0 )   //R_C0_ITagHi
	MTC0_SEL_OPCODE( 0, 28, 0 )

0:	
	li	a2, START_ADDR
	li  a1,_CACHELINE_SIZE
	li  a3,START_ADDR+_ICACHE_SIZE-_CACHELINE_SIZE


	/* Loop through all lines, invalidating each of them */
1:	

	cache	ICACHE_INDEX_STORE_TAG, 0(a2)	/* clear tag */


	bne	a2, a3, 1b
	addu	a2, a1
	//--------------------------------------------
init_L2_cache:

#ifdef CONFIG_SPI_FLASH
#define __disable_L2_cache    0
#endif

#ifdef CONFIG_NAND_FLASH_BOOTING
#define __disable_L2_cache    0
#endif

#if __disable_L2_cache
	//disable L2 cache
	li t0,(1<<12)
	MTC0_SEL_OPCODE( 8,16,  2);
#else	
#define RTL98_V0 0x8198C000
	nop
        REG32_R(0xb8000000,t8);
        nop
	nop
//	IF_EQ(t8, RTL98_V0, lab_disSC); //jason
        nop
	nop
	li t0,(0<<12)
	MTC0_SEL_OPCODE( 0, 28, 4 )  //R_C0_SelL23TagLo
	nop
	nop
0:
	li	a2, START_ADDR
	li  a1,_CACHELINE_SIZE
	li  a3,START_ADDR+_SCACHE_SIZE-_CACHELINE_SIZE
	

	/* Loop through all lines, invalidating each of them */
1:	

	cache	SCACHE_INDEX_STORE_TAG, 0(a2)	/* clear tag */

	bne	a2, a3, 1b
	addu	a2, a1
	nop
	nop
	j SC_EXIT
lab_disSC:
	//disable L2 cache
	li t0,(1<<12)
	MTC0_SEL_OPCODE( 8,16,  2);
	nop
	nop
	
#endif
SC_EXIT:
	nop
	
	j ra
	nop
    nop


#endif

//==============================================================================





	END(start)

//-------------------------------------------------------------------------
//==============================================================================




__FINIT
