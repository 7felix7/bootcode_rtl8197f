#ifndef __RTL_START_H__
#define __RTL_START_H__

#include "../autoconf.h"
#include <bspchip.h>

#ifdef CONFIG_NONO_COMPRESS
        #define	BOOT_ADDR		0xA0000000    // no compress
#else
        #define	BOOT_ADDR		0xa0100000	//compress
#endif


// Using register: t6, t7           //wei add this code
#define IF_EQ(a,b,lab)          or t6,zero,a;\
                                or t7,zero,b;\
                                beq t6,t7,lab;\
                                nop;

#define IF_NEQ(a,b,lab)         or t6,zero,a;\
                                or t7,zero,b;\
                                bne t6,t7,lab;\
                                nop;



#define  ADDR   0xA0380000
	#define  PATT0  0x5a0000a5
	#define  PATT1  0xA5A55A5A
	#define  PATT2  0x005AA500
	#define  PATT3  0xA5A55A5A
	#define  PATT4  0xAAAAAAAA
	#define  PATT5  0xffffffff
	#define  PATT6  0x00000000
	#define  PATT7  0x00000000
	#define  PATT8  0xffffffff
	#define CLK_MANAGER 0xb8000010
	#define DCR 0xb8001004


#define DRAM_DDR2 1
	#define DRAM_DDR3 2

	#define GET_BITVAL(v,bitpos,pat) ((v& ((unsigned int)pat<<bitpos))>>bitpos)
	#define RANG1 1
	#define RANG2 3
	#define RANG3  7
	#define RANG4 0xf		
	#define RANG5 0x1f	
	

	#define SYS_HW_STRAP   (0xb8000000 +0x08)	


#define SRAM_REG_SET_SIZE	(0x10)
#define SRAM_REG_SET_NUM	(4)
#define SRAM_REG_BASE_ADDR	(0xB8004000)



#define C0SRAMSAR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x0)
#define C0SRAMSSR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x4)
#define C0SRAMSBR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x8)

#define C1SRAMSAR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x40)
#define C1SRAMSSR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x44)
#define C1SRAMSBR_REG_ADDR	(SRAM_REG_BASE_ADDR + 0x48)

#define SRAM_SEG_ENABLE		(0x1)

#define SRAM_SIZE_256B		(0x1)
#define SRAM_SIZE_512B		(0x2)
#define SRAM_SIZE_1KB		(0x3)
#define SRAM_SIZE_2KB		(0x4)
#define SRAM_SIZE_4KB		(0x5)
#define SRAM_SIZE_8KB		(0x6)
#define SRAM_SIZE_16KB		(0x7)
#define SRAM_SIZE_32KB		(0x8)
#define SRAM_SIZE_64KB		(0x9)
#define SRAM_SIZE_128KB		(0xA)
#define SRAM_SIZE_256KB		(0xB)
#define SRAM_SIZE_512KB		(0xC)
#define SRAM_SIZE_1MB		(0xD)



//-------------------------------------------------
// Using register: t6, t7
// t6=value
// t7=address
#define REG32_R(addr,v)  	li t7,addr;\
						lw v, 0(t7);\
						nop;

#ifdef CONFIG_NAND_FLASH_BOOTING
#define REG32_W(addr,v) 		or t6,zero,v;\
					li t7,addr;\
					sw t6, 0(t7);\
					nop;
#else
// Using register: t6, t7           value support "constant" and "register" access, so use "or" to instead "li"
#define REG32_W(addr,v) 		li t6,v;\
					li t7,addr;\
					sw t6, 0(t7);\
					nop;
#endif

#define REG32_ANDOR(addr,andV,orV) 	 li t7,addr;\
				         lw t6, 0(t7);\
				         and t6,t6,andV;\
				         or t6,t6,orV;\
				         sw t6, 0(t7);\
				         nop;
									   
//---------------------------------------
//#define SYS_CLK_RATE	  	(200000000) 	
//#define SYS_CLK_RATE	  	(  33868800)      //33.8688MHz
//#define SYS_CLK_RATE	  	(  20000000)      //20MHz

#if 0//def CONFIG_FPGA_ROMCODE
//#define SYS_CLK_RATE	  	(  40000000)      //40MHz
#define SYS_CLK_RATE	  	(  25000000)      //25MHz
//#define SYS_CLK_RATE	  	(  33868800)      //25MHz
#else
#define SYS_CLK_RATE	  	( 200000000)      //200MHz
#endif


#define BAUD_RATE	  	(38400) 

//Using reg: t6,t7
#define UART_WRITE(c)		  1:   REG32_R(BSP_UART0_LSR,t6);\
								and t6,t6,0x60;\
								beqz		t6,  1b;\
								nop	   ;\
								REG32_W(BSP_UART0_THR, c);

//----------------------------------------------------


// Using register: t5, t6, t7     t5=msg(idx)
#define UART_PRINT(msg)		la   t5,msg;\
								1:	lbu  t6,0(t5);\
									beqz		t6,  2f;\
									addu	t5, 1;\
									              ;\
					                                li t7,BSP_UART0_THR;\
					                                sw t6, 0(t7);\
									j 1b;\
									nop;\
								2:	


#ifdef CONFIG_NAND_FLASH_BOOTING
 /*For showing NAND Flash booting DMA times*/
#define UART_PRINT_DELAY(msg)		la   s5,msg;\
								1:	lbu  s6,0(s5);\
									beqz		s6,  2f;\
									addu	s5, 1;\
									;\
									sll s6,s6,24;\
									REG32_W(UART_THR, s6);\
									li s4,800;\
								3:	nop;\
									subu s4,s4,1;\
									bnez s4,3b;\
									nop	;\
									j 1b;\
									nop;\
								2:	
#else
// Using register: t4, t5, t6, t7     t5=msg(idx), t4=delay loop count
#define UART_PRINT_DELAY(msg)		la   t5,msg;\
								1:	lbu  t6,0(t5);\
									beqz		t6,  2f;\
									addu	t5, 1;\
									;\
									sll t6,t6,24;\
									REG32_W(UART_THR, t6);\
									li t4,0x100;\
								3:	nop;\
									subu t4,t4,1;\
									bnez t4,3b;\
									nop	;\
									j 1b;\
									nop;\
								2:	
#endif

//0x00 show ascii '0'
//0x0a show ascii 'a'
//0x1a show ascii 'a', skip 1
#define UART_BIN2HEX(v) 	or t6,zero,v;\
	                                          and t6,t6,0x000f;\
							li t7,'0';\
							add t6,t6,t7;\
							li t7,'9';\
							bleu t6,t7,1f;\
							nop;\
							li t7,'a'-'9'-1;\
							add t6,t6,t7;\
						1:;\
							sll t6,t6,24;\
							REG32_W(UART_THR, t6);






#define VIR2PHY(x) (x&0x1fffffff)

//#define SRAM_BASE (0xbfc00000+0x8000)	//ROM Booting	//24K		
//#define SRAM_BASE (0x80000000)	//SPI NOR
//#define SRAM_BASE (0xbfc00000)	//NFBI NAND							
//#define SRAM_BASE (0x80000000+(64<<20))  //32M



//#define SRAM_TOP (SRAM_BASE+0x1000)  //4K
//#define SRAM_TOP (SRAM_BASE+0x2000)  //8K

#define MFC0_SEL(dst, src, sel)  .word (0x40000000 | ((dst) << 16) | ((src) << 11) | (sel))
#define MTC0_SEL(dst, src, sel)  .word (0x40800000 | ((dst) << 16) | ((src) << 11) | (sel))


//----------------------------------------------------
#endif

