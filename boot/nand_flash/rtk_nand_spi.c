#include "./rtkn_nand/rtknflash.h"
#include <nand_spi/soc.h>

int nflashpioread_Page(unsigned int page,unsigned int imageaddr, int len)
{
	winbond_pio_read_data(imageaddr,len,page,0);
	return 0;
}

int nflashpiowrite_Page(unsigned int page,unsigned int imageaddr, int len)
{
	winbond_pio_write_data(imageaddr,len,page,0);
	return 0;
}

int nflash_ecc_decode_func(void* src_addr,void*  dest_addr,void* p_eccbuf,unsigned int pagenum)
{
	winbond_2KB_ecc_encode_buffered((void*)src_addr, (void*)dest_addr,p_eccbuf,pagenum);
	return 0;
}

/* nand scan function, used by rtknflash driver */
static unsigned long __ffs(unsigned long value)
{
        int num = 0;

#if BITS_PER_LONG == 64
        if ((value & 0xffffffff) == 0) {
                num += 32;
                value >>= 32;
        }
#endif
        if ((value & 0xffff) == 0) {
                num += 16;
                value >>= 16;
        }
        if ((value & 0xff) == 0) {
                num += 8;
                value >>= 8;
        }
        if ((value & 0xf) == 0) {
                num += 4;
                value >>= 4;
        }
        if ((value & 0x3) == 0) {
                num += 2;
                value >>= 2;
        }
        if ((value & 0x1) == 0)
                num += 1;
        return num;
}

int nand_scan(struct mtd_info *mtd, int maxchips)
{

	/* probe chip */
	plr_nand_spi_info_t *nand_info = probe_winbond_spi_nand_chip();
	if(nand_info == VZERO){
		printf("cannot find nand spi chip\n");
		return -1;
	}
	/* set mtd nand info */
	unsigned int chunk_size = nand_info->chunk_size;
	unsigned int ppb = nand_info->num_chunk_per_block;
	unsigned int blocknum = nand_info->num_block; 
	struct nand_chip *nand = rtkn->nand_chip;

	/* nand_chip */
	nand->chipsize = blocknum*ppb*chunk_size;
	nand->page_shift = __ffs(chunk_size);
    nand->phys_erase_shift = __ffs(ppb*chunk_size);
    nand->chip_shift =  __ffs(nand->chipsize );
    nand->pagemask = (nand->chipsize >> nand->page_shift) - 1;
    nand->numchips = 1;						//temp code 

	/* MTD */
	mtd->erasesize = ppb*chunk_size;
	mtd->writesize = chunk_size;
	mtd->writebufsize = chunk_size;
	mtd->oobsize = chunk_size/32;			//test code
	mtd->erasesize_shift = nand->phys_erase_shift;
	mtd->writesize_shift =  nand->page_shift;

	/* set chip param */
	rtkn->chip_param.isLastPage = 0;

	prom_printf("\n");
	prom_printf("chipsize=%x\n",nand->chipsize);
	prom_printf("blocksize=%x,\n",mtd->erasesize);
	prom_printf("pagesize=%x,\n",mtd->writesize);
	prom_printf("oobsize=%x,\n",mtd->oobsize);
	
	/* malloc oob_poi */
	nand->oob_poi = (unsigned char*)malloc(mtd->oobsize);
	if(nand->oob_poi == NULL){
		prom_printf("malloc nand->oob_poi fail\n");
		return -1;
	}

	extern unsigned char* rtk_boot_oob_poi;
	rtk_boot_oob_poi = nand->oob_poi;

#if 1
	/* bbt init */
	if(rtkn_scan_bbt(mtd) < 0){
		prom_printf("init bbt failed\n");
		return -1;
	}
#endif
	
	return 0;	
}

/*----------------------------------------------------------------------------------------------------------------*/