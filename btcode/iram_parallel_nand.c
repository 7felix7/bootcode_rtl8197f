#include "../boot/nand_flash/rtkn_nand/rtkn_parallel_regs.h"
#include "start.h"

#if 0
extern char  __boot_start[];
extern char  __boot_end[];
#endif

#define IMAGE_START			(0x0)
#define IMAGE_OFFSET		(0x40000)
#define IMAGE_SIZE			(0x40000)
#define BACK_IAMGE_NUM		4
#define BOOT_START_ADDR		(0xA0500000)

#define VIR_TO_PHY(x) (x & (~0xa0000000))
#define smp_wmb()
typedef unsigned int size_t;

#ifndef CONFIG_CUSTOMER_BOOT_SIGNATURE_SUPPORT
#define BOOT_SIGNATURE		"boot"
#else
#define BOOT_SIGNATURE		CONFIG_CUSTOMER_BOOT_SIGNATURE
#endif
#define SIGNATURE_LEN		4

typedef struct img_header {
	unsigned char signature[SIGNATURE_LEN];
	unsigned int startAddr;
	unsigned int burnAddr;
	unsigned int len;
}IMG_HEADER_T, *IMG_HEADER_Tp;

void * memcpy(void * dest,const void *src,int count)
{
        char *tmp = (char *) dest, *s = (char *) src;

        while (count--)
                *tmp++ = *s++;

        return dest;
}

int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	signed char res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}


void WAIT_DONE(unsigned int addr, unsigned int mask, unsigned int value)
{
	while ( (REG_READ_U32(addr) & mask) != value )
	{
		asm("nop");
		smp_wmb();
	}
}

#define GPIO_PABCD_CNR 0xb8003500
#define GPIO_PABCD_DIR 0xb8003508
#define GPIO_PABCD_DAT 0xb800350c

static void WP_HIGH(void)
{
	REG_WRITE_U32(GPIO_PABCD_CNR, (REG_READ_U32(GPIO_PABCD_CNR) & 0xFFFFBFFF) | (0<<14)); //set GPIOB[6] as GPIO pin
	REG_WRITE_U32(GPIO_PABCD_DIR, (REG_READ_U32(GPIO_PABCD_DIR) & 0xFFFFBFFF) | (1<<14)); //set as output pin
	REG_WRITE_U32(GPIO_PABCD_DAT, (REG_READ_U32(GPIO_PABCD_DAT) & 0xFFFFBFFF) | (1<<14)); //set high
}

static void WP_LOW(void)
{
	REG_WRITE_U32(GPIO_PABCD_CNR, (REG_READ_U32(GPIO_PABCD_CNR) & 0xFFFFBFFF) | (0<<14)); //set GPIOB[6] as GPIO pin
	REG_WRITE_U32(GPIO_PABCD_DIR, (REG_READ_U32(GPIO_PABCD_DIR) & 0xFFFFBFFF) | (1<<14)); //set as output pin
	REG_WRITE_U32(GPIO_PABCD_DAT, (REG_READ_U32(GPIO_PABCD_DAT) & 0xFFFFBFFF) | (0<<14)); //set low
}

static int rtk_nand_register_init()
{
	/* enable parallel nand clock */
	REG_WRITE_U32(REG_CLK_MANAGE2,(REG_READ_U32(REG_CLK_MANAGE2) & 0xFFBFFFFF | (1<<21)));
	/* set pin mux */
	REG_WRITE_U32(REG_PIN_MUX00,REG_READ_U32(REG_PIN_MUX00) & (~0xffff0000));
	REG_WRITE_U32(REG_PIN_MUX01,REG_READ_U32(REG_PIN_MUX01) & (~0xffff0000));
	REG_WRITE_U32(REG_PIN_MUX02,REG_READ_U32(REG_PIN_MUX02) & (~0xffff000));
	REG_WRITE_U32(REG_PINMUX_15,REG_READ_U32(REG_PINMUX_15) & (~0xffff0000));

	REG_WRITE_U32(REG_PIN_MUX00,REG_READ_U32(REG_PIN_MUX00) | 0x21110000);
	REG_WRITE_U32(REG_PIN_MUX01,REG_READ_U32(REG_PIN_MUX01) | 0x12220000);
	REG_WRITE_U32(REG_PIN_MUX02,REG_READ_U32(REG_PIN_MUX02) | 0x1121000);
	REG_WRITE_U32(REG_PINMUX_15,REG_READ_U32(REG_PINMUX_15) | 0x44440000);
	
	REG_WRITE_U32(REG_SPR_DDR_CTL, NF_SPR_DDR_CTL_spare_ddr_ena(1) | NF_SPR_DDR_CTL_per_2k_spr_ena(1)| NF_SPR_DDR_CTL_spare_dram_sa(0));
	REG_WRITE_U32( REG_PD,~(0x1 << 0) );
	REG_WRITE_U32( REG_ND_CMD,0xFF);
	REG_WRITE_U32( REG_ND_CTL,(NF_ND_CTL_xfer(0x01) | NF_ND_CTL_tran_mode(0)) );

	WAIT_DONE(REG_ND_CTL,0x80,0);
	WAIT_DONE(REG_ND_CTL,0x40,0x40);

	REG_WRITE_U32( REG_MULTI_CHNL_MODE,0x00);
	REG_WRITE_U32( REG_TIME_PARA1,NF_TIME_PARA1_T1(0x01));
	REG_WRITE_U32( REG_TIME_PARA2,NF_TIME_PARA2_T2(0x01));
	REG_WRITE_U32( REG_TIME_PARA3,NF_TIME_PARA3_T3(0x01));

	REG_WRITE_U32( REG_DELAY_CTL,NF_DELAY_CTL_T_WHR_ADL(0x09) );

	REG_WRITE_U32( REG_MULTI_CHNL_MODE,NF_MULTI_CHNL_MODE_edo(0x00));
	REG_WRITE_U32( REG_ECC_STOP,NF_ECC_STOP_ecc_n_stop(0x01));

	REG_WRITE_U32( REG_ND_CA0,0);
	REG_WRITE_U32( REG_ND_CA1,0);
	
	/* enable wp */
	WP_HIGH();
	
	return 0;
}

int parallel_nand_read(unsigned char* buff,unsigned int offset,unsigned int end)
{
	int dram_sa,oob_sa,dma_len,page_len;
	int page_size = CONFIG_NAND_PAGE_SIZE;
	unsigned int pagenum,pagestart,pageend;
	unsigned char addr_mode = 1,auto_trigger_mode = 2;;
	unsigned int is_sync_mode;
	unsigned char *oobBuf = buff + 0x100000;
	unsigned char* buf = buff;

	if((offset % page_size) != 0){
		//prom_printf("offset size not page aligned\n");
		return -1;
	}
	if(((unsigned int)buff % page_size) != 0){
		//prom_printf("buff addr not page aligned\n");
		return -1;
	}

	pagestart = (offset+CONFIG_NAND_PAGE_SIZE - 1)/CONFIG_NAND_PAGE_SIZE;
	pageend = (end+CONFIG_NAND_PAGE_SIZE - 1)/CONFIG_NAND_PAGE_SIZE;
	
	for(pagenum = pagestart;pagenum<pageend;pagenum++){
		while(1){					
			REG_WRITE_U32(REG_DATA_TL0,NF_DATA_TL0_length0(0));
			{
				// set random read

				REG_WRITE_U32(REG_RND_EN, 1);
				REG_WRITE_U32(REG_RND_CMD1, 0x5);
				REG_WRITE_U32(REG_RND_CMD2, 0xe0);
				REG_WRITE_U32(REG_RND_DATA_STR_COL_H, 0);	// data start address MSB (always 0)
				REG_WRITE_U32(REG_RND_SPR_STR_COL_H, page_size >> 8);	// spare start address MSB
				REG_WRITE_U32(REG_RND_SPR_STR_COL_L, page_size & 0xff); // spare start address LSB
				REG_WRITE_U32(REG_RMZ_CTRL, 0);
				
				page_len = page_size >> 9;
				REG_WRITE_U32(REG_DATA_TL1,NF_DATA_TL1_access_mode(1)|NF_DATA_TL1_length1(2));
			}

			REG_WRITE_U32(REG_PAGE_LEN,NF_PAGE_LEN_page_len(page_len));

			//Set PP
			REG_WRITE_U32(REG_READ_BY_PP,NF_READ_BY_PP_read_by_pp(1));
			REG_WRITE_U32(REG_PP_CTL1,NF_PP_CTL1_pp_start_addr(0));
			REG_WRITE_U32(REG_PP_CTL0,0);

			// enable blank check
			//REG_WRITE_U32(REG_BLANK_CHK,NF_BLANK_CHK_blank_ena(1) );		//need check

			//Set command
			REG_WRITE_U32(REG_ND_CMD,NF_ND_CMD_cmd(CMD_PG_READ_C1));
			REG_WRITE_U32(REG_CMD2,NF_CMD2_cmd2(CMD_PG_READ_C2));
			REG_WRITE_U32(REG_CMD3,NF_CMD3_cmd3(CMD_PG_READ_C3));

			//Set address
			REG_WRITE_U32( REG_ND_PA0,NF_ND_PA0_page_addr0( 0xff&pagenum ));
			REG_WRITE_U32( REG_ND_PA1,NF_ND_PA1_page_addr1( 0xff&(pagenum>>8) ));
			REG_WRITE_U32( REG_ND_PA2,NF_ND_PA2_addr_mode(addr_mode)|NF_ND_PA2_page_addr2( 0x1f&(pagenum>>16) ));
			REG_WRITE_U32( REG_ND_PA3,NF_ND_PA3_page_addr3( 0x7&(pagenum>>21) ));

			REG_WRITE_U32(REG_ND_CA0, 0);
			REG_WRITE_U32(REG_ND_CA1, 0);

			is_sync_mode = (REG_READ_U32(REG_NF_MODE) & 0x2);
			//Set ECC

			REG_WRITE_U32( REG_MULTI_CHNL_MODE,NF_MULTI_CHNL_MODE_edo(1));//add by alexchang0205-2010.
			if(is_sync_mode == 0x2)
				REG_WRITE_U32(REG_MULTI_CHNL_MODE,REG_READ_U32(REG_MULTI_CHNL_MODE) & (~(1<<5)));//sync mode doesn't support edo

			REG_WRITE_U32(REG_ECC_SEL, 0x0);

			/* set oobbuf */
			oob_sa = ( VIR_TO_PHY((unsigned int)oobBuf) >> 3);
			REG_WRITE_U32( REG_SPR_DDR_CTL,NF_SPR_DDR_CTL_spare_ddr_ena(1)|NF_SPR_DDR_CTL_per_2k_spr_ena(0)|NF_SPR_DDR_CTL_spare_dram_sa(oob_sa));

			/* set buf */
			dram_sa = ( VIR_TO_PHY((unsigned int)buf) >> 3);
			REG_WRITE_U32(REG_DMA_CTL1,NF_DMA_CTL1_dram_sa(dram_sa));
			dma_len = page_size >> 9;
			REG_WRITE_U32(REG_DMA_CTL2,NF_DMA_CTL2_dma_len(dma_len));
			REG_WRITE_U32(REG_DMA_CTL3,NF_DMA_CTL3_ddr_wr(1)|NF_DMA_CTL3_dma_xfer(1));

#if 0
			dma_cache_wback_inv(buf,page_size);
			dma_cache_wback_inv(oobBuf,oob_size);
#endif
			//Enable Auto mode
			REG_WRITE_U32( REG_AUTO_TRIG,NF_AUTO_TRIG_auto_trig(1)|NF_AUTO_TRIG_spec_auto_case(0)| NF_AUTO_TRIG_auto_case(auto_trigger_mode));
			WAIT_DONE(REG_AUTO_TRIG,0x80,0);
			WAIT_DONE(REG_DMA_CTL3,0x01,0);

			if (REG_READ_U32(REG_BLANK_CHK) & 0x2){
				/* data all 0xff */
				//printk("data all one \n");
				REG_WRITE_U32(REG_BLANK_CHK,NF_BLANK_CHK_blank_ena(1)|NF_BLANK_CHK_read_ecc_xnor_ena(0));
				buf += page_size;
				break;
			}
			else if (REG_READ_U32(REG_ND_ECC) & 0x8) {
				REG_WRITE_U32(REG_BLANK_CHK,NF_BLANK_CHK_blank_ena(1)|NF_BLANK_CHK_read_ecc_xnor_ena(0));
				return -1;	// ECC not correctable
			}
			else { 
				REG_WRITE_U32(REG_BLANK_CHK,NF_BLANK_CHK_blank_ena(1)|NF_BLANK_CHK_read_ecc_xnor_ena(0));
				buf += page_size;
				break;
			}
		}
		
	}
	return 0;
}

static int iram_check_image(void)
{
	int i;
	IMG_HEADER_Tp	Header;
	unsigned short temp=0;
	unsigned short sum=0;
	Header = (IMG_HEADER_Tp)BOOT_START_ADDR;

	if(memcmp(Header->signature,BOOT_SIGNATURE,4) != 0)
		return -1;

	for (i=0; i< Header->len; i+=2) 
	{
		memcpy(&temp, (void *)(BOOT_START_ADDR + sizeof(IMG_HEADER_T) + i), 2);
		sum+=temp;
	}

	return sum;																		
}

int iram_nflash_read(void)
{
	int i;
	rtk_nand_register_init();	
#if 0
	unsigned char id_data[8];
	memset(id_data,0,8);
	parallel_nand_read_id_cmd(id_data);
	parallel_nand_get_chip_info(&chipsize,&blocksize,&pagesize,&oobsize);
#endif
	unsigned int offset = (unsigned int)IMAGE_START;
	unsigned int end = (unsigned int)(IMAGE_START+IMAGE_SIZE);


	for(i = 0;i < BACK_IAMGE_NUM;i++){
		if(parallel_nand_read(BOOT_START_ADDR,IMAGE_START+i*IMAGE_OFFSET,IMAGE_START+i*IMAGE_OFFSET+IMAGE_SIZE) != 0)
			return 0;
		if(iram_check_image() != 0)
			continue;
		else{
			void (*jump_func)(void);
			jump_func = (void*)(0xa0500010);
			jump_func();
		}
	}
	
	return 0;
}
