



/*
 * Register access macro
 */
#ifndef REG32
#define REG32(reg)		(*(volatile unsigned int   *)(reg))
#endif
#ifndef REG16
#define REG16(reg)		(*(volatile unsigned short *)(reg))
#endif
#ifndef REG08
#define REG08(reg)		(*(volatile unsigned char  *)(reg))
#endif


//================================

#define Virtual2Physical(x)		(((unsigned int)x) & 0x1fffffff)
#define Physical2Virtual(x)		(((unsigned int)x) | 0x80000000)
#define Virtual2NonCache(x)		(((unsigned int)x) | 0x20000000)
#define Physical2NonCache(x)		(((unsigned int)x) | 0xa0000000)
#define UNCACHE_MASK			0x20000000  


#define SPECIAL_EHB() __asm__ volatile(".word 0x000000c0")
#define SPECIAL_ERET() __asm__ volatile(".word 0x42000018")