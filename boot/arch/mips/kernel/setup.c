

//===============================================================================
//
// This code can be run un-cache address.
//
//================================================================================


#include <asm/asm.h>
#include <asm/bootinfo.h>
#include <asm/cachectl.h>
#include <asm/io.h>
#include <asm/stackframe.h>
#include <asm/system.h>
#include <asm/cpu.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>

#include <asm/rtl8198c.h>



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


//============================================================================
void init_icache()
{
	#define KSEG0BASE                 0x80000000
	#define CONFIG_SYS_CACHELINE_SIZE 32

	unsigned int t;
	t=0;
	write_32bit_cp0_register_sel( 29,t, 0);  //CP0_ITagHi
	write_32bit_cp0_register_sel( 28,t, 0);  //CP0_ITagLo

	
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = KSEG0BASE;
	unsigned long aend = addr+(64<<10)-lsize;   //DCACHE=64K

	while (1) 
	{
		cache_op(INDEX_STORE_TAG_I, addr);
		if (addr >= aend)
			break;
		addr += lsize;
	}

}
//==============================================================================
void init_dcache()
{
	#define KSEG0BASE                 0x80000000
	#define CONFIG_SYS_CACHELINE_SIZE 32

	unsigned int t;
	t=0;
	write_32bit_cp0_register_sel( 29, t, 2);  //CP0_DTagHi
	write_32bit_cp0_register_sel( 28, t, 2);  //CP0_DTagLo

	
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = KSEG0BASE;
	unsigned long aend = addr+(32<<10)-lsize;   //DCACHE=32K

	while (1) 
	{
		cache_op(INDEX_STORE_TAG_D, addr);
		if (addr >= aend)
			break;
		addr += lsize;
	}

}

//============================================================================

init_cpu_config()
{
	unsigned int s;

#if 0
	//setting K23, KU, MM
	s = read_32bit_cp0_register(CP0_CONFIG);
	s &= ~((3<<28) | (3<<25) | (1<<18));
	s |=  ((3<<28) | (3<<25) | (1<<18));
	write_32bit_cp0_register(CP0_CONFIG, s);
#endif

#if CONFIG_RTL8198C
	//set_cp0_config(0x07, 2); //2:uncache
	//set_cp0_config(0x07, 3); //3:cache
#endif

	
	s = read_32bit_cp0_register(CP0_STATUS);
	s &= ~(ST0_CU1|ST0_CU2|ST0_CU3);
	s |= ST0_CU0;
	write_32bit_cp0_register(CP0_STATUS, s);

	SPECIAL_EHB();

	//---------------------------------------------config2
#if 0	
	//disable L2
	s=(1<<12);
	write_32bit_cp0_register_sel(16, s, 2);
#endif


	//----------------------------------------------config 7
#if 0	
	s = read_32bit_cp0_register_sel(16, 7);
	//s &= ~((3<<28) | (3<<25) | (1<<18));
	s |=  (1<<29);
	write_32bit_cp0_register_sel(16, s,7);
#endif
	

//	sys_init_icache();
//	sys_init_dcache();	
}

//============================================================================
typedef void (jmp_func_t)(void);

asmlinkage void init_arch(int argc, char **argv, char **envp, int *prom_vec)
{
	//init_icache();
	//init_icache();		
	
	init_cpu_config();	

   // start_kernel();
	jmp_func_t *jmp;
   	extern void start_kernel(void);

	//jmp=((int)start_kernel)& ~UNCACHE_MASK;
	jmp = (jmp_func_t *)(((int)start_kernel)& ~UNCACHE_MASK);
	jmp();
}


//============================================================================


