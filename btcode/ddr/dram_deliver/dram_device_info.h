#ifndef DRAM_DEVICE_INFO_H
#define DRAM_DEVICE_INFO_H
//#include "common/1.01a/DW_common.h"
//#include "spi_flash/DW_common.h"

#if 0
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

#include "../../../autoconf.h"
#define CONFIG_AUTO_PARAM_DDR_SWITCH

enum dram_type {
  NON_DDR = 0,
  DDR_1 = 1,
  DDR_2 = 2,
  DDR_3 = 3,
  DDR_4 = 4, 
  SDR   = 8
};

enum dram_page_size {
  PAGE_256K = 0,
  PAGE_512K = 1,
  PAGE_1K   = 2,
  PAGE_2K   = 3,
  PAGE_4K   = 4,
  PAGE_8K   = 5,
  PAGE_16K  = 6,
  PAGE_32K  = 7,
  PAGE_64K  = 8 
};

enum dram_bank_size {
  BANK_2   = 0,
  BANK_4   = 1,
  BANK_8   = 2 
};

enum dram_dq_width {
  DQ_16     = 0,
  DQ_32     = 1, 
  HALF_DQ32 = 2 
};

enum mode0_bst_len {
  BST_LEN_4   = 0,
  BST_LEN_FLY = 1,
  BST_LEN_8   = 2 
};

enum mode0_bst_type {
  SENQUENTIAL = 0,
  INTERLEAVE  = 1 
};   

enum dfi_ratio_type {
  DFI_RATIO_1 = 0,  // DFI= 1:1, or SDR
  DFI_RATIO_2 = 1, 
  DFI_RATIO_4 = 2  
};   


//enum mode0_cas {
//
//};
//enum mode0_wr          write_recovery;
//enum mode1_dll_en      dll_enalbe;
//enum mode1_all_lat     all_latency;
//enum mode2_cwl         wr_cas;
//***** dram_.data/
// * DESCRIPTION
// *  This data type is used to describe the manufacturer identifiation of dram_ 
// */
struct dram_device_info {
  const struct   dram_info          *dev;
  const struct   dram_mode_reg_info *mode_reg;
  const struct   dram_timing_info   *timing;
  const struct   dram_phy_ip_info   *phy;
  uint32_t ddr_period_ps;
  enum     dfi_ratio_type     *dfi_rate; 
};

struct dram_info {
  enum dram_type         device_type;   
  enum dram_page_size    page;   
  enum dram_bank_size    bank;   
  enum dram_dq_width     dq_width;
} ;

struct dram_mode_reg_info {
  enum mode0_bst_len     bst_len;
  enum mode0_bst_type    bst_type;
  //enum mode0_cas         rd_cas;
  uint32_t mode0_cas;
  uint32_t mode0_wr;
  uint32_t mode1_dll_en_n;
  uint32_t mode1_ocd;
  uint32_t mode1_odt;
  uint32_t mode1_all_lat;
  uint32_t mode2_cwl;
  uint32_t dram_par_lat;      // for DDR4: paraity_lat
  uint32_t dram_wr_lat; 
  uint32_t dram_rd_lat;
  uint32_t dram_add_lat;
};

struct dram_timing_info {
  uint32_t  trfc_ps;
  uint32_t  trefi_ps;
  uint32_t  wr_max_tck;
  uint32_t  trcd_ps; 
  uint32_t  trp_ps;
  uint32_t  tras_ps;
  uint32_t  trrd_tck;
  uint32_t  twr_ps;
  uint32_t  twtr_tck; 
  //uint32_t  trtp_ps;
  uint32_t  tmrd_tck;
  uint32_t  trtp_tck;
  uint32_t  tccd_tck;
  uint32_t  trc_ps;
  uint32_t  tccd_s_tck;
  uint32_t  twtr_s_tck;
  uint32_t  *trfc_ps_array;
};

struct dram_phy_ip_info {
  int  mck_default; 
  int  mdqs_default; 
  uint8_t  dqs_en_default;
  uint8_t  rx_fifo_default;
  uint32_t odt_default;
  uint8_t dqs_driving;   //nnt, npt, pnt,ppt
  uint8_t dck_driving;   //nnt, npt, pnt,ppt
  uint8_t cmd_driving;   //nt,pt
  uint8_t ba_addr_driving;   //nt,pt
  uint8_t dq_driving;   //nt,pt
  uint32_t rx_delay_tap_0;  //bit 0 ~ 3, default 0x10101010, 0x28c
  uint32_t rx_delay_tap_1;  //bit 4 ~ 7, default 0x10101010 , 0x29c
  uint32_t rx_delay_tap_2;  //bit 8 ~ 11, default 0x10101010, 0x290
  uint32_t rx_delay_tap_3;  //bit 12 ~ 15, default 0x10101010 , 0x2a0
  uint32_t rx_delay_tap_n_0;  //bit 0 ~ 3, default 0x10101010, 0x2ac
  uint32_t rx_delay_tap_n_1;  //bit 4 ~ 7, default 0x10101010 , 0x2bc
  uint32_t rx_delay_tap_n_2;  //bit 8 ~ 11, default 0x10101010, 0x2b0
  uint32_t rx_delay_tap_n_3;  //bit 12 ~ 15, default 0x10101010 , 0x2c0

  uint32_t pre_shift_0;             	//bit 0 ~ 7, default 0x33333333, 0x26c
  uint32_t pre_shift_1;		     	//bit 8 ~ 15, default 0x33333333, 0x270
  uint32_t post_shift_0;	     	//bit 0 ~ 7, default 0x33333333, 0x27c
  uint32_t post_shift_1;	    	//bit 8 ~ 15, default 0x33333333, 0x280

  uint32_t tx_delay_tap_0;  // bit 0 ~ 7, default 0x44444444, 0x230
  uint32_t tx_delay_tap_1;	// bit 8 ~ 15, default 0x44444444, 0x230
  uint32_t dqs_dm_delay_tap; // bit 15~0 , default 0x4444
  //uint32_t dm_dly_sel_0; // bit 7~4 , default 0x4
  //uint32_t dm_dly_sel_1; // bit 12~15, default 0x4
};
#endif /* DRAM_DEVICE_INFO_H */

