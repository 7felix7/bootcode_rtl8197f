/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999 by Ralf Baechle
 * Modified for R3000 by Paul M. Antoine, 1995, 1996
 * Complete output from die() by Ulf Carlsson, 1998
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999 by Ralf Baechle
 * Modified for R3000 by Paul M. Antoine, 1995, 1996
 * Complete output from die() by Ulf Carlsson, 1998
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/config.h>
#include <linux/linkage.h>
#include <asm/ptrace.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/system.h>
#define printf	prom_printf

#if 0
#ifdef CONFIG_RTL865XC
/* For Realtek RTL865XC Network platform series */
#define _ICACHE_SIZE		(16 * 1024)		/* 16K bytes */
#define _DCACHE_SIZE		(8 * 1024)		/* 8K bytes */
#define _CACHE_LINE_SIZE	4			/* 4 words */
#endif

void _flush_dcache_()
{
	/* Flush D-Cache using its range */
	unsigned char *p;
	unsigned int size;
	unsigned int flags;
	unsigned int i;

	unsigned int start=KSEG0;
	unsigned int end=KSEG0 + _DCACHE_SIZE;

	size = end - start;

	/* correctness check : flush all if any parameter is illegal */
// david	
//	if (	(size >= dcache_size) ||
	if (	(size >= _DCACHE_SIZE) ||

		(KSEGX(start) != KSEG0)	)
	{
		/*
		 *	ghhguang
		 *		=> For Realtek Lextra CPU,
		 *		   the cache would NOT be flushed only if the Address to-be-flushed
		 *		   is the EXPLICIT address ( which is really stored in that cache line ).
		 *		   For the aliasd addresses, the cache entry would NOT be flushed even
		 *		   it matchs same cache-index.
		 *
		 *		   => This is different from traditional MIPS-based CPU's configuration.
		 *		      So if we want to flush ALL-cache entries, we would need to use "mtc0"
		 *		      instruction instead of simply modifying the "size" to "dcache_size"
		 *		      and "start" to "KSEG0".
		 *
		 */
		__asm__ volatile(
			"mtc0 $0,$20\n\t"
			"nop\n\t"
			"li $8,512\n\t"
			"mtc0 $8,$20\n\t"
			"nop\n\t"
			"nop\n\t"
			"mtc0 $0,$20\n\t"
			"nop"
			: /* no output */
			: /* no input */
				);
	} else
	{
		/* Start to isolate cache space */
		p = (char *)start;

		flags = read_32bit_cp0_register();

		/* isolate cache space */
		write_32bit_cp0_register(CP0_XCONTEXT, (ST0_ISC | flags) &~ ST0_IEC );

		for (i = 0; i < size; i += 0x040)
		{
			asm ( 	
				"cache 0x15, 0x000(%0)\n\t"
			 	"cache 0x15, 0x010(%0)\n\t"
			 	"cache 0x15, 0x020(%0)\n\t"
			 	"cache 0x15, 0x030(%0)\n\t"
				:		/* No output registers */
				:"r"(p)		/* input : 'p' as %0 */
				);
			p += 0x040;
		}

		write_32bit_cp0_register(CP0_XCONTEXT, flags);
	}       
}

void _flush_icache_()
{
	unsigned int start=KSEG0;
	unsigned int end=KSEG0 + _ICACHE_SIZE;

	/*
		Flush data cache at first in write-back platform.

		Ghhuang (2007/3/9):

		RD-Center suggest that we need to flush D-cache entries which
		might match to same address as I-cache ... when we flush
		I-cache.
		( Maybe some data is treated as data/instruction, both. )
	*/
	_flush_dcache_();

	/*Invalidate I-Cache*/
	__asm__ volatile(
		"mtc0 $0,$20\n\t"
		"nop\n\t"
		"li $8,2\n\t"
		"mtc0 $8,$20\n\t"
		"nop\n\t"
		"nop\n\t"
		"mtc0 $0,$20\n\t"
		"nop"
		: /* no output */
		: /* no input */
			);
}


void flush_cache(void)
{
	_flush_dcache_();
	_flush_icache_();	
}

#endif
//==============================================================================

/* Wei add, modify from RDC init_cache.S */

#include <asm/mipsregs.h>
#include <asm/cacheops.h>



#define CONFIG_SYS_CACHELINE_SIZE 32
#define _ICACHE_SIZE		(64 * 1024)		
#define _DCACHE_SIZE		(32 * 1024)		
#define _SCACHE_SIZE		(512 * 1024)		


//==============================================================================
#define cache_op(op,addr)						\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	noreorder				\n"	\
	"	.set	mips3\n\t				\n"	\
	"	cache	%0, %1					\n"	\
	"	.set	pop					\n"	\
	:								\
	: "i" (op), "R" (*(unsigned char *)(addr)))



//==============================================================================
void flush_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	while (1) 
	{
		cache_op(HIT_WRITEBACK_INV_D, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}
//==============================================================================
//==============================================================================
void flush_cache_range(ulong start_addr, ulong size)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	/* aend will be miscalculated when size is zero, so we return here */
	if (size == 0)
		return;

	while (1) 
	{
		cache_op(HIT_WRITEBACK_INV_D, addr);
		cache_op(HIT_INVALIDATE_I, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}

void flush_icache(ulong start_addr, ulong size)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	/* aend will be miscalculated when size is zero, so we return here */
	if (size == 0)
		return;

	while (1) 
	{
		cache_op(INDEX_INVALIDATE_I, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}
void flush_dcache(ulong start_addr, ulong size)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	/* aend will be miscalculated when size is zero, so we return here */
	if (size == 0)
		return;

	while (1) 
	{
		cache_op(INDEX_WRITEBACK_INV_D, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}


//==============================================================================
void invalidate_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	while (1) 
	{
		cache_op(HIT_INVALIDATE_D, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}
//==============================================================================
void invalidate_icache_range(ulong start_addr, ulong stop)  //wei add
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	while (1) 
	{
		cache_op(HIT_INVALIDATE_I, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}
//==============================================================================

void invalidate_cache(void)   //wei add
{
	#define START_ADDR 0x80000000
	invalidate_dcache_range(START_ADDR, START_ADDR+_DCACHE_SIZE);
	invalidate_icache_range(START_ADDR, START_ADDR+_ICACHE_SIZE);	
}
//==============================================================================

void flush_cache(void)   //wei add
{
	#define START_ADDR 0x80000000
	//flush_cache_range(START_ADDR, _ICACHE_SIZE);
	//flush_dcache_range(START_ADDR, START_ADDR+_DCACHE_SIZE);	
	//winfred_wang
	//invalidate_cache();
	flush_dcache(START_ADDR,_DCACHE_SIZE);
	flush_icache(START_ADDR,_ICACHE_SIZE);
}
//==============================================================================

