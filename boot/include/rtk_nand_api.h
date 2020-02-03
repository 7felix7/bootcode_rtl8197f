#ifndef __RTK_NAND_SPI_H__
#define __RTK_NAND_SPI_H__


int nflashwrite(unsigned long dst, unsigned long src, unsigned long length);
int nflashread (unsigned long dst, unsigned int src, unsigned long length,int checkEsc);
int nflasherase(unsigned int addr, unsigned int len);
int nflashscanbbt(void);
int nflashprobe(void);
int nflashpioread(unsigned int flashaddr,unsigned int imageaddr,unsigned int len);
int nflashpiowrite(unsigned int flashaddr,unsigned int imageaddr,unsigned int len);
void nflashblockcheck(unsigned int blockstart,unsigned int blockend);


#define DRAM_DIMAGE_ADDR       		0xa1000000
#define DRAM_DOOB_ADDR           	0xa1000000

#define printf_test prom_printf
#endif