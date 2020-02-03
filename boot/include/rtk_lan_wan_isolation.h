#ifndef __RTK_LAN_WAN_ISOLATION__
#define __RTK_LAN_WAN_ISOLATION__

//#define CONFIG_LAN_WAN_ISOLATION 1

#define WRITE_MEM32(addr, val)   (*(volatile unsigned int *) (addr)) = (val)
#define READ_MEM32(addr)         (*(volatile unsigned int *) (addr))
#define WRITE_MEM16(addr, val)   (*(volatile unsigned short *) (addr)) = (val)
#define READ_MEM16(addr)         (*(volatile unsigned short *) (addr))
#define WRITE_MEM8(addr, val)    (*(volatile unsigned char *) (addr)) = (val)
#define READ_MEM8(addr)          (*(volatile unsigned char *) (addr))

#endif /* __RTK_LAN_WAN_ISOLATION__ */

