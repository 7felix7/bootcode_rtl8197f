//+FHDR-------------------------------------------------------------------------
// Copyright (c) 2006, Realtek Semiconductor Corporation
// Realtek's Proprietary/Confidential
//
// All rights reserved. No part of this design may be reproduced or stored
// in a retrieval system, or transmitted, in any form or by any means,
// electronic, mechanical, photocopying, recording, or otherwise,
// without prior written permission of the Realtek Semiconductor Corporation.
// Unauthorized reproduction, duplication, use, or disclosure of this
// design will be deemed as infringement.
//------------------------------------------------------------------------------
// Description:
//     Sheipa platform supervisor subsystem initialize code.
//     database.
//-FHDR-------------------------------------------------------------------------

//#include <stdio.h>
//#include "config.h"
//#include "runtime.h"
//#include "platform_defs.h"
//#include "ps_smu_defs.h"
//#include "ps_pmu_defs.h"
//#include <common.h>

//Add for RXI-310 Memory_Controller
//#include <malloc.h>

#include "dram_deliver/ms_rxi310_defs.h"
#include "dram_deliver/dram_param_defs.h"
#include "dram_deliver/dram_device_info.h"

#include <rlxboard.h>
#include <sys_reg.h>


#define udelay hal_delay_us
#ifndef CONFIG_IRAM_IMAGE
#define printf      btprintf
#else
#define printf
#endif
#ifndef REG32
#define REG32(reg)		(*(volatile unsigned int   *)(reg))
#endif

#define VER_0 0x0 
#define VER_1 0x1
#define VER_2 0x2

//#define _8198E_FPGA 0

//#include "SPIC_deliver/spi_flash/spi_flash_rtk.h"
//#include "spi_flash/DW_common.h"
//#include "spi_flash/spi_flash_public.h"
//#include "spi_flash/spi_flash_private.h"

//#include "rtk_phy_defs.h"

//#define CPU_800
//#define DIRECT_JUMP

#define DRAM_FEQ 1
#ifndef SDRAM_DDR1_SUPPORT
#define ENABLE_DRAM_ODT
#endif

//#define DDR_CALIBRATION
//#define DDR_PHY_BIST
//#define DDR_K_MCK
//#define DCK_POS_SCAN

//#define DDR_K_DQS
//#define DQS_POS_SCAN

//#define DDR_K_DQS_EN

//#define DDR_RX_MAP
//#define DDR_TX_MAP
//#define DDR_TX_MAP_ONLY_HOLD_TIME

//#define DDR_RX_FIFO_MAP
//#define DDR_ODT_MAP
//#define DDR_ADDRESS_MAP
//efine DDR_ADDRESS_MAP_POSITIVE

//#define DRAM_ADD_OUTPUT_DRIVING
//#define DDR_TEST
//#define ONLY_WRITE
//#define READ_ONLY

#ifdef DDR_CALIBRATION
#define _memctl_calibration_printf printf
#else
#define _memctl_calibration_printf(...)
#endif

//#define _memctl_debug_printf printf
#define _memctl_debug_printf(...)

/* Register Macro */
#ifndef REG32
#define REG32(reg)      (*(volatile u32 *)(reg))
#endif
#ifndef REG16
#define REG16(reg)      (*(volatile u16 *)(reg))
#endif
#ifndef REG8
#define REG8(reg)       (*(volatile u8  *)(reg))
#endif

#define REG_READ_U8(register)         		(*(volatile unsigned char *)(register))
#define REG_READ_U16(register)         		(*(volatile unsigned short *)(register))

#ifndef REG_READ_U32
#define REG_READ_U32(register)         		(*(volatile unsigned long *)(register))
#endif

#ifndef REG_WRITE_U32
#define REG_WRITE_U32(register, value)    		(*(volatile unsigned long *)(register) = value)
#endif

#define TURN_ON_CLOCK() \
	REG_WRITE_U32(REG_DDR_PLL_1, REG_READ_U32(REG_DDR_PLL_1) | 0xf); \
	REG_WRITE_U32(REG_DDR_PLL_2, REG_READ_U32(REG_DDR_PLL_2) | (0xf<<16)); 

#define TURN_OFF_CLOCK() \
	REG_WRITE_U32(REG_DDR_PLL_1, REG_READ_U32(REG_DDR_PLL_1) & 0xfffffff2); \
	REG_WRITE_U32(REG_DDR_PLL_2, REG_READ_U32(REG_DDR_PLL_2) & (~(0xD<<16)));	

#define     BIT_SHIFT_CHIP_ID_CODE              0
#define     BIT_MASK_CHIP_ID_CODE               0xf
#define     IS_FB_BONDING(x)                    (x <= 3)
#define     BIT_GET_CHIP_ID_CODE(x)             (((x) >> BIT_SHIFT_CHIP_ID_CODE) & BIT_MASK_CHIP_ID_CODE)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

void dram_init_rxi310_FPGA (struct ms_rxi310_portmap *, struct dram_device_info *);
void dram_init_pll_ASIC (struct ms_rxi310_portmap *,  const struct dram_device_info *, unsigned int *pstrap_pin, int bd_idx);
void dram_init_dpi_ip_ASIC (struct ms_rxi310_portmap *,  const struct dram_device_info *, int bd_idx);
#ifndef _8198E_FPGA
void dram_init_rxi310_ASIC (struct ms_rxi310_portmap *dev_map,const struct dram_device_info *dram_info, int bd_idx);
#else
void dram_init_rxi310_FPGA (struct ms_rxi310_portmap *dev_map, struct dram_device_info *dram_info);
#endif
typedef unsigned int uint32;
extern int dram_addr_rot(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int dram_com_addr_rot(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int dram_byte_access(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int dram_half_word_access(uint32 dram_start, uint32 dram_size,uint32 area_size);
extern int dram_walking_of_1(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int dram_walking_of_0(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int memcpy_test(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern void udelay(unsigned long usec);

#ifdef ONLY_WRITE
extern int dram_normal_patterns_write_only(uint32 dram_start, uint32 dram_size, uint32 area_size);
#endif

#ifdef READ_ONLY
extern int dram_normal_patterns_read_only(uint32 dram_start, uint32 dram_size, uint32 area_size);
#endif

extern int dram_addr_rot_read_repeat(uint32 dram_start, uint32 dram_size, uint32 area_size);
extern int dram_normal_patterns(uint32 dram_start, uint32 dram_size, uint32 area_size);

//extern void WAIT_DONE_RTK(unsigned int addr, unsigned int mask, unsigned int value);
//#define BIST_CMD_SRAM_BASE 0xB8160000
//#define BIST_WD_SRAM_BASE   (BIST_CMD_SRAM_BASE + 0x3800)
//#define BIST_RG_SRAM_BASE    (BIST_CMD_SRAM_BASE + 0xC000)
//#define BIST_RD_SRAM_BASE    (BIST_CMD_SRAM_BASE + 0x14800)



const static unsigned short rl6387_pll_frequence_table[7][13]=
{  //{2C8[5:4], 2C8[8], 2C8[18:16], 2CC[5:4], 2CC[9:8], 2CC[1], 2C8[29:28], 2C8[1:0], 2C8[25:24], 2CC[0], 2C8[15:12], 2C8[23], 2C8[22:20]}
	{3          ,1         ,5               ,2            ,2           ,1         ,2               ,2            ,0               ,1        ,11             ,1           ,2},  //1440 MHz
	{3          ,1         ,4               ,2            ,2           ,1         ,2               ,2            ,0               ,1        ,11             ,1           ,2},  //1280MHz
	{3          ,0         ,4               ,2            ,1           ,0         ,2               ,1            ,0               ,1        ,9               ,1           ,2},  //1120MHz
	{3          ,0         ,3               ,2            ,1           ,0         ,2               ,2            ,0               ,1        ,7               ,1           ,2},  //960MHz
	{3          ,0         ,2               ,0            ,0           ,1         ,2               ,1            ,0               ,1        ,7               ,1           ,2},  //800MHz
	{2          ,0         ,2               ,0            ,0           ,1         ,2               ,2            ,0               ,1        ,5               ,1           ,2},  //640MHz
	{2          ,0         ,1               ,0            ,0           ,0         ,2               ,2            ,0               ,1        ,5               ,1           ,2}   //320MHz
};

#define TOTAL_WORD_CNT 256

#if 1
#define dram_test_printf(...)
int dram_addr_rot_write(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  uint32 i;
  uint32 j;
  //uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  dram_test_printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 4; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_addr = ((uint32) start);

    start = (uint32 *) (((uint32) start));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = 0x5aa5a55a;//(read_start_addr << i);
      start++;
      read_start_addr = read_start_addr + 4;
    }
  }
  return 0;
test_fail:
  dram_test_printf("%s test fails.\n\n", __FUNCTION__);
  return 1;
}

int dram_addr_rot_read(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  uint32 i;
  uint32 j;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  dram_test_printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 4; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_addr = ((uint32) start);
    //start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)));
    start = (uint32 *) (((uint32) start));

    //_cache_flush();
    read_start_addr = ((uint32) read_start);
    /* check data reversing order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != ((read_start_addr) << i)) {
        //printf("decr addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
       //        read_start, start_value, ((read_start_addr) << i),
        //       __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr - 4;
      read_start--;
    }

    read_start_addr += 4;
    read_start++;

    /* check data sequential order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != ((read_start_addr) << i)) {
        //printf("seq addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
       //        read_start, start_value, ((read_start_addr) << i),
        //       __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr + 4;
      read_start++;
    }

    dram_test_printf("rotate %d 0x%x passed\r\n", i, start);
  }
  dram_test_printf("%s test completed.\r\n", __FUNCTION__);
  return 0;
test_fail:
  dram_test_printf("%s test fails.\n\n", __FUNCTION__);
  return 1;
}

#if 0
int dram_bist_read_only_test(unsigned int row_addr,unsigned int ba_addr, unsigned int col_addr)
{
	unsigned int row_bit, col_bit, bank_bit,bst_len;
	//unsigned int row_addr, col_addr, ba_addr;
	unsigned int wd_data_addr, rg_addr, rd_addr;
	unsigned int INIT_cnt, bstc_mode, i, err_cnt, rg_d, rd_d;
	struct ms_rxi310_portmap *ms_ctrl_0_map;

	//_memctl_debug_printf("Enter BTSC function \n");
	
	ms_ctrl_0_map = (struct ms_rxi310_portmap*) BSP_MS_I_DRAMC_0_BASE;
	wd_data_addr = BIST_WD_SRAM_BASE;
	rg_addr = BIST_RG_SRAM_BASE;
	
	col_bit = 10;
	row_bit = 13;
	bank_bit = 3;

	bst_len  = 4;
	//row_addr = 0x8;
	//col_addr = 0x4;
	//ba_addr = 0x1;
	
	//Enable BSTC function
	ms_ctrl_0_map->csr = 0x300;
	while(1)
	{
		if (((ms_ctrl_0_map-> csr) & 0x400) == 0x400)
		{
			udelay (10); /* Delay 10ns	*/
	  		 INIT_cnt++;
			_memctl_debug_printf("\nDRAM BSTC init is still active\n");
			 if (INIT_cnt>10)
		 		break;
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
		 	break;
		}
	}
	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

//jimmy comment
//flush bstc cmd/wd/msk/rg sram
//ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr | 0xf00;
//ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr & 0xFFFFF0FF;	

	//set BSTC related function
	bstc_mode = ms_ctrl_0_map->bcr;
	bstc_mode = bstc_mode | (1<<3) | (1<<2) | (1<<1);// | (1<<4);   // disable mask, loop mode, compare mode
	//RD_EXPC_LEVEL, command cnt * DQ bits * burst length 
	bstc_mode = bstc_mode | (((8*16*64)/32)<<16);
	ms_ctrl_0_map->bcr = bstc_mode;
	//_memctl_debug_printf("ms_ctrl_0_map->bcr = 0x%x\n", ms_ctrl_0_map->bcr);
	
  //jimmy added
  //set loop count
	ms_ctrl_0_map->bct = 0x18;

//jimmy added
//flush bstc cmd/wd/msk/rg sram
ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr | 0xf00;
ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr & 0xFFFFF0FF;	

#if 1
	//write CMD
	//REG_WRITE_U32(BIST_CMD_SRAM_BASE,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len%8) << 1)| 0x1);
	//REG_WRITE_U32(BIST_CMD_SRAM_BASE+0x4,(ba_addr&0x7));
	//_memctl_debug_printf("write cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE));
	
	REG_WRITE_U32(BIST_CMD_SRAM_BASE,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len%8) << 1)| 0x0);
	REG_WRITE_U32(BIST_CMD_SRAM_BASE,(ba_addr&0x7));
	//_memctl_debug_printf("read cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0x8));
#else
	addr_gen();
#endif
  //jimmy comment
	//set loop count
//	ms_ctrl_0_map->bct = 0x10;
#if 1
/*
	//write data
	REG_WRITE_U32(wd_data_addr, 0xffffffff);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0x00000000);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xaaaaaaaa);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0x55555555);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xa55a5aa5);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xa5a5a5a5);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xfe5566ef);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xdead5566);
*/



	//read golden
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);	
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x5aa5a55a);
#else
	pat_gen();
#endif
	//bist function start
	ms_ctrl_0_map->ccr = 0x4;
	while(1)
	{
		if (((ms_ctrl_0_map-> ccr) & 0x4) == 0x0)
		{
			//udelay (1000); /* Delay 1s	*/

			//_memctl_debug_printf("~");
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC test is done \n");
		 	break;
		}
	}	
	//read err cnt
	err_cnt = (ms_ctrl_0_map->bst & 0xFFFF0000)>>16;
	//_memctl_debug_printf("BSTC compare errpr cnt : %d\n",err_cnt);

// jimmy comment
	//flush fifo
//	ms_ctrl_0_map->ccr = 0x100;

	//read data in rd_sram
	rg_addr = BIST_RG_SRAM_BASE;
	rd_addr = BIST_RD_SRAM_BASE;
#if 0
	if(err_cnt != 0)
	{
		_memctl_debug_printf("BSTC compare errpr cnt : %d\n",err_cnt);
		_memctl_calibration_printf("RG(0x%x) : \n", rg_addr);
		for(i = 0; i < 512; i++)
		{
			_memctl_calibration_printf(" %x ", REG_READ_U32(rg_addr + i*4));
			if((i != 0) && (!(i%16)))
				_memctl_calibration_printf("\n");
		}
		_memctl_calibration_printf("\n");

		_memctl_calibration_printf("RD(0x%x) : \n", rd_addr);
		for(i = 0; i < 512; i++)
		{
			_memctl_calibration_printf(" %x ", REG_READ_U32(rd_addr + i*4));
			if((i != 0) && (!(i%16)))
				_memctl_calibration_printf("\n");

		}
		_memctl_calibration_printf("\n");
		return -1;
	}

	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

#else
	if(err_cnt != 0)
	{
		// jimmy added
			//flush fifo
			ms_ctrl_0_map->ccr = 0x100;
		
			//disable BSTC function
			ms_ctrl_0_map->csr = 0x700;
			while(1)
			{
				if (((ms_ctrl_0_map-> csr) & 0x700) != 0x700)
				{
					udelay (10); /* Delay 10ns	*/
					 INIT_cnt++;
					_memctl_debug_printf("\nDRAM BSTC disable is still active\n");
					 if (INIT_cnt>10)
						break;
				}
				else
				{
					//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
					break;
				}
			}
		return -1;
		for(i = 0; i < 512; i++)
		{
			rg_d = REG_READ_U32(rg_addr + i*4);
			rd_d = REG_READ_U32(rd_addr + i*4);
			if(rg_d != rd_d)
			{
				
				_memctl_calibration_printf("1bist test failed(%d) 0x%x != 0x%x \n", i,rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+1)*4);
				rd_d = REG_READ_U32(rd_addr + (i+1)*4);
				_memctl_calibration_printf("2bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+2)*4);
				rd_d = REG_READ_U32(rd_addr + (i+2)*4);
				_memctl_calibration_printf("3bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+3)*4);
				rd_d = REG_READ_U32(rd_addr + (i+3)*4);
				_memctl_calibration_printf("4bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+4)*4);
				rd_d = REG_READ_U32(rd_addr + (i+4)*4);
				_memctl_calibration_printf("5bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				
				_memctl_debug_printf("write cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE));
				_memctl_debug_printf("write cmd2 : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE + 0x4));
				_memctl_debug_printf("read cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0x8));
				_memctl_debug_printf("read cmd2 : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0xc));
				return -1;
			}
		}
		
	}	
	//flush fifo
//	ms_ctrl_0_map->ccr = 0x100;

#endif
	
// jimmy added
	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

	//disable BSTC function
	ms_ctrl_0_map->csr = 0x700;
	while(1)
	{
		if (((ms_ctrl_0_map-> csr) & 0x700) != 0x700)
		{
			udelay (10); /* Delay 10ns	*/
	  		 INIT_cnt++;
			_memctl_debug_printf("\nDRAM BSTC disable is still active\n");
			 if (INIT_cnt>10)
		 		break;
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
		 	break;
		}
	}

	//udelay(1000000);
	return 0;
}
#endif

#endif

#if 0//def DDR_TEST

int  row_loop; 
int  ba_loop;  
int  col_loop; 

int  dq_loop_07; // dq[07:00]
int  dq_loop_15; // dq[15:08]
int  dq_loop_23; // dq[23:16]
int  dq_loop_31; // dq[31:24]

void addr_gen()
{
	unsigned int bist_sram_addr = BIST_CMD_SRAM_BASE; 
	//unsigned int Mode = 1;
	//int a = SDR_IDR;
	unsigned char in_bit[127];
	unsigned int  prb7_array[4];
	//unsigned int  data_trans[32];
	unsigned int  row_array[16];
	//unsigned int  ba_array[127];
	//unsigned int  col_array[127];
	unsigned int bst_len;

	unsigned int  row_addr; // 15-bit
	unsigned int  ba_addr;  //  3-bit
	unsigned int  col_addr; // 10-bit: 7-bit + 3'b000
	unsigned int  col_sh;   // 10-bit: 7-bit + 3'b000

	//int  f_line_length   = 0; 
	//int  pattern_dum_cnt = 0;
	//int  dum_cnt         = 0; 
	//int  cmd_times       = 0;
	int  cmd_cnt         = 0;
	int  pat_cnt         = 0;
	//int  pat_inc         = 1;

	int  array_cnt       = 0;
	int  array_num       = 0;

	//int  wrrd_cnt        = 1; //wr=1, rd=0
	//int  isu_single_data_len     = 0;
	//int  isu_max_cmd_len         = 0;

	//unsigned int ddr_addr = 0; 

	//unsigned int bst_size = 0;
	int	bit_cnt;
	unsigned int max_col_bit, max_row_bit,max_ba_bit;
	max_col_bit = 10;
	max_row_bit = 13;
	max_ba_bit = 2;

	prb7_array[0] = 0xfe041851;
	prb7_array[1] = 0xe459d4fa;
	prb7_array[2] = 0x1c49b5bd;
	prb7_array[3] = 0x8d2ee654;

  
	// fprintf(f_data, "// cmd_times              = 0x%x                                                                \n", CMD_TIMES);
	// fprintf(f_data, "// ocp isu_bst_len = 0x%x\n", CMD_TIMES * 8);
	// fprintf(f_data, "                                                                                                \n");

	// fprintf(f_data, "ew @$BCR                = @@$BCR | (0x%x << 0n16)                                               \n", (CMD_TIMES*8));
	// fprintf(f_data, "ew $rd_pattern_times    = 0x%x                                                                  \n", (CMD_TIMES*8));

	//  fprintf(f_data, "// CMD_SRAM DATA\n");
	//  fprintf(f_data, "ew $bist_sram_addr      = $BIST_CMD_SRAM_BASE\n");

	for (bit_cnt =0; bit_cnt<127; bit_cnt++) {
		in_bit[bit_cnt] = (prb7_array[(bit_cnt/32)] & (0x1 << (31-bit_cnt))) >> (31-bit_cnt);
	}

	// generate Addr

	row_addr = 0; 
	ba_addr  = 0; 
	col_addr = 0; 

	bst_len = 16;

	for (cmd_cnt =0; cmd_cnt < 16; cmd_cnt++)
	{

		gen_addr:
		// row_addr: 13-bit
		for(bit_cnt=12; bit_cnt >=0; bit_cnt--) 
		{
			row_addr = (in_bit[row_loop] << bit_cnt) | row_addr; 
			if (row_loop == 126){
			row_loop = 0;
			}
			else {
			row_loop = row_loop + 1;
			} 
		}

		// ba_addr: 3-bit
		for(bit_cnt=2; bit_cnt >=0; bit_cnt--) 
		{
			ba_addr = (in_bit[ba_loop] << bit_cnt) | ba_addr; 
			if(ba_loop == 126){
			ba_loop = 0;
			}
			else {
			ba_loop = ba_loop + 1;
			} 
		}

		// col_addr: 5-bit + 2'h0 + 3'h0
		for(bit_cnt=4; bit_cnt >=0; bit_cnt--) 
		{
			col_addr = (in_bit[col_loop] << bit_cnt) | col_addr; 
			if(col_loop == 126){
			col_loop = 0;
			}
			else {
			col_loop = col_loop + 1;
			} 
		}
		row_addr = row_addr & ((1<<max_row_bit) - 1);
		ba_addr = ba_addr & ((1<<max_ba_bit) - 1);
		col_addr = col_addr & ((1<<max_col_bit) - 1);

		// check can't repeat the address 
		for (array_cnt=0; array_cnt < array_num; array_cnt++ ) 
		{
			//if(row_addr == row_array[array_cnt]){    // row_addr match??
			//  if(ba_addr == ba_array[array_cnt]){  // ba_addr match??
			//    if(col_addr == col_array[array_cnt]){  // col_addr match??
			//      goto gen_addr; 
			//    }
			//  }
			//}
			if(row_addr == row_array[array_cnt]){    // row_addr match??
				goto gen_addr; 
			} 

		}
		row_array[array_num] = row_addr;
		array_num= array_num+1;
		#if 0
		_memctl_calibration_printf("\nAddr row : 0x%x, ba : 0x%x, col : 0x%x\n",row_addr,ba_addr,  col_addr );
		#endif

		//fill write command
		//if(cmd_cnt < 16)
		{
			REG_WRITE_U32(bist_sram_addr,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len) << 1)| 0x1);
			bist_sram_addr+=4;
			REG_WRITE_U32(bist_sram_addr,(ba_addr&0x7));
			bist_sram_addr+=4;
		}
		//read command
		{
			REG_WRITE_U32(bist_sram_addr,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len) << 1)| 0x0);
			bist_sram_addr+=4;
			REG_WRITE_U32(bist_sram_addr,(ba_addr&0x7));
			bist_sram_addr+=4;
		}
		//for (cmd_cnt = ((pat_inc + 1)*2) ; cmd_cnt >0; cmd_cnt = cmd_cnt-8 ) {

		col_sh = ((col_addr&0x1f) << 5);



		row_addr = 0; 
		ba_addr = 0;  
		col_addr = 0;
	} 

}

void pat_gen()
{
  //int a = SDR_IDR;
  unsigned int bist_rg_sram_addr = BIST_RG_SRAM_BASE;
  unsigned int bist_wd_sram_addr = BIST_WD_SRAM_BASE;

  unsigned char in_bit[127];
  unsigned int  data_array[4];
  //unsigned int  data_trans[32];

  int  bit_cnt;

  int  dq_07; // dq[07:00]
  int  dq_15; // dq[15:08]
  int  dq_23; // dq[23:16]
  int  dq_31; // dq[31:24]
  int  dq;    // dq[31:00]

  //int  f_line_length   = 0; 
  //int  pattern_dum_cnt = 0;
  //int  dum_cnt         = 0; 
  //int  cmd_times       = 0;
  //int  cmd_cnt         = 0;
  int  pat_cnt         = 0;

  //int  wrrd_cnt        = 1; //wr=1, rd=0
  //int  isu_single_data_len     = 0;
  //int  isu_max_cmd_len         = 0;

  //unsigned int ddr_addr = 0; 
  //unsigned int row_addr = 0; 
  //unsigned int col_addr = 0;
  //unsigned int ba_addr  = 0;
  //unsigned int bst_size = 0;
  //data_array[0] = 0x158140ef;
  //data_array[1] = 0xaf4d954e;
  //data_array[2] = 0xdb5b94c1;
  //data_array[3] = 0x456ee2d8;
  //data_array[0] = 0xFFFFFFFF;
  //data_array[1] = 0xFFFFFFFF;
  //data_array[2] = 0xFFFFFFFF;
  //data_array[3] = 0xFFFFFFF0;
  data_array[0] = 0xfe041851;
  data_array[1] = 0xe459d4fa;
  data_array[2] = 0x1c49b5bd;
  data_array[3] = 0x8d2ee654;

  //for (bit_cnt =0; bit_cnt<32; bit_cnt++) {
  //  data_trans[bit_cnt] (data_array[(bit_cnt/32)] >> bit_cnt) & 0x1;

  //}



  for (bit_cnt =0; bit_cnt<127; bit_cnt++) {
  //for (bit_cnt =0; bit_cnt < 33; bit_cnt++) {
    in_bit[bit_cnt] = (data_array[(bit_cnt/32)] & (0x1 << (31-bit_cnt))) >> (31-bit_cnt);
    //if (bit_cnt ==0) {
    //  printf ("data_array = %x\n", (data_array[(bit_cnt/32)] & (0x1 << (31-bit_cnt)))>> 4);
    //}
    //printf ("in_bit[%x] = %x\n", bit_cnt, in_bit[bit_cnt]);
  }

  // generate DQ[31:0]

  dq_07 = 0; // dq[07:00]
  dq_15 = 0; // dq[15:08]
  dq_23 = 0; // dq[23:16]
  dq_31 = 0; // dq[31:24]

  for (pat_cnt =0; pat_cnt < TOTAL_WORD_CNT; pat_cnt ++) {

    // dq[07:00]
    for(bit_cnt=7; bit_cnt >=0; bit_cnt--) {
      dq_07 = (in_bit[dq_loop_07] << bit_cnt) | dq_07; 
      if(dq_loop_07 == 126){
        dq_loop_07 = 0;
      }
      else {
        dq_loop_07 = dq_loop_07 + 1;
      } 
    }

    // dq[15:08]
    for(bit_cnt=7; bit_cnt >=0; bit_cnt--) {
      dq_15 = (in_bit[dq_loop_15] << bit_cnt) | dq_15; 
      if(dq_loop_15 == 126){
        dq_loop_15 = 0;
      }
      else {
        dq_loop_15 = dq_loop_15 + 1;
      } 
    }

    // dq[23:16]
    for(bit_cnt=7; bit_cnt >=0; bit_cnt--) {
      dq_23 = (in_bit[dq_loop_23] << bit_cnt) | dq_23; 
      if(dq_loop_23 == 126){
        dq_loop_23 = 0;
      }
      else {
        dq_loop_23 = dq_loop_23 + 1;
      } 
    } 

    // dq[34:24]
    for(bit_cnt=7; bit_cnt >=0; bit_cnt--) {
      dq_31 = (in_bit[dq_loop_31] << bit_cnt) | dq_31; 
      if(dq_loop_31 == 126){
        dq_loop_31 = 0;
      }
      else {
        dq_loop_31 = dq_loop_31 + 1;
      } 
    } 

   	dq = (dq_31 <<24) | (dq_23 <<16) |(dq_15 <<8) | dq_07;

    #if 0
    _memctl_debug_printf ("%8x ", dq);
    
    if((!(pat_cnt%16)) && (pat_cnt != 0))
		_memctl_debug_printf("\n");
    #endif 
    //printf (f_data, "%8x\n", dq);
   
    REG_WRITE_U32(bist_wd_sram_addr, (dq));
    bist_wd_sram_addr = bist_wd_sram_addr + 4;

    REG_WRITE_U32(bist_rg_sram_addr, (dq));
    bist_rg_sram_addr = bist_rg_sram_addr + 4;
    

    dq_07 = 0; // dq[07:00]
    dq_15 = 0; // dq[15:08]
    dq_23 = 0; // dq[23:16]
    dq_31 = 0; // dq[31:24]
  } 


}


int dram_bist_read_write_test(unsigned int row_addr,unsigned int ba_addr, unsigned int col_addr)
{
	unsigned int row_bit, col_bit, bank_bit,bst_len;
	//unsigned int row_addr, col_addr, ba_addr;
	unsigned int wd_data_addr, rg_addr, rd_addr;
	unsigned int INIT_cnt, bstc_mode, i, err_cnt, rg_d, rd_d;
	struct ms_rxi310_portmap *ms_ctrl_0_map;

	//_memctl_debug_printf("Enter BTSC function \n");
	
	ms_ctrl_0_map = (struct ms_rxi310_portmap*) BSP_MS_I_DRAMC_0_BASE;
	wd_data_addr = BIST_WD_SRAM_BASE;
	rg_addr = BIST_RG_SRAM_BASE;
	
	col_bit = 10;
	row_bit = 13;
	bank_bit = 3;

	bst_len  = 4;
	//row_addr = 0x8;
	//col_addr = 0x4;
	//ba_addr = 0x1;
	
	//Enable BSTC function
	ms_ctrl_0_map->csr = 0x300;
	while(1)
	{
		if (((ms_ctrl_0_map-> csr) & 0x400) == 0x400)
		{
			udelay (10); /* Delay 10ns	*/
	  		 INIT_cnt++;
			_memctl_debug_printf("\nDRAM BSTC init is still active\n");
			 if (INIT_cnt>10)
		 		break;
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
		 	break;
		}
	}
	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

//jimmy comment
//flush bstc cmd/wd/msk/rg sram
//ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr | 0xf00;
//ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr & 0xFFFFF0FF;	

	//set BSTC related function
	bstc_mode = ms_ctrl_0_map->bcr;
	bstc_mode = bstc_mode | (1<<3) | (1<<2) | (1<<1);// | (1<<4);   // disable mask, loop mode, compare mode
	//RD_EXPC_LEVEL, command cnt * DQ bits * burst length 
	bstc_mode = bstc_mode | (((8*16*64)/32)<<16);
	ms_ctrl_0_map->bcr = bstc_mode;
	//_memctl_debug_printf("ms_ctrl_0_map->bcr = 0x%x\n", ms_ctrl_0_map->bcr);
	
  //jimmy added
  //set loop count
	ms_ctrl_0_map->bct = 0x10;

//jimmy added
//flush bstc cmd/wd/msk/rg sram
ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr | 0xf00;
ms_ctrl_0_map->bcr = ms_ctrl_0_map->bcr & 0xFFFFF0FF;	

#if 0
	//write CMD
	REG_WRITE_U32(BIST_CMD_SRAM_BASE,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len%8) << 1)| 0x1);
	REG_WRITE_U32(BIST_CMD_SRAM_BASE+0x4,(ba_addr&0x7));
	//_memctl_debug_printf("write cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE));
	
	REG_WRITE_U32(BIST_CMD_SRAM_BASE+0x8,((row_addr &0x7fff) << 17) | ((col_addr&0x7ff) << 6) | ((bst_len%8) << 1)| 0x0);
	REG_WRITE_U32(BIST_CMD_SRAM_BASE+0xc,(ba_addr&0x7));
	//_memctl_debug_printf("read cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0x8));
#else
	addr_gen();
#endif
  //jimmy comment
	//set loop count
//	ms_ctrl_0_map->bct = 0x10;
#if 0
	//write data
	REG_WRITE_U32(wd_data_addr, 0xffffffff);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0x00000000);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xaaaaaaaa);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0x55555555);
	//_memctl_debug_printf("WD data(%x) : 0x%x\n",wd_data_addr, REG_READ_U32(wd_data_addr));
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xa55a5aa5);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xa5a5a5a5);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xfe5566ef);
	wd_data_addr+=0x4;
	REG_WRITE_U32(wd_data_addr, 0xdead5566);




	//read golden
	REG_WRITE_U32(rg_addr, 0xffffffff);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x00000000);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0xaaaaaaaa);
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0x55555555);	
	//_memctl_debug_printf("RG data(%x) : 0x%x\n",rg_addr, REG_READ_U32(rg_addr));
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0xa55a5aa5);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0xa5a5a5a5);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0xfe5566ef);
	rg_addr+=0x4;
	REG_WRITE_U32(rg_addr, 0xdead5566);
#else
	pat_gen();
#endif
	//bist function start
	ms_ctrl_0_map->ccr = 0x4;
	while(1)
	{
		if (((ms_ctrl_0_map-> ccr) & 0x4) == 0x0)
		{
			//udelay (1000); /* Delay 1s	*/

			//_memctl_debug_printf("~");
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC test is done \n");
		 	break;
		}
	}	
	//read err cnt
	err_cnt = (ms_ctrl_0_map->bst & 0xFFFF0000)>>16;
	//_memctl_debug_printf("BSTC compare errpr cnt : %d\n",err_cnt);

// jimmy comment
	//flush fifo
//	ms_ctrl_0_map->ccr = 0x100;

	//read data in rd_sram
	rg_addr = BIST_RG_SRAM_BASE;
	rd_addr = BIST_RD_SRAM_BASE;
#if 0
	if(err_cnt != 0)
	{
		_memctl_debug_printf("BSTC compare errpr cnt : %d\n",err_cnt);
		_memctl_calibration_printf("RG(0x%x) : \n", rg_addr);
		for(i = 0; i < 512; i++)
		{
			_memctl_calibration_printf(" %x ", REG_READ_U32(rg_addr + i*4));
			if((i != 0) && (!(i%16)))
				_memctl_calibration_printf("\n");
		}
		_memctl_calibration_printf("\n");

		_memctl_calibration_printf("RD(0x%x) : \n", rd_addr);
		for(i = 0; i < 512; i++)
		{
			_memctl_calibration_printf(" %x ", REG_READ_U32(rd_addr + i*4));
			if((i != 0) && (!(i%16)))
				_memctl_calibration_printf("\n");

		}
		_memctl_calibration_printf("\n");
		return -1;
	}

	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

#else
	if(err_cnt != 0)
	{
		// jimmy added
			//flush fifo
			ms_ctrl_0_map->ccr = 0x100;
		
			//disable BSTC function
			ms_ctrl_0_map->csr = 0x700;
			while(1)
			{
				if (((ms_ctrl_0_map-> csr) & 0x700) != 0x700)
				{
					udelay (10); /* Delay 10ns	*/
					 INIT_cnt++;
					_memctl_debug_printf("\nDRAM BSTC disable is still active\n");
					 if (INIT_cnt>10)
						break;
				}
				else
				{
					//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
					break;
				}
			}
		return -1;
		for(i = 0; i < 512; i++)
		{
			rg_d = REG_READ_U32(rg_addr + i*4);
			rd_d = REG_READ_U32(rd_addr + i*4);
			if(rg_d != rd_d)
			{
				
				_memctl_calibration_printf("1bist test failed(%d) 0x%x != 0x%x \n", i,rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+1)*4);
				rd_d = REG_READ_U32(rd_addr + (i+1)*4);
				_memctl_calibration_printf("2bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+2)*4);
				rd_d = REG_READ_U32(rd_addr + (i+2)*4);
				_memctl_calibration_printf("3bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+3)*4);
				rd_d = REG_READ_U32(rd_addr + (i+3)*4);
				_memctl_calibration_printf("4bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				rg_d = REG_READ_U32(rg_addr + (i+4)*4);
				rd_d = REG_READ_U32(rd_addr + (i+4)*4);
				_memctl_calibration_printf("5bist test failed 0x%x != 0x%x \n", rg_d, rd_d);
				
				_memctl_debug_printf("write cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE));
				_memctl_debug_printf("write cmd2 : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE + 0x4));
				_memctl_debug_printf("read cmd : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0x8));
				_memctl_debug_printf("read cmd2 : 0x%x \n", REG_READ_U32(BIST_CMD_SRAM_BASE+0xc));
				return -1;
			}
		}
		
	}	
	//flush fifo
//	ms_ctrl_0_map->ccr = 0x100;

#endif
	
// jimmy added
	//flush fifo
	ms_ctrl_0_map->ccr = 0x100;

	//disable BSTC function
	ms_ctrl_0_map->csr = 0x700;
	while(1)
	{
		if (((ms_ctrl_0_map-> csr) & 0x700) != 0x700)
		{
			udelay (10); /* Delay 10ns	*/
	  		 INIT_cnt++;
			_memctl_debug_printf("\nDRAM BSTC disable is still active\n");
			 if (INIT_cnt>10)
		 		break;
		}
		else
		{
		 	//_memctl_debug_printf("\nDRAM BSTC enable is done \n");
		 	break;
		}
	}

	//udelay(1000000);
	return 0;
}
#ifndef DDR_CALIBRATION
void dram_all_mem_bist_test()
{
	unsigned int total_mem_size;
	unsigned int max_col_addr, max_bank_addr, max_row_addr;
	unsigned int col_bits, bank_bit, row_bit;
	unsigned int i,j,k,dram_result;

	col_bits = 10;
	bank_bit = 3;
	row_bit = 13;
	
	max_col_addr = (1<<col_bits);
	max_row_addr = (1<<row_bit);
	max_bank_addr = (1<<bank_bit);
	row_loop = 0; 
  	ba_loop = 32;  
	col_loop = 64; 

	dq_loop_07 = 0; // dq[07:00]
	dq_loop_15 = 32; // dq[15:08]
	dq_loop_23 = 64; // dq[23:16]
	dq_loop_31 = 96; // dq[31:24]
	for(i = 0; i < max_row_addr; i++)
	{
		for(j = 0; j < max_bank_addr; j++)
		{
			for(k = 0; k < (max_col_addr -32); k++)
			{
				// 32 byte test one time
				dram_result = dram_bist_read_write_test(i, j, k);
				if(dram_result == -1)
				{
					_memctl_calibration_printf("======================= \n", i, j ,k);
					//break;
				}
				//_memctl_calibration_printf("~");
			}
			dprintf(".");
		}
		
	}

	_memctl_calibration_printf("full dram bstc test PASS!\n");
	return;
}
#endif

int dram_test_repeat(void)
{

	unsigned int count = 0;
	unsigned int ddr_size = 0x4000000;
	while(1)
	{
		 if(dram_normal_patterns(0xa0000000, ddr_size, ddr_size))
		 {
		 	_memctl_calibration_printf("full memory dram_normal_patterns failed\n");
			return -1;//fail
		 }
		else
			_memctl_calibration_printf("full memory dram_normal_patterns OK\n");

		if(dram_addr_rot(0xa0000000, ddr_size, ddr_size))
		{
			_memctl_calibration_printf("full memory dram_addr_rot failed\n");	
			return -1;//fail
		}
		else
			_memctl_calibration_printf("full memory dram_addr_rot OK\n");	
	 
		if(dram_com_addr_rot(0xa0000000, ddr_size, ddr_size))
		{
			_memctl_calibration_printf("full memory dram_com_addr_rot failed\n");
			return -1;//fail
		}
		else
			_memctl_calibration_printf("full memory dram_com_addr_rot OK\n");
		
		//comment for code size 
		if(dram_byte_access(0xa0000000, ddr_size, ddr_size))
		{
			_memctl_calibration_printf("full memory dram_byte_access failed\n");
			return -1;//fail
		}
		else
			_memctl_calibration_printf("full memory dram_byte_access OK\n");
		
		if(dram_half_word_access(0xa0000000, ddr_size, ddr_size))
		{
			_memctl_calibration_printf("full memory dram_half_word_access failed\n");
			return -1;//fail
		}
		else
			_memctl_calibration_printf("full memory dram_half_word_access OK\n");		
			
		if(dram_walking_of_1(0xa0000000, ddr_size, ddr_size))
		{
			_memctl_calibration_printf("full memory dram_walking_of_1 failed\n");
			return -1;//fail
		}
		else
			_memctl_calibration_printf("full memory dram_walking_of_1 OK\n");
		
		 if(dram_walking_of_0(0xa0000000, ddr_size, ddr_size))
		 {
		 	_memctl_calibration_printf("full memory dram_walking_of_0 failed\n");
			return -1;//fail
		 }
		else
			_memctl_calibration_printf("full memory dram_walking_of_0 OK\n");	
			
		 if(memcpy_test(0xa0000000, ddr_size, ddr_size))
		 {
		 	 _memctl_calibration_printf("full memory memcpy_test failed\n");
			return -1;//fail
		 }
		 else
			 _memctl_calibration_printf("full memory memcpy_test OK\n");

		count++;
		if((count%1000) == 0)
			_memctl_calibration_printf("test OK print count : %d\n", count);
		if(count == 0xffffffff)
			count = 0;
	}
		return 0;

}
#endif

//#ifndef DDR_CALIBRATION
void dram_calibration_turn_on_odt(const struct dram_device_info *dram_info)
{
	_memctl_calibration_printf("\n dram_init.c : dram_calibration_turn_on_odt.\n");
	
	//REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcfffff) | 0x00300000); 
	REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcf8000) | dram_info->phy->odt_default); 
	//#ifdef DDR_K_DQS_EN
	//REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00090006); 
	//REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00090006); 
	//#else
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00060009); 
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00060009); 
	//#endif
	//set DQ ODT 79 ohm
	//REG_WRITE_U32(PAD_DQ, (REG_READ_U32(PAD_DQ) & 0xFFFFFFF0) | 0xa);             // DQ 0 ,default 1, set to 2
	//REG_WRITE_U32(PAD_DQ_1, (REG_READ_U32(PAD_DQ_1) & 0xFFFFFFF0) | 0xa);             // DQ 8, default 1, set to 2

	#if 1//def DRAM_ADD_OUTPUT_DRIVING
	REG_WRITE_U32(PAD_DCK, (REG_READ_U32(PAD_DCK) & 0xFFFFFF00) | (dram_info->phy->dck_driving));  //ck default 0, set to 1
	REG_WRITE_U32(PAD_CMD, (REG_READ_U32(PAD_CMD) & 0xFFFFFFF0) | (dram_info->phy->cmd_driving));  // CMD default 0, set to 1
	REG_WRITE_U32(PAD_BK_ADR, (REG_READ_U32(PAD_BK_ADR) & 0xFFFFFFF0) | (dram_info->phy->ba_addr_driving)); //bank addr, default 0, set to 1
	REG_WRITE_U32(PAD_DQ, (REG_READ_U32(PAD_DQ) & 0xFFFFF0FF) | (dram_info->phy->dq_driving<<8) );//| (1 << 5) | (1 << 7));             // DQ 0 ,default 1, set to 2
	REG_WRITE_U32(PAD_DQ_1, (REG_READ_U32(PAD_DQ_1) & 0xFFFFF0FF) | (dram_info->phy->dq_driving<<8) );//| (1 << 5) | (1 << 7));             // DQ 8, default 1, set to 2
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xFFFF00FF) | (dram_info->phy->dqs_driving << 8));   // DQSL, default 1, set to 3
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xFFFF00FF) | (dram_info->phy->dqs_driving << 8)); //DQSU, default 1, set to 3
	#endif

	#if 0
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);
	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xfffff0f0) | 0x0f0f);
	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0xff);
	REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	#endif
	
	return;
}
//#endif

void dram_set_wrlvl_fb(int mck_dqs)
{
	if(mck_dqs <= -58)
	{
		_memctl_calibration_printf("mck_ck = %d, %d\n", mck_dqs, __LINE__);
		REG_WRITE_U32(WRLVL_CTRL, (REG_READ_U32(WRLVL_CTRL) & 0xfffffffc) | 0x00000003); // wrlvl_fb = 1
		REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xfcffffff) | 0x00000000); // wrlv_delay = 0
	}
	else if((mck_dqs >= -57) && (mck_dqs <= -1))
	{
		_memctl_calibration_printf("mck_ck = %d, %d\n", mck_dqs, __LINE__);
		REG_WRITE_U32(WRLVL_CTRL, (REG_READ_U32(WRLVL_CTRL) & 0xfffffffc) | 0x00000000); // wrlvl_fb = 0
		REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xfcffffff) | 0x00000000); // wrlv_delay = 0

	}
	else if((mck_dqs >= 0) && (mck_dqs <= 26))
	{
		_memctl_calibration_printf("mck_ck = %d, %d\n", mck_dqs, __LINE__);
		REG_WRITE_U32(WRLVL_CTRL, (REG_READ_U32(WRLVL_CTRL) & 0xfffffffc) | 0x00000003); // wrlvl_fb = 1
		REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xfcffffff) | 0x00000000); // wrlv_delay = 0

	}
	else if((mck_dqs >= 27) && (mck_dqs <= 57))
	{
		_memctl_calibration_printf("mck_ck = %d, %d\n", mck_dqs, __LINE__);
		REG_WRITE_U32(WRLVL_CTRL, (REG_READ_U32(WRLVL_CTRL) & 0xfffffffc) | 0x00000000); // wrlvl_fb = 0
		REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xfcffffff) | 0x03000000); // wrlv_delay = 1

	}
	else if((mck_dqs >= 58) && (mck_dqs <= 63))
	{
		_memctl_calibration_printf("mck_ck = %d, %d\n", mck_dqs, __LINE__);
		REG_WRITE_U32(WRLVL_CTRL, (REG_READ_U32(WRLVL_CTRL) & 0xfffffffc) | 0x00000003); // wrlvl_fb = 1
		REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xfcffffff) | 0x03000000); // wrlv_delay = 1
	}

}

#ifdef DDR_CALIBRATION
int dram_test_full(void)
{
	#ifndef DDR_TEST
		unsigned int test_size = 0x100;//0x4000
		if(dram_addr_rot(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail

		#if 0
		 if(dram_normal_patterns(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail

		if(dram_addr_rot(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
	
	 
		if(dram_com_addr_rot(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
	
		//comment for code size 
		if(dram_byte_access(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
			
		if(dram_half_word_access(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
		
			
		if(dram_walking_of_1(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
			
		 if(dram_walking_of_0(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
	
			
		 if(memcpy_test(CONFIG_SYS_MEMTEST_START, test_size, test_size))
			return -1;//fail
		#endif
		return 0;
	#else
	int i;
	for( i = 0; i < 200; i++)
	{
		row_loop = 0; 
		ba_loop = 32;  
		col_loop = 64; 
		
		dq_loop_07 = 0; // dq[07:00]
		dq_loop_15 = 32; // dq[15:08]
		dq_loop_23 = 64; // dq[23:16]
		dq_loop_31 = 96; // dq[31:24]

		if(dram_bist_read_write_test(0,0,0) == -1)
			return -1;
	}
	return 0;
	#endif
}


void dram_calibration_odt_always_on(const struct dram_device_info *dram_info)
{
	_memctl_calibration_printf("\n dram_init.c : dram_calibration_turn_on_odt.\n");
	REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcfffff) | 0x00300000); 
	//#ifdef DDR_K_DQS_EN
	//REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00090006); 
	//REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00090006); 
	//#else
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00060009); 
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00060009); 
	//#endif

#if 1//def DRAM_ADD_OUTPUT_DRIVING
	REG_WRITE_U32(PAD_DCK, (REG_READ_U32(PAD_DCK) & 0xFFFFFF00) | (dram_info->phy->dck_driving));  //ck default 0, set to 1
	REG_WRITE_U32(PAD_CMD, (REG_READ_U32(PAD_CMD) & 0xFFFFFFF0) | (dram_info->phy->cmd_driving));  // CMD default 0, set to 1
	REG_WRITE_U32(PAD_BK_ADR, (REG_READ_U32(PAD_BK_ADR) & 0xFFFFFFF0) | (dram_info->phy->ba_addr_driving)); //bank addr, default 0, set to 1
	REG_WRITE_U32(PAD_DQ, (REG_READ_U32(PAD_DQ) & 0xFFFFF0FF) | (dram_info->phy->dq_driving<<8));             // DQ 0 ,default 1, set to 2
	REG_WRITE_U32(PAD_DQ_1, (REG_READ_U32(PAD_DQ_1) & 0xFFFFF0FF) | (dram_info->phy->dq_driving<<8));             // DQ 8, default 1, set to 2
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xFFFF00FF) | (dram_info->phy->dqs_driving << 8));   // DQSL, default 1, set to 3
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xFFFF00FF) | (dram_info->phy->dqs_driving << 8)); //DQSU, default 1, set to 3
#endif

	
	return;
}
/*
void dram_phy_bist_function()
{
	unsigned int bist_fail_data;
	//	DQS PI the same as mck_cmd (0), set this in dram_phy_ip_info

	//	RX path setting=========================
	//	rst_fifo_mode = 0x2 (disable reset FIFO pointer)
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xFFFFFFF3) | (0x2<<2));
	//	fw_set_mode = 0x2 (immediately)
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xFFFFFFFC) | (0x2));
	//	fw_rd_dly_pos_sel_0 = 0x0 (rising edge delay of DQ[0])
	REG_WRITE_U32(DQS_IN_DLY_0, (REG_READ_U32(DQS_IN_DLY_0) & 0xFFFFFFE0) | (0x0));
	//	fw_rd_dly_neg_sel_0 = 0x0 (falling edge delay of DQ[0])
	REG_WRITE_U32(DQS_IN_DLY_2, (REG_READ_U32(DQS_IN_DLY_2) & 0xFFFFFFE0) | (0x0));
	//	rd_dly_follow_dq0 = 0x1 (all DQ delay follow DQ[0])
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xFFFFFBFF) | (0x1 << 10));
	//	dqsen_mode = 0x1 (PUPD3)
	//REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & READ_CTRL_1) | (0x1 << 10));
	//	tm_dqs_en = 0xd
	REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xFFFFFFE0) | (0xd));
	//	tm_rd_fifo = 0x8
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xFFFFFFE0) | 0x8);
	REG_WRITE_U32(DPI_CTRL_1, REG_READ_U32(DPI_CTRL_1) | 0xc);

	//Loop back setting
	//	lbk_mode = 0x1 (bypass PAD) or 0x2 (through PAD)
	REG_WRITE_U32(TEST_CTRL0, (REG_READ_U32(TEST_CTRL0) & 0x3FFFFFFF) | (0x1<<30));
	//	bist_mode = 0x0 (PRBS7) or 0x1(PRBS13) or 0x2(PRBS31)
	REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xFF8FFFFF) | (0x0<<22));

	//	BIST test enable
	REG_WRITE_U32(BIST_2TO1_0, (REG_READ_U32(BIST_2TO1_0) & 0xFF7FFFFF) | (0x1<<23));

	//	wait BIST result
	while(1)
	{
		if(REG_READ_U32(BIST_2TO1_1) & 0x20)
			break;
		else
		{
			//_memctl_calibration_printf("PHY ip bist function still work.\n");
			udelay(1000);
			//maybe set a timeout 
		}
	}

	bist_fail_data = REG_READ_U32(BIST_2TO1_1) & 0x3;
	if(bist_fail_data)
		_memctl_calibration_printf("phy bist test failed!! \n");
	else
		_memctl_calibration_printf("phy bist test pass!! \n");

	return;
}
*/
void dram_calibration_set_oe_sync(int mck_ck)
{
	if((mck_ck >= 0) && (mck_ck <= 15))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0x0 << 16)); 
	else	if((mck_ck >= 16) && (mck_ck <= 31))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0x1 << 16)); //0xd -> 0x1 joyce
	else if((mck_ck >= 32) && (mck_ck <= 47))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0x0 << 16)); 
	else if((mck_ck >= 48) && (mck_ck <= 63))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0x1 << 16)); //0x1 joyce

}

void dram_calibration_set_oe_sync_dqs(int mck_dqs)
{
	if((mck_dqs >= 0) && (mck_dqs <= 15))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF1FFFF) | (0x0 << 16));		
	else if((mck_dqs >= 16) && (mck_dqs <= 31))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF1FFFF) | (0xC << 16)); 
	else if((mck_dqs >= 32) && (mck_dqs <= 47))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF1FFFF) | (0x0 << 16)); 
	else if((mck_dqs >= 48) && (mck_dqs <= 63))
		REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF1FFFF) | (0xC << 16)); 
}

void dram_calibration_set_ck_dqs_default(const struct dram_device_info *dram_info)
{
	int mck_ck_default, mck_dqs_default, mck_in_sequence, mdqs_in_sequence;
	int mck_ck = 0, mck_dqs = 0;
	mck_ck_default = dram_info->phy->mck_default;  // -63 ~ 63
	mck_dqs_default = dram_info->phy->mdqs_default;
	mck_ck = mck_in_sequence = mck_ck_default;
	mck_dqs = mdqs_in_sequence = mck_dqs_default;
	
	if(mck_in_sequence < 0)
		mck_ck = (64+mck_in_sequence);	// if -1, it means -63 in actually, PI phase should set 63
	else
		mck_ck = mck_in_sequence;
		
	if(mdqs_in_sequence < 0)
		mck_dqs = (64+mdqs_in_sequence);	// if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
	else
		mck_dqs = mdqs_in_sequence;	
		
	TURN_OFF_CLOCK();
	dram_calibration_set_oe_sync(mck_ck); 
	dram_calibration_set_oe_sync_dqs(mck_dqs); //joyce

	REG_WRITE_U32(REG_DDR_PLL_3, (mck_dqs << 24) | (mck_ck << 8)); //joyce
	REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs)); //joyce
	
	dram_set_wrlvl_fb((mdqs_in_sequence < 0)?(-mck_dqs):mck_dqs); //joyce
	TURN_ON_CLOCK();
}

void dram_calibration_set_dqs_default(const struct dram_device_info *dram_info)
{
	int mck_dqs_default, mdqs_in_sequence;
	int mck_dqs = 0;
	mck_dqs_default = dram_info->phy->mdqs_default;// -63 ~ 63
	mck_dqs = mdqs_in_sequence = mck_dqs_default;
	
	if(mdqs_in_sequence < 0)
		mck_dqs = (64+mdqs_in_sequence);	// if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
	else
		mck_dqs = mdqs_in_sequence;	
		
	TURN_OFF_CLOCK();
	dram_calibration_set_oe_sync_dqs(mck_dqs); //joyce

	REG_WRITE_U32(REG_DDR_PLL_3, (mck_dqs << 24)); //joyce
	REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs)); //joyce
	
	dram_set_wrlvl_fb((mdqs_in_sequence < 0)?(-mck_dqs):mck_dqs); //joyce
	TURN_ON_CLOCK();
}

void dram_calibration_set_rx_default(const struct dram_device_info *dram_info)
{
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);  // fw_set_mode=10Œccal_set_mode=11
		
		REG_WRITE_U32(DQS_IN_DLY_0, dram_info->phy->rx_delay_tap_0); 
		REG_WRITE_U32(DQS_IN_DLY_1, dram_info->phy->rx_delay_tap_1); 
		REG_WRITE_U32(DQS_IN_DLY_0_1, dram_info->phy->rx_delay_tap_2); 
		REG_WRITE_U32(DQS_IN_DLY_1_1, dram_info->phy->rx_delay_tap_3); 
		REG_WRITE_U32(DQS_IN_DLY_2, dram_info->phy->rx_delay_tap_n_0); 
		REG_WRITE_U32(DQS_IN_DLY_3, dram_info->phy->rx_delay_tap_n_1); 
		REG_WRITE_U32(DQS_IN_DLY_2_1, dram_info->phy->rx_delay_tap_n_2); 
		REG_WRITE_U32(DQS_IN_DLY_3_1, dram_info->phy->rx_delay_tap_n_3); 
		
		REG_WRITE_U32(CAL_LS_SEL, dram_info->phy->pre_shift_0); //dq 0 ~ 7 pre
		REG_WRITE_U32(CAL_LS_SEL_1, dram_info->phy->pre_shift_0); //dq 8 ~ 15 pre
		REG_WRITE_U32(CAL_RS_SEL, dram_info->phy->post_shift_0); //dq 0 ~ 7 post
		REG_WRITE_U32(CAL_RS_SEL_1, dram_info->phy->post_shift_1); //dq 8 ~ 15 post	

		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
		
		//enable auto calibration
		REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcf) | 0x00);  // cal_set_mode=00	
	
}

void dram_calibration_set_tx_default(const struct dram_device_info *dram_info)
{
		REG_WRITE_U32(DQ_DLY_0, dram_info->phy->tx_delay_tap_0);
		REG_WRITE_U32(DQ_DLY_0_1, dram_info->phy->tx_delay_tap_1);
		REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xFFFF0000) | dram_info->phy->dqs_dm_delay_tap);
		REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0x44);
					
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);	
}

void dram_calibration_set_rx_odt_default(const struct dram_device_info *dram_info)
{
	int i, left_bond, right_bond;

		for(i = 0; i< 15; i++)
    {
    	if((dram_info->phy->odt_default<<i & 0x4000) == 0x4000)
            {
                left_bond = 14-i;
                break;
            }
    }

    for(i = 0; i< 15; i++)
    {
            if((dram_info->phy->odt_default>>i & 0x1) == 0x1)
            {
                right_bond = i;
                break;
            }
    }

    udelay(10000);
    for(i = 0; i < left_bond+1 ; i++)
    {
            REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) ) | ((1<<i) & 0x7fff));
						dram_test_full();
    }

    for(i = 0; i < right_bond+1; i++)
    {
            REG_WRITE_U32(READ_CTRL_2, REG_READ_U32(READ_CTRL_2) & (0xffffffff<<i));
						dram_test_full();
    }


    REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcf8000) | dram_info->phy->odt_default);
}



void set_default_values(const struct dram_device_info *dram_info)
{
			//CK DQS
			dram_calibration_set_ck_dqs_default(dram_info);	
			udelay(1000);	
			//DQS_EN
			REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xffffffe0) |  dram_info->phy->dqs_en_default);
			REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xffffffe0) | dram_info->phy->dqs_en_default);
			udelay(1000);
			//RX
			dram_calibration_set_rx_default(dram_info);		
			udelay(1000);
			// ODT
      //REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcf8000) | dram_info->phy->odt_default);
      dram_calibration_set_rx_odt_default(dram_info);
			//TX
			dram_calibration_set_tx_default(dram_info);
			udelay(1000);
}

#endif

#ifdef DDR_K_DQS
int dram_calibration_dqs_map(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	
	printf("\n dram_init.c : dram_dqs_calibration.\n");
	int i;
	int dram_test_fail = 0;
	int mck_dqs = 0, mck_dqs_pos = 0, mck_dqs_neg = 0, mck_dqs_mid = 0, dqs_in_sequence = 0;
	int mck_dqs_scale, is_cross_boundary = 0;

	_memctl_calibration_printf("*****DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
		REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
		, REG_READ_U32(BIST_2TO1_0));
	//return;
	// scan dqs positive
	mck_dqs = dqs_in_sequence = phy_info->mck_default;//dram_info->phy->mdqs_default;
#if 1//def DQS_POS_SCAN
	if((mck_dqs >= 0) && (mck_dqs <= 31))
		mck_dqs_scale = 31;
	else if((mck_dqs >= 32) && (mck_dqs <= 63))
		mck_dqs_scale = 63;
	else if((mck_dqs <= -1) && (mck_dqs >= -32))
		mck_dqs_scale = -1;
	else if((mck_dqs <= -33) && (mck_dqs >= -63))
		mck_dqs_scale = -33;


	while(!dram_test_fail)	 // it means default value is a valid value for ddr W/R
	{
		if(dqs_in_sequence < 0)
			mck_dqs = (64+dqs_in_sequence);	// if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
		else
			mck_dqs = dqs_in_sequence;
		
		if(dqs_in_sequence > mck_dqs_scale)
		{
			if ( mck_dqs_scale == 63)
				break;
			else
			{
				is_cross_boundary = 1;
				_memctl_calibration_printf("============cross boundary %d ===========\n", dqs_in_sequence);
				
			}
		}

		TURN_OFF_CLOCK();

		dram_calibration_set_oe_sync_dqs(mck_dqs); 
		
		REG_WRITE_U32(REG_DDR_PLL_3, (REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | (mck_dqs << 24));
		REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs));
		
		_memctl_calibration_printf("DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
			REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
			, REG_READ_U32(BIST_2TO1_0));
		
		dram_set_wrlvl_fb((dqs_in_sequence < 0)?(-mck_dqs):mck_dqs);
			
		if(is_cross_boundary)
		{
			is_cross_boundary = 0;
			
		}
		
		TURN_ON_CLOCK();

		//for(i = 0; i < 20; i++)
		{
			dram_test_fail = dram_test_full();
			if(dram_test_fail)
				_memctl_calibration_printf("pos dqs_in_sequence %d failed\n", dqs_in_sequence);
		}
		if(!dram_test_fail)
			_memctl_calibration_printf("pos dqs_in_sequence %d OK\n", dqs_in_sequence);

		if((dqs_in_sequence >= 0) && (dqs_in_sequence <= 31))
			mck_dqs_scale = 31;
		else if((dqs_in_sequence >= 32) && (dqs_in_sequence <= 63))
			mck_dqs_scale = 63;
		else if((dqs_in_sequence <= -1) && (dqs_in_sequence >= -32))
			mck_dqs_scale = -1;
		else if((dqs_in_sequence <= -33) && (dqs_in_sequence >= -63))
			mck_dqs_scale = -33;

		dqs_in_sequence ++;

	}
	mck_dqs_pos = dqs_in_sequence - 1;   
	//dram_calibration_set_dqs_default(dram_info);
//#else
//=====================================================================
	_memctl_calibration_printf("right shift scan done, scan left shift ==========\n ");
	mck_dqs = dqs_in_sequence = phy_info->mck_default;//dram_info->phy->mdqs_default;

	//scan mck negitive
	if((mck_dqs >= 0) && (mck_dqs <= 31))
		mck_dqs_scale = 0;
	else if((mck_dqs >= 32) && (mck_dqs <= 63))
		mck_dqs_scale = 32;
	else if((mck_dqs <= -1) && (mck_dqs >= -32))
		mck_dqs_scale = -32;
	else if((mck_dqs <= -33) && (mck_dqs >= -63))
		mck_dqs_scale = -63;
	

	dram_test_fail = 0;
	//mck_dqs = dqs_in_sequence = dram_info->phy->mck_default;
	while(!dram_test_fail)
	{
		if(dqs_in_sequence < 0)
			mck_dqs = (64+dqs_in_sequence);	// if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
		else
			mck_dqs = dqs_in_sequence;
		
		if(dqs_in_sequence < mck_dqs_scale)
		{
			if ( mck_dqs_scale == -63)
				break;
			else
			{
				is_cross_boundary = 1;
				_memctl_calibration_printf("============cross boundary %d ===========\n", dqs_in_sequence);
				
			}
		}
		
		TURN_OFF_CLOCK();

		dram_calibration_set_oe_sync_dqs(mck_dqs); 

		REG_WRITE_U32(REG_DDR_PLL_3, (REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | (mck_dqs << 24));
		REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs));
		_memctl_calibration_printf("DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
			REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
			, REG_READ_U32(BIST_2TO1_0));
		
		dram_set_wrlvl_fb((dqs_in_sequence < 0)?(-mck_dqs):mck_dqs);
		
		if(is_cross_boundary)
		{
			is_cross_boundary = 0;
			
		}
		
		TURN_ON_CLOCK();
		
		//for(i = 0; i < 20; i++)
		{
			dram_test_fail = dram_test_full();
			if(dram_test_fail)
				_memctl_calibration_printf("neg dqs_in_sequence %d failed\n", dqs_in_sequence);
		}
		if(!dram_test_fail)
			_memctl_calibration_printf("neg dqs_in_sequence %d OK\n", dqs_in_sequence);
		
		if((dqs_in_sequence >= 0) && (dqs_in_sequence <= 31))
			mck_dqs_scale = 0;
		else if((dqs_in_sequence >= 32) && (dqs_in_sequence <= 63))
			mck_dqs_scale = 32;
		else if((dqs_in_sequence <= -1) && (dqs_in_sequence >= -32))
			mck_dqs_scale = -32;
		else if((dqs_in_sequence <= -33) && (dqs_in_sequence >= -63))
			mck_dqs_scale = -63;

		dqs_in_sequence --;

	}	
	mck_dqs_neg = dqs_in_sequence + 1;

#endif
#ifdef DDR_CALIBRATION_RESULT_SET
	// 1. set mck to middle point
	#ifdef DDR_CALIBRATION_RESULT_SET_97F_DS
	mck_dqs_mid = mck_dqs_pos - 6;
	#else
	mck_dqs_mid = (mck_dqs_pos + mck_dqs_neg)/2;
	#endif
	
	if(mck_dqs_mid < 0)
		mck_dqs = (64+mck_dqs_mid);	// if -1, it means -63 in actually, PI phase should set 63
	else
		mck_dqs = mck_dqs_mid;
		
	TURN_OFF_CLOCK();
	
	dram_calibration_set_oe_sync_dqs(mck_dqs); 
		
	
	REG_WRITE_U32(REG_DDR_PLL_3, (REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | (mck_dqs << 24));
	REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs));
	
	dram_set_wrlvl_fb((mck_dqs_mid < 0)?(-mck_dqs):mck_dqs);
	
	TURN_ON_CLOCK();

	phy_info->mdqs_default = mck_dqs_mid;
	
	dram_test_fail = dram_test_full();
	
	if(dram_test_fail)
	{
		printf("Dram test fail after dqs calibration, it should never happen.  Adopt initial dqs values. Detected: %d. Default: %d\n", phy_info->mdqs_default, dram_info->phy->mdqs_default);
				
		dram_calibration_set_ck_dqs_default(dram_info); 
		
		return -1;
	}
	else
	{
		printf("Dram test success after dqs calibration. Detected: %d. Default: %d\n", phy_info->mdqs_default, dram_info->phy->mdqs_default);

	}
		
	#ifdef DDR_CALIBRATION_RESULT_SET_97F_DS
		mck_dqs_mid = (mck_dqs_pos + mck_dqs_neg)/2;
		
		if(mck_dqs_mid < 0)
		mck_dqs = (64+mck_dqs_mid);	// if -1, it means -63 in actually, PI phase should set 63
		else
		mck_dqs = mck_dqs_mid;
		
		TURN_OFF_CLOCK();
	
		dram_calibration_set_oe_sync_dqs(mck_dqs); 
		
		REG_WRITE_U32(REG_DDR_PLL_3, (REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | (mck_dqs << 24));
		REG_WRITE_U32(REG_DDR_PLL_4, (mck_dqs));
	
		dram_set_wrlvl_fb((mck_dqs_mid < 0)?(-mck_dqs):mck_dqs);
	
		TURN_ON_CLOCK();

		dram_test_fail = dram_normal_patterns(CONFIG_SYS_MEMTEST_START, 0x100, 0x10);
	
		if(dram_test_fail)
		{
			printf("Dram test fail after dqs calibration, it should never happen.  Adopt initial dqs values. Detected: %d. Default: %d\n", phy_info->mdqs_default, dram_info->phy->mdqs_default);
					
			dram_calibration_set_ck_dqs_default(dram_info); 
			
			return -1;
		}
		else
		{
			printf("Use middle value of dqs calibration for the following calibrating. Middle value: %d. Default: %d\n", mck_dqs_mid, dram_info->phy->mdqs_default);
	
		}
	#endif
#endif
	return 0;

}
#endif

#ifdef DDR_K_MCK

int dram_calibration_dck_map(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	printf("\ndram_init.c : dram_dck_calibration.\n");
	int dram_test_fail = 0;
	int mck_ck_default, mck_ck_scale, is_cross_boundary = 0;
	int mck_ck = 0, mck_ck_pos = 0, mck_ck_neg = 0, mck_ck_mid = 0, mck_in_sequence = 0;
	
	// scan mck positive
	mck_ck_default = dram_info->phy->mck_default;  // -63 ~ 63
	mck_ck = mck_in_sequence = mck_ck_default;
	_memctl_calibration_printf("*****DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
		REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
		, REG_READ_U32(BIST_2TO1_0));

#if 1//def DCK_POS_SCAN
	if((mck_ck >= 0) && (mck_ck <= 31))
		mck_ck_scale = 31;
	else if((mck_ck >= 32) && (mck_ck<= 63))
		mck_ck_scale = 63;
	else if((mck_ck <= -1) && (mck_ck >= -32))
		mck_ck_scale = -1;
	else if((mck_ck <= -33) && (mck_ck >= -63))
		mck_ck_scale = -33;

	while(!dram_test_fail)   // it means default value is a valid value for ddr W/R
	{

		if(mck_in_sequence < 0)
			mck_ck = (64+mck_in_sequence);  // if -1, it means -63 in actually, PI phase should set 63
		else
			mck_ck = mck_in_sequence;
		if(mck_in_sequence > mck_ck_scale)
		{
			if ( mck_ck_scale == 63)
				break;
			else
			{
				is_cross_boundary = 1;
				//mck_ck_scale+=32;
				_memctl_calibration_printf("============cross boundary %d ===========\n", mck_in_sequence);
			}
		}
		
		TURN_OFF_CLOCK();

		dram_calibration_set_oe_sync(mck_ck); 
		dram_calibration_set_oe_sync_dqs(mck_ck); 

		REG_WRITE_U32(REG_DDR_PLL_3, (mck_ck << 24) | (mck_ck << 8));
		REG_WRITE_U32(REG_DDR_PLL_4, (mck_ck));

		_memctl_calibration_printf("DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
			REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
			, REG_READ_U32(BIST_2TO1_0));

		dram_set_wrlvl_fb((mck_in_sequence < 0)?(-mck_ck):mck_ck);
		if(is_cross_boundary)
		{
			is_cross_boundary = 0;
		}

		TURN_ON_CLOCK();
			
		//udelay(40000);
		dram_test_fail = dram_test_full();
		if(!dram_test_fail)
			_memctl_calibration_printf("pos mck_in_sequence %d OK\n", mck_in_sequence);
		else
			_memctl_calibration_printf("pos mck_in_sequence %d failed\n", mck_in_sequence);

		if((mck_in_sequence >= 0) && (mck_in_sequence <= 31))
			mck_ck_scale = 31;
		else if((mck_in_sequence >= 32) && (mck_in_sequence<= 63))
			mck_ck_scale = 63;
		else if((mck_in_sequence <= -1) && (mck_in_sequence >= -32))
			mck_ck_scale = -1;
		else if((mck_in_sequence <= -33) && (mck_in_sequence >= -63))
			mck_ck_scale = -33;

		mck_in_sequence ++;
	}
	mck_ck_pos = mck_in_sequence - 1;

	dram_calibration_set_ck_dqs_default(dram_info);
	
//#else
//=====================================================================
	_memctl_calibration_printf("right shift scan done, scan left shift ==========\n ");
	mck_ck_default = dram_info->phy->mck_default;  // -63 ~ 63
	mck_ck = mck_in_sequence = mck_ck_default;
	if((mck_ck >= 0) && (mck_ck <= 31))
		mck_ck_scale = 0;
	else if((mck_ck >= 32) && (mck_ck <= 63))
		mck_ck_scale = 32;
	else if((mck_ck <= -1) && (mck_ck >= -32))
		mck_ck_scale = -32;
	else if((mck_ck <= -33) && (mck_ck >= -63))
		mck_ck_scale = -63;

	//scan mck negitive
	dram_test_fail = 0;
	//mck_ck = mck_in_sequence = mck_ck_default;
	while(!dram_test_fail)
	{
		if(mck_in_sequence < 0)
			mck_ck = (64+mck_in_sequence); // if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
		else
			mck_ck = mck_in_sequence;
		if(mck_in_sequence < (mck_ck_scale))
		{
			if ( mck_ck_scale == -63)
				break;
			else
			{
				is_cross_boundary = 1;
				//mck_ck_scale+=32;
				_memctl_calibration_printf("============cross boundary %d ===========\n", mck_in_sequence);
				
			}
		}
		
		TURN_OFF_CLOCK();

		dram_calibration_set_oe_sync(mck_ck); 
		dram_calibration_set_oe_sync_dqs(mck_ck); 

		/*
		if((mck_ck >= 16) && (mck_ck <= 31))
			REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0xD << 16)); 
		else if((mck_ck >= 32) && (mck_ck <= 47))
			REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0x0 << 16)); 
		else if((mck_ck >= 48) && (mck_ck <= 63))
			REG_WRITE_U32(REG_DDR_PLL_7,(REG_READ_U32(REG_DDR_PLL_7) & 0xFFF0FFFF) | (0xD << 16)); 
	*/
		
		REG_WRITE_U32(REG_DDR_PLL_3, (mck_ck << 24) | (mck_ck << 8));
		REG_WRITE_U32(REG_DDR_PLL_4, (mck_ck));
		_memctl_calibration_printf("DQS 0 : 0x%x, DQS1 : 0x%x, OE sync : 0x%x, fb : 0x%x, 2to1 : 0x%x\n",REG_READ_U32(REG_DDR_PLL_3),
			REG_READ_U32(REG_DDR_PLL_4),  REG_READ_U32(REG_DDR_PLL_7),REG_READ_U32(WRLVL_CTRL)
			, REG_READ_U32(BIST_2TO1_0));


		dram_set_wrlvl_fb((mck_in_sequence < 0)?(-mck_ck):mck_ck);
		if(is_cross_boundary)
		{
			is_cross_boundary = 0;
			
		}
		
		TURN_ON_CLOCK();
		
		dram_test_fail = dram_test_full();
		if(!dram_test_fail)
			_memctl_calibration_printf("negative mck_in_sequence %d OK\n", mck_in_sequence);
		else
			_memctl_calibration_printf("negative mck_in_sequence %d failed\n", mck_in_sequence);

		if((mck_in_sequence >= 0) && (mck_in_sequence <= 31))
			mck_ck_scale = 0;
		else if((mck_in_sequence >= 32) && (mck_in_sequence <= 63))
			mck_ck_scale = 32;
		else if((mck_in_sequence <= -1) && (mck_in_sequence >= -32))
			mck_ck_scale = -32;
		else if((mck_in_sequence <= -33) && (mck_in_sequence >= -63))
			mck_ck_scale = -63;

		mck_in_sequence --;
	}	
	mck_ck_neg = mck_in_sequence + 1;
#endif	
	#ifdef DDR_CALIBRATION_RESULT_SET
	// 1. set mck to middle point
	mck_ck_mid = (mck_ck_pos + mck_ck_neg)/2;
	if(mck_ck_mid < 0)
		mck_ck = (64+mck_ck_mid);	// if mck_in_sequence -1, it means -63 in mapping table, PI phase should set 63
	else
		mck_ck = mck_ck_mid;
	
	TURN_OFF_CLOCK();
	
	dram_calibration_set_oe_sync(mck_ck); 
	dram_calibration_set_oe_sync_dqs(mck_ck);  

	dram_set_wrlvl_fb((mck_ck_mid < 0)?(-mck_ck):mck_ck);
	REG_WRITE_U32(REG_DDR_PLL_3, (mck_ck << 24) | (mck_ck << 8));
	REG_WRITE_U32(REG_DDR_PLL_4, (mck_ck));

	TURN_ON_CLOCK();

	// 2. scan DQS map
	//dram_calibration_dqs_map(mck_ck_mid);
	
	phy_info->mck_default = mck_ck_mid;

	dram_test_fail = dram_test_full();
		
	if(dram_test_fail)
	{
		printf("Dram test fail after ck calibration, it should never happen.  Adopt initial ck values. Detected: %d. Default:%d\n", mck_ck_mid, dram_info->phy->mck_default);
			
		dram_calibration_set_ck_dqs_default(dram_info);		
		
		return -1;
	}
	else
	{
		printf("Dram test success after ck calibration. Detected: %d\n", phy_info->mck_default);
	}

	#endif
	return 0;

}
#endif

#ifdef DDR_K_DQS_EN
int dram_calibration_dqs_en(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	printf("dqs enable calibrating...\n");

	unsigned int dqs_en_default_v, dqs_en;
	unsigned int dqs_init = 0;
	unsigned int read_addr = 0xa0000000;
	unsigned int* read_start;
	unsigned int read_data;
	unsigned int dram_test_fail = 0;
	unsigned int dqs_en_max = 0, dqs_en_min = 0, dqs_en_continue = 0;
	
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00090006); 
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00090006); 

	read_start = (unsigned int*)read_addr;
	dqs_en_default_v = 0x9;

	for (dqs_en  = dqs_en_default_v; dqs_en <= 0x1f; dqs_en++)
	{
		REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xffffffe0) |  dqs_en);
		REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xffffffe0) | dqs_en);

		_memctl_calibration_printf("====DQS EN : 0x%x \n", REG_READ_U32(READ_CTRL_0));

		_memctl_calibration_printf("====DQS EN : 0x%x \n", REG_READ_U32(READ_CTRL_0_1));

		 //(*read_start) = 0xfeedbacc;
		//read_data = (*read_start);
		dram_test_fail = dram_test_full();
		udelay(80000);
	
		dqs_init = REG_READ_U32(INT_STATUS_0); 
		_memctl_calibration_printf("dqs_init = %d\n", dqs_init);
		
		if(!dram_test_fail)
		{
			_memctl_calibration_printf("dqs_en %d OK\n", dqs_en);		
		//_memctl_calibration_printf("====read data : 0x%x \n", read_data);
		//dqs_init = REG_READ_U32(INT_STATUS_0);
		// _memctl_calibration_printf("dqs_init = %d\n", dqs_init);
			
			if(dqs_init == 0x4)
			{
				if(dqs_en_max == 0 && dqs_en_min == 0)
				{
					dqs_en_min = dqs_en;
					dqs_en_max = dqs_en;	
					dqs_en_continue = 1;
				}
				else if(dqs_en_continue == 1 && dqs_en > dqs_en_max)
				{
					dqs_en_max = dqs_en;		
					
					if((dqs_en_max - dqs_en_min) == 1)
						break;
				}
				
			}
			else
				dqs_en_continue = 0;
		
		}
		else if(dqs_init == 0x4)
		{
			dqs_en_min = dram_info->phy->dqs_en_default;
			
			break;	
		}

		REG_WRITE_U32(INT_CTRL, (REG_READ_U32(INT_CTRL) & 0xffffffcf) | 0x30);
		REG_WRITE_U32(INT_STATUS_0, (REG_READ_U32(INT_STATUS_0) & 0xfffffffc) | 0x0);
	}
	
	#ifdef DDR_CALIBRATION_RESULT_SET
	
	if(dqs_en_min != -1)
	{
		REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xffffffe0) |  dqs_en_min);
		REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xffffffe0) | dqs_en_min);
		
		phy_info->dqs_en_default = dqs_en_min;
	
		dram_test_fail = dram_test_full();
	}
	else
		dram_test_fail = 1;

	if(dram_test_fail)
	{
		
		printf("Dram test fail after dqs_en calibration, it should never happen.  Adopt initial dqs en values. Detected: %d, Default: %d\n", dqs_en_min, dram_info->phy->dqs_en_default);
		
		REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xffffffe0) |  dram_info->phy->dqs_en_default);
		REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xffffffe0) | dram_info->phy->dqs_en_default);
	
		return -1;
	}
	else
	{
		printf("Dram test success after dqs en calibration\n");
	}
	#endif
	
	REG_WRITE_U32(PAD_DQS, (REG_READ_U32(PAD_DQS) & 0xfff0fff0) | 0x00060009); 
	REG_WRITE_U32(PAD_DQS_2, (REG_READ_U32(PAD_DQS_2) & 0xfff0fff0) | 0x00060009);

	return 0;
}
#endif

#ifdef DDR_RX_MAP
int dram_calibration_three_point(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	 printf("Start scan rx delay map\n");
	
	unsigned int i, j, k,dqs_in_dly_reg, bit_shift, dram_test_fail;
	unsigned int pre_shift_pos[16], post_shift_pos[16], dly_pos_mid;
	unsigned int pre_shift_neg[16], post_shift_neg[16], dly_neg_mid;
	unsigned int first_pre_shift;
	unsigned int min_pre_shift_0 = 0, min_pre_shift_1 = 0, min_post_shift_0 = 0, min_post_shift_1 = 0;
	unsigned int min_pre_temp, min_post_temp ;
	unsigned int default_value;
	uint32_t *rx_delay;
	
	#ifdef DDR_CALIBRATION_RESULT_SET_97F
	unsigned int min_rx_delay = 0, max_rx_delay = 31;
	#endif
	
	#ifdef RD_DELAY_FOLLOW_DQ0
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xfffffbff) | (1 << 10)); //  rd_dly_follow_dq0 =0
	#else
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xfffffbff) | 0); //  rd_dly_follow_dq0 =0
	#endif
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);  // fw_set_mode=10Œccal_set_mode=11

	#if 1 // all delay 63 tap 
	REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xFC0FFFFF) | (0x0 << 20));
	REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xFC0FFFFF) | (0x0 << 20));
	#endif
	
	// scan positive edge
	for(i = 0; i < RD_DLY_DQ_BIT; i++)    // DQ 0 ~ 15
	{
		if(i < 4)
		{
			dqs_in_dly_reg = DQS_IN_DLY_0;
			rx_delay = &(phy_info->rx_delay_tap_0);
		}
		else if( (i >=4) && (i <= 7))
		{
			dqs_in_dly_reg = DQS_IN_DLY_1;
			rx_delay = &(phy_info->rx_delay_tap_1);
			
		}
		else if( (i >=8) && (i <= 11))
		{
			dqs_in_dly_reg = DQS_IN_DLY_0_1;
			rx_delay = &(phy_info->rx_delay_tap_2);
		}
		else if( (i >=12) && (i <= 15))
		{
			dqs_in_dly_reg = DQS_IN_DLY_1_1;
			rx_delay = &(phy_info->rx_delay_tap_3);
		}
		default_value = REG_READ_U32(dqs_in_dly_reg);
		first_pre_shift = 0;
		
		for(j = 0; j < 32; j ++)  // 32 phase
		{
			bit_shift = (i%4)*8;
			REG_WRITE_U32(dqs_in_dly_reg, (REG_READ_U32(dqs_in_dly_reg) & (~(0x1f << bit_shift))) | (j << bit_shift));
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);

			//for(k = 0; k < 20; k++)
			{
				dram_test_fail = dram_test_full();
				if(dram_test_fail)
				{
					_memctl_calibration_printf("DQ%d,phase %d positive failed.\n", i, j);
					//break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("DQ%d,phase %d positive OK. 0x%x = 0x%x\n", i, j, dqs_in_dly_reg, REG_READ_U32(dqs_in_dly_reg));
				if(first_pre_shift == 0)
				{
					pre_shift_pos[i] = j;
					post_shift_pos[i] = j;
					first_pre_shift = 1;
				}
				else if(j > post_shift_pos[i])
				{
					if( (j - post_shift_pos[i]) > 1)
					{
						//_memctl_calibration_printf("avalible phase are not continue : (%d, %d) \n", post_shift_pos[i], j);
						break;
					}
					post_shift_pos[i] = j;
				}
			}
		}
		
		#ifdef DDR_CALIBRATION_RESULT_SET_97F
		
		if(pre_shift_pos[i] > min_rx_delay)
		{
			min_rx_delay = pre_shift_pos[i];
		}
		
		if(post_shift_pos[i] < max_rx_delay)
		{
			max_rx_delay = post_shift_pos[i];	
		}
		#endif
		
		dly_pos_mid = (pre_shift_pos[i] + post_shift_pos[i])/2;

		#ifndef DDR_CALIBRATION_RESULT_SET_97F
		(*rx_delay) |= (dly_pos_mid << bit_shift);
		#endif

		//REG_WRITE_U32(dqs_in_dly_reg, (REG_READ_U32(dqs_in_dly_reg) & (~(0x1f << bit_shift))) | (dly_pos_mid << bit_shift));
		REG_WRITE_U32(dqs_in_dly_reg, default_value); //return to default
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
		_memctl_calibration_printf("positive pre & post shift of DQ%d : (%d, %d), middle : %d \n", i, pre_shift_pos[i], 
			post_shift_pos[i], dly_pos_mid);
	}

	// scan negitive  edge
	for(i = 0; i < RD_DLY_DQ_BIT; i++)    // DQ 0 ~ 15
	{
		if(i < 4)
		{
			dqs_in_dly_reg = DQS_IN_DLY_2;
			rx_delay =  &(phy_info->rx_delay_tap_n_0);
		}
		else if( (i >=4) && (i <= 7))
		{
			dqs_in_dly_reg = DQS_IN_DLY_3;
			rx_delay =  &(phy_info->rx_delay_tap_n_1);

		}
		else if( (i >=8) && (i <= 11))
		{
			dqs_in_dly_reg = DQS_IN_DLY_2_1;
			rx_delay =  &(phy_info->rx_delay_tap_n_2);

		}
		else if( (i >=12) && (i <= 15))
		{
			dqs_in_dly_reg = DQS_IN_DLY_3_1;
			rx_delay =  &(phy_info->rx_delay_tap_n_3);

		}
		default_value = REG_READ_U32(dqs_in_dly_reg);
		first_pre_shift = 0;
		
		for(j = 0; j < 32; j ++)  // 32 phase
		{
			bit_shift = (i%4)*8;
			REG_WRITE_U32(dqs_in_dly_reg, (REG_READ_U32(dqs_in_dly_reg) & (~(0x1f << bit_shift))) | (j << bit_shift));	
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);

			//for(k = 0; k < 20; k++)
			{
				dram_test_fail = dram_test_full();
				if(dram_test_fail)
				{
					_memctl_calibration_printf("DQ%d,phase %d negative failed.\n", i, j);
					//break;
				}
			}

			if(!dram_test_fail)
			{
				_memctl_calibration_printf("DQ%d,phase %d negative OK.0x%x = 0x%x\n", i, j, dqs_in_dly_reg, REG_READ_U32(dqs_in_dly_reg));
				if(first_pre_shift == 0)
				{
					pre_shift_neg[i] = j;
					post_shift_neg[i] = j;
					first_pre_shift = 1;
				}
				else if(j > post_shift_neg[i])
				{
					if( (j - post_shift_neg[i]) > 1)
					{
						//_memctl_calibration_printf("avalible phase are not continue : (%d, %d) \n", post_shift_pos[i], j);
						break;
					}
					post_shift_neg[i] = j;
				}
			}
		}
		
		#ifdef DDR_CALIBRATION_RESULT_SET_97F
		
		if(pre_shift_neg[i] > min_rx_delay)
		{
			min_rx_delay = pre_shift_neg[i];	
		}
		
		if(post_shift_neg[i] < max_rx_delay)
		{
			max_rx_delay = post_shift_neg[i];	
		}
		#endif
		
		dly_neg_mid = (pre_shift_neg[i] + post_shift_neg[i])/2;
		//REG_WRITE_U32(dqs_in_dly_reg, (REG_READ_U32(dqs_in_dly_reg) & (~(0x1f << bit_shift))) | (dly_neg_mid << bit_shift));
		
		#ifndef DDR_CALIBRATION_RESULT_SET_97F
		(*rx_delay) |= dly_neg_mid << bit_shift;
		#endif

		REG_WRITE_U32(dqs_in_dly_reg, default_value); //return to default
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
		_memctl_calibration_printf("negitive pre & post shift of DQ%d : (%d, %d), middle : %d \n", i, pre_shift_neg[i], 
			post_shift_neg[i], dly_neg_mid);
	}

	// set pre_shift_sel & post_shift_sel 
	#ifdef DDR_CALIBRATION_RESULT_SET
		#ifdef DDR_CALIBRATION_RESULT_SET_97F
		
		#ifdef DDR_CALIBRATION_RESULT_SET_97F_DS
		dly_pos_mid = min_rx_delay + 6;
		#else
		dly_pos_mid = (min_rx_delay + max_rx_delay)/2;
		
		for(i = 0; i < RD_DLY_DQ_BIT; i++)
		{
				pre_shift_pos[i] = min_rx_delay;
				post_shift_pos[i] = max_rx_delay;
				pre_shift_neg[i] = min_rx_delay;
				post_shift_neg[i] = max_rx_delay;
		}
		#endif
		
		phy_info->rx_delay_tap_0 = (phy_info->rx_delay_tap_0 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_1 = (phy_info->rx_delay_tap_1 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_2 = (phy_info->rx_delay_tap_2 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_3 = (phy_info->rx_delay_tap_3 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_n_0 = (phy_info->rx_delay_tap_n_0 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_n_1 = (phy_info->rx_delay_tap_n_1 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_n_2 = (phy_info->rx_delay_tap_n_2 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		phy_info->rx_delay_tap_n_3 = (phy_info->rx_delay_tap_n_3 & 0x0) | dly_pos_mid | dly_pos_mid <<8 | dly_pos_mid<<16 | dly_pos_mid<<24;
		
		REG_WRITE_U32(DQS_IN_DLY_0, phy_info->rx_delay_tap_0); 
		REG_WRITE_U32(DQS_IN_DLY_1, phy_info->rx_delay_tap_1); 
		REG_WRITE_U32(DQS_IN_DLY_0_1, phy_info->rx_delay_tap_2); 
		REG_WRITE_U32(DQS_IN_DLY_1_1, phy_info->rx_delay_tap_3); 
		REG_WRITE_U32(DQS_IN_DLY_2, phy_info->rx_delay_tap_n_0); 
		REG_WRITE_U32(DQS_IN_DLY_3, phy_info->rx_delay_tap_n_1); 
		REG_WRITE_U32(DQS_IN_DLY_2_1, phy_info->rx_delay_tap_n_2); 
		REG_WRITE_U32(DQS_IN_DLY_3_1, phy_info->rx_delay_tap_n_3); 
		
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
		
		#ifdef DDR_CALIBRATION_RESULT_SET_97F_DS
			phy_info->pre_shift_0 = 0x11111111;
			phy_info->pre_shift_1 = 0x11111111;
			phy_info->post_shift_0 = 0x11111111;
			phy_info->post_shift_1 = 0x11111111;
			
			REG_WRITE_U32(CAL_LS_SEL, phy_info->pre_shift_0); //dq 0 ~ 7 pre
			REG_WRITE_U32(CAL_LS_SEL_1, phy_info->pre_shift_1); //dq 8 ~ 15 pre
			REG_WRITE_U32(CAL_RS_SEL, phy_info->post_shift_0); //dq 0 ~ 7 post
			REG_WRITE_U32(CAL_RS_SEL_1, phy_info->post_shift_1); //dq 8 ~ 15 post	
			
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
			
			dram_test_fail = dram_test_full();
	
			if(dram_test_fail)
			{
				printf("Dram test fail after 97F DS rx calibration, it never happened. Sampling values use the middle value for the following calibration.\n");
			}
			else
			{
				printf("Dram test success after 97F DS rx calibration. Sampling values use the middle value to count for the following calibration.\n");
			}
		#endif
		
		#else
		REG_WRITE_U32(DQS_IN_DLY_0, phy_info->rx_delay_tap_0); 
		REG_WRITE_U32(DQS_IN_DLY_1, phy_info->rx_delay_tap_1); 
		REG_WRITE_U32(DQS_IN_DLY_0_1, phy_info->rx_delay_tap_2); 
		REG_WRITE_U32(DQS_IN_DLY_1_1, phy_info->rx_delay_tap_3); 
		REG_WRITE_U32(DQS_IN_DLY_2, phy_info->rx_delay_tap_n_0); 
		REG_WRITE_U32(DQS_IN_DLY_3, phy_info->rx_delay_tap_n_1); 
		REG_WRITE_U32(DQS_IN_DLY_2_1, phy_info->rx_delay_tap_n_2); 
		REG_WRITE_U32(DQS_IN_DLY_3_1, phy_info->rx_delay_tap_n_3); 
		
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
		#endif
	
		for( i = 0; i < RD_DLY_DQ_BIT; i++)
		{
			bit_shift = (i%8)*4;
			min_pre_temp = (min((pre_shift_pos[i] + post_shift_pos[i])/2 - max(pre_shift_pos[i], pre_shift_neg[i]), (pre_shift_neg[i] + post_shift_neg[i])/2 - max(pre_shift_pos[i], pre_shift_neg[i]))/3*2)/2 - 1; // keep boundary 1/3
			min_post_temp = (min(min(post_shift_pos[i], post_shift_neg[i]) - (pre_shift_pos[i] + post_shift_pos[i])/2 , min(post_shift_pos[i], post_shift_neg[i])-(pre_shift_neg[i] + post_shift_neg[i])/2)/3*2)/2 - 1;
			
			//printf("%d min_pre_temp=%x, pre_shift_pos=%x, post_shift_pos=%x, pre_shift_neg=%x, post_shift_neg=%x, min_post_temp=%x \n",i, min_pre_temp, pre_shift_pos[i], post_shift_pos[i], pre_shift_neg[i], post_shift_neg[i], min_post_temp);
			
			if( i < 8 )
			{
				min_pre_shift_0 = min_pre_shift_0 | (min_pre_temp << bit_shift);
				min_post_shift_0 =  min_post_shift_0 | (min_post_temp << bit_shift);
			}
			else
			{
				min_pre_shift_1 = min_pre_shift_1 | (min_pre_temp << bit_shift);
				min_post_shift_1 =  min_post_shift_1 | (min_post_temp << bit_shift);
			}
		}
	
		REG_WRITE_U32(CAL_LS_SEL, min_pre_shift_0); //dq 0 ~ 7 pre
		REG_WRITE_U32(CAL_LS_SEL_1, min_pre_shift_1); //dq 8 ~ 15 pre
		REG_WRITE_U32(CAL_RS_SEL, min_post_shift_0); //dq 0 ~ 7 post
		REG_WRITE_U32(CAL_RS_SEL_1, min_post_shift_1); //dq 8 ~ 15 post	
		
		REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffff3) | 0xc);
	
		#ifndef DDR_CALIBRATION_RESULT_SET_97F_DS
		phy_info->pre_shift_0 =  min_pre_shift_0; 
		phy_info->pre_shift_1 =  min_pre_shift_1;
		phy_info->post_shift_0 =  min_post_shift_0;
		phy_info->post_shift_1 =  min_post_shift_1;
		#endif

		dram_test_fail = dram_test_full();
		
		if(dram_test_fail)
		{
			printf("Dram test fail after rx calibration, it should never happen. Adopt initial rx values.\n");
			
			dram_calibration_set_rx_default(dram_info);		
			
			return -1;
				
		}
		else
		{
			printf("Dram test success after rx calibration\n");
		}
		#endif
		
		//enable auto calibration
		REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcf) | 0x00);  // cal_set_mode=00
	
	return 0;
}
#endif

#ifdef DDR_TX_MAP
int dram_calibration_tx_map(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	printf("Start scan tx delay map\n");
	unsigned int i, dram_test_fail, dq_dly_reg, default_value, bit_shift, dm_shift;
	int dq_record = 0, suc_record = 0, setup_delay_min = 0x10, setup_delay_max = 0x10, hold_delay_min = 0x10, hold_delay_max = 0x10;
	int j;
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);
#ifndef DDR_TX_MAP_ONLY_HOLD_TIME
	//first scan
	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffff0000) | 0x0000); //test: 0xffff0000
	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0x0);
	for(i = 0; i < RD_DLY_DQ_BIT; i++)
	{
		if(i < 8)
		{
			dq_dly_reg = DQ_DLY_0;
			dm_shift = 4;
		}
		else if( (i >=8) && (i <= 15))
		{
			dq_dly_reg = DQ_DLY_0_1;
			dm_shift = 12;
		}
		REG_WRITE_U32(DQ_DLY_0, 0x00000000);
		REG_WRITE_U32(DQ_DLY_0_1, 0x00000000);		
		default_value = REG_READ_U32(dq_dly_reg);
		
		for(j = 0; j < 16; j++)
		{
			bit_shift = (i%8)*4;
			REG_WRITE_U32(dq_dly_reg, (REG_READ_U32(dq_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//for(k = 0; k < 16; k++)
			//{
			//	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & (~(0xf0f0))) | (k << dm_shift));
				//REG_WRITE_U32();  something need to be done
				REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	
				//for(k = 0; k < 20; k++)
				{
					dram_test_fail = dram_test_full();
					if(dram_test_fail)
					{
						_memctl_calibration_printf("TX DQ%d,phase %d positive failed.\n", i, j);
						//break;
					}
				}
				if(!dram_test_fail)
				{
					_memctl_calibration_printf("TX DQ%d,phase %d positive OK. 0x%x = 0x%x 0x%x=0x%x\n", i, j, dq_dly_reg, REG_READ_U32(dq_dly_reg), DQ_DLY_1, REG_READ_U32(DQ_DLY_1));
					
					if(setup_delay_min == 0x10)
	        {
	           setup_delay_min = j;
	  			}
	        else if((i > dq_record) && (j > setup_delay_min))
	        {
	           setup_delay_min = j;
	        }
	        
	        dq_record = i;
	        suc_record = j;
	        //printf("setup_delay_min=%d, setup_delay_max=%d\n", setup_delay_min, setup_delay_max);
				}
					
			//}
			
			
			REG_WRITE_U32(dq_dly_reg, default_value); //return to default
		}

			if((setup_delay_max == 0x10) || (suc_record < setup_delay_max))
				setup_delay_max = suc_record;
				
			printf("setup_delay_min=%d, setup_delay_max=%d\n", setup_delay_min, setup_delay_max);	
	}
#endif
	//second scan
	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffff0000) | 0xffff);
	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0xff);
	
	for(i = 0; i < RD_DLY_DQ_BIT; i++)
	{
		if(i < 8)
		{
			dq_dly_reg = DQ_DLY_0;
			dm_shift = 4;
		}
		else if( (i >=8) && (i <= 15))
		{
			dq_dly_reg = DQ_DLY_0_1;
			dm_shift = 12;
		}
	
		REG_WRITE_U32(DQ_DLY_0, 0xffffffff);
		REG_WRITE_U32(DQ_DLY_0_1, 0xffffffff);
		
		default_value = REG_READ_U32(dq_dly_reg);
		
		for(j = 0xf; j > -1; j--)
		{
			bit_shift = (i%8)*4;
			REG_WRITE_U32(dq_dly_reg, (REG_READ_U32(dq_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			
			//for(k = 0xf; k > -1; k--)
			//{
			//	REG_WRITE_U32(DQ_DLY_1, ((REG_READ_U32(DQ_DLY_1) | (0xf0f0)) & (~(0xf << dm_shift))) | (k << dm_shift));
				
				//REG_WRITE_U32();  something need to be done
				REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	
				//for(k = 0; k < 20; k++)
				{
					dram_test_fail = dram_test_full();
					if(dram_test_fail)
					{
						_memctl_calibration_printf("TX DQ%d,phase %d negative failed.\n", i, j);
						//break;
					}
				}
				if(!dram_test_fail)
				{
					_memctl_calibration_printf("TX DQ%d,phase %d negative OK. 0x%x = 0x%x 0x%x=0x%x\n", i, j, dq_dly_reg, REG_READ_U32(dq_dly_reg), DQ_DLY_1, REG_READ_U32(DQ_DLY_1));
					
					if(hold_delay_max == 0x10)
	          hold_delay_max = j;
	  			else if((i > dq_record) && (j < hold_delay_max))
	          hold_delay_max = j;

	          dq_record = i;
	        	suc_record = j;
				      
				  //printf("hold_delay_min=%d, hold_delay_max=%d\n", hold_delay_min, hold_delay_max);  
				}
			//}
			
			REG_WRITE_U32(dq_dly_reg, default_value); //return to default
		}
		
		if((hold_delay_min == 0x10) || (suc_record > hold_delay_min))
				hold_delay_min = suc_record;
	}
	
	#ifdef DDR_CALIBRATION_RESULT_SET
        	unsigned int tx_value;
        	
        	if(hold_delay_min == 0x10)
        		tx_value = 0x4 + (setup_delay_min + setup_delay_max)/2; 
        	else
        		tx_value = 0x4 + (hold_delay_min - (0xf - setup_delay_max))/2;
        	        	
        	phy_info->tx_delay_tap_0 = (0xf & tx_value) | (0xf & tx_value) << 4 | (0xf & tx_value) << 8 | (0xf & tx_value) << 12 | (0xf & tx_value) << 16 | (0xf & tx_value) << 20 | (0xf & tx_value) << 24 | (0xf & tx_value) << 28;
        	phy_info->tx_delay_tap_1 = (0xf & tx_value) | (0xf & tx_value) << 4 | (0xf & tx_value) << 8 | (0xf & tx_value) << 12 | (0xf & tx_value) << 16 | (0xf & tx_value) << 20 | (0xf & tx_value) << 24 | (0xf & tx_value) << 28;
        	//phy_info->dqs_dm_delay_tap = 4 | (0xf & tx_value) << 4 | 4 << 8 | (0xf & tx_value) << 12;
        	
        	REG_WRITE_U32(DQ_DLY_0, phy_info->tx_delay_tap_0);
					REG_WRITE_U32(DQ_DLY_0_1, phy_info->tx_delay_tap_1);
					REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xFFFF0000) | phy_info->dqs_dm_delay_tap);
					REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0x44);
					
					REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
					
					//enable auto calibration
					REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcf) | 0x00);  // cal_set_mode=00
					
					dram_test_fail = dram_test_full();
					
					if(dram_test_fail)
					{
							printf("Dram test fail after tx calibration, it should never happen. Adopt initial tx values.\n");
							
							/*
							REG_WRITE_U32(DQ_DLY_0, dram_info->phy->tx_delay_tap_0);
							REG_WRITE_U32(DQ_DLY_0_1, dram_info->phy->tx_delay_tap_1);
							REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xFFFF0000) | dram_info->phy->dqs_dm_delay_tap);
							REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0x44);
					
							REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);*/
							dram_calibration_set_tx_default(dram_info);
							
							return -1;
					}
					else
					{
							printf("Dram test success after tx calibration\n");
					}
        	
  #endif

	return 0;
}


int dram_calibration_dm_map(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	printf("Start scan dm delay map\n");
	unsigned int i, dram_test_fail, dq_dly_reg, default_value, bit_shift, dm_shift;
	int dq_record = 0, suc_record = 0, setup_delay_min = 0x10, setup_delay_max = 0x10, hold_delay_min = 0x10, hold_delay_max = 0x10;
	int j;
	REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);
#ifndef DDR_TX_MAP_ONLY_HOLD_TIME
	//first scan
	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffff0000) | 0);
	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0);
	for(i = 0; i < RD_DLY_DQ_BIT; i+=8)
	{
		if(i < 8)
		{
			dq_dly_reg = DQ_DLY_0;
			dm_shift = 4;
		}
		else if( (i >=8) && (i <= 15))
		{
			dq_dly_reg = DQ_DLY_0_1;
			dm_shift = 12;
		}
		REG_WRITE_U32(DQ_DLY_0, 0x00000000);
		REG_WRITE_U32(DQ_DLY_0_1, 0x00000000);		
		default_value = REG_READ_U32(dq_dly_reg);
		
		for(j = 0; j < 16; j++)
		{
			bit_shift = (i%8)*4;
			//REG_WRITE_U32(dq_dly_reg, (REG_READ_U32(dq_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//for(k = 0; k < 16; k++)
			//{
				REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & (~(0xf0f0))) | (j << dm_shift));
				//REG_WRITE_U32();  something need to be done
				REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	
				//for(k = 0; k < 20; k++)
				{
					dram_test_fail = dram_test_full();
					if(dram_test_fail)
					{
						_memctl_calibration_printf("DM DQ%d,phase %d positive failed.\n", i, j);
						//break;
					}
				}
				if(!dram_test_fail)
				{
					_memctl_calibration_printf("DM DQ%d,phase %d positive OK. 0x%x = 0x%x 0x%x=0x%x\n", i, j, dq_dly_reg, REG_READ_U32(dq_dly_reg), DQ_DLY_1, REG_READ_U32(DQ_DLY_1));
					
					if(setup_delay_min == 0x10)
	        {
	           setup_delay_min = j;
	  			}
	        else if((i > dq_record) && (j > setup_delay_min))
	        {
	           setup_delay_min = j;
	        }
	        
	        dq_record = i;
	        suc_record = j;
	        //printf("setup_delay_min=%d, setup_delay_max=%d\n", setup_delay_min, setup_delay_max);
				}
					
			//}
			
			REG_WRITE_U32(dq_dly_reg, default_value); //return to default
		}
		
		if((setup_delay_max == 0x10) || (suc_record < setup_delay_max))
				setup_delay_max = suc_record;
	}
#endif
	//second scan
	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffff0000) | 0xffff);
	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0xff);
	
	for(i = 0; i < RD_DLY_DQ_BIT; i+=8)
	{
		if(i < 8)
		{
			dq_dly_reg = DQ_DLY_0;
			dm_shift = 4;
		}
		else if( (i >=8) && (i <= 15))
		{
			dq_dly_reg = DQ_DLY_0_1;
			dm_shift = 12;
		}
	
		REG_WRITE_U32(DQ_DLY_0, 0xffffffff);
		REG_WRITE_U32(DQ_DLY_0_1, 0xffffffff);
		
		default_value = REG_READ_U32(dq_dly_reg);
		
		for(j = 0xf; j > -1; j--)
		{
			bit_shift = (i%8)*4;
			//REG_WRITE_U32(dq_dly_reg, (REG_READ_U32(dq_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			
			//for(k = 0xf; k > -1; k--)
			//{
				REG_WRITE_U32(DQ_DLY_1, ((REG_READ_U32(DQ_DLY_1) | (0xf0f0)) & (~(0xf << dm_shift))) | (j << dm_shift));
				
				//REG_WRITE_U32();  something need to be done
				REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	
				//for(k = 0; k < 20; k++)
				{
					dram_test_fail = dram_test_full();
					if(dram_test_fail)
					{
						_memctl_calibration_printf("DM DQ%d,phase %d negative failed.\n", i, j);
						//break;
					}
				}
				if(!dram_test_fail)
				{
					_memctl_calibration_printf("DM DQ%d,phase %d negative OK. 0x%x = 0x%x 0x%x=0x%x\n", i, j, dq_dly_reg, REG_READ_U32(dq_dly_reg), DQ_DLY_1, REG_READ_U32(DQ_DLY_1));
					
					if(hold_delay_max == 0x10)
	          hold_delay_max = j;
	  			else if((i > dq_record) && (j < hold_delay_max))
	          hold_delay_max = j;

	          dq_record = i;
	        	suc_record = j;
				      
				  //printf("hold_delay_min=%d, hold_delay_max=%d\n", hold_delay_min, hold_delay_max);  
				}
			//}
			
			REG_WRITE_U32(dq_dly_reg, default_value); //return to default
		}
		
		if((hold_delay_min == 0x10) || (suc_record > hold_delay_min))
				hold_delay_min = suc_record;
	}
	
	#ifdef DDR_CALIBRATION_RESULT_SET
        	unsigned int tx_value;
        	
        	if(hold_delay_min == 0x10)
        		tx_value = 0x4 + (setup_delay_min + setup_delay_max)/2; 
        	else
        		tx_value = 0x4 + (hold_delay_min - (0xf - setup_delay_max))/2;
        	        	
        	//phy_info->tx_delay_tap_0 = (0xf & tx_value) | (0xf & tx_value) << 4 | (0xf & tx_value) << 8 | (0xf & tx_value) << 12 | (0xf & tx_value) << 16 | (0xf & tx_value) << 20 | (0xf & tx_value) << 24 | (0xf & tx_value) << 28;
        	//phy_info->tx_delay_tap_1 = (0xf & tx_value) | (0xf & tx_value) << 4 | (0xf & tx_value) << 8 | (0xf & tx_value) << 12 | (0xf & tx_value) << 16 | (0xf & tx_value) << 20 | (0xf & tx_value) << 24 | (0xf & tx_value) << 28;
        	phy_info->dqs_dm_delay_tap = 4 | (0xf & tx_value) << 4 | 4 << 8 | (0xf & tx_value) << 12;
        	
        	REG_WRITE_U32(DQ_DLY_0, phy_info->tx_delay_tap_0);
					REG_WRITE_U32(DQ_DLY_0_1, phy_info->tx_delay_tap_1);
					REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xFFFF0000) | phy_info->dqs_dm_delay_tap);
					REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0x44);
					
					REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
					
					//enable auto calibration
					REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcf) | 0x00);  // cal_set_mode=00
					
					dram_test_fail = dram_test_full();
					
					if(dram_test_fail)
					{
							printf("Dram test fail after dm calibration, it should never happen. Adopt initial dm values.\n");
							
							dram_calibration_set_tx_default(dram_info);
							
							return -1;
					}
					else
					{
							printf("Dram test success after dm calibration\n");
					}
        	
  #endif

	return 0;
}
#endif

#if 0//def DDR_RX_FIFO_MAP
void dram_calibration_rx_fifo_map(void)
{
	unsigned int fifo_dly_default, tm_rd_fifo, i, dram_test_fail;
	unsigned int min_dly = 0x1f, max_dly = 0x7;
	fifo_dly_default = 0x7;
	tm_rd_fifo = fifo_dly_default;
	dram_addr_rot_write(0xa0000000, 0x4000000, 0x4000000);
	_memctl_calibration_printf("Start scan RX fifo map\n");
	for(;tm_rd_fifo <= 0x1f; tm_rd_fifo++)
	{
		REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xffffffe0) | tm_rd_fifo);
		//dram_test_fail = dram_test_full();
		dram_test_fail = dram_bist_read_only_test(0, 0, 0);
		if(!dram_test_fail)
		{
			_memctl_calibration_printf("RX FIFO 0x%x test OK \n", tm_rd_fifo);
			if(tm_rd_fifo < min_dly)
				min_dly = tm_rd_fifo;
			if(tm_rd_fifo > max_dly)
				max_dly = tm_rd_fifo;
		}
		else
			_memctl_calibration_printf("RX FIFO 0x%x test failed \n", tm_rd_fifo);
	}
	_memctl_calibration_printf("The valid interval of fifo delay is 0x%x ~ 0x%x \n", min_dly, max_dly);

	return;
}
#endif

#ifdef DDR_ODT_MAP
int dram_calibration_rx_odt_map(const struct dram_device_info *dram_info, struct dram_phy_ip_info *phy_info)
{
	// left blank
	printf("Start scan ODT map\n");
	unsigned int i, dram_test_fail;
	unsigned int odt_map_right = 0, odt_map_left = 0, odt_map;
	
	int test = REG_READ_U32(REG_HW_STRAP_DDR);
	
	struct ms_rxi310_portmap *ms_ctrl_0_map;
  ms_ctrl_0_map = (struct ms_rxi310_portmap*) BSP_MS_I_DRAMC_0_BASE;

	REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcf8000) | (0x7fff));
	for(i = 0; i < 15 ; i++)
	{
		REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffff8000) | ((0x7fff<<i) & 0x7fff));
		
		_memctl_debug_printf("right READ_CTRL_2 0x%x\n", REG_READ_U32(READ_CTRL_2));
		dram_test_fail = dram_test_full();
		if(!dram_test_fail)
		{
			_memctl_calibration_printf("ODT right check %d OK \n", i);
			
			odt_map_right |= 1<<i;
		}
		else
		{
			_memctl_calibration_printf("ODT right check %d failed \n", i);
			break;
		}
	}
	udelay(10000);

	for(i = 0; i < 15 ; i++)
	{
		REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffff8000) | ((0x7fff>>i) & 0x7fff));
		
		_memctl_debug_printf("left READ_CTRL_2 0x%x\n", REG_READ_U32(READ_CTRL_2));
		dram_test_fail = dram_test_full();
		if(!dram_test_fail)
		{
			_memctl_calibration_printf("ODT left check %d OK \n", i);
			
			odt_map_left |= 1<<(14-i);
		}
		else
		{
			_memctl_calibration_printf("ODT left check %d failed \n", i);
			break;
		}
	} 
	
	#ifdef DDR_CALIBRATION_RESULT_SET
	//int left_bond = 0, right_bond = 0;
	
	odt_map = (odt_map_right & odt_map_left & 0x7fff);

	if(odt_map == 0)
	{
		odt_map = 0x60;
	}

	odt_map |= odt_map >> 4 | odt_map >> 3 | odt_map >> 2| odt_map >> 1 | odt_map << 4  | odt_map << 3  | odt_map << 2  | odt_map << 1;

	//printf("READ_CTRL_2=%x\n", REG_READ_U32(READ_CTRL_2));

	phy_info->odt_default = odt_map;
	
	REG_WRITE_U32(READ_CTRL_2, (REG_READ_U32(READ_CTRL_2) & 0xffcf8000) | phy_info->odt_default);

	dram_test_fail = dram_test_full();
	
	if(dram_test_fail)
	{
			printf("Dram test fail after odt calibration, it should never happen. Adopt initial odt values. Detected: %x. Default:%x\n", odt_map, dram_info->phy->odt_default);

			dram_calibration_set_rx_odt_default(dram_info);
	
			return -1;
	}
	else
	{
			printf("Dram test success after odt calibration. Detected: %x. Default:%x\n", odt_map, dram_info->phy->odt_default);
	}
	#endif
	
	return 0;
}
#endif

#if 0//def DDR_ADDRESS_MAP

#define COL_ADDR
#define BANK_ADDR
#define CMD_ADDR

#ifdef DDR_ADDRESS_MAP_POSITIVE
void dram_calibration_address_map(void)
{
	//TODO
	unsigned int i, j , k , adr_dly_reg,bit_shift,dram_test_fail,default_value;
	unsigned int test_size = 0x40000;
	REG_WRITE_U32(ADR_DLY_0, 0x00000000);
	REG_WRITE_U32(ADR_DLY_1, 0x00000000);	
	REG_WRITE_U32(ADR_DLY_2, 0x00000000);
	REG_WRITE_U32(CMD_DLY_0, 0x00000000);
	REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	//write before tunning
	dram_addr_rot_write(CONFIG_SYS_MEMTEST_START, test_size, test_size);


	/********************************************************************
	Colume Address

	*********************************************************************/
#ifdef COL_ADDR
	for(i = 0; i < COL_ADDR_BIT ; i++)
	{
		if(i < 8)
			adr_dly_reg = ADR_DLY_0;
		else if( (i >=8) && (i <= 15))
			adr_dly_reg = ADR_DLY_1;
		REG_WRITE_U32(ADR_DLY_0, 0x00000000);
		REG_WRITE_U32(ADR_DLY_1, 0x00000000);		
		default_value = REG_READ_U32(adr_dly_reg);
		
		for(j = 0; j < 16; j++)
		{
			bit_shift = (i%8)*4;
			REG_WRITE_U32(adr_dly_reg, (REG_READ_U32(adr_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);
				if(dram_test_fail)
				{
					_memctl_calibration_printf("addr bit %d,phase %d positive failed.\n", i, j);
					//break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("addr bit %d,phase %d positive OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(adr_dly_reg));
			}
			REG_WRITE_U32(adr_dly_reg, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	/********************************************************************
	Bank Address

	*********************************************************************/
#ifdef BANK_ADDR
	for(i = 0; i < BANK_ADDR_BIT ; i++)
	{
		bit_shift = (i%8)*4;
		REG_WRITE_U32(ADR_DLY_2, 0x00000000);
		default_value = REG_READ_U32(ADR_DLY_2);
		for(j = 0; j < 16; j++)
		{
			REG_WRITE_U32(ADR_DLY_2, (REG_READ_U32(ADR_DLY_2) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
//				dram_test_fail = dram_test_full();
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);
				if(dram_test_fail)
				{
					_memctl_calibration_printf("bank addr bit %d,phase %d positive failed.\n", i, j);
					//break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("bank addr bit %d,phase %d positive OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(ADR_DLY_2));
			}
			REG_WRITE_U32(ADR_DLY_2, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	/********************************************************************
	CMD 

	*********************************************************************/
#ifdef CMD_ADDR
	for(i = 0; i < CMD_ADDR_BIT ; i++)
	{
		bit_shift = (i%8)*4;
		REG_WRITE_U32(CMD_DLY_0, 0x00000000);
		default_value = REG_READ_U32(CMD_DLY_0);
		for(j = 0; j < 16; j++)
		{
			REG_WRITE_U32(CMD_DLY_0, (REG_READ_U32(CMD_DLY_0) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
//				dram_test_fail = dram_test_full();
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);			
				if(dram_test_fail)
				{
					_memctl_calibration_printf("cmd bit %d,phase %d positive failed.\n", i, j);
					//break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("cmd bit %d,phase %d positive OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(CMD_DLY_0));
			}
			REG_WRITE_U32(CMD_DLY_0, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	return;
}
#else
void dram_calibration_address_map(void)
{
	//TODO
	_memctl_calibration_printf(" %s, %d\n", __FUNCTION__, __LINE__);
	unsigned int i, k , adr_dly_reg,bit_shift,dram_test_fail,default_value;
	int j;
	unsigned int test_size = 0x40000;
	
	REG_WRITE_U32(ADR_DLY_0, 0xffffffff);
	REG_WRITE_U32(ADR_DLY_1, 0xffffffff);	
	REG_WRITE_U32(ADR_DLY_2, 0xffffffff);
	REG_WRITE_U32(CMD_DLY_0, 0xffffffff);
	REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
	
	//write before tunning
	dram_addr_rot_write(CONFIG_SYS_MEMTEST_START, test_size, test_size);

	/********************************************************************
	Colume Address

	*********************************************************************/
#ifdef COL_ADDR
	for(i = 0; i < COL_ADDR_BIT ; i++)
	{
		if(i < 8)
			adr_dly_reg = ADR_DLY_0;
		else if( (i >=8) && (i <= 15))
			adr_dly_reg = ADR_DLY_1;
		REG_WRITE_U32(ADR_DLY_0, 0xffffffff);
		REG_WRITE_U32(ADR_DLY_1, 0xffffffff);		
		default_value = REG_READ_U32(adr_dly_reg);
		for(j = 0xf; j > -1; j--)
		{
			bit_shift = (i%8)*4;
			REG_WRITE_U32(adr_dly_reg, (REG_READ_U32(adr_dly_reg) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);
				if(dram_test_fail)
				{
					_memctl_calibration_printf("addr bit %d,phase %d negative failed.\n", i, j);
					break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("addr bit %d,phase %d negative OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(adr_dly_reg));
			}
			REG_WRITE_U32(adr_dly_reg, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	/********************************************************************
	Bank Address

	*********************************************************************/
#ifdef BANK_ADDR
	for(i = 0; i < BANK_ADDR_BIT ; i++)
	{
		bit_shift = (i%8)*4;
		REG_WRITE_U32(ADR_DLY_2, 0xffffffff);
		default_value = REG_READ_U32(ADR_DLY_2);
		for(j = 0xf; j > -1; j--)
		{
			REG_WRITE_U32(ADR_DLY_2, (REG_READ_U32(ADR_DLY_2) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
//				dram_test_fail = dram_test_full();
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);
				if(dram_test_fail)
				{
					_memctl_calibration_printf("bank addr bit %d,phase %d negative failed.\n", i, j);
					break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("bank addr bit %d,phase %d negative OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(ADR_DLY_2));
			}
			REG_WRITE_U32(ADR_DLY_2, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	/********************************************************************
	CMD 

	*********************************************************************/
#ifdef CMD_ADDR
	for(i = 0; i < CMD_ADDR_BIT ; i++)
	{
		bit_shift = (i%8)*4;
		REG_WRITE_U32(CMD_DLY_0, 0xffffffff);
		default_value = REG_READ_U32(CMD_DLY_0);
		for(j = 0xf; j > -1; j--)
		{
			REG_WRITE_U32(CMD_DLY_0, (REG_READ_U32(CMD_DLY_0) & (~(0x0f << bit_shift))) | (j << bit_shift));
			//REG_WRITE_U32();  something need to be done
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
			for(k = 0; k < 32; k++)
			{
//				dram_test_fail = dram_test_full();
				dram_test_fail = dram_addr_rot_read(CONFIG_SYS_MEMTEST_START, test_size, test_size);			
				if(dram_test_fail)
				{
					_memctl_calibration_printf("cmd bit %d,phase %d negative failed.\n", i, j);
					break;
				}
			}
			if(!dram_test_fail)
			{
				_memctl_calibration_printf("cmd bit %d,phase %d negative OK. 0x%x = 0x%x\n", i, j, adr_dly_reg, REG_READ_U32(CMD_DLY_0));
			}
			REG_WRITE_U32(CMD_DLY_0, default_value); //return to default
			REG_WRITE_U32(DPI_CTRL_1, (REG_READ_U32(DPI_CTRL_1) & 0xfffffffc) | 0x3);
		}
	}
#endif
	return;
}
#endif
#endif

#ifdef CPU_800
void Setting_CPU_Speed()
{
	#define SYS_STRAP_REG 0xb8000008
	REG32(SYS_STRAP_REG)&= ~(3<<19);   //wei add, bit [20:19]=0   //setting 800MHz
}
#endif
//#define CONFIG_SDRAM_400_temp

uint32_t getStructArrayValue(uint32_t *array, int row_index, int column)
{
	int i = -1;
	int assign_column = -1;
	int column_length = -1;

	do
	{
		i++;

		if(*(array+i)!=0 && *(array+i)==row_index)
			assign_column = i;
		else if(*(array+i) == 0)
			column_length = i+1;

	}while(*(array+i)!=0);

	
	if(assign_column == -1)
	{
//		printf("RFC timing array doesn't support the size of the memory. Default RFC timing will be set.\n");
		return -1;
	}

	//printf("RFC timing: %d\n", *(array+(column+1)*column_length+assign_column));

	return *(array+(column+1)*column_length+assign_column);
}

void dram_auto_size_detect_rxi310(struct ms_rxi310_portmap *dev_map,
		const struct dram_device_info *dram_info, int bd_idx)
{
	unsigned int page_size_local, bank_size, cr_bst_len, real_page_size, real_dram_size;
	unsigned int max_page = 0x7, min_bank = 0x0, max_bank = 0x3;
	volatile unsigned int * dram_addr = (unsigned int *)CONFIG_SYS_SDRAM_BASE_UNCACHE;
	unsigned int trfc;
	uint32_t dfi_rate, dram_period;
	uint32_t set_size;
	
	dfi_rate = 1 << (uint32_t) (dram_info->dfi_rate);
	dram_period = (dram_info-> ddr_period_ps)*(dfi_rate); // according DFI_RATE to setting

	//unsigned int dram_data;
	_memctl_debug_printf("Enter %s, page_size = %d, bank_size = %d \n", 
		__FUNCTION__, dram_info->dev->page, dram_info->dev->bank);
	if(dram_info->dev->device_type == DDR_2)
	{
		max_page = 0x5; // DDR2 max page size is 2KB, we set 8KB for software detect
		bank_size = 0x2; 
		max_bank = 0x3;
		page_size_local = 0x3;
		real_page_size = 0x800;
	}
	else if(dram_info->dev->device_type == DDR_3)
	{
		max_page = 0x5; // 
		bank_size = 0x2; 
		max_bank = 0x3;
		page_size_local = 0x3;
		real_page_size = 0x800;
	}
	else 
	{
		max_page = 0x5; // DDR1 max page size is 2KB, we set 8KB for software detect
		bank_size = 0x1; //only 4 bank in DDR 1
		max_bank = 0x1;
		page_size_local = 0x3;
		real_page_size = 0x800;
	}

	
	if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
	  cr_bst_len = 0; //bst_4
	   _memctl_debug_printf("\nDDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
	}
	else { // BST_LEN_8
	  cr_bst_len = 1; // bst_8
		 _memctl_debug_printf("\nDDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
	}

	//step 1, set largest page_size & smallest bank_size to check correct page_size
	dev_map-> misc = (
					(max_page  << WRAP_MISC_PAGE_SIZE_BFO) |  
					(min_bank << WRAP_MISC_BANK_SIZE_BFO) |
					(cr_bst_len  << WRAP_MISC_BST_SIZE_BFO )  
				   );
	//write a random value to 0x80000000
	*dram_addr = 0xfe5566ef; // no meaning of fe5566ef, just random, 0x100 /4 because dram_addr is unsigned int, +1 will add 4 byte address
	if((*(dram_addr + (0x100/4))) == 0xfe5566ef)  //page size is 256B, because multiple mapping on 0x80000000 & 0x80000100
	{	
		page_size_local = 0x0; 
		real_page_size = 0x100;
		_memctl_debug_printf("\nDetect page_size = 256B (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x200/4))) == 0xfe5566ef)  //page size is 512B
	{	
		page_size_local = 0x1; 
		real_page_size = 0x200;
		_memctl_debug_printf("\nDetect page_size = 512B (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x400/4))) == 0xfe5566ef)  //page size is 1KB
	{	
		page_size_local = 0x2; 
		real_page_size = 0x400;
		_memctl_debug_printf("\nDetect page_size = 1KB (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x800/4))) == 0xfe5566ef)  //page size is 2KB
	{	
		page_size_local = 0x3; 
		real_page_size = 0x800; 
		_memctl_debug_printf("\nDetect page_size = 2KB (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x1000/4))) == 0xfe5566ef)	//page size is 4KB
	{	
		page_size_local = 0x4; 
		real_page_size = 0x1000;
		_memctl_debug_printf("\nDetect page_size = 4KB (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x2000/4))) == 0xfe5566ef)	//page size is 8KB
	{	
		page_size_local = 0x5; 
		real_page_size = 0x2000;
		_memctl_debug_printf("\nDetect page_size = 8KB (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x4000/4))) == 0xfe5566ef)	//page size is 16KB
	{	
		page_size_local = 0x6; 
		real_page_size = 0x4000;
		_memctl_debug_printf("\nDetect page_size = 16KB (%d)\n",page_size_local);
	}
	else if((*(dram_addr + (0x8000/4))) == 0xfe5566ef)	//page size is 32KB
	{	
		page_size_local = 0x7; 
		real_page_size = 0x8000;
		_memctl_debug_printf("\nDetect page_size = 32KB (%d)\n",page_size_local);
	}
	else
	{
		_memctl_debug_printf("\nDetect page_size = %d, not detected\n",page_size_local);
	}

	if((dram_info->dev->device_type != DDR_1))
	{
		//step2, set bank size to max bank size & page size to real page size,	detect bank
		dev_map-> misc = (
						(page_size_local  << WRAP_MISC_PAGE_SIZE_BFO) |  
						(max_bank << WRAP_MISC_BANK_SIZE_BFO) |
						(cr_bst_len  << WRAP_MISC_BST_SIZE_BFO )  
					   );	
		//write a random to mem
		*dram_addr = 0x77feef88; // no meaning of 77feef88, just random
		if((*(dram_addr + 2*real_page_size/4)) == 0x77feef88)
		{
			bank_size = 0x0;
			_memctl_debug_printf("\nDetect bank_size = 2 banks(0x%x)\n",bank_size);
		}
		else if((*(dram_addr + 4*real_page_size/4)) == 0x77feef88)
		{
			bank_size = 0x1;
			_memctl_debug_printf("\nDetect bank_size =	4 banks(0x%x)\n",bank_size);
		}
		else if((*(dram_addr + 8*real_page_size/4)) == 0x77feef88)
		{
			bank_size = 0x2;
			_memctl_debug_printf("\nDetect bank_size = 8 banks(0x%x)\n",bank_size);
		}
		else
		{// if not all of above , set default by DDR type
			_memctl_debug_printf("\nDetect bank_size = %d , not detected\n",bank_size);
		}
	}
	// set final page size & bank size 
	dev_map-> misc = (
					(page_size_local	<< WRAP_MISC_PAGE_SIZE_BFO) |  
					(bank_size << WRAP_MISC_BANK_SIZE_BFO) |
					(cr_bst_len  << WRAP_MISC_BST_SIZE_BFO )  
				   );	

	//detect dram size
	// get size from bonding id first
  set_size = REG32(0xB8000F00);
	
	REG32(0xB8000F00) = 0; // michael
	*dram_addr = 0x95318CA9;
	if((*(dram_addr + (0x800000/4))) == 0x95318CA9)
	{
			real_dram_size = 0x800000;
			_memctl_debug_printf("\nDetect dram size = 8MB (0x%x)\n",real_dram_size);
	}
	else if((*(dram_addr + (0x1000000/4))) == 0x95318CA9)
	{			
			real_dram_size = 0x1000000;
			_memctl_debug_printf("\nDetect dram size = 16MB (0x%x)\n",real_dram_size);
	}
	else if((*(dram_addr + (0x2000000/4))) == 0x95318CA9)
	{
			real_dram_size = 0x2000000;
			_memctl_debug_printf("\nDetect dram size = 32MB (0x%x)\n",real_dram_size);
	}	
	else if((*(dram_addr + (0x4000000/4))) == 0x95318CA9)
	{
			real_dram_size = 0x4000000;
			_memctl_debug_printf("\nDetect dram size = 64MB (0x%x)\n",real_dram_size);
	}
	else if((*(dram_addr + (0x8000000/4))) == 0x95318CA9)
	{
			real_dram_size = 0x8000000;
			_memctl_debug_printf("\nDetect dram size = 128MB (0x%x)\n",real_dram_size);
	}
	// michael, redundant
	/* 
	else if((*(dram_addr + (0x8000000/4))) == 0x95318CA9)
	{
			real_dram_size = 0x8000000;
			_memctl_debug_printf("\nDetect dram size = 128MB (0x%x)\n",real_dram_size);
	}
	*/
	else
	{
			real_dram_size = 0x10000000;
			_memctl_debug_printf("\nDetect dram size >= 256MB \n");
	}
	
	if(set_size != 0 && set_size != (real_dram_size >> 20))
	{
		printf("\nWarning: Auto ddr size detection is not same with setting size. Detected size: %d. Set size: %d\n", (real_dram_size >> 20), set_size);
		REG32(0xB8000F00) = set_size;
	}
	else
	{
		REG32(0xB8000F00) = (real_dram_size >> 20); // michael, in MB
	}
	//printf("size: %d\n", real_dram_size >> 20);

	trfc = getStructArrayValue(dram_info-> timing -> trfc_ps_array, REG32(0xB8000F00), bd_idx);

	//printf("get trfc after auto size detect: %d\n", trfc);

	if (trfc == -1)
	{
		trfc = dram_info-> timing -> trfc_ps;
	}
	
	dev_map-> drr = ( (0	<< PCTL_DRR_REF_DIS_BFO) |
			  (9 	<< PCTL_DRR_REF_NUM_BFO) |
			 ((((dram_info-> timing -> trefi_ps)/dram_period)+1)<< PCTL_DRR_TREF_BFO   ) |
			 (((trfc/dram_period)+1) << PCTL_DRR_TRFC_BFO   )
			);
	
	return;

}

void ddr_array_assign(struct dram_device_info (*dram_array)[], struct dram_device_info ddr2_800, struct dram_device_info ddr2_1066, 
struct dram_device_info other_400, struct dram_device_info other_500)
{
	(*dram_array)[0] = ddr2_800;
	(*dram_array)[1] = ddr2_1066;
	(*dram_array)[2] = other_400;
	(*dram_array)[3] = other_500;
}

__attribute__((section(".init_ram_entry")))
int ss_init_rxi310(void)
{
  //ss_print ("\nHello 3081!\n\n");

 _memctl_debug_printf("\ndram_init.c : ss_init_rxi310()\n");
 
 	//unsigned int sys_mode;
  //unsigned int enabled_cpu;
  //unsigned int active_cpu;

  //struct ps_smu_portmap *ps_smu_map;
  //ps_smu_map = (struct ps_smu_portmap*) PS_SMU_BASE;

  //struct ps_pmu_portmap *ps_pmu_map;
  //unsigned int pmu_id_code;
  //unsigned int cpusel_reg0;

  struct ms_rxi310_portmap *ms_ctrl_0_map;
  //struct ms_rxi310_portmap *ms_ctrl_1_map;
  //struct dpi_pll_portmap *dpi_pll_map;
  //struct dpi_dll_portmap *dpi_dll_map;

  //ps_pmu_map = (struct ps_smu_portmap*) PS_PMU_BASE;

  ms_ctrl_0_map = (struct ms_rxi310_portmap*) BSP_MS_I_DRAMC_0_BASE;
  //ms_ctrl_1_map = (struct ms_rxi310_portmap*) BSP_MS_I_DRAMC_1_BASE;


   _memctl_debug_printf("\ndram_init.c : ms_ctrl_0_map=0x%x\n",(unsigned int)ms_ctrl_0_map );

  //dpi_pll_map  = (struct dpi_pll_portmap*) BSP_MS_I_PHY_0_BASE;
  //dpi_dll_map  = (struct dpi_dll_portmap*) BSP_MS_I_PHY_1_BASE;

  // ----- test PMU module id code reg access -----
  // pmu_id_code = ps_pmu_map->id_code;
  // if (pmu_id_code != PS_PMU_IDCODE_VALUE)
  //   return 0xdead;

//PLL0 700M
//	ps_pmu_map->pll0_pwdn = 0x3;
//	ps_pmu_map->pll0_rstb = 0x0;
//	ps_pmu_map->pll0_rstb = 0x10001;
//	ps_pmu_map->pll0_cfg  = 0x01b42c32;
//	ps_pmu_map->pll0_oeb  = 0x0;
//	ps_pmu_map->pll0_codeready = 0x1;

#if 1 //for 8198E SDR FPGA
	//dram_init(ms_ctrl_0_map, &pll_dram_info);
	//dram_init_rxi310(ms_ctrl_0_map, &pll_dram_info);
#ifdef _8198E_FPGA
	dram_init_rxi310_FPGA(ms_ctrl_0_map, &sdr_dram_info);//for SDR
	dram_auto_size_detect_rxi310(ms_ctrl_0_map, &ddr3_dram_info_FPGA);
#else
	unsigned int strap_pin, pdram_type_idx, pdram_freq_idx, ver_idx, bd_idx;
	struct dram_device_info dram_table_array[4] = 
	{ddr2_800_dram_M14D5121632A_info, ddr2_1066_dram_M14D5121632A_info, 
	ddr1_400_dram_info, ddr1_500_dram_info};
	unsigned char bond = BIT_GET_CHIP_ID_CODE(REG_READ_U32(REG_BOND_OPTION));

	strap_pin = REG_READ_U32(REG_HW_STRAP_DDR);
	pdram_type_idx = BIT_GET_STRAP_PIN_DRAM_TYPE_INV(strap_pin);
	if ((strap_pin & BIT_STRAP_PIN_HW_DBG_DISABLE) == BIT_STRAP_PIN_HW_DBG_DISABLE) {
	   pdram_freq_idx = BIT_GET_STRAP_PIN_DRAM_FEQ(strap_pin);
	} else { // HW_DBG_DISABLE == 0
	   pdram_freq_idx = 0;
	}

	bd_idx = 0;
	ver_idx = 0; // VER_0
	
	#ifdef CONFIG_AUTO_PARAM_DDR_SWITCH
	if((REG32(0xb8000000)&0xf)== VER_1) //VER_1
	{	
		unsigned int buf=0; 
		ver_idx = 1;

		// ver 1
		ddr_array_assign(&dram_table_array, ddr2_800_dram_M14D5121632A_info, 
		ddr2_1066_dram_M14D5121632A_info_B, ddr1_400_dram_info, ddr1_500_dram_info);
		
		{
		   extern unsigned char load_efuse_data_to_reg(void);
			load_efuse_data_to_reg();

		}
		buf= REG32(0xb80007f0);
		printf(" %x ",REG32(0xb80007f0));
		
		if((buf&0xff)>=0x1)
		{
			switch((buf&0xff00)>>8)
			{
			case 0x2:
				if(bond == 0xB) //128MB
        {
				//printf("128 MB\n");
					printf("M ");

          ver_idx = 4;
	        ddr_array_assign(&dram_table_array, ddr2_800_dram_M14D5121632A_info_C, 
					ddr2_1066_dram_M14D5121632A_info_C, ddr1_400_dram_info, ddr1_500_dram_info);
        }
			break;
			case 0x1:
				printf("W ");
				bd_idx = 1;
				ver_idx = 2;  //winbond
	
				// ver 2
				ddr_array_assign(&dram_table_array, ddr2_800_dram_M14D5121632A_info_C, 
				ddr2_1066_dram_M14D5121632A_info_C, ddr1_400_dram_info, ddr1_500_dram_info);
			break;
			case 0x0:
				if(bond == 0xB) //128MB
        {
				//printf("128 MB\n");
				printf("E ");

         	ver_idx = 3;
	        ddr_array_assign(&dram_table_array, ddr2_800_dram_M14D5121632A_info_C, 
					ddr2_1066_dram_M14D5121632A_info_C, ddr1_400_dram_info, ddr1_500_dram_info);
        }
			break;
			}
		}
	}
	#endif


	//pdram_freq_idx = 0;	
	_memctl_debug_printf("strap pin : 0x%x\n", strap_pin);

	// TODO: modify it to DDR1 200,  DDR1 400
	// (dram_type, dram_freq)
	// 0,0 => DDR2 400 MHz
	// 0,1 => DDR2 533 MHz
	// 1,0 => DDR1 200 MHz
	// 1,1 => DDR1 250 MHz

	#ifdef CONFIG_AUTO_DISCRET_DDR_SWITCH
	if (IS_FB_BONDING(bond)) { // CONFIG_DDR2_DISCRET, for TFBGA
		//printf("FB cut\n");
		printf("B ");
		ddr_array_assign(&dram_table_array, ddr2_800_dram_discret_info, 
		ddr2_1066_dram_discret_info, ddr1_400_dram_info, ddr1_500_dram_info);
	} 
	#endif

	struct dram_device_info *dram_table = dram_table_array;

	#ifdef CONFIG_SDRAM_400_temp  
	dram_init_pll_ASIC(ms_ctrl_0_map, &ddr2_400_dram_M14D5121632A_info, &strap_pin, bd_idx);
	dram_init_dpi_ip_ASIC(ms_ctrl_0_map, &ddr2_400_dram_M14D5121632A_info, bd_idx);
	dram_init_rxi310_ASIC(ms_ctrl_0_map, &ddr2_400_dram_M14D5121632A_info, bd_idx);
	#else
	#ifdef DDR_CUSTOM_PHY
	(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy = &phy_calibrating_custom;
	#endif
	
	dram_init_pll_ASIC(ms_ctrl_0_map, dram_table + pdram_type_idx*2 + pdram_freq_idx, &strap_pin, bd_idx);
	dram_init_dpi_ip_ASIC(ms_ctrl_0_map, dram_table + pdram_type_idx*2 + pdram_freq_idx, bd_idx);
	dram_init_rxi310_ASIC(ms_ctrl_0_map, dram_table + pdram_type_idx*2 + pdram_freq_idx, bd_idx);
	dram_auto_size_detect_rxi310(ms_ctrl_0_map, dram_table + pdram_type_idx*2 + pdram_freq_idx, bd_idx);
	#endif

#endif
	
	
	//mainKdram();
#endif

#ifdef DDR_CALIBRATION
		struct dram_phy_ip_info phy_calibrating;
		int calibrating_result = 0;
		int i;
		unsigned int dqsen_rxfifo[] = 
		{0xd, 0x9,
		 0xe, 0xa,
		 0xf, 0xb,
		 0x10, 0xc};
		
		memset(&phy_calibrating, 0, sizeof(struct dram_phy_ip_info));

		dram_calibration_odt_always_on(dram_table + pdram_type_idx*2 + pdram_freq_idx);

	
	#ifdef DDR_K_MCK
		calibrating_result += dram_calibration_dck_map(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.mck_default = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->mck_default;
	#endif
		
	#ifdef DDR_K_DQS
		calibrating_result += dram_calibration_dqs_map(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.mdqs_default = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->mdqs_default;
	#endif
		
	#ifdef DDR_K_DQS_EN
		calibrating_result += dram_calibration_dqs_en(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.dqs_en_default = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dqs_en_default;
	#endif
	
		for(i = 0; i < sizeof(dqsen_rxfifo); i+=2)
		{
			if(dqsen_rxfifo[i] == phy_calibrating.dqs_en_default)	
			{
				break;	
			}
		}
		
		if(i<sizeof(dqsen_rxfifo))
		{
			phy_calibrating.rx_fifo_default = dqsen_rxfifo[i+1];
		}
		else
		{
			phy_calibrating.rx_fifo_default = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_fifo_default;
		}
	
	#ifdef DDR_RX_MAP
		calibrating_result += dram_calibration_three_point(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.rx_delay_tap_0 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_0;
		phy_calibrating.rx_delay_tap_1 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_1;
		phy_calibrating.rx_delay_tap_2 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_2;
		phy_calibrating.rx_delay_tap_3 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_3;
		phy_calibrating.rx_delay_tap_n_0 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_n_0;
		phy_calibrating.rx_delay_tap_n_1 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_n_1;
		phy_calibrating.rx_delay_tap_n_2 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_n_2;
		phy_calibrating.rx_delay_tap_n_3 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->rx_delay_tap_n_3;
		
		phy_calibrating.pre_shift_0 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->pre_shift_0;
		phy_calibrating.pre_shift_1 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->pre_shift_1;
		phy_calibrating.post_shift_0 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->post_shift_0;
		phy_calibrating.post_shift_1 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->post_shift_1;
	#endif
	
	//#ifdef DDR_RX_FIFO_MAP
	//	dram_calibration_rx_fifo_map();
	//#endif

	#ifdef DDR_TX_MAP
		calibrating_result += dram_calibration_tx_map(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
		calibrating_result += dram_calibration_dm_map(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.tx_delay_tap_0 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->tx_delay_tap_0;
		phy_calibrating.tx_delay_tap_1 = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->tx_delay_tap_1;
		phy_calibrating.dqs_dm_delay_tap = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dqs_dm_delay_tap;
	#endif
	
	#ifdef DDR_ODT_MAP
		calibrating_result += dram_calibration_rx_odt_map(dram_table + pdram_type_idx*2 + pdram_freq_idx, &phy_calibrating);
	#else
		phy_calibrating.odt_default = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->odt_default;
	#endif
	
	//#ifdef DDR_PHY_BIST
	//dram_phy_bist_function();
	//#endif

	//#ifdef DDR_ADDRESS_MAP
	//dram_calibration_address_map();
	//#endif
	
		phy_calibrating.dqs_driving = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dqs_driving; //default value
		phy_calibrating.dck_driving = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dck_driving; //default value
		phy_calibrating.cmd_driving = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->cmd_driving; //default value
		phy_calibrating.ba_addr_driving = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->ba_addr_driving; //default value
		phy_calibrating.dq_driving = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dq_driving; //default value			
		
		#ifdef DDR_CALIBRATION_RESULT_SET_PRINT
		if(calibrating_result == 0)
		{
		printf("\nCalibration Recommand values (Fine tune is necessory)---\n");
		
		#ifdef DDR_K_MCK
		printf(".mck_default = %d,\n", phy_calibrating.mck_default);
		#endif
		#ifdef DDR_K_DQS
		printf(".mdqs_default = %d,\n", phy_calibrating.mdqs_default);
		#endif
		#ifdef DDR_K_DQS_EN
		
		printf(".dqs_en_default = 0x%x,\n"
					 ".rx_fifo_default = 0x%x,\n", 
					 phy_calibrating.dqs_en_default,
					 phy_calibrating.rx_fifo_default);
		
		#endif
		#ifdef DDR_ODT_MAP
		printf(".odt_default = 0x%x,\n", phy_calibrating.odt_default);
		#endif
		
		printf(".dqs_driving = 0x%x, //default value\n"
					 ".dck_driving = 0x%x, //default value\n"
					 ".cmd_driving = 0x%x, //default value\n"
					 ".ba_addr_driving = 0x%x, //default value\n"
					 ".dq_driving = 0x%x, //default value\n", 
						(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dqs_driving,
						(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dck_driving,
						(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->cmd_driving,
						(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->ba_addr_driving,
						(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy->dq_driving);

		#ifdef DDR_RX_MAP
		printf(".rx_delay_tap_0 = 0x%08x,\n"
					 ".rx_delay_tap_1 = 0x%08x,\n"
					 ".rx_delay_tap_2 = 0x%08x,\n"
					 ".rx_delay_tap_3 = 0x%08x,\n"
					 ".rx_delay_tap_n_0 = 0x%08x,\n"
					 ".rx_delay_tap_n_1 = 0x%08x,\n"
					 ".rx_delay_tap_n_2 = 0x%08x,\n"
					 ".rx_delay_tap_n_3 = 0x%08x,\n"
					 ".pre_shift_0 = 0x%08x,\n"
					 ".pre_shift_1 = 0x%08x,\n"
					 ".post_shift_0 = 0x%08x,\n"
					 ".post_shift_1 = 0x%08x,\n"
					 , phy_calibrating.rx_delay_tap_0
					 , phy_calibrating.rx_delay_tap_1
					 , phy_calibrating.rx_delay_tap_2
					 , phy_calibrating.rx_delay_tap_3
					 , phy_calibrating.rx_delay_tap_n_0
					 , phy_calibrating.rx_delay_tap_n_1
					 , phy_calibrating.rx_delay_tap_n_2
					 , phy_calibrating.rx_delay_tap_n_3
					 , phy_calibrating.pre_shift_0
					 , phy_calibrating.pre_shift_1
					 , phy_calibrating.post_shift_0
					 , phy_calibrating.post_shift_1);
		
		#endif
		#ifdef DDR_TX_MAP
		printf(".tx_delay_tap_0 = 0x%08x,\n"
					 ".tx_delay_tap_1 = 0x%08x,\n"
					 ".dqs_dm_delay_tap = 0x%04x,\n"
					 , phy_calibrating.tx_delay_tap_0
					 , phy_calibrating.tx_delay_tap_1
					 , phy_calibrating.dqs_dm_delay_tap);
		#endif
		
		printf("--------------------------------------------------------\n");
			
		} 
	#endif
	
		struct dram_phy_ip_info *phy_temp = (dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy;
		
		(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy = &phy_calibrating;
		
		//
		//dram_test_full();
		
		if(dram_normal_patterns(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))	
		{
			printf("Warning: Calibration result test fail. Use default value to enter booting flow.\n");
			
			(dram_table + pdram_type_idx*2 + pdram_freq_idx)->phy = phy_temp;
	
			set_default_values(dram_table + pdram_type_idx*2 + pdram_freq_idx);
		}
		
#endif

	dram_calibration_turn_on_odt(dram_table + pdram_type_idx*2 + pdram_freq_idx);

	//dram_normal_patterns_write_only(0xa0000000, 50000, 50000);

	//dram_normal_patterns(0x01000000, 0x10, 0x0);//test starting from 16MB , fail:  return 0
		//dram_normal_patterns(dram_start, dram_size, area_size);	
	//dram_normal_patterns(CONFIG_SYS_MEMTEST_START, 0x100, 0x10);   
	
#ifdef CONFIG_DRAM_BOOT_TEST
	_memctl_debug_printf("\nPerform sample dram testing\n");

	if(dram_normal_patterns(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}//fail

	if(dram_addr_rot(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}
 
	if(dram_com_addr_rot(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}

	//comment for code size 
	if(dram_byte_access(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}
		
	if(dram_half_word_access(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}
	
		
	if(dram_walking_of_1(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}
		
	 if(dram_walking_of_0(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}

		
	 if(memcpy_test(CONFIG_SYS_MEMTEST_START, 0x100, 0x10))
		{_memctl_debug_printf("\nDDR init FAILED\n");return -1;}
 #endif
	//printf("\nDDR init OK\n");
 //dram_normal_patterns_write_only(0xa0000000, 50000, 50000);
#ifdef DDR_TEST
		//dram_all_mem_bist_test();
		dram_test_repeat();
		//dram_bist_read_write_test(0,0,0);
#endif

//#ifdef ONLY_WRITE
//	dram_normal_patterns_write_only(0x80000000, 50000, 50000);
//#endif
//#ifdef READ_ONLY
//	dram_normal_patterns_read_only(0xa0000000, 50000, 50000);
//#endif
/*
	 unsigned long dst,src;
        unsigned long  src_value;
        unsigned int length;
        unsigned long error;
int i;
	for(i=0;i<128;i+=4) {
                src_value = *(volatile unsigned int *)(0xb0006cc0+i);
                *(volatile unsigned int *)(0x80200000+i) = src_value;
        }
*/

/*
#ifdef DIRECT_JUMP
	{
	    unsigned int addr;
	    unsigned char sys_boot_type;
	    unsigned int strap_pin;
	    EFUSE_DATA efuse_data;
	    void (*jump_func)(void);

	    dprintf("%s(%d): init ram function 20160401_1\n", __func__, __LINE__);

	    strap_pin = REG32(REG_HW_STRAP);
	    sys_boot_type = BIT_GET_STRAP_PIN_BOOT_SEL(strap_pin);
	    addr = (unsigned int)UBOOT_ADDR;  
	    memset(&efuse_data, 0, sizeof(EFUSE_DATA));

	    load_data_from_storage(sys_boot_type, &efuse_data, addr, UBOOT_SIGNATURE);

	    addr += 0x10;

	    dprintf("%s(%d): addr:0x%x \n", __func__, __LINE__, addr);    
	    jump_func = (void *)(addr);
	    jump_func(); 
	}
#endif*/

	return 0;
}

#if 1
void WAIT_DONE_RTK(unsigned int addr, unsigned int mask, unsigned int value)
{
        int timeout = 0;

	while ( (REG_READ_U32(addr) & mask) != value )
	{

                if(timeout++>100000){
                    _memctl_debug_printf("[%s]timeout..%d\n",__FUNCTION__, timeout);
                    return;
                 }
    
		asm("nop");
	}
}

#endif
#define REG_CCO_BAND 4
#define REG_CCO_KVCO 8
#define REG_LOOP_PI_ISEL 16
#define REG_PLL_LDO_VSEL 28
#define REG_V10_LDO_VSEL 0
#define REG_PDIV 24
#define REG_ICP 12
#define REG_LPF_CP 23
#define REG_LPF_SR 20

#define REG_POST_PI_BIAS 4
#define REG_POST_PI_RL 8
#define REG_POST_PI_RS 1
#define REG_PLL_CPMODE 0
void dram_set_frequence_register(const struct dram_device_info *dram_info,const unsigned short* pll_table)
{	
	//2c8
	unsigned int ddr_pll_6, ddr_pll_7, mck_dqs,mck_ck;
	ddr_pll_6 = (pll_table[0] << REG_CCO_BAND) | (pll_table[1] << REG_CCO_KVCO) | (pll_table[2] << REG_LOOP_PI_ISEL)  | 
		(pll_table[6] << REG_PLL_LDO_VSEL) | (pll_table[7] << REG_V10_LDO_VSEL) | (pll_table[8] << REG_PDIV) | 
		(pll_table[10] << REG_ICP) |(pll_table[11] << REG_LPF_CP) | (pll_table[12] << REG_LPF_SR);
	ddr_pll_7 = (pll_table[3] << REG_POST_PI_BIAS) | (pll_table[4] << REG_POST_PI_RL) | (pll_table[5] << REG_POST_PI_RS) | 
		(pll_table[9] << REG_PLL_CPMODE);

	//set output enable	
	if(dram_info->phy->mck_default < 0 )
		mck_ck = dram_info->phy->mck_default + 64;
	else
		mck_ck = dram_info->phy->mck_default;
	if(dram_info->phy->mdqs_default <0 )
		mck_dqs = dram_info->phy->mdqs_default + 64;
	else
		mck_dqs = dram_info->phy->mdqs_default;

	if((mck_ck >= 16) && (mck_ck <= 31))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF0FFFF) | (0x1 << 16); 
	else if((mck_ck >= 32) && (mck_ck <= 47))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF0FFFF) | (0x0 << 16); 
	else if((mck_ck >= 48) && (mck_ck <= 63))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF0FFFF) | (0x1 << 16); 

	if((mck_dqs >= 16) && (mck_dqs <= 31))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF3FFFF) | (0xC << 16); 
	else if((mck_dqs >= 32) && (mck_dqs <= 47))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF3FFFF) | (0x0 << 16); 
	else if((mck_dqs >= 48) && (mck_dqs <= 63))
		ddr_pll_7 = (ddr_pll_7 & 0xFFF3FFFF) | (0xC << 16); 
	
	REG_WRITE_U32(REG_DDR_PLL_6, ddr_pll_6);
	REG_WRITE_U32(REG_DDR_PLL_7, ddr_pll_7);
	return;
}

void dram_init_clk_frequency(const struct dram_device_info *dram_info, unsigned int *pstrap_pin)
{
	unsigned int ddr_freq = (1000000/(dram_info -> ddr_period_ps) *2); // unit: MHz/s
	unsigned int dram_ck_n_code;// = (((1000000/(dram_info -> ddr_period_ps))*2) /DDRPLL_reference_clock) -3;
	unsigned int dram_ck_f_code;// =((ddr_freq*1000/DDRPLL_reference_clock - (dram_ck_n_code+3)*1000)*8192)/1000;
	unsigned int freq;

	if ((*pstrap_pin & BIT_STRAP_PIN_SEL_40M) == BIT_STRAP_PIN_SEL_40M) {
		dram_ck_n_code = ((1000000/(dram_info ->ddr_period_ps))*2 /DDRPLL_reference_clock_40) -3;
		dram_ck_f_code = ((ddr_freq*1000/DDRPLL_reference_clock_40 - (dram_ck_n_code+3)*1000)*8192)/1000;
	}
	else {
		dram_ck_n_code = ((1000000/(dram_info ->ddr_period_ps))*2 /DDRPLL_reference_clock_25) -3;
		dram_ck_f_code = ((ddr_freq*1000/DDRPLL_reference_clock_25 - (dram_ck_n_code+3)*1000)*8192)/1000;
	}
	//Clock frequency control
	//reg_dpi_pdiv , (set  sys_reg 0x1800_02C8[25:24] )
	if(ddr_freq <= 500)
	{
		//rl6387_pll_frequence_table[6][]
		freq = 6;
	}
	else if( ddr_freq > 500 && ddr_freq <= 750 )
	{
		//rl6387_pll_frequence_table[5][]
		freq = 5;
	}
	else if( ddr_freq > 750 && ddr_freq <= 800)
	{
		//rl6387_pll_frequence_table[4][]
		freq = 4;
	}
	else if( ddr_freq > 800 && ddr_freq <= 1100)
	{
		//rl6387_pll_frequence_table[3][]
		freq = 2;
	}
	else if( ddr_freq > 1100 && ddr_freq <= 1150)
	{
		//rl6387_pll_frequence_table[2][]
		freq = 2;
	}
	else if( ddr_freq > 1150 && ddr_freq <= 1400)
	{
		//rl6387_pll_frequence_table[1][]
		freq = 1;
	}
	else if( ddr_freq >= 1400)
	{
		//rl6387_pll_frequence_table[0][]
		freq = 0;
	}


    REG_WRITE_U32(REG_DDR_PLL_11, (dram_ck_f_code << BIT_CRT_F_CODE) | (dram_ck_n_code << BIT_CRT_N_CODE));
	
	dram_set_frequence_register(dram_info,&(rl6387_pll_frequence_table[freq][0]));
	_memctl_debug_printf("\n %s ,ddr_freq=%d (Mbps), %d (MHZ) \n",__FUNCTION__, ddr_freq, ddr_freq/2);
	
	return;
}

void dram_init_pll_ASIC (struct ms_rxi310_portmap *dev_map,
               const struct dram_device_info *dram_info,
                unsigned int *pstrap_pin, int bd_idx)
{
	unsigned int reg_temp;

    // BIT_STRAP_PIN_DDR_LDO_SEL: 1: LDO, 0:SWR
    unsigned char switching_regulator = ((*pstrap_pin & BIT_STRAP_PIN_DDR_LDO_SEL) == BIT_STRAP_PIN_DDR_LDO_SEL) ? 0 : 1;

	/*
	DDRPHY CHIP CONTROL PART (in RL6387 system register)
	*/
	// DRAM TYPE register 0x180002b0
	if(dram_info->dev->device_type == DDR_1)
	{
		REG_WRITE_U32(REG_DDR_PLL_0, 0x0);
		REG_WRITE_U32(REG_DDR_PLL_3, 0x20 << 8); // fixed ddr1 bounding error on CLK, CLKn
	}
	else if(dram_info->dev->device_type == DDR_2)
	{
        reg_temp = REG_READ_U32(REG_BOND_OPTION) & 0xF;
        
		if(reg_temp >= 4) // bonding MCM
			REG_WRITE_U32(REG_DDR_PLL_0, 0x0);
		else
			REG_WRITE_U32(REG_DDR_PLL_0, 0x1);		
	}

	//set pow_ldo14 0x1800028c [28]
	REG_WRITE_U32(REG_SWR_DDR_3, REG_READ_U32(REG_SWR_DDR_3) | (1<<28));
	//wait 1 ms
	udelay(15000); 
    
	//pow_ldo15 read strap pin, then fill 0x1800028c [27] LOD = 1, SW = 1
	REG_WRITE_U32(REG_SWR_DDR_3, REG_READ_U32(REG_SWR_DDR_3) | (1<<27));
	if(!switching_regulator)
	{
		//if ldo wait 100us, then enable enb_LDO_diode_L 0x18000284[12] = 1
		//REG_WRITE_U32(REG_SWR_DDR_3, REG_READ_U32(REG_SWR_DDR_3) & 0xefffffff);
		udelay(1000);
		REG_WRITE_U32(REG_SWR_DDR_1, REG_READ_U32(REG_SWR_DDR_1) | 0x1000);
	}

	//V10_LDO_EN_33V , en_ldo_ddr 0x1800_0024[31] = 1
	REG_WRITE_U32(REG_PLL_1, REG_READ_U32(REG_PLL_1) |(1<<31) );
	//Enable DDRPLL reference clk (reference clk gating), en_ref_clk_ddr 0x1800_0024[23] = 1
	REG_WRITE_U32(REG_PLL_1, REG_READ_U32(REG_PLL_1) | (1<<23) );

	/*
	DDRPHY CRT PART (in RL6387 system register)
	*/
	//Enable RBUS control circuit, dpi_crt_rst_n = 1 ( set  sys_reg 0x1800_02EC[1] = 1)
	REG_WRITE_U32(REG_DDR_PLL_15, REG_READ_U32(REG_DDR_PLL_15) | 0x2);

	//PLL LDO enable, dpi_pll_ldo_rst_n = 1 ( set  sys_reg 0x1800_02EC[0] = 1)
	REG_WRITE_U32(REG_DDR_PLL_15, REG_READ_U32(REG_DDR_PLL_15) | 0x1);

	//PLL init value setting,  1. Enable post PI, reg_dpi_en_post_pi = 0x1F ( set  sys_reg 0x1800_02B8[4:0] = 0x1F)
	REG_WRITE_U32(REG_DDR_PLL_2, REG_READ_U32(REG_DDR_PLL_2) | 0x1F);

	//PLL init value setting,  2. Clock frequency control
	dram_init_clk_frequency(dram_info,pstrap_pin);

	//PLL init value setting,  3. DFI_ratio select, reg_dpi_dfi_rate_sel (set  sys_reg 0x1800_02B4[17] ) 
	//read efuse 	
	if(dram_info->dfi_rate == DFI_RATIO_1)
		REG_WRITE_U32(REG_DDR_PLL_1, REG_READ_U32(REG_DDR_PLL_1) | 0x1<<BIT_CRT_DFI_RATE_SEL);
	else
		REG_WRITE_U32(REG_DDR_PLL_1, REG_READ_U32(REG_DDR_PLL_1) & (~(0x1<<BIT_CRT_DFI_RATE_SEL)));
	
	// set DCK, DQS PI phase
	
	if( 0 <= dram_info->phy->mck_default)
	{
		REG_WRITE_U32(REG_DDR_PLL_3,(REG_READ_U32(REG_DDR_PLL_3) & 0xFFFFC0FF) | (dram_info->phy->mck_default << 8));
	}
	
	else if(dram_info->phy->mck_default < 0)
	{
		REG_WRITE_U32(REG_DDR_PLL_3,(REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | ((dram_info->phy->mck_default+64) << 8));
	}
	
	if( 0 <= dram_info->phy->mdqs_default)
	{
		REG_WRITE_U32(REG_DDR_PLL_3,(REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | (dram_info->phy->mdqs_default << 24));
		REG_WRITE_U32(REG_DDR_PLL_4, (dram_info->phy->mdqs_default));
	}
	
	else if(dram_info->phy->mdqs_default < 0)
	{
		REG_WRITE_U32(REG_DDR_PLL_3,(REG_READ_U32(REG_DDR_PLL_3) & 0xC0FFFFFF) | ((dram_info->phy->mdqs_default+64) << 24));
		REG_WRITE_U32(REG_DDR_PLL_4, (dram_info->phy->mdqs_default+64)); 	
	}
	
	//PLL init value setting,  4. 	Enable PLL and PHY, dpi_rst_n = 1 ( set  sys_reg 0x1800_02EC[2]  = 1)
	REG_WRITE_U32(REG_DDR_PLL_15, REG_READ_U32(REG_DDR_PLL_15) | 0x4);
	//and wait done
	WAIT_DONE_RTK(REG_DDR_PLL_15, 0x10000, 0x10000);
	//wait stable
	udelay(8000);
	
	//Turn on clock flow, reg_dpi_ck_en = 0xf (set  sys_reg 0x1800_02B4[3:0] = 0xf) ---> reg_dpi_clk_oe = 0xf (set  sys_reg 0x1800_02B8[19:16] = 0xf)
	REG_WRITE_U32(REG_DDR_PLL_1, REG_READ_U32(REG_DDR_PLL_1) | 0xf);
	REG_WRITE_U32(REG_DDR_PLL_2, REG_READ_U32(REG_DDR_PLL_2) | (0xf<<16));

	return;
}

void dram_init_dpi_ip_ASIC (struct ms_rxi310_portmap *dev_map,
                const struct dram_device_info *dram_info, int bd_idx)
{
	//set DDR1 or DDR2 mode, PAD_REF 0x1800_02D0[3]
	if(dram_info->dev->device_type == DDR_1)
		REG_WRITE_U32(PAD_REF, 0x0);
	else if(dram_info->dev->device_type == DDR_2)
		REG_WRITE_U32(PAD_REF, 0x8);

	//PAD-DQS single end, DDR1/2 always differential
	if(dram_info->dev->device_type == DDR_1)
	{
		REG_WRITE_U32(PAD_DQS, REG_READ_U32(PAD_DQS) | (1 << SINGLE_END_ENABLE));
		REG_WRITE_U32(PAD_DQS_2, REG_READ_U32(PAD_DQS_2) | (1 << SINGLE_END_ENABLE));
	}
	else if(dram_info->dev->device_type == DDR_2)
	{
		REG_WRITE_U32(PAD_DQS, REG_READ_U32(PAD_DQS) & (~(1 << SINGLE_END_ENABLE)));
		REG_WRITE_U32(PAD_DQS_2, REG_READ_U32(PAD_DQS_2) & (~(1 << SINGLE_END_ENABLE)));
	}

	//DFI_ratio select, set BIST_2TO1_0(0x1800_03A0) Set 2to1_en [26]
	// Set 2to1_rddelay[28] and 2to1_wrdelay [27], to enable odd delay
	if(dram_info->dfi_rate == DFI_RATIO_1)
		REG_WRITE_U32(BIST_2TO1_0, 0x1b000000);
	else
		REG_WRITE_U32(BIST_2TO1_0, 0x14000000);

		dram_set_wrlvl_fb((dram_info->phy->mdqs_default < 0)?(-(dram_info->phy->mdqs_default + 64)):dram_info->phy->mdqs_default);
	
	//set command/address  output delay control,CMD_CTRL 0x1800_0224, // if set bit[2] = 0, the delay is 7
	REG_WRITE_U32(CMD_CTRL, 0x0);
	//REG_WRITE_U32(DPI_CTRL_0, (REG_READ_U32(DPI_CTRL_0) & 0xffffffcc) | 0x32);

	//set address delay chain, (0x200 & 0x204), //TODO, how to set?
	REG_WRITE_U32(ADR_DLY_0, REG_READ_U32(ADR_DLY_0)); //set default
	REG_WRITE_U32(ADR_DLY_1, REG_READ_U32(ADR_DLY_1));
	REG_WRITE_U32(ADR_DLY_2, REG_READ_U32(ADR_DLY_2));
	REG_WRITE_U32(CMD_DLY_0, REG_READ_U32(CMD_DLY_0));
	//REG_WRITE_U32(ADR_DLY_0, 0xffffffff);
	//REG_WRITE_U32(ADR_DLY_1, 0xffffffff);	
	//REG_WRITE_U32(ADR_DLY_2, 0xffffffff);
	//REG_WRITE_U32(CMD_DLY_0, 0xffffffff);	
//	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xfffff0f0) | 0x0f0f);
//	REG_WRITE_U32(DQ_DLY_2, (REG_READ_U32(DQ_DLY_2) & 0xffffff00) | 0xff);


	REG_WRITE_U32(DQ_DLY_0, dram_info->phy->tx_delay_tap_0);
	REG_WRITE_U32(DQ_DLY_0_1, dram_info->phy->tx_delay_tap_1);

	//REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffffff0f) | dram_info->phy->dm_dly_sel_0 <<4); //joyce
	//REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xffff0fff) | dram_info->phy->dm_dly_sel_1 <<12); //joyce

	REG_WRITE_U32(DQ_DLY_1, (REG_READ_U32(DQ_DLY_1) & 0xFFFF0000) |  dram_info->phy->dqs_dm_delay_tap);
	//REG_WRITE_U32(DPI_CTRL_0, REG_READ_U32(DPI_CTRL_0))	
	
	
	//set write_en_0 & fw_set_wr_dly  DPI_CTRL_1, (0x31C)
	REG_WRITE_U32(DPI_CTRL_1, 0x3);

	//set READ_CTRL_0(0x244) : tm_dqs_en, TODO how to set? need efuse?
/*
	#ifdef CONFIG_SDRAM_400_temp
	REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xFFFFFFE0) | 0xe);
	REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xFFFFFFE0) | 0xe);

	//set READ_CTRL_1(0x254) : tm_rd_fifo, rd_dly_follow_dq0
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xFFFFFFE0) | 0x9);
	#else*/
	REG_WRITE_U32(READ_CTRL_0, (REG_READ_U32(READ_CTRL_0) & 0xFFFFFFE0) | dram_info->phy->dqs_en_default);
	REG_WRITE_U32(READ_CTRL_0_1, (REG_READ_U32(READ_CTRL_0_1) & 0xFFFFFFE0) | dram_info->phy->dqs_en_default);
	
	//set READ_CTRL_1(0x254) : tm_rd_fifo, rd_dly_follow_dq0
	REG_WRITE_U32(READ_CTRL_1, (REG_READ_U32(READ_CTRL_1) & 0xFFFFFFE0) | dram_info->phy->rx_fifo_default);
//	#endif
	//set DPI_CTRL_0(0x318) : fw_set_mode, cal_set_mode
	//REG_WRITE_U32(DPI_CTRL_0, REG_READ_U32(DPI_CTRL_0));

	//set DQS_IN_DLY_0(0x28C) : fw_rd_dly_pos_sel_0
	REG_WRITE_U32(DQS_IN_DLY_0, dram_info->phy->rx_delay_tap_0);
	REG_WRITE_U32(DQS_IN_DLY_0_1, dram_info->phy->rx_delay_tap_2);
	REG_WRITE_U32(DQS_IN_DLY_1, dram_info->phy->rx_delay_tap_1);
	REG_WRITE_U32(DQS_IN_DLY_1_1, dram_info->phy->rx_delay_tap_3);

	//set DQS_IN_DLY_2(0x2AC) : fw_rd_dly_neg_sel_0
	REG_WRITE_U32(DQS_IN_DLY_2, dram_info->phy->rx_delay_tap_n_0);
	REG_WRITE_U32(DQS_IN_DLY_2_1, dram_info->phy->rx_delay_tap_n_2);
	REG_WRITE_U32(DQS_IN_DLY_3, dram_info->phy->rx_delay_tap_n_1);
	REG_WRITE_U32(DQS_IN_DLY_3_1, dram_info->phy->rx_delay_tap_n_3);

	//set 3-point calibration pre/post shift
#if 1
	REG_WRITE_U32(CAL_LS_SEL, dram_info->phy->pre_shift_0);
	REG_WRITE_U32(CAL_LS_SEL_1, dram_info->phy->pre_shift_1);
	REG_WRITE_U32(CAL_RS_SEL, dram_info->phy->post_shift_0);
	REG_WRITE_U32(CAL_RS_SEL_1, dram_info->phy->post_shift_0);
#else
	REG_WRITE_U32(CAL_LS_SEL, 0x55555555);
	REG_WRITE_U32(CAL_LS_SEL_1, 0x55555555);
	REG_WRITE_U32(CAL_RS_SEL, 0x55555555);
	REG_WRITE_U32(CAL_RS_SEL_1, 0x55555555);
#endif

	//set DPI_CTRL_1(0x31C) : fw_set_rd_dly
	REG_WRITE_U32(DPI_CTRL_1, REG_READ_U32(DPI_CTRL_1) | 0xc);

	return;
}

#ifndef _8198E_FPGA



void dram_init_rxi310_ASIC (struct ms_rxi310_portmap *dev_map,
                const struct dram_device_info *dram_info, int bd_idx)
{

  _memctl_debug_printf("\nJSW :  dram_init_rxi310 ,dev_map=0x%x \n",(unsigned int)dev_map);

  _memctl_debug_printf("\nJSW :  dram_init_rxi310 ,dram_info=0x%x \n",(unsigned int)dram_info);

 

  uint32_t cr_bst_len = 0 ; // 0:bst_4, 1:bst_8
  uint32_t cas_wr, tphy_wd = 1, mrinfo_wr_lat;    // cas write latency,cas_wr_t,
  uint32_t cas_rd, cas_rd_t, tphy_rd = 0, crl_srt, mrinfo_rd_lat;    // cas read latency, cas_rd_t,crl_srt, 
  uint32_t cas_add, add_lat, mrinfo_add_lat; //add_lat,
  uint32_t dram_emr2 = 0, dram_mr0 = 0;
  uint32_t cr_twr, dram_max_wr, dram_wr;
  uint32_t cr_trtw, cr_trtw_t = 0;
  uint32_t dram_period; //, dram_periodx2;
  enum dram_type         ddr_type = DDR_1;
  enum dram_dq_width     dq_width;
  enum dram_page_size    page;
  uint32_t dfi_rate;



_memctl_debug_printf("\nJSW :  dram_init_rxi310 001\n");
  dfi_rate = 1 << (uint32_t) (dram_info->dfi_rate);
  dram_period = (dram_info-> ddr_period_ps)*(dfi_rate); // according DFI_RATE to setting


_memctl_debug_printf("\ndfi_rate=%d,dram_period=%d\n",dfi_rate,dram_period);

    cas_wr = (dram_info-> mode_reg-> dram_wr_lat ) + 
    	   (dram_info-> mode_reg-> dram_par_lat )+
    	   (dram_info-> mode_reg-> dram_add_lat );
    
    cas_rd = (dram_info-> mode_reg-> dram_rd_lat ) + 
    	   (dram_info-> mode_reg-> dram_par_lat )+
    	   (dram_info-> mode_reg-> dram_add_lat );
    
    cas_add = (dram_info-> mode_reg-> dram_add_lat );


_memctl_debug_printf("\nJSW :  dram_init_rxi310 002\n");
  // In PHY, write latency == 3
  dram_max_wr= (dram_info-> timing -> wr_max_tck)/(dfi_rate) +1;
  dram_wr = ((dram_info-> timing -> twr_ps) / dram_period)+1;

#if 0
  if ((dram_info-> dev-> device_type) == DDR_3) {
    cr_twr = ((dram_info-> timing -> twr_ps) / dram_period) + 1;
	_memctl_debug_printf("\nDDR 3 dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
  }
  else
#endif
  { // SDR, DDR1
    cr_twr = ((dram_info-> timing -> twr_ps) / dram_period);
	_memctl_debug_printf("\nSDR D1  dram_info-> dev-> device_type=%d ,cr_twr=%d \n",dram_info-> dev-> device_type,cr_twr );
  }

  if (cr_twr < dram_max_wr) {
    cr_twr = cr_twr;
  }
  else {
    cr_twr = dram_max_wr;
  }

  if ((dram_info-> dev-> device_type) == DDR_2) {
   _memctl_debug_printf("\nDDR2  dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
	
    ddr_type = DDR_2;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      cr_bst_len = 0; //bst_4
      cr_trtw_t = 2+2; //4/2+2
      dram_mr0 = 0x2;
	   _memctl_debug_printf("\nDDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    else { // BST_LEN_8
      cr_bst_len = 1; // bst_8
      cr_trtw_t = 4+2; // 8/2+2
      dram_mr0 = 0x3;
	     _memctl_debug_printf("\nDDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    if (dram_info-> mode_reg-> dram_rd_lat <= PHY_READ_LATENCY) {
      tphy_rd = 1;
    }
    else {
      tphy_rd = (dram_info-> mode_reg-> dram_rd_lat - PHY_READ_LATENCY) / (dfi_rate) - 1; 
    }
	
    tphy_wd = (dram_info-> mode_reg-> dram_wr_lat + (PHY_ADDR_CMD_LATENCY - PHY_WRITE_DATA_LATENCY))/(dfi_rate) + 1; 
	
    //cas_rd = dram_info-> mode_reg -> mode0_cas;
    //add_lat = dram_info-> mode_reg ->  mode1_all_lat;
    //cas_wr = cas_rd + add_lat -1;
    dram_emr2 = 0;

    dram_mr0  =(((dram_wr%6)-1)                  << (PCTL_MR_OP_BFO+1)) | // write_recovery
               (0                                << PCTL_MR_OP_BFO    ) | // dll
               (dram_info-> mode_reg-> mode0_cas << PCTL_MR_CAS_BFO   ) |
               (dram_info-> mode_reg-> bst_type  << PCTL_MR_BT_BFO    ) |
               dram_mr0;
  } // DDR2
  #if 0
  else if ((dram_info-> dev-> device_type) == DDR_3) {
  	  _memctl_debug_printf("\nDDR3-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
    ddr_type = DDR_3;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      cr_bst_len = 0; //bst_4
      dram_mr0 = 0x2;
    }
    else { // BST_LEN_8
      cr_bst_len = 1; // bst_8
      dram_mr0 = 0x0;
    }
	
    if (dram_info-> mode_reg-> dram_rd_lat <= 6) {
      tphy_rd = 1;
    }
    else {
      tphy_rd = (dram_info-> mode_reg-> dram_rd_lat +
                 dram_info-> mode_reg-> dram_par_lat - 6) / (dfi_rate);
    }

#if 0
    crl_srt = (dram_info-> mode_reg -> mode0_cas >> 1);
    if (((dram_info-> mode_reg -> mode0_cas) & 0x1) ) {
      cas_rd_t = crl_srt+ 12;
    }
    else {
      cas_rd_t = crl_srt+ 4;
    }

    if (dram_info-> mode_reg ->  mode1_all_lat == 1) { // CL-1
      add_lat = cas_rd -1;
    }
    else if (dram_info-> mode_reg ->  mode1_all_lat == 2){ // CL-2
      add_lat = cas_rd -2;
    }
    else {
      add_lat = 0;
    }

    cas_rd = cas_rd_t + add_lat -4;

    cas_wr =  dram_info-> mode_reg -> mode2_cwl + 5 + add_lat -3;
#else
    if (dram_info-> mode_reg-> dram_wr_lat == 10) { 
	tphy_wd = 2+1; // cwl:10
    }
    else {
	tphy_wd = 1; // cwl:9
    }
#endif

    dram_emr2 = dram_info-> mode_reg -> mode2_cwl << 3;
#if 0
    if (dram_wr == 16) {
      dram_wr = 0;
    }
    else if (dram_wr <= 9) {  // 5< wr <= 9
      dram_wr = dram_wr - 4;
    }
    else {
      dram_wr = (dram_wr + 1) / 2;
    }
#else
    if (dram_wr == 16) {
      dram_wr = 0;
    }
    else {
      if (dram_wr < 8 && dram_wr > 4) {
    	dram_wr = dram_wr -4 ;
      }
      else {
    	dram_wr = dram_wr;
      }
    }

#endif
    dram_mr0  =(dram_wr                                  << (PCTL_MR_OP_BFO+1) ) | // write_recovery
               (0                                        << PCTL_MR_OP_BFO     ) | // dll
               ((dram_info-> mode_reg-> mode0_cas >>1 )  << PCTL_MR_CAS_BFO    ) |
               (dram_info-> mode_reg-> bst_type          << PCTL_MR_BT_BFO     ) |
               ((dram_info-> mode_reg-> mode0_cas & 0x1) << 2                  ) |
               dram_mr0;
#if 0
    cr_trtw_t = (cas_rd_t + 6) - cas_wr;
#else
    cr_trtw_t = (dram_info-> mode_reg-> dram_rd_lat + 6) -
		    	     dram_info-> mode_reg-> dram_wr_lat;

#endif

  }  // ddr3
  #endif
  else if ((dram_info-> dev-> device_type) == DDR_1) {
  	  _memctl_debug_printf("\nDDR1-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);

    REG_WRITE_U32(CMD_CTRL, 1<<2);
    ddr_type = DDR_1;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      dram_mr0 = 2; // bst_4
      cr_bst_len = 0; //bst_4
    }
    else { // BST_LEN_8
      dram_mr0 = 3; // bst_8
      cr_bst_len = 1; // bst_8
    }
    //cas_rd = dram_info-> mode_reg -> mode0_cas;
    if (dram_info-> mode_reg-> dram_rd_lat <= 1) {
      tphy_rd = 1;
    }
    else {
      tphy_rd = (dram_info-> mode_reg-> dram_rd_lat - 0) / (dfi_rate); //ddr1_cas
    }

    tphy_wd = 1 + 1;

    
    dram_mr0  =(cas_rd                          << PCTL_MR_CAS_BFO) |
               (dram_info-> mode_reg-> bst_type << PCTL_MR_BT_BFO ) |
               dram_mr0;

    cr_trtw_t = 0; // tic: cas_rd + rd_rtw + rd_pipe

    if (dfi_rate == 1) { // DFI_RATE== 2
      cas_rd = 1;
    }
    else { // DFI_RATE == 1
      cas_rd = cas_rd -1;
    }

    cas_wr = 1;

  } // ddr1
  #if 0
  else if ((dram_info-> dev-> device_type) == SDR) {
  	 _memctl_debug_printf("\nSDR1-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
    ddr_type = SDR;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      dram_mr0 = 2; // bst_4
      cr_bst_len = 0; //bst_4
        _memctl_debug_printf("\nSDR B4  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    else { // BST_LEN_8
      dram_mr0 = 3; // bst_8
      cr_bst_len = 1; // bst_8
        _memctl_debug_printf("\nSDR B8  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    cas_rd = dram_info-> mode_reg -> mode0_cas;

    cas_wr = 0;

   #if 1
    dram_mr0  =(cas_rd                          << PCTL_MR_CAS_BFO) |
               (dram_info-> mode_reg-> bst_type << PCTL_MR_BT_BFO ) |
               dram_mr0;
  #else
  dram_mr0  =0x1033;
    _memctl_debug_printf("\nSDR   dram_mr0=0x%x\n", dram_mr0);
  #endif
  
    cr_trtw_t = 0; // tic: cas_rd + rd_rtw + rd_pipe
  } // SDR
 #endif
  if (dfi_rate == 1) {
    mrinfo_wr_lat = cas_wr;
    mrinfo_rd_lat = cas_rd;
    mrinfo_add_lat = (dram_info-> mode_reg-> dram_add_lat ); 
  }
  else {
    if (cas_wr%dfi_rate) {
      mrinfo_wr_lat = (cas_wr+1)/ dfi_rate;
    }
    else {
       mrinfo_wr_lat = cas_wr / dfi_rate;
    }
  
    if (cas_rd%dfi_rate) {
      mrinfo_rd_lat = (cas_rd+1)/ dfi_rate;
    }
    else {
       mrinfo_rd_lat = cas_rd / dfi_rate;
    }

    if (cas_add%dfi_rate) {
      mrinfo_add_lat = (cas_add+1)/ dfi_rate;
    }
    else {
       mrinfo_add_lat = cas_add / dfi_rate;
    }
  }


  // countting tRTW
  if ((cr_trtw_t & 0x1)) {
    cr_trtw = (cr_trtw_t+1) /(dfi_rate);
  }
  else {
    cr_trtw = cr_trtw_t /(dfi_rate);
  }

  dq_width = (dram_info-> dev-> dq_width); 

  _memctl_debug_printf("\nSDR   dq_width=%d\n", dq_width);
  //page = dram_info-> dev-> page +1; // DQ16 -> memory:byte_unit *2
  page = dram_info-> dev-> page ; // DQ16 -> memory:byte_unit *1 // page size=1K


  // _memctl_debug_printf("\nDCR(0xf8142004)=0x%x\n", REG32(0xf8142004));
  if (dq_width == DQ_32) { // paralle dq_16 => page + 1
    page = page +1;
  }

  if (dq_width == HALF_DQ32) {
    dq_width = 2; // {half_dq, DQ32}
  }


     _memctl_debug_printf("\nSDR   page=%d\n", page);
	  _memctl_debug_printf("\nSDR  dram_info-> dev-> bank =%d\n", dram_info-> dev-> bank );
	    _memctl_debug_printf("\ncr_bst_len =%d\n", cr_bst_len);
  //set CR_CSR(0xc): idle mode
  dev_map->csr = (1 << PCTL_CSR_MEM_IDLE_BFO) | (1 << PCTL_CSR_BIST_IDLE_BFO);
   
  // WRAP_MISC setting
  dev_map-> misc = (
                    (page                   << WRAP_MISC_PAGE_SIZE_BFO) |  
                    (dram_info-> dev-> bank << WRAP_MISC_BANK_SIZE_BFO) |
                    (cr_bst_len             << WRAP_MISC_BST_SIZE_BFO )  
                   );

#if 0 //for 8198E FPGA
	dfi_rate=0;
	
#endif
 _memctl_debug_printf("\nSDR   dfi_rate=%d\n", dfi_rate);
	  _memctl_debug_printf("\nSDR  dq_width=%d\n", dq_width );
	    _memctl_debug_printf("\nddr_type =%d\n", ddr_type);

  // PCTL setting
  dev_map-> dcr = (
                   (dfi_rate << PCTL_DCR_DFI_RATE_BFO) |
                   (dq_width << PCTL_DCR_DQ32_BFO    ) |
                   (ddr_type << PCTL_DCR_DDR3_BFO    )
                  );


     _memctl_debug_printf("\nSDR   cas_rd /(dfi_rate)=%d\n", cas_rd /(dfi_rate));
	  _memctl_debug_printf("\nSDR  cas_wr /(dfi_rate) =%d\n", cas_wr /(dfi_rate) );
	    _memctl_debug_printf("\nSDR  dev_map-> dcr =0x%x\n", dev_map-> dcr );
	  

  dev_map-> iocr = (
                    (/*(cas_rd /(dfi_rate))*/tphy_rd     << PCTL_IOCR_TPHY_RD_EN_BFO ) |
                    (0                        << PCTL_IOCR_TPHY_WL_BFO   ) |
                    (/*(cas_wr /(dfi_rate))*/tphy_wd    << PCTL_IOCR_TPHY_WD_BFO   ) |
                    (0                        << PCTL_IOCR_RD_PIPE_BFO   ) |
                    (1<<4)
                   // (2                        << PCTL_IOCR_RD_PIPE_BFO   ) //For 8198E FPGA RD_PIPE
                   );

      _memctl_debug_printf("\nSDR  dev_map-> iocr=0x%x\n", dev_map-> iocr );

  if (((dram_info-> dev-> device_type) != SDR) || 
      ((dram_info-> dev-> device_type) != DDR_1)) { // DDR2/3 
    dev_map-> emr2 = dram_emr2;

    dev_map-> emr1 = (
                      ((dram_info->mode_reg->mode1_odt & 0x11) << 2 ) | //RTT
                      ((dram_info->mode_reg->mode1_ocd & 0x1)     << 1 ) | //D.I.C
                      (dram_info-> mode_reg -> mode1_dll_en_n )
                     );
  } // DDR2/3
	else if(((dram_info-> dev-> device_type) == DDR_1))
  {
      dev_map-> emr1 = (
                      ((dram_info->mode_reg->mode1_ocd & 0x21)     << 1 ) | //OCD
                      (dram_info-> mode_reg -> mode1_dll_en_n )
                     );
  }

    _memctl_debug_printf("\ndev_map-> emr1=0x%x\n", dev_map->emr1);
  

  dev_map->emr0 = dram_mr0;

    _memctl_debug_printf("\ndev_map-> mr=0x%x\n", dev_map->emr0);

    // _memctl_debug_printf("\nSoC dram_mr0(0xf8142020)=0x%x\n", REG32(0xf8142020));

  dev_map->mrinfo = (
                      ((mrinfo_add_lat & 0x1f)                        << PCTL_MRINFO_ADD_LAT_BFO) |
                      ((mrinfo_rd_lat  & 0x1f)                        << PCTL_MRINFO_RD_LAT_BFO ) |
                      ((mrinfo_wr_lat  & 0x1f)                        << PCTL_MRINFO_WR_LAT_BFO )
                     );


  
  #if 1
//  if(!dram_info-> timing -> trfc_ps_array)
  dev_map-> drr = (
                   (0                                                 << PCTL_DRR_REF_DIS_BFO) |
                   (9                                                 << PCTL_DRR_REF_NUM_BFO) |
                   ((((dram_info-> timing -> trefi_ps)/dram_period)+1)<< PCTL_DRR_TREF_BFO   ) |
                   ((((dram_info-> timing -> trfc_ps)/dram_period)+1) << PCTL_DRR_TRFC_BFO   )
                  );
  /*else
	{
	  uint32_t rfc_value = getStructArrayValue(dram_info-> timing -> trfc_ps_array, REG32(0xB8000F00), bd_idx);

	  if (rfc_value == -1)
	  {
			rfc_value = dram_info-> timing -> trfc_ps;
	  }
	
	  dev_map-> drr = (
							 (0 												<< PCTL_DRR_REF_DIS_BFO) |
							 (9 												<< PCTL_DRR_REF_NUM_BFO) |
							 ((((dram_info-> timing -> trefi_ps)/dram_period)+1)<< PCTL_DRR_TREF_BFO   ) |
							 (((rfc_value/dram_period)+1) << PCTL_DRR_TRFC_BFO   )
							);
	}*/

  
 #else
	dev_map-> drr =0x01007842; // For 8198E FPGA 25MHZ DRAM OSC
 #endif


    _memctl_debug_printf("\nSDR  dev_map-> drr=0x%x\n", dev_map-> drr );

 #if 1
  dev_map-> tpr0= (
                   ((((dram_info-> timing -> trtp_tck)/dfi_rate)+1)   << PCTL_TPR0_TRTP_BFO) |
                   (cr_twr                                            << PCTL_TPR0_TWR_BFO ) |
                   ((((dram_info-> timing -> tras_ps)/dram_period)+1) << PCTL_TPR0_TRAS_BFO) |
                   ((((dram_info-> timing -> trp_ps)/dram_period)+1)  << PCTL_TPR0_TRP_BFO )
                  );
 #else
 dev_map-> tpr0=0xffff;

 #endif
 
 _memctl_debug_printf("\nSDR  dev_map-> tpr0=0x%x\n", dev_map-> tpr0);


  #if 1

  dev_map-> tpr1= (
                   (cr_trtw                                           << PCTL_TPR1_TRTW_BFO) |
                   ((((dram_info-> timing -> twtr_tck)/dfi_rate)+3)   << PCTL_TPR1_TWTR_BFO) |
                   ((((dram_info-> timing -> tccd_tck)/dfi_rate)+1)   << PCTL_TPR1_TCCD_BFO) |
                   ((((dram_info-> timing -> trcd_ps)/dram_period)+1) << PCTL_TPR1_TRCD_BFO) |
                   ((((dram_info-> timing -> trc_ps)/dram_period)+1)  << PCTL_TPR1_TRC_BFO ) |
                   (((dram_info-> timing -> trrd_tck/dfi_rate)+1)       << PCTL_TPR1_TRRD_BFO)
                  );
 #else
 dev_map-> tpr1=0xffffffff ;

 #endif
 
   _memctl_debug_printf("\nSDR  dev_map-> tpr1=0x%x\n", dev_map-> tpr1);


  #if 1
  dev_map-> tpr2= (
                   ((dram_info-> timing -> tmrd_tck/dfi_rate + 1) << PCTL_TPR2_TMRD_BFO        ) |
                   (0                              << PCTL_TPR2_INIT_NS_EN_BFO  ) |
                   (2                              << PCTL_TPR2_INIT_REF_NUM_BFO)
                  );
#else
  dev_map-> tpr2= 0xffffffff ; 
#endif


 _memctl_debug_printf("\nSDR  dev_map-> tpr2=0x%x\n", dev_map-> tpr2);


  // set all_mode _idle
  dev_map-> csr = 0x700;

   dev_map-> ccr = 0x0;
   _memctl_debug_printf("\nDRAM init disable\n");
  // start to init
  dev_map-> ccr = 0x1;
   _memctl_debug_printf("\nDRAM init enable\n");
   
  unsigned int INIT_cnt=0;
  #if 0
  while (((dev_map-> ccr)& 0x1) == 0x0); // init done
   #else
   while(1)
   {
   if (((dev_map-> ccr)& 0x1) == 0x0)
   {
       udelay (10);	/* Delay 10ns  */
      INIT_cnt++;
       _memctl_debug_printf("\nDRAM init is still active\n");
   	if (INIT_cnt>100)
		break;
   }
   else
   {
	_memctl_debug_printf("\nDRAM init is done , jump to DRAM\n");
   	break;
   }
   }
 #ifdef ENABLE_DRAM_ODT
  if ((dram_info-> dev-> device_type) == DDR_2)
  {
   //enable DRAM ODT 
   _memctl_debug_printf("enable DRAM ODT \n");
   dev_map->tdpin = 0x7;
   dev_map->ccr = 0x8;
  }
#endif

   #endif
  // enter mem_mode
  dev_map-> csr= 0x600;
    _memctl_debug_printf("\nSDR  init done , dev_map=0x%x\n",(unsigned int)dev_map );
} // dram_init

#else


void dram_init_rxi310_FPGA (struct ms_rxi310_portmap *dev_map,
                struct dram_device_info *dram_info)
{
	_memctl_debug_printf("\nJSW :  dram_init_rxi310 ,dev_map=0x%x \n",dev_map);

       _memctl_debug_printf("\nJSW :  dram_init_rxi310 ,dram_info=0x%x \n",dram_info);

	#if 0
		

	#else
	

 

  uint32_t cr_bst_len ; // 0:bst_4, 1:bst_8
  uint32_t cas_wr, cas_wr_t;    // cas write latency
  uint32_t cas_rd, cas_rd_t, crl_srt;    // cas read latency
  uint32_t add_lat;
  uint32_t dram_emr2, dram_mr0;
  uint32_t cr_twr, dram_max_wr, dram_wr;
  uint32_t cr_trtw, cr_trtw_t;
  uint32_t dram_period, dram_periodx2;
  enum dram_type         ddr_type;
  enum dram_dq_width     dq_width;
  enum dram_page_size    page;
  uint32_t dfi_rate;




  dfi_rate = 1 << (uint32_t) (dram_info->dfi_rate);
  dram_period = (dram_info-> ddr_period_ps)*(dfi_rate); // according DFI_RATE to setting


//_memctl_debug_printf("\ndfi_rate=%d,dram_period=%d\n",dfi_rate,dram_period);


//_memctl_debug_printf("\nJSW :  dram_init_rxi310 002\n");
  // In PHY, write latency == 3
  dram_max_wr= (dram_info-> timing -> wr_max_tck)/(dfi_rate) +1;
  dram_wr = ((dram_info-> timing -> twr_ps) / dram_period)+1;


#if 0 //DDR3
  if ((dram_info-> dev-> device_type) == DDR_3) 
  {
    cr_twr = ((dram_info-> timing -> twr_ps) / dram_period) + 3;
	//_memctl_debug_printf("\nDDR 3 dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
  }
  else { // SDR, DDR1
    cr_twr = ((dram_info-> timing -> twr_ps) / dram_period);
	//_memctl_debug_printf("\nSDR D1  dram_info-> dev-> device_type=%d ,cr_twr=%d \n",dram_info-> dev-> device_type,cr_twr );
  }

  if (cr_twr < dram_max_wr) {
    cr_twr = cr_twr;
  }
  else {
    cr_twr = dram_max_wr;
  }
#endif
  


  if ((dram_info-> dev-> device_type) == DDR_2) 
  {
  // _memctl_debug_printf("\nDDR2  dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
	
    ddr_type = DDR_2;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      cr_bst_len = 0; //bst_4
      cr_trtw_t = 2+2; //4/2+2
      dram_mr0 = 0x2;
	   _memctl_debug_printf("\nDDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    else { // BST_LEN_8
      cr_bst_len = 1; // bst_8
      cr_trtw_t = 4+2; // 8/2+2
      dram_mr0 = 0x3;
	     _memctl_debug_printf("\nNot DDR2  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }

	
    cas_rd = dram_info-> mode_reg -> mode0_cas;
    add_lat = dram_info-> mode_reg ->  mode1_all_lat;
    cas_wr = cas_rd + add_lat -1;
    dram_emr2 = 0;

    dram_mr0  =(((dram_wr%6)-1)                  << (PCTL_MR_OP_BFO+1)) | // write_recovery
               (0                                << PCTL_MR_OP_BFO    ) | // dll
               (dram_info-> mode_reg-> mode0_cas << PCTL_MR_CAS_BFO   ) |
               (dram_info-> mode_reg-> bst_type  << PCTL_MR_BT_BFO    ) |
               dram_mr0;
  } // DDR2
  else if ((dram_info-> dev-> device_type) == DDR_3) {
  	//  _memctl_debug_printf("\nDDR3-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
    ddr_type = DDR_3;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      cr_bst_len = 0; //bst_4
      dram_mr0 = 0x2;
    }
    else { // BST_LEN_8
      cr_bst_len = 1; // bst_8
      dram_mr0 = 0x0;
    }

    crl_srt = (dram_info-> mode_reg -> mode0_cas >> 1);
    if (((dram_info-> mode_reg -> mode0_cas) & 0x1) ) {
      cas_rd_t = crl_srt+ 12;
    }
    else {
      cas_rd_t = crl_srt+ 4;
    }

    if (dram_info-> mode_reg ->  mode1_all_lat == 1) { // CL-1
      add_lat = cas_rd -1;
    }
    else if (dram_info-> mode_reg ->  mode1_all_lat == 2){ // CL-2
      add_lat = cas_rd -2;
    }
    else {
      add_lat = 0;
    }

    cas_rd = cas_rd_t + add_lat -4;

    cas_wr =  dram_info-> mode_reg -> mode2_cwl + 5 + add_lat -3;

    dram_emr2 = dram_info-> mode_reg -> mode2_cwl << 3;

    if (dram_wr == 16) {
      dram_wr = 0;
    }
    else if (dram_wr <= 9) {  // 5< wr <= 9
      dram_wr = dram_wr - 4;
    }
    else {
      dram_wr = (dram_wr + 1) / 2;
    }

    dram_mr0  =(dram_wr                                  << (PCTL_MR_OP_BFO+1) ) | // write_recovery
               (0                                        << PCTL_MR_OP_BFO     ) | // dll
               ((dram_info-> mode_reg-> mode0_cas >>1 )  << PCTL_MR_CAS_BFO    ) |
               (dram_info-> mode_reg-> bst_type          << PCTL_MR_BT_BFO     ) |
               ((dram_info-> mode_reg-> mode0_cas & 0x1) << 2                  ) |
               dram_mr0;

    cr_trtw_t = (cas_rd_t + 6) - cas_wr;

  }  // ddr3
  else if ((dram_info-> dev-> device_type) == DDR_1) {
  	//  _memctl_debug_printf("\nDDR1-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
    ddr_type = DDR_1;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      dram_mr0 = 2; // bst_4
      cr_bst_len = 0; //bst_4
    }
    else { // BST_LEN_8
      dram_mr0 = 3; // bst_8
      cr_bst_len = 1; // bst_8
    }
    cas_rd = dram_info-> mode_reg -> mode0_cas;

    
    dram_mr0  =(cas_rd                          << PCTL_MR_CAS_BFO) |
               (dram_info-> mode_reg-> bst_type << PCTL_MR_BT_BFO ) |
               dram_mr0;

    cr_trtw_t = 0; // tic: cas_rd + rd_rtw + rd_pipe

    if (dfi_rate == 1) { // DFI_RATE== 2
      cas_rd = 1;
    }
    else { // DFI_RATE == 1
      cas_rd = cas_rd -1;
    }

    cas_wr = 1;

  } // ddr1
  else if ((dram_info-> dev-> device_type) == SDR) {
  	// _memctl_debug_printf("\nSDR1-1   dram_info-> dev-> device_type=%d\n",dram_info-> dev-> device_type);
    ddr_type = SDR;
    if (dram_info-> mode_reg-> bst_len == BST_LEN_4) {
      dram_mr0 = 2; // bst_4
      cr_bst_len = 0; //bst_4
       // _memctl_debug_printf("\nSDR B4  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    else { // BST_LEN_8
      dram_mr0 = 3; // bst_8
      cr_bst_len = 1; // bst_8
     //   _memctl_debug_printf("\nSDR B8  dram_info-> mode_reg-> bst_len=%d\n",dram_info-> mode_reg-> bst_len);
    }
    cas_rd = dram_info-> mode_reg -> mode0_cas;

    cas_wr = 0;

   #if 0
  
  #else
  dram_mr0  =0x1033; //MR0 => CL=3 , BL=8
 // dram_mr0  =0x1032; //MR0 => CL=3 , BL=4 for SDR
   _memctl_debug_printf("\nSDR   dram_mr0=0x%x\n", dram_mr0);

  //return ; //JSW bebug
  
  #endif
  
    cr_trtw_t = 0; // tic: cas_rd + rd_rtw + rd_pipe
  } // SDR

 

  // countting tRTW
  if ((cr_trtw_t & 0x1)) {
    cr_trtw = (cr_trtw_t+1) /(dfi_rate);
  }
  else {
    cr_trtw = cr_trtw_t /(dfi_rate);
  }

  dq_width = (dram_info-> dev-> dq_width); 

  _memctl_debug_printf("\nSDR   dq_width=%d\n", dq_width);
  //page = dram_info-> dev-> page +1; // DQ16 -> memory:byte_unit *2
  page = dram_info-> dev-> page ; // DQ16 -> memory:byte_unit *1 // page size=1K


  
  if (dq_width == DQ_32) { // paralle dq_16 => page + 1
    page = page +1;
  }

  if (dq_width == HALF_DQ32) {
    dq_width = 2; // {half_dq, DQ32}
  }


  _memctl_debug_printf("\nSDR   page=%d\n", page);
  _memctl_debug_printf("\nSDR  dram_info-> dev-> bank =%d\n", dram_info-> dev-> bank );
  _memctl_debug_printf("\ncr_bst_len =%d\n", cr_bst_len);
   
  // WRAP_MISC setting
  dev_map-> misc = (
                    (page                   << WRAP_MISC_PAGE_SIZE_BFO) |  
                    (dram_info-> dev-> bank << WRAP_MISC_BANK_SIZE_BFO) |
                    (cr_bst_len             << WRAP_MISC_BST_SIZE_BFO )  
               
                   );


 _memctl_debug_printf("\n001:dev_map-> misc =0x%x\n", dev_map-> misc); //SDR suppose "0x52"


#if 1 //for 8198E FPGA
	dfi_rate=0;
	
#endif
//_memctl_debug_printf("\nSDR   dfi_rate=%d\n", dfi_rate);
	// _memctl_debug_printf("\nSDR  dq_width=%d\n", dq_width );
	   // _memctl_debug_printf("\nddr_type =%d\n", ddr_type);

  // PCTL setting
  dev_map-> dcr = (
                   (dfi_rate << PCTL_DCR_DFI_RATE_BFO) |
                   (dq_width << PCTL_DCR_DQ32_BFO    ) |
                   (ddr_type << PCTL_DCR_DDR3_BFO    )
                  );


   //  _memctl_debug_printf("\nSDR   cas_rd /(dfi_rate)=%d\n", cas_rd /(dfi_rate));
//	  _memctl_debug_printf("\nSDR  cas_wr /(dfi_rate) =%d\n", cas_wr /(dfi_rate) );
	    _memctl_debug_printf("\n002:SDR  dev_map-> dcr =0x%x\n", dev_map-> dcr );
	  
 #if 0
  dev_map-> iocr = (
                    ((cas_rd /(dfi_rate))     << PCTL_IOCR_TPHY_RD_EN_BFO ) |
                    (0                        << PCTL_IOCR_TPHY_WL_BFO   ) |
                    ((cas_wr /(dfi_rate))     << PCTL_IOCR_TPHY_WD_BFO   ) |
                   // (0                        << PCTL_IOCR_RD_PIPE_BFO   )
                 //   (2                        << PCTL_IOCR_RD_PIPE_BFO   ) //For 8198E FPGA RD_PIPE 33MHZ , CAS Latency=3
                    //for 98E FPGA SDR 125MHZ
                     (3                        << PCTL_IOCR_RD_PIPE_BFO   ) //For 8198E FPGA RD_PIPE 125MHZ , CAS Latency=4 //
                   );
#else
	dev_map-> iocr =0x300;

#endif
      _memctl_debug_printf("\n003:SDR  dev_map-> iocr=0x%x\n", dev_map-> iocr );


#if 0
  if (((dram_info-> dev-> device_type) != SDR) || 
      ((dram_info-> dev-> device_type) != DDR_1)) { // DDR2/3 
    dev_map-> emr2 = dram_emr2;

    dev_map-> emr1 = (
                      (1               << 2 ) | //RTT
                      (1               << 1 ) | //D.I.C
                      (dram_info-> mode_reg -> mode1_dll_en_n )
                     );
  } // DDR2/3
#else

    dev_map-> emr1 = 0x6;
 

#endif

  
  dev_map-> emr0 = dram_mr0;


    _memctl_debug_printf("\ndev_map-> mr2=0x%x\n", dev_map-> emr2);
    _memctl_debug_printf("\ndev_map-> mr1=0x%x\n", dev_map-> emr1);
   _memctl_debug_printf("\ndev_map-> mr0=0x%x\n", dev_map-> emr0);

	

 dev_map-> mrinfo = 0x60;


   _memctl_debug_printf("\ndev_map-> mf_info=0x%x\n", dev_map-> mrinfo);
   
  
  #if 0
  dev_map-> drr = (
                   (0                                                 << PCTL_DRR_REF_DIS_BFO) |
                   (9                                                 << PCTL_DRR_REF_NUM_BFO) |
                   ((((dram_info-> timing -> trefi_ps)/dram_period)+1)<< PCTL_DRR_TREF_BFO   ) |
                   ((((dram_info-> timing -> trfc_ps)/dram_period)+1) << PCTL_DRR_TRFC_BFO   )
                  );
 #else
	//dev_map-> drr =0x01007842; // For 8198E FPGA 25MHZ DRAM OSC
	dev_map-> drr =0x01003842; // For 8198E FPGA 16MHZ DRAM OSC
	//dev_map-> drr =0x01001842; // For 8198E FPGA 16MHZ DRAM OSC
	//dev_map-> drr =0x01008042 ; // For 8198E FPGA 125MHZ DRAM OSC
	//dev_map-> drr =0x0801C072; // For 8198E FPGA 16MHZ DRAM OSC
 #endif


    _memctl_debug_printf("\nSDR  dev_map-> drr=0x%x\n", dev_map-> drr );

 #if 0
  dev_map-> tpr0= (
                   ((((dram_info-> timing -> trtp_tck)/dfi_rate)+1)   << PCTL_TPR0_TRTP_BFO) |
                   (cr_twr                                            << PCTL_TPR0_TWR_BFO ) |
                   ((((dram_info-> timing -> tras_ps)/dram_period)+1) << PCTL_TPR0_TRAS_BFO) |
                   ((((dram_info-> timing -> trp_ps)/dram_period)+1)  << PCTL_TPR0_TRP_BFO )
                  );
 #else
 //dev_map-> tpr0=0xffff;
 dev_map-> tpr0=0x22; //

 #endif
 
_memctl_debug_printf("\nSDR  dev_map-> tpr0=0x%x\n", dev_map-> tpr0);


  #if 0

  dev_map-> tpr1= (
                   (cr_trtw                                           << PCTL_TPR1_TRTW_BFO) |
                   ((((dram_info-> timing -> twtr_tck)/dfi_rate)+3)   << PCTL_TPR1_TWTR_BFO) |
                   ((((dram_info-> timing -> tccd_tck)/dfi_rate)+1)   << PCTL_TPR1_TCCD_BFO) |
                   ((((dram_info-> timing -> trcd_ps)/dram_period)+1) << PCTL_TPR1_TRCD_BFO) |
                   ((((dram_info-> timing -> trc_ps)/dram_period)+1)  << PCTL_TPR1_TRC_BFO ) |
                   ((dram_info-> timing -> trrd_tck/dfi_rate)+1       << PCTL_TPR1_TRRD_BFO)
                  );
 #else
 //dev_map-> tpr1=0x1FFFFFFF ;

dev_map-> tpr1=0x504812; 
 #endif
 
   _memctl_debug_printf("\nSDR  dev_map-> tpr1=0x%x\n", dev_map-> tpr1);


  #if 0
  dev_map-> tpr2= (
                   (dram_info-> timing -> tmrd_tck << PCTL_TPR2_TMRD_BFO        ) |
                   (0                              << PCTL_TPR2_INIT_NS_EN_BFO  ) |
                   (2                              << PCTL_TPR2_INIT_REF_NUM_BFO)
                  );
#else
  //dev_map-> tpr2= 0x1ff ; 
  dev_map-> tpr2= 0x24; 
#endif


 _memctl_debug_printf("\nSDR  dev_map-> tpr2=0x%x\n", dev_map-> tpr2);



 #if 0
 
#else
 // dev_map-> tpr3= 0xffffffff ; 
#endif


 _memctl_debug_printf("\nSDR  dev_map-> tpr3=0x%x\n", dev_map-> tpr3);


 // dev_map-> phy_dly0= 0x40 ; 
// dev_map-> phy_dly0= 0x70 ; 

 //dev_map-> phy_dly0= 0x70 ; //for 98E FPGA SDR 125MHZ
 dev_map-> phy_dly0= 0xfe ; //for 98E FPGA SDR 125MHZ


 _memctl_debug_printf("\nSDR  dev_map-> phy_dly0=0x%x\n", dev_map-> phy_dly0);


 //  dev_map-> ccr = 0x0;
//   _memctl_debug_printf("\nDRAM init disable\n");

   //udelay(100);
  // start to init
//  dev_map-> ccr = 0x1;
//   _memctl_debug_printf("\nDRAM init enable\n");

_memctl_debug_printf("\nCheck:  dev_map-> ccr=0x%x\n", dev_map-> ccr);
  unsigned int INIT_cnt=0;

  #if 0
  while (((dev_map-> ccr)& 0x1) == 0x0); // init done
  #endif

  #if 1
   while(1)
   {
      if (((dev_map-> ccr)& 0x1) == 0x1)
	  	break; //DRAM init done
	
  // set all_mode _idle
    
          _memctl_debug_printf("\nDisable DRAM access \n");
    dev_map-> csr = 0x700; //disable DRAM access


     _memctl_debug_printf("\nDRAM init is still active , write CCR bit[0]=1\n");
    dev_map-> ccr = 0x1;
   
      _memctl_debug_printf("\n  dev_map-> ccr=0x%x\n", dev_map-> ccr);
   if (((dev_map-> ccr)& 0x1) == 0x0)
   {
   
   	
        _memctl_debug_printf("\nRe-enable DRAM init flow\n");
        dev_map-> ccr = 0x1;
      // udelay (10);	/* Delay 10ns  */
      #if 1
      INIT_cnt++;
   
   	if (INIT_cnt>10)
   	{
   		   dev_map-> csr = 0x600;  //enable DRAM access
		break;
   	}
	#endif
   }
   else
   {
       dev_map-> csr = 0x600;  //enable DRAM access
	_memctl_debug_printf("\nDRAM init is done , jump to DRAM\n");
   	break;
   }
   }

   #endif
  // enter mem_mode
   dev_map-> csr= 0x600;
    _memctl_debug_printf("\nSDR  init done , dev_map=0x%x\n",dev_map );
	
  #endif
     

}
#endif

