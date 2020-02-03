
#ifndef _RTL8196_H
#define _RTL8196_H


/*Cyrus Tsai*/
#ifndef __ASSEMBLY__
typedef unsigned long Int32;  
typedef unsigned short Int16;  
typedef unsigned char Int8;  
//
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef char CHAR8;
typedef unsigned char UCHAR8;

#define WRITE_MEM32(addr, val)   (*(volatile unsigned int *) (addr)) = (val)
#define READ_MEM32(addr)         (*(volatile unsigned int *) (addr))
#define WRITE_MEM16(addr, val)   (*(volatile unsigned short *) (addr)) = (val)
#define READ_MEM16(addr)         (*(volatile unsigned short *) (addr))
#define WRITE_MEM8(addr, val)    (*(volatile unsigned char *) (addr)) = (val)
#define READ_MEM8(addr)          (*(volatile unsigned char *) (addr))



#define Virtual2Physical(x)		(((Int32)x) & 0x1fffffff)
#define Physical2Virtual(x)		(((Int32)x) | 0x80000000)
#define Virtual2NonCache(x)		(((Int32)x) | 0x20000000)
#define Physical2NonCache(x)		(((Int32)x) | 0xa0000000)
#define UNCACHE_MASK			0x20000000  //wei add

#endif

#ifndef TRUE
	#define TRUE		0x01
	#define FALSE	0x0
#endif	

#define BIT(x)	(1 << (x))

#define rtl_inb(offset)		(*(volatile unsigned char *)(mips_io_port_base + offset))
#define rtl_inw(offset)	(*(volatile unsigned short *)(mips_io_port_base + offset))
#define rtl_inl(offset)		(*(volatile unsigned long *)(mips_io_port_base + offset))

#define rtl_outb(offset,val)	(*(volatile unsigned char *)(mips_io_port_base + offset) = val)
#define rtl_outw(offset,val)	(*(volatile unsigned short *)(mips_io_port_base + offset) = val)
#define rtl_outl(offset,val)	(*(volatile unsigned long *)(mips_io_port_base + offset) = val)



#define mips_io_port_base (0xB8000000)

// For Interrupt Controller
#define GIMR0		0x3000
#define GISR		0x3004
#define IRR0		0x3008
#define IRR1		0x300c
#define IRR2		0x3010
#define IRR3		0x3014

/*Replace the 0xbfc0 0000 with 0xB0000000      */
#define FLASH_BASE 		0xB0000000

//-----------------------------------------------------------------------
/* Register access macro
*/
#if 0
#define REG32(reg) (*(volatile unsigned int *)(reg))
#define REG8(reg) (*(volatile unsigned char *)(reg))
#endif

#define SYS_BASE					mips_io_port_base

#define MEM_CONTROLLER_REG						(SYS_BASE+0x1000)	// memory contoller register
#define MCR_REG                            		(0x000 + MEM_CONTROLLER_REG)
#define DCR_REG                            		(0x004 + MEM_CONTROLLER_REG)
#define DTR_REG                            		(0x008 + MEM_CONTROLLER_REG)
#define MPMR_REG                            	(0x040 + MEM_CONTROLLER_REG)
#define DDCR_REG                            	(0x050 + MEM_CONTROLLER_REG)

/* Timer control registers 
*/
#define GICR_BASE								(SYS_BASE+0x3000)
#define TC0DATA_REG                             (0x100 + GICR_BASE)       /* Timer/Counter 0 data */
#define TC1DATA_REG                             (0x104 + GICR_BASE)       /* Timer/Counter 1 data */
#define TC0CNT_REG                              (0x108 + GICR_BASE)       /* Timer/Counter 0 count */
#define TC1CNT_REG                              (0x10C + GICR_BASE)       /* Timer/Counter 1 count */
#define TCCNR_REG                               (0x110 + GICR_BASE)       /* Timer/Counter control */
#define TCIR_REG                                (0x114 + GICR_BASE)       /* Timer/Counter intertupt */
#define CDBR_REG                                (0x118 + GICR_BASE)       /* Clock division base */
#define WDTCNR_REG                              (0x11C + GICR_BASE)       /* Watchdog timer control */


/* Global interrupt control registers 
*/
#define GIMR_REG                                (0x000 + GICR_BASE)       /* Global interrupt mask */
#define GISR_REG                                (0x004 + GICR_BASE)       /* Global interrupt status */
#define IRR_REG                                 (0x008 + GICR_BASE)       /* Interrupt routing */
#define IRR1_REG                                (0x00C + GICR_BASE)       /* Interrupt routing */
#define IRR2_REG                                (0x010 + GICR_BASE)       /* Interrupt routing */
#define IRR3_REG                                (0x014 + GICR_BASE)       /* Interrupt routing */


#define HW_STRAP_REG 0xb8000008
#define BIST_CONTROL_REG 0xb8000014
#define BIST_DONE_REG 0xb8000018
#define BIST_FAIL_REG 0xb800001C



#define ALL_PORT_MASK	0x3F


//---------------------------------------------------


//-----------------------------------------------------------------------
extern int dprintf(char *fmt, ...);
#define printf dprintf
#define prom_printf dprintf

#define CONFIG_RTL865XC 1



#endif
