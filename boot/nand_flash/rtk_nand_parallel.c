#include "./rtkn_nand/rtknflash.h"
#include "rtk_nand_ids.h"
#include "./rtkn_nand/rtkn_parallel_regs.h"


int nflashpioread_Page(unsigned int page,unsigned int imageaddr, int len)
{
//	parallel_nand_pio_read_page(imageaddr,len,page,0);
	return 0;
}

int nflashpiowrite_Page(unsigned int page,unsigned int imageaddr, int len)
{
	
	return 0;
}

int nflash_ecc_decode_func(void* src_addr,void*  dest_addr,void* p_eccbuf,unsigned int pagenum)
{
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
	struct nand_chip *this = rtkn->nand_chip;
	unsigned int device_size=0,page_size,oob_size;
	unsigned int isLastPage;
	unsigned int i;
	unsigned int nand_type_id ;
	int rtk_lookup_table_flag=0;
	unsigned char maker_code;	
	unsigned char device_code; 
	unsigned char B5th;
	unsigned char B6th;
	unsigned int block_size;
	unsigned int num_chips = 1;
	uint64_t chip_size=0;
	unsigned int num_chips_probe = 1;
	uint64_t result = 0;
	uint64_t div_res = 0;
	unsigned int flag_size, mempage_order;

	
	/* read id */
	rtknflash_read_id_cmd(rtkn);
	/* chip sel */
	rtknflash_ops_select_chip(rtkn->mtd,1);

	/* get id_data */	
	maker_code = rtkn->id_data[0];
	device_code = rtkn->id_data[1];
	nand_type_id = maker_code<<24 | device_code<<16 | rtkn->id_data[2]<<8 | rtkn->id_data[3];
	B5th = rtkn->id_data[4];
	B6th = rtkn->id_data[5];

#if 1	
	for (i = 0; nand_device[i].name; i++){
			if ( nand_device[i].id==nand_type_id &&
				((nand_device[i].CycleID5th==0xff)?1:(nand_device[i].CycleID5th==B5th))&&
				((nand_device[i].CycleID6th==0xff)?1:(nand_device[i].CycleID6th==B6th))){
				REG_WRITE_U32( REG_TIME_PARA1,nand_device[i].T1);
				REG_WRITE_U32( REG_TIME_PARA2,nand_device[i].T2);
				REG_WRITE_U32( REG_TIME_PARA3,nand_device[i].T3);

				REG_WRITE_U32(REG_NF_MODE, 0x0);		//asyc mode
				
				if ( nand_type_id != HY27UT084G2M ){
					REG_WRITE_U32( REG_MULTI_CHNL_MODE,0x20);
				}
				#if 0 // sync mode flash		//modify
				if ( nand_type_id == MT29F64G08CBABB ){
					printk("[Phoenix] nand_device[i].name:%s \n", nand_device[i].name);
					rtk_set_sync_mode_micron(0x10);
					REG_WRITE_U32(REG_NF_MODE, 0x2);
					REG_WRITE_U32(REG_TIME_PARA4, 0x2);
					REG_WRITE_U32( REG_MULTI_CHNL_MODE,NF_MULTI_CHNL_MODE_edo(0x00));
				}
				#endif
				printk("[Phoenix] nand_device[i].name:%s \n", nand_device[i].name);
				if (nand_device[i].size == num_chips_probe * nand_device[i].chipsize){
					if ( num_chips_probe == nand_device[i].num_chips ){
						printk("One %s chip has %d die(s) on board with index(%d)\n",
						nand_device[i].name, nand_device[i].num_chips, i);
						
						//mtd->PartNum = nand_device[i].name;
						device_size = nand_device[i].size;
						chip_size = nand_device[i].chipsize;
						page_size = nand_device[i].PageSize;
						block_size = nand_device[i].BlockSize;
						oob_size = nand_device[i].OobSize;
						num_chips = nand_device[i].num_chips;
						isLastPage = nand_device[i].isLastPage;
						rtk_lookup_table_flag = 1;
						
						REG_WRITE_U32( REG_TIME_PARA1,nand_device[i].T1);
						REG_WRITE_U32( REG_TIME_PARA2,nand_device[i].T2);
						REG_WRITE_U32( REG_TIME_PARA3,nand_device[i].T3);

						printk("index(%d) nand part=%s, id=0x%x, device_size=0x%x, page_size=0x%x, block_size=0x%x, oob_size=0x%x, num_chips=0x%x, isLastPage=0x%x, ecc_sel=0x%x \n",
							i,
							nand_device[i].name,
							nand_device[i].id,
							device_size,
							page_size,
							block_size,
							oob_size,
							num_chips,
							isLastPage,
							nand_device[i].eccSelect);
						
						break;
					}
				}
				else{
					if ( !strcmp(nand_device[i].name, "HY27UF084G2M" ) )
						continue;
					else{
						printk("We have %d the same %s chips on board\n",
							num_chips_probe/nand_device[i].num_chips, nand_device[i].name);
						//mtd->PartNum = nand_device[i].name;
						device_size = nand_device[i].size;
						chip_size = nand_device[i].chipsize;
						page_size = nand_device[i].PageSize;
						block_size = nand_device[i].BlockSize;
						oob_size = nand_device[i].OobSize;
						num_chips = nand_device[i].num_chips;
						isLastPage = nand_device[i].isLastPage;
						rtk_lookup_table_flag = 1;
						printk("nand part=%s, id=%x, device_size=%llu, chip_size=%llu, num_chips=%d, isLastPage=%d, eccBits=%d\n",
							nand_device[i].name, nand_device[i].id, nand_device[i].size, nand_device[i].chipsize,
							nand_device[i].num_chips, nand_device[i].isLastPage, nand_device[i].eccSelect);
						break;
					}
				}
			}
		}
#endif

	if(nand_device[i].name == NULL){
		printf("nand flash chip cannot find\n");
		return -1;
	}

	this->page_shift = __ffs(page_size); 
	this->phys_erase_shift = __ffs(block_size);
	this->chipsize = chip_size;
	this->chip_shift =  __ffs(this->chipsize );
	this->pagemask = (this->chipsize >> this->page_shift) - 1;
	this->numchips = 1;	
	
	
	mtd->oobsize  = oob_size;
	mtd->writesize = page_size;//add by alexchang 0923-2010
	mtd->erasesize = block_size;
	mtd->writebufsize = page_size;
	mtd->erasesize_shift = this->phys_erase_shift;
	mtd->writesize_shift = this->page_shift;

#if 0
	rtkn->ecc_select = nand_device[i].eccSelect;
	if(is_jupiter_cpu())
	{
		if(rtkn->ecc_select ==0x18)//ecc bit 24 bit
		{
    			printk("Jupiter does NOT support this flash!!!\n");
    			return -1;
		  	}
	}
#endif

	prom_printf("\n");
	prom_printf("chipsize=%x\n",this->chipsize);
	prom_printf("blocksize=%x,\n",mtd->erasesize);
	prom_printf("pagesize=%x,\n",mtd->writesize);
	prom_printf("oobsize=%x,\n",mtd->oobsize);
	prom_printf("ecc_seletc=%x,\n",rtkn->chip_param.ecc_select);
	
	/* mtd releate; not need */
#if 0
	if(this->ecc_select>=0x18)
		mtd->ecc_strength = 1024;
	else
		mtd->ecc_strength = 512;
#endif

	this->oob_poi = (unsigned char*)malloc(mtd->oobsize);
	if(this->oob_poi == NULL){
		prom_printf("malloc nand->oob_poi fail\n");
		return -1;
	}

	extern unsigned char* rtk_boot_oob_poi;
	rtk_boot_oob_poi = this->oob_poi;
	
	//printf("%s:%d\n",__func__,__LINE__);
#if 1
	/* bbt init */
	if(rtkn_scan_bbt(mtd) < 0){
		prom_printf("init bbt failed\n");
		return -1;
	}
#endif
	//printf("%s:%d\n",__func__,__LINE__);
	return 0;

}
/*----------------------------------------------------------------------------------------------------------------*/
