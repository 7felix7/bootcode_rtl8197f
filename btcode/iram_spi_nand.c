#include "start.h"
#include "../boot/include/nand_spi/soc.h"
#include "../boot/include/nand_spi/nand_spi.h"
#include "../boot/include/nand_spi/nand_spi_ctrl.h"
#include "../boot/include/nand_spi/ecc_ctrl.h"


extern char  __boot_start[];
extern char  __boot_end[];

static cmd_info_t winbond_cmd_info = {
    .w_cmd = WINBOND_PROGRAM_LOAD,
    .w_addr_io = SIO_WIDTH,
    .w_data_io = SIO_WIDTH,
    .r_cmd = WINBOND_NORMAL_READ,
    .r_addr_io =        SIO_WIDTH,
    .r_data_io = SIO_WIDTH,
    ._wait_spi_nand_ready = winbond_wait_spi_nand_ready,
};

typedef unsigned int ulong;
typedef unsigned int size_t;


#define IMAGE_START			(0x0)
#define IMAGE_OFFSET		(0x40000)
#define IMAGE_SIZE			(0x40000)
#define BACK_IAMGE_NUM		4
#define BOOT_START_ADDR		(0xA0500000)

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



#if 0
#define CONFIG_SYS_CACHELINE_SIZE 32
#define _ICACHE_SIZE		(64 * 1024)		
#define _DCACHE_SIZE		(32 * 1024)		
#define _SCACHE_SIZE		(512 * 1024)		

#define HIT_WRITEBACK_INV_D	0x15

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
static void flush_dcache_range(ulong start_addr, ulong stop)
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
#endif

#if 0
#define dma_cache_wback_inv(x,y)	flush_dcache_range(x,(x+y))
#else
#define dma_cache_wback_inv(x,y)
#endif

#define printf(fmt,args...)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rtk_nand_register_init()
{
	#define BIT(x)				(1<<x)
	#define REG_CLK_MANAGE2		0xB8000014
	#define REG_PINMUX_06		0xB8000818
	#define REG_PINMUX_15		0xB800081C


	/* set spi nand pin mux */
	REG32(REG_PINMUX_06) = 	REG32(REG_PINMUX_06) & (~((15<<28) | (15 << 24))) | ((3<<28) | (3 << 24));
	REG32(REG_PINMUX_15) = REG32(REG_PINMUX_15) & (~((15<<28) | (15 << 24) | (15 << 16))) | ((3<<28) | (3 << 24) | (3<<16));

	/*enable spi nand clock */
	if((REG32(REG_CLK_MANAGE2) & (BIT(19) | BIT(20))) != (BIT(19) | BIT(20))){
		printf("SPI NAND clock not enable\n");
		REG32(REG_CLK_MANAGE2) = REG32(REG_CLK_MANAGE2) | BIT(19) | BIT(20);
	}	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 unsigned int 
__spi_nand_pio_cmd(unsigned int opcode, snffcn_ptr sffunc, unsigned int data, unsigned int w_io_len, unsigned int r_io_len, unsigned int wr_bytes, void *wr_buf) 
{
    unsigned int ret=0x0;

  
    WAIT_SPI_NAND_CTRLR_RDY();
    cs_high(); /* deactivate CS */

	
    WAIT_SPI_NAND_CTRLR_RDY();
    cs_low();  /* activate CS */
  

    ret = sffunc(opcode, data, w_io_len, r_io_len, wr_bytes, wr_buf);
  

    WAIT_SPI_NAND_CTRLR_RDY();
	
    cs_high(); /* deactivate CS */

	
    WAIT_SPI_NAND_CTRLR_RDY();

  
    return ret;
}


 unsigned int 
snffcn_pio_raw_cmd(unsigned int opcode, unsigned int data, unsigned int w_io_len, unsigned int r_io_len, unsigned int wr_bytes, void *wr_buf)
{
    unsigned int rdval = 0;
    unsigned int w_data = opcode;

    if(data != SNF_DONT_CARE){
        unsigned int temp = (w_io_len&0x3);
        if(0 != temp)
        w_data = opcode | (data<<((3-temp)*8));
    }

    WAIT_SPI_NAND_CTRLR_RDY();
    REG32(SNFWCMR) = w_io_len;
    REG32(SNFWDR)= w_data; //Trigger PIO Write
    WAIT_SPI_NAND_CTRLR_RDY();

    if (r_io_len != SNF_DONT_CARE) {
        REG32(SNFRCMR) = r_io_len; //Trigger PIO read
        WAIT_SPI_NAND_CTRLR_RDY();
        rdval = REG32(SNFRDR); //Getting r_len-BYTE data @REG32(SNFRDR)
    }
    return rdval;
}


 void winbond_reset(void)
{
    unsigned int    w_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(1));
    PIO_RAW_CMD(WINBOND_RESET, SNF_DONT_CARE, w_io_len, SNF_DONT_CARE);
}

 void
winbond_set_feature_register(unsigned int feature_addr, unsigned int setting
)
{
    unsigned int    w_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(3));
    PIO_RAW_CMD(WINBOND_SET_FEATURE, ((feature_addr << 8) | setting),
                w_io_len, SNF_DONT_CARE);
}

 unsigned int
winbond_get_feature_register(unsigned int feature_addr
)
{
    unsigned int    w_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(2));
    unsigned int    r_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(1));
    unsigned int    ret =
        PIO_RAW_CMD(WINBOND_GET_FEATURE, feature_addr, w_io_len, r_io_len);
    return ((ret >> 24) & 0xFF);
}


 void winbond_set_buffer_mode(void)
{
    unsigned int    feature_addr = 0xB0;
    unsigned int    value = winbond_get_feature_register(feature_addr);
    value |= (1 << 3);
    winbond_set_feature_register(feature_addr, value);
}

 void winbond_block_unprotect(void)
{
    unsigned int    feature_addr = 0xA0;
    unsigned int    value = 0x00;
    value &= winbond_get_feature_register(feature_addr);
    winbond_set_feature_register(feature_addr, value);
}

 void winbond_disable_on_die_ecc(void)
{
    unsigned int    feature_addr = 0xB0;
    unsigned int    value = winbond_get_feature_register(feature_addr);
    value &= ~(1 << 4);
    winbond_set_feature_register(feature_addr, value);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void
winbond_wait_spi_nand_ready(void
)
{
    unsigned int    feature_addr = 0xC0;
    unsigned int    oip = winbond_get_feature_register(feature_addr);

    while ((oip & 0x1) != 0) {
        oip = winbond_get_feature_register(feature_addr);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void 
cs_low(void)
{
    REG32(SNFCCR)=0; //Active CS#
    while((REG32(SNFSR)&0x1)!=0); //Polling CS# status @REG32(SNFSR) until it is ¡§0¡¨
}

 void 
cs_high(void)
{
    REG32(SNFCCR)=1; //De-active CS#
    while((REG32(SNFSR)&0x1)!=1); //Polling CS# status @REG32(SNFSR) until it is ¡§1¡¨
}

 void 
_pio_write(unsigned int w_data, unsigned int w_io_len) 
/* 1. Setting PIO Control Register
  * 2. Trigger it to write
  */
{
    WAIT_SPI_NAND_CTRLR_RDY();
    REG32(SNFWCMR)= w_io_len;
    REG32(SNFWDR) = w_data; //Trigger PIO Write
    WAIT_SPI_NAND_CTRLR_RDY();
}

 void 
_dma_write_read(unsigned int dma_phy_addr, unsigned int dma_io_len, unsigned int wr_dir)
/* 1. Setting DMA Engine
  * 2. Trigger it to write or read
  */
{
    WAIT_SPI_NAND_CTRLR_RDY();
    REG32(SNFDRSAR) = dma_phy_addr;
    //printf("\n\n\n_dma_write_read()001\n");

   //printf("\nREG32(SNFDRSAR,0x%x)dma_phy_addr=0x%x\n",SNFDRSAR,dma_phy_addr);

	
    REG32(SNFDLR) = dma_io_len;
    WAIT_SPI_NAND_CTRLR_RDY();
    REG32(SNFDTR) = wr_dir; //Trigger DMA write or read

     //  printf("\n_dma_write_read()002 ,dma_io_len=0x%x \n",dma_io_len);
     //  printf("\nREG32(SNFDTR,0x%x)=0x%x\n\n\n",SNFDTR,REG32(SNFDTR) );
    WAIT_DMA_CTRLR_RDY();
}


 void 
page_data_read_to_cache_buf(unsigned int blk_pge_addr, cmd_info_t *snf_cmd_info)
{
    /* 1-BYTE CMD + 1-BYTE Dummy + 2-BYTE Address */
    unsigned int w_io_len = IO_WIDTH_LEN(SIO_WIDTH,SNFCMR_LEN(4));
    PIO_RAW_CMD(SNF_PAGE_DATA_READ, blk_pge_addr, w_io_len, SNF_DONT_CARE); // SNF_PAGE_DATA_READ=0x13
    snf_cmd_info->_wait_spi_nand_ready();
}


/* read data */
 void  dma_read_data(void *dma_addr, u32_t dma_len, u32_t blk_pge_addr, cmd_info_t *snf_cmd_info)
/*
  * Read More or Less than One Chunk (More or less than 2112-Byte)
  * Start from the initial cache register (CA=0x0000)
  */
{
    u32_t w_data, w_io_len, dma_io_len;
    u32_t column_addr=0x0000;
    u32_t dummy=0x00;


    

    while(dma_len > 0){
        /*(Step1) Read data from nand flash to cache register */
        page_data_read_to_cache_buf(blk_pge_addr, snf_cmd_info);

        /*(Step2) Enable CS */
        WAIT_SPI_NAND_CTRLR_RDY();
        cs_high(); /* deactivate CS */
        WAIT_SPI_NAND_CTRLR_RDY();
        cs_low();  /* activate CS */

        /*(Step3) Send 1-Byte Command */
        w_data = snf_cmd_info->r_cmd<<24; //r_cmd= 0x03
        w_io_len = IO_WIDTH_LEN(SIO_WIDTH,SNFCMR_LEN(1));
        _pio_write(w_data, w_io_len);

	


       
	#ifdef NAND_SPI_USE_QIO	
		/*(Step4) Send 4-Byte Address for QIO , Fast read op(0xeb)need 4T's addr + 4T's dummy*/
		w_data = (column_addr<<24)|(dummy<<16);
        	w_io_len = IO_WIDTH_LEN(snf_cmd_info->r_addr_io,SNFCMR_LEN(4));
        	_pio_write(w_data, w_io_len);
       
	#else  /*(Step4) Send 3-Byte Address for SIO/DIO */
	
		w_data = (column_addr<<16)|(dummy<<8);
        	w_io_len = IO_WIDTH_LEN(snf_cmd_info->r_addr_io,SNFCMR_LEN(3));
        	_pio_write(w_data, w_io_len);
	#endif

        /*(Step5) DMA Read from cache register to RAM */
        if(dma_len >= MAX_BYTE_PER_DMA){
            //_lplr_basic_io.dcache_writeback_invalidate_range((u32_t)dma_addr, (u32_t)(dma_addr+MAX_BYTE_PER_DMA));
            dma_io_len = IO_WIDTH_LEN(snf_cmd_info->r_data_io, MAX_BYTE_PER_DMA);
            dma_len -= MAX_BYTE_PER_DMA;
        }else{
           // _lplr_basic_io.dcache_writeback_invalidate_range((u32_t)dma_addr, (u32_t)(dma_addr+dma_len));
            dma_io_len = IO_WIDTH_LEN(snf_cmd_info->r_data_io, dma_len);
            dma_len = 0;
        }
        _dma_write_read(PADDR(dma_addr), dma_io_len, DMA_READ_DIR);

	//dma_addr+=MAX_BYTE_PER_DMA;//JSW

        WAIT_SPI_NAND_CTRLR_RDY();
        cs_high(); /* deactivate CS */
    }
}


 void winbond_dma_read_data(void *dma_addr, unsigned int dma_len, unsigned int blk_pge_addr)
/*
 * Read More or Less than One Chunk (More or less than 2112-Byte)
 * Start from the initial cache register (CA=0x0000)
 */
{
	/* cache invalid and writeback */
	dma_cache_wback_inv(dma_addr,dma_len);
	
    dma_read_data(dma_addr, dma_len, blk_pge_addr, &winbond_cmd_info);
}


 void winbond_2KB_chunk_read(void *dma_addr, unsigned int blk_pge_addr)
/*
 * Read One Chunk (2048-Byte SRC Data + 64-Byte Spare Data)
 * Start from the initial cache register (CA=0x0000)
 */
{
    winbond_dma_read_data(dma_addr, WINBOND_2KB_CHUNK_SPARE_SIZE,
                          blk_pge_addr);
}


 int winbond_2KB_chunk_read_with_ecc_decode(void *dma_addr,
                                       unsigned int blk_pge_addr,
                                       void *p_eccbuf
)
{
	int i;
	unsigned char* value = (unsigned char*)dma_addr;
    winbond_2KB_chunk_read(dma_addr, blk_pge_addr);     // whole
                                                        // chunk=2048+64
   		                                                 // bytes
   		                                                 
    return ecc_decode_bch_6t(dma_addr, p_eccbuf,blk_pge_addr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* nand ecc ctrl */
 int
ecc_check_decode_status(void
)
/* Return value =-1   : ECC decode fail
  * Return value =0~6: Number of bits that is correctted
  */
{
  unsigned int ecsr_val = ECC_STATUS_REG;
  int ret = (((ecsr_val >> 8) & 0x1)) ? ECC_DECODE_FAIL : ECC_DECODE_SUCCESS;

  //printf("register value=%x\n",ecsr_val);


  if (ECC_DECODE_FAIL == ret) {
    if (!((ecsr_val >> 4) & 0x1))
      printf(" <ecc decode fail>\n");   //Can't recover
     else{
		/* all ff/ after erase command */
		return 0;
     }
  }
#if  CONF_SPI_NAND_ECC_DBG
  else {
    ret = ((ecsr_val >> 12) & 0xFF);    //Can recover and return uumber of bits that is correctted
    //printf(" <ecc decode pass>, corrected bit=%d \n", ret);
  }
#endif
  return ret;
}


 int ecc_decode_bch_6t(void *dma_addr, void *p_eccbuf, unsigned int pageaddr)
/*  Return value:
  *  -1: ECC decode fail
  *  0~6: Number of bits that is correctted
  */
{
  int ret = ECC_DECODE_SUCCESS;
  unsigned int j;
  unsigned int ecc_start_addr = (unsigned int) dma_addr;
  u8_t *tag_src_addr = (u8_t *) (ecc_start_addr + BCH6_BYTE_PER_CHUNK); //BCH6_BYTE_PER_CHUNK=512*4=2048
  u8_t *syn_src_addr = (u8_t *) (tag_src_addr + BCH6_TAG_SIZE * BCH6_SECTOR_PER_CHUNK); //offset=2048+24

  // 1. Cache Flush ......

  // _lplr_basic_io.dcache_writeback_invalidate_range((unsigned   int)dma_addr,(unsigned   int)(dma_addr+BCH6_ECC_SPARE_SIZE));

	/* set DMA_endian */
  if(pageaddr >= BOOT_END_PAGE)
  	REG32(ECCFR) = REG32(ECCFR) | (1<<13);
  else
  	REG32(ECCFR) = REG32(ECCFR) & ~(1<<13);
	
  SET_ECC_DMA_TAG_ADDR(PADDR(p_eccbuf));

  /* 2. Move 1st Tag & Syndrome to buffer, decode 1st 512B */
  for (j = 0; j < BCH6_SECTOR_PER_CHUNK;
       j++, ecc_start_addr += BCH6_SECTOR_SIZE, tag_src_addr +=
       BCH6_TAG_SIZE, syn_src_addr += BCH6_SYN_SIZE) {
    memcpy(p_eccbuf, tag_src_addr, BCH6_TAG_SIZE);
    memcpy((p_eccbuf + BCH6_TAG_SIZE), syn_src_addr, BCH6_SYN_SIZE);

	/* cache invalid and writeback */
    dma_cache_wback_inv(p_eccbuf,(BCH6_SYN_SIZE+BCH6_TAG_SIZE));
    dma_cache_wback_inv(ecc_start_addr,BCH6_SECTOR_SIZE);

    //_lplr_basic_io.dcache_writeback_invalidate_range((unsigned   int)p_eccbuf, (unsigned   int)(p_eccbuf+BCH6_ECC_BUF_SIZE));

    SET_ECC_DMA_START_ADDR(PADDR((ecc_start_addr)));
    ECC_DECODE_KICKOFF();
    WAIT_ECC_RDY();

    ret = ecc_check_decode_status();
    if (ECC_DECODE_FAIL == ret) {
      printf("ECC_DECODE_FAIL,ecc_start_addr = 0x%x\n", ecc_start_addr);
      return ret;
    }

    memcpy(tag_src_addr, p_eccbuf, BCH6_TAG_SIZE);
    //  _lplr_basic_io.dcache_writeback_invalidate_range((unsigned   int)tag_src_addr, (unsigned   int)(tag_src_addr+BCH6_ECC_BUF_SIZE));
  }
  return ret;
}

#if 0
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 unsigned int winbond_read_id(void)
{
    unsigned int    dummy = 0x00;
    unsigned int    w_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(2));  // JSW:write 
                                                                        // length 
                                                                        // = 
                                                                        // 2byte

    unsigned int    r_io_len = IO_WIDTH_LEN(SIO_WIDTH, SNFCMR_LEN(3));  // JSW:write 
                                                                        // length 
                                                                        // = 
                                                                        // 3byte


    // #define WINBOND_RDID 0x9f
    unsigned int    ret =
        PIO_RAW_CMD(WINBOND_RDID, dummy, w_io_len, r_io_len);

    return ((ret >> 8) & 0xFFFFFF);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

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


int iram_nflash_read(unsigned long flash_address,unsigned long image_addr,unsigned long image_size)
{
	int i;
	unsigned int pagestart,pageend,pagenum;
	unsigned char* buffer;
	unsigned char* eccbuffer;

	rtk_nand_register_init();
	
	winbond_reset();
	winbond_set_buffer_mode();
	winbond_block_unprotect();
	winbond_disable_on_die_ecc();

#if 0
	if(((unsigned int)BOOT_ADDR % CONFIG_NAND_PAGE_SIZE) != 0){
		printf("%s:%d\n",__func__,__LINE__);
		return;
	}
	if((((unsigned int)__boot_start) % CONFIG_NAND_PAGE_SIZE) != 0){
		printf("%s:%d\n",__func__,__LINE__);
		return;
	}
#endif
	
	buffer = (unsigned char*)BOOT_START_ADDR;
	eccbuffer = buffer + 0x100000;

	for(i= 0;i < BACK_IAMGE_NUM;i++){
		pagestart = (IMAGE_START+IMAGE_OFFSET*i+CONFIG_NAND_PAGE_SIZE-1)/CONFIG_NAND_PAGE_SIZE;
		pageend = (IMAGE_START+IMAGE_OFFSET*i+IMAGE_SIZE+CONFIG_NAND_PAGE_SIZE-1)/CONFIG_NAND_PAGE_SIZE;
		
		for(pagenum = pagestart;pagenum <= pageend;pagenum++){
			if(winbond_2KB_chunk_read_with_ecc_decode(buffer,pagenum,eccbuffer) < 0){
				return 0;
			}
			buffer += CONFIG_NAND_PAGE_SIZE;
		}

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
