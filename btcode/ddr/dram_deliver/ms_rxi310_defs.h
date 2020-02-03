#ifndef MS_RXI310_DEFS_H

#if 1  //michael
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

#define MS_RXI310_DEFS_H

#ifndef BIT
#define BIT(x)			(1 << (x))
#endif

//#define RD_DELAY_FOLLOW_DQ0
#ifdef RD_DELAY_FOLLOW_DQ0
#define RD_DLY_DQ_BIT 1
#else
#define RD_DLY_DQ_BIT 16
#endif

//#define DDR_PHY_CALIBRATION

#define     BIT_SHIFT_STRAP_PIN_DRAM_TYPE_INV   4
#define     BIT_MASK_STRAP_PIN_DRAM_TYPE_INV    0x1
#define     BIT_GET_STRAP_PIN_DRAM_TYPE_INV(x)  (((x) >> BIT_SHIFT_STRAP_PIN_DRAM_TYPE_INV) & BIT_MASK_STRAP_PIN_DRAM_TYPE_INV)

#define     BIT_SHIFT_STRAP_PIN_DRAM_FEQ        5
#define     BIT_MASK_STRAP_PIN_DRAM_FEQ         0x1
#define     BIT_GET_STRAP_PIN_DRAM_FEQ(x)       (((x) >> BIT_SHIFT_STRAP_PIN_DRAM_FEQ) & BIT_MASK_STRAP_PIN_DRAM_FEQ)

#define     BIT_STRAP_PIN_HW_DBG_DISABLE        BIT(6)
#define     BIT_STRAP_PIN_DDR_LDO_SEL           BIT(7)
#define     BIT_STRAP_PIN_SEL_40M               BIT(24)

#define DRAM_512M_TRFC 327500
#define DRAM_256M_TRFC 197500
#define DRAM_128M_TRFC 127500
#define DRAM_64M_TRFC 105000
#define DRAM_32M_TRFC 75000


#define DDRPLL_reference_clock_25 25
#define DDRPLL_reference_clock_40 40
//#define CONFIG_DDR2_DISCRET  useless now

//#define BSP_MS_I_DRAMC_0_BASE         0xf8142000//for 98E ARM
#define BSP_MS_I_DRAMC_0_BASE         0xb8142000//for 96F MIPS24K
#define DPI_DLL_BASE                 0xb8146200 // for 96F PHY IP 
#define SYSTEM_REGISTER_BASE         0xb8000000 // for 97F ASIC

//system register
#define REG_HW_STRAP_DDR           (SYSTEM_REGISTER_BASE + 0x0008)
#define REG_PLL_1                             (SYSTEM_REGISTER_BASE + 0x024)
#define REG_DDR_PLL_0                    (SYSTEM_REGISTER_BASE + 0x2b0)
#define REG_SWR_DDR_1                  (SYSTEM_REGISTER_BASE + 0x284)
#define REG_SWR_DDR_3                  (SYSTEM_REGISTER_BASE + 0x28c)
#define REG_DDR_PLL_1                    (SYSTEM_REGISTER_BASE + 0x2b4)
#define REG_DDR_PLL_2                    (SYSTEM_REGISTER_BASE + 0x2b8)
#define REG_DDR_PLL_3                    (SYSTEM_REGISTER_BASE + 0x2bc)
#define REG_DDR_PLL_4                    (SYSTEM_REGISTER_BASE + 0x2c0)
#define REG_DDR_PLL_6                    (SYSTEM_REGISTER_BASE + 0x2c8)
#define REG_DDR_PLL_7                    (SYSTEM_REGISTER_BASE + 0x2cc)
#define REG_DDR_PLL_11                  (SYSTEM_REGISTER_BASE + 0x2dc)
#define REG_DDR_PLL_15                  (SYSTEM_REGISTER_BASE + 0x2ec)
#define REG_BOND_OPTION					(SYSTEM_REGISTER_BASE + 0xc)	

//bit offset
#define BIT_CRT_DFI_RATE_SEL 17

#define BIT_CRT_F_CODE 16
#define BIT_CRT_N_CODE 0

#define SINGLE_END_ENABLE 4

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


// DPI PHY IP register
#define ADR_DLY_0               (DPI_DLL_BASE + 0x000)
#define ADR_DLY_1               (DPI_DLL_BASE + 0x004)
#define ADR_DLY_2               (DPI_DLL_BASE + 0x008)                                                
#define CMD_DLY_0               (DPI_DLL_BASE + 0x00C)                                                
#define PAD_CMD              (DPI_DLL_BASE + 0x010)                                                
#define PAD_BK_ADR        (DPI_DLL_BASE + 0x014)
#define PAD_DQ                 (DPI_DLL_BASE + 0x018)
#define PAD_DQ_1             (DPI_DLL_BASE + 0x01C)                                            
#define CMD_CTRL              (DPI_DLL_BASE + 0x024)                                                
#define WRLVL_CTRL           (DPI_DLL_BASE + 0x028)       
#define TEST_CTRL0            (DPI_DLL_BASE + 0x02C)
                                                
#define DQ_DLY_0              (DPI_DLL_BASE + 0x030)   
#define DQ_DLY_0_1		 (DPI_DLL_BASE + 0x034) 
#define DQ_DLY_1              (DPI_DLL_BASE + 0x040)                                                
#define DQ_DLY_2              (DPI_DLL_BASE + 0x180)                                                
//#define $DQ_DLY_0_3              (DPI_DLL_BASE + 0x03C)                                                
//#define $DQ_DLY_1                (DPI_DLL_BASE + 0x040)                                                
#define READ_CTRL_0           (DPI_DLL_BASE + 0x044)                                                
#define READ_CTRL_0_1           (DPI_DLL_BASE + 0x048)                                                
//#define $READ_CTRL_0_2           (DPI_DLL_BASE + 0x04C)                                                
//#define $READ_CTRL_0_3           (DPI_DLL_BASE + 0x050)                                                
#define READ_CTRL_1             (DPI_DLL_BASE + 0x054)                                                
#define READ_CTRL_2             (DPI_DLL_BASE + 0x058)                                                
#define PAD_DQS            (DPI_DLL_BASE + 0x05C)                                                
#define PAD_DQS_2            (DPI_DLL_BASE + 0x060)                                                
#define PAD_DCK            (DPI_DLL_BASE + 0x064)                                                
#define CAL_SHIFT_CTRL          (DPI_DLL_BASE + 0x068)                                                
#define CAL_LS_SEL            (DPI_DLL_BASE + 0x06C)                                                
#define CAL_LS_SEL_1            (DPI_DLL_BASE + 0x070)                                                
//#define $CAL_LS_SEL_2            (DPI_DLL_BASE + 0x074)                                                
//#define $CAL_LS_SEL_3            (DPI_DLL_BASE + 0x078)                                                
#define CAL_RS_SEL            (DPI_DLL_BASE + 0x07C)                                                
#define CAL_RS_SEL_1            (DPI_DLL_BASE + 0x080)                                                
//#define $CAL_RS_SEL_2            (DPI_DLL_BASE + 0x084)                                                
//#define $CAL_RS_SEL_3            (DPI_DLL_BASE + 0x088)                                                
#define DQS_IN_DLY_0          (DPI_DLL_BASE + 0x08C)                                                
#define DQS_IN_DLY_0_1          (DPI_DLL_BASE + 0x090)                                                
//#define $DQS_IN_DLY_0_2          (DPI_DLL_BASE + 0x094)                                                
//#define $DQS_IN_DLY_0_3          (DPI_DLL_BASE + 0x098)                                                
#define DQS_IN_DLY_1          (DPI_DLL_BASE + 0x09C)                                                
#define DQS_IN_DLY_1_1          (DPI_DLL_BASE + 0x0A0)                                                
//#define $DQS_IN_DLY_1_2          (DPI_DLL_BASE + 0x0A4)                                                
//#define $DQS_IN_DLY_1_3          (DPI_DLL_BASE + 0x0A8)                                                
#define DQS_IN_DLY_2          (DPI_DLL_BASE + 0x0AC)                                                
#define DQS_IN_DLY_2_1          (DPI_DLL_BASE + 0x0B0)                                                
//#define $DQS_IN_DLY_2_2          (DPI_DLL_BASE + 0x0B4)                                                
//#define $DQS_IN_DLY_2_3          (DPI_DLL_BASE + 0x0B8)                                                
#define DQS_IN_DLY_3          (DPI_DLL_BASE + 0x0BC)                                                
#define DQS_IN_DLY_3_1          (DPI_DLL_BASE + 0x0C0)                                                
//#define $DQS_IN_DLY_3_2          (DPI_DLL_BASE + 0x0C4)                                                
//#define $DQS_IN_DLY_3_3          (DPI_DLL_BASE + 0x0C8)                                                
//#define $ANA_DLL_0               (DPI_DLL_BASE + 0x0CC)                                                
#define PAD_REF                 (DPI_DLL_BASE + 0x0D0)                                                
//#define $ODT_TTF_SET2            (DPI_DLL_BASE + 0x0D4)                                                
//#define $ODT_TTF_SET3            (DPI_DLL_BASE + 0x0D8)                                                
//#define $ODT_TTF_SET5            (DPI_DLL_BASE + 0x0DC)                                                
//#define $ODT_TTC_SET0            (DPI_DLL_BASE + 0x0E0)                                                
//#define $ODT_TTC_SET1            (DPI_DLL_BASE + 0x0E4)                                                
//#define $ODT_TTC_SET4            (DPI_DLL_BASE + 0x0E8)                                                
//#define $ODT_TTF_SET0            (DPI_DLL_BASE + 0x0EC)                                                
//#define $ODT_TTF_SET1            (DPI_DLL_BASE + 0x0F0)                                                
//#define $ODT_TTF_SET4            (DPI_DLL_BASE + 0x0F4)                                                
//#define $OCD_SET0                (DPI_DLL_BASE + 0x0F8)                                                
//#define $OCD_SET1                (DPI_DLL_BASE + 0x0FC)                                                
//#define $OCD_SET2                (DPI_DLL_BASE + 0x100)                                                
//#define $OCD_SET3                (DPI_DLL_BASE + 0x104)                                                
//#define $OCD_SET4                (DPI_DLL_BASE + 0x108)                                                
//#define $PAD_BUS_0               (DPI_DLL_BASE + 0x10C)                                                
//#define $PAD_BUS_1               (DPI_DLL_BASE + 0x110)                                                
//#define $ZQCMP                   (DPI_DLL_BASE + 0x114)                                                
#define DPI_CTRL_0              (DPI_DLL_BASE + 0x118)                                                
#define DPI_CTRL_1              (DPI_DLL_BASE + 0x11C)                                                
#define CAL_OUT_SEL             (DPI_DLL_BASE + 0x120)                                                
#define CAL_OUT_0             (DPI_DLL_BASE + 0x124)                                                
//#define $CAL_OUT_0_1             (DPI_DLL_BASE + 0x128)                                                
//#define $CAL_OUT_0_2             (DPI_DLL_BASE + 0x12C)                                                
//#define $CAL_OUT_0_3             (DPI_DLL_BASE + 0x130)                                                
#define WR_FIFO_0             (DPI_DLL_BASE + 0x134)                                                
//#define $WR_FIFO_0_1             (DPI_DLL_BASE + 0x138)                                                
//#define $WR_FIFO_0_2             (DPI_DLL_BASE + 0x13C)                                                
//#define $WR_FIFO_0_3             (DPI_DLL_BASE + 0x140)                                                
#define WR_FIFO_1             (DPI_DLL_BASE + 0x144)                                                
//#define $WR_FIFO_1_1             (DPI_DLL_BASE + 0x148)                                                
//#define $WR_FIFO_1_2             (DPI_DLL_BASE + 0x14C)                                                
//#define $WR_FIFO_1_3             (DPI_DLL_BASE + 0x150)                                                
#define RD_FIFO_0             (DPI_DLL_BASE + 0x154)                                               
//#define $RD_FIFO_0_1             (DPI_DLL_BASE + 0x158)                                                
//#define $RD_FIFO_0_2             (DPI_DLL_BASE + 0x15C)                                                
//#define $RD_FIFO_0_3             (DPI_DLL_BASE + 0x160)                                                
#define RD_FIFO_1             (DPI_DLL_BASE + 0x164)                                                
//#define $RD_FIFO_1_1             (DPI_DLL_BASE + 0x168)                                                
//#define $RD_FIFO_1_2             (DPI_DLL_BASE + 0x16C)                                                
//#define $RD_FIFO_1_3             (DPI_DLL_BASE + 0x170)                                                
//#define $ODT_TTC_SET5            (DPI_DLL_BASE + 0x174)                                                
//#define $ODT_TTC_SET6            (DPI_DLL_BASE + 0x178)                                                
#define TEST_CTRL1              (DPI_DLL_BASE + 0x17C)                                                
//#define $DQ_DLY_2                (DPI_DLL_BASE + 0x180)                                                
//#define $PAD_RZCTRL_STATUS       (DPI_DLL_BASE + 0x188)                                                
//#define $ANA_DLL_1               (DPI_DLL_BASE + 0x18C)                                                
//#define $CRT_ALL                 (DPI_DLL_BASE + 0x194)                                                
#define BIST_DEBUG              (DPI_DLL_BASE + 0x198)                                                
#define BIST_2TO1_0             (DPI_DLL_BASE + 0x1A0)                                                
#define BIST_2TO1_1             (DPI_DLL_BASE + 0x1A4)                                                
#define BIST_PT                 (DPI_DLL_BASE + 0x1A8)                                                
#define INT_CTRL                (DPI_DLL_BASE + 0x1AC)                                                
#define INT_STATUS_0            (DPI_DLL_BASE + 0x1B0)                                                
#define INT_STATUS_1          (DPI_DLL_BASE + 0x1B4)                                                
//#define $INT_STATUS_1_1          (DPI_DLL_BASE + 0x1B8)                                                
//#define $INT_STATUS_1_2          (DPI_DLL_BASE + 0x1BC)                                                
//#define $INT_STATUS_1_3          (DPI_DLL_BASE + 0x1C0)                                                
//#define $INT_STATUS_2            (DPI_DLL_BASE + 0x1C4)                                                
//#define $PLL_DLL_EN              (DPI_DLL_BASE + 0x1C8)                                                
#define DUMMY_0                 (DPI_DLL_BASE + 0x1CC)                                                
#define DUMMY_1                 (DPI_DLL_BASE + 0x1D0)                                                
#define DUMMY_2                 (DPI_DLL_BASE + 0x1D4)                                                
#define DUMMY_3                 (DPI_DLL_BASE + 0x1D8)                                                


#define MS_0_CTRL_BASE            BSP_MS_I_DRAMC_0_BASE
#define MS_0_CTRL_PHY_BASE        (BSP_MS_I_DRAMC_0_BASE-0xa0000000)
#define MS_0_WRAP_BASE            (MS_0_CTRL_BASE + 0x200)

#define MS_1_CTRL_BASE            BSP_MS_I_DRAMC_1_BASE
#define MS_1_CTRL_PHY_BASE        (BSP_MS_I_DRAMC_1_BASE-0xa0000000)
#define MS_1_WRAP_BASE            (MS_1_CTRL_BASE + 0x200)

#define MS_PCTL_CCR_OFFSET        0x000
#define MS_PCTL_DCR_OFFSET        0x004
#define MS_PCTL_IOCR_OFFSET       0x008
#define MS_PCTL_CSR_OFFSET        0x00c
#define MS_PCTL_DRR_OFFSET        0x010
#define MS_PCTL_TPR0_OFFSET       0x014
#define MS_PCTL_TPR1_OFFSET       0x018
#define MS_PCTL_TPR2_OFFSET       0x01c
#define MS_PCTL_MR_OFFSET         0x020
#define MS_PCTL_EMR1_OFFSET       0x024
#define MS_PCTL_EMR2_OFFSET       0x028
#define MS_PCTL_EMR3_OFFSET       0x02c
#define MS_PCTL_CSR2_OFFSET       0x030       
#define MS_PCTL_SRST_OFFSET       0x034      
#define MS_PCTL_DTR2_OFFSET       0x038
#define MS_PCTL_DTR3_OFFSET       0x03c
#define MS_PCTL_GDLLCR_OFFSET     0x040
#define MS_PCTL_DLLCR0_OFFSET     0x044
#define MS_PCTL_DLLCR1_OFFSET     0x048
#define MS_PCTL_DLLCR2_OFFSET     0x04c
#define MS_PCTL_DLLCR3_OFFSET     0x050
#define MS_PCTL_DLLCR4_OFFSET     0x054
#define MS_PCTL_DLLCR5_OFFSET     0x058
#define MS_PCTL_DLLCR6_OFFSET     0x05c
#define MS_PCTL_DLLCR7_OFFSET     0x060
#define MS_PCTL_DLLCR8_OFFSET     0x064
#define MS_PCTL_DQTR0_OFFSET      0x068
#define MS_PCTL_DQTR1_OFFSET      0x06c
#define MS_PCTL_DQTR2_OFFSET      0x070
#define MS_PCTL_DQTR3_OFFSET      0x074
#define MS_PCTL_DQTR4_OFFSET      0x078
#define MS_PCTL_DQTR5_OFFSET      0x07c
#define MS_PCTL_DQTR6_OFFSET      0x080
#define MS_PCTL_DQTR7_OFFSET      0x084
#define MS_PCTL_DQSTR_OFFSET      0x088
#define MS_PCTL_DQSBTR_OFFSET     0x08c
#define MS_PCTL_ODTCR_OFFSET      0x090
#define MS_PCTL_DTR0_OFFSET       0x094
#define MS_PCTL_DTR1_OFFSET       0x098
#define MS_PCTL_DTAR_OFFSET       0x09c
#define MS_PCTL_ZQCR0_OFFSET      0x0a0
#define MS_PCTL_ZQCR1_OFFSET      0x0a4
#define MS_PCTL_ZQSR_OFFSET       0x0a8
#define MS_PCTL_RSLR0_OFFSET      0x0ac
#define MS_PCTL_RSLR1_OFFSET      0x0b0
#define MS_PCTL_RSLR2_OFFSET      0x0b4
#define MS_PCTL_RSLR3_OFFSET      0x0b8
#define MS_PCTL_RDGR0_OFFSET      0x0bc
#define MS_PCTL_RDGR1_OFFSET      0x0c0
#define MS_PCTL_RDGR2_OFFSET      0x0c4
#define MS_PCTL_RDGR3_OFFSET      0x0c8
#define MS_PCTL_MXSL_OFFSET       0x0cc       

#define MS_PCTL_BCR_OFFSET        0x0d0
#define MS_PCTL_BALR0_OFFSET      0x0d4
#define MS_PCTL_BALR1_OFFSET      0x0d8
#define MS_PCTL_BDR0_OFFSET       0x0dc
#define MS_PCTL_BDR1_OFFSET       0x0e0
#define MS_PCTL_BBR_OFFSET        0x0e4
#define MS_PCTL_BSR_OFFSET        0x0e8
#define MS_PCTL_BYR_OFFSET        0x0ec
#define MS_PCTL_BFA_OFFSET        0x0f0
#define MS_PCTL_IDR_OFFSET        0x0f8
#define MS_PCTL_ERR_OFFSET        0x0fc

#define MS_WRAP_SCR_OFFSET        0x224
#define MS_WRAP_QCR_OFFSET        0x230
#define MS_WRAP_PCR_OFFSET        0x234
#define MS_WRAP_QTR0_OFFSET       0x240
#define MS_WRAP_QTR1_OFFSET       0x244
#define MS_WRAP_QTR2_OFFSET       0x248
#define MS_WRAP_QTR3_OFFSET       0x24c
#define MS_WRAP_QTR4_OFFSET       0x250
#define MS_WRAP_QTR5_OFFSET       0x254
#define MS_WRAP_QTR6_OFFSET       0x258
#define MS_WRAP_QTR7_OFFSET       0x25c
#define MS_WRAP_QTR8_OFFSET       0x260
#define MS_WRAP_QTR9_OFFSET       0x264
#define MS_WRAP_QTR10_OFFSET      0x268
#define MS_WRAP_QTR11_OFFSET      0x26c
#define MS_WRAP_QTR12_OFFSET      0x270
#define MS_WRAP_QTR13_OFFSET      0x274
#define MS_WRAP_QTR14_OFFSET      0x278
#define MS_WRAP_QTR15_OFFSET      0x27c

#define MS_PHY_DLY0               0x100  
#define MS_PHY_DLY1_RST           0x104  
#define MS_PHY_DLY_CLK            0x108  
#define MS_PHY_DLY_ST             0x10c  
#define MS_PHY_DLY_NUM            0x110 

#ifndef LANGUAGE_ASSEMBLY
#define PCTL_CCR_INIT_BFO          0
#define PCTL_CCR_INIT_BFW          1
#define PCTL_CCR_DTT_BFO           1
#define PCTL_CCR_DTT_BFW           1
#define PCTL_CCR_BTT_BFO           2
#define PCTL_CCR_BTT_BFW           1
#define PCTL_CCR_DPIT_BFO          3 
#define PCTL_CCR_DPIT_BFW          1
#define PCTL_CCR_FLUSH_FIFO_BFO    8 
#define PCTL_CCR_FLUSH_FIFO_BFW    1

#define PCTL_DCR_DDR3_BFO          0
#define PCTL_DCR_DDR3_BFW          3
#define PCTL_DCR_SDR_BFO           3
#define PCTL_DCR_SDR_BFW           1
#define PCTL_DCR_DQ32_BFO          4
#define PCTL_DCR_DQ32_BFW          1
#define PCTL_DCR_DFI_RATE_BFO      8
#define PCTL_DCR_DFI_RATE_BFW      3

#define PCTL_IOCR_RD_PIPE_BFO      8
#define PCTL_IOCR_RD_PIPE_BFW      4
#define PCTL_IOCR_TPHY_WD_BFO      12
#define PCTL_IOCR_TPHY_WD_BFW      5
#define PCTL_IOCR_TPHY_WL_BFO      17
#define PCTL_IOCR_TPHY_WL_BFW      3
#define PCTL_IOCR_TPHY_RD_EN_BFO   20
#define PCTL_IOCR_TPHY_RD_EN_BFW   5

#define PCTL_CSR_MEM_IDLE_BFO      8
#define PCTL_CSR_MEM_IDLE_BFW      1
#define PCTL_CSR_DT_IDLE_BFO       9 
#define PCTL_CSR_DT_IDLE_BFW       1
#define PCTL_CSR_BIST_IDLE_BFO     10
#define PCTL_CSR_BIST_IDLE_BFW     1
#define PCTL_CSR_DT_FAIL_BFO       11
#define PCTL_CSR_DT_FAIL_BFW       1
#define PCTL_CSR_BT_FAIL_BFO       12
#define PCTL_CSR_BT_FAIL_BFW       1

#define PCTL_DRR_TRFC_BFO          0
#define PCTL_DRR_TRFC_BFW          7
#define PCTL_DRR_TREF_BFO          8 
#define PCTL_DRR_TREF_BFW          24
#define PCTL_DRR_REF_NUM_BFO       24
#define PCTL_DRR_REF_NUM_BFW       4
#define PCTL_DRR_REF_DIS_BFO       28
#define PCTL_DRR_REF_DIS_BFW       1

#define PCTL_TPR0_TRP_BFO          0
#define PCTL_TPR0_TRP_BFW          4
#define PCTL_TPR0_TRAS_BFO         4 
#define PCTL_TPR0_TRAS_BFW         5 
#define PCTL_TPR0_TWR_BFO          9 
#define PCTL_TPR0_TWR_BFW          4
#define PCTL_TPR0_TRTP_BFO         13
#define PCTL_TPR0_TRTP_BFW         3

#define PCTL_TPR1_TRRD_BFO         0
#define PCTL_TPR1_TRRD_BFW         4
#define PCTL_TPR1_TRC_BFO          4 
#define PCTL_TPR1_TRC_BFW          6 
#define PCTL_TPR1_TRCD_BFO         10
#define PCTL_TPR1_TRCD_BFW         4
#define PCTL_TPR1_TCCD_BFO         14
#define PCTL_TPR1_TCCD_BFW         3
#define PCTL_TPR1_TWTR_BFO         17
#define PCTL_TPR1_TWTR_BFW         3
#define PCTL_TPR1_TRTW_BFO         20
#define PCTL_TPR1_TRTW_BFW         4

#define PCTL_TPR2_INIT_REF_NUM_BFO 0
#define PCTL_TPR2_INIT_REF_NUM_BFW 4
#define PCTL_TPR2_INIT_NS_EN_BFO   4 
#define PCTL_TPR2_INIT_NS_EN_BFW   1 
#define PCTL_TPR2_TMRD_BFO         5 
#define PCTL_TPR2_TMRD_BFW         2

#define PCTL_MRINFO_WR_LAT_BFO     0 
#define PCTL_MRINFO_WR_LAT_BFW     5
#define PCTL_MRINFO_RD_LAT_BFO     5 
#define PCTL_MRINFO_RD_LAT_BFW     5
#define PCTL_MRINFO_ADD_LAT_BFO    10
#define PCTL_MRINFO_ADD_LAT_BFW    5

#define PCTL_MR_BL_BFO             0 
#define PCTL_MR_BL_BFW             3
#define PCTL_MR_BT_BFO             3 
#define PCTL_MR_BT_BFW             1
#define PCTL_MR_CAS_BFO            4 
#define PCTL_MR_CAS_BFW            3
#define PCTL_MR_OP_BFO             8 
#define PCTL_MR_OP_BFW             12

#define PCTL_EMR1_ADDLAT_BFO       3 
#define PCTL_EMR1_ADDLAT_BFW       3

#define PCTL_CMD_DPIN_RSTN_BFO     0 
#define PCTL_CMD_DPIN_RSTN_BFW     1
#define PCTL_CMD_DPIN_CKE_BFO      1 
#define PCTL_CMD_DPIN_CKE_BFW      1
#define PCTL_CMD_DPIN_ODT_BFO      2 
#define PCTL_CMD_DPIN_ODT_BFW      1 

#define PCTL_BCR_STOP_BFO          0 
#define PCTL_BCR_STOP_BFW          1
#define PCTL_BCR_CMP_BFO           1 
#define PCTL_BCR_CMP_BFW           1
#define PCTL_BCR_LOOP_BFO          2 
#define PCTL_BCR_LOOP_BFW          1 
#define PCTL_BCR_DIS_MASK_BFO      3 
#define PCTL_BCR_DIS_MASK_BFW      1
#define PCTL_BCR_AT_STOP_BFO       4 
#define PCTL_BCR_AT_STOP_BFW       1
#define PCTL_BCR_FLUSH_CMD_BFO     8 
#define PCTL_BCR_FLUSH_CMD_BFW     1
#define PCTL_BCR_FLUSH_WD_BFO      9 
#define PCTL_BCR_FLUSH_WD_BFW      1
#define PCTL_BCR_FLUSH_RGD_BFO     10
#define PCTL_BCR_FLUSH_RGD_BFW     1
#define PCTL_BCR_FLUSH_RD_BFO      11
#define PCTL_BCR_FLUSH_RD_BFW      1
#define PCTL_BCR_FLUSH_RD_EXPC_BFO 16
#define PCTL_BCR_FLUSH_RD_EXPC_BFW 14

#define PCTL_BST_ERR_FST_TH_BFO    0 
#define PCTL_BST_ERR_FST_TH_BFW    12
#define PCTL_BST_ERR_CNT_BFO       16
#define PCTL_BST_ERR_CNT_BFW       14

#define PCTL_BSRAM0_CMD_LEVEL_BFO  0 
#define PCTL_BSRAM0_CMD_LEVEL_BFW  12
#define PCTL_BSRAM0_WD_LEVEL_BFO   16
#define PCTL_BSRAM0_WD_LEVEL_BFW   14

#define PCTL_BSRAM1_RG_LEVEL_BFO   0 
#define PCTL_BSRAM1_RG_LEVEL_BFW   14
#define PCTL_BSRAM1_RD_LEVEL_BFO   16
#define PCTL_BSRAM1_RD_LEVEL_BFW   14

#define WRAP_MISC_PAGE_SIZE_BFO    0 
#define WRAP_MISC_PAGE_SIZE_BFW    4
#define WRAP_MISC_BANK_SIZE_BFO    4 
#define WRAP_MISC_BANK_SIZE_BFW    2
#define WRAP_MISC_BST_SIZE_BFO     6 
#define WRAP_MISC_BST_SIZE_BFW     2 
#define WRAP_MISC_DDR_PARAL_BFO    8 
#define WRAP_MISC_DDR_PARAL_BFW    1

#if 0
struct ms_rxi310_portmap {
  volatile unsigned int ccr;           /* 0x000 */
  volatile unsigned int dcr;           /* 0x004 */
  volatile unsigned int iocr;          /* 0x008 */
  volatile unsigned int csr;           /* 0x00c */
  volatile unsigned int drr;           /* 0x010 */
  volatile unsigned int tpr0;          /* 0x014 */
  volatile unsigned int tpr1;          /* 0x018 */
  volatile unsigned int tpr2;          /* 0x01c */
  volatile unsigned int mr;            /* 0x020 */
  volatile unsigned int emr1;          /* 0x024 */
  volatile unsigned int emr2;          /* 0x028 */
  volatile unsigned int emr3;          /* 0x02c */
  volatile unsigned int cdpin;         /* 0x030 */
  volatile unsigned int tdpin;         /* 0x034 */
  volatile unsigned int dtr2;          /* 0x038 */
  volatile unsigned int dtr3;          /* 0x03c */
  volatile unsigned int gdllcr;        /* 0x040 */
  volatile unsigned int dllcr0;        /* 0x044 */
  volatile unsigned int dllcr1;        /* 0x048 */
  volatile unsigned int dllcr2;        /* 0x04c */
  volatile unsigned int dllcr3;        /* 0x050 */
  volatile unsigned int dllcr4;        /* 0x054 */
  volatile unsigned int dllcr5;        /* 0x058 */
  volatile unsigned int dllcr6;        /* 0x05c */
  volatile unsigned int dllcr7;        /* 0x060 */
  volatile unsigned int dllcr8;        /* 0x064 */
  volatile unsigned int dqtr0;         /* 0x068 */
  volatile unsigned int dqtr1;         /* 0x06c */
  volatile unsigned int dqtr2;         /* 0x070 */
  volatile unsigned int dqtr3;         /* 0x074 */
  volatile unsigned int dqtr4;         /* 0x078 */
  volatile unsigned int dqtr5;         /* 0x07c */
  volatile unsigned int dqtr6;         /* 0x080 */
  volatile unsigned int dqtr7;         /* 0x084 */
  volatile unsigned int dqstr;         /* 0x088 */
  volatile unsigned int dqsbtr;        /* 0x08c */
  volatile unsigned int odtcr;         /* 0x090 */
  volatile unsigned int dtr0;          /* 0x094 */
  volatile unsigned int dtr1;          /* 0x098 */
  volatile unsigned int dtar;          /* 0x09c */
  volatile unsigned int zqcr0;         /* 0x0a0 */
  volatile unsigned int zqcr1;         /* 0x0a4 */
  volatile unsigned int zqsr;          /* 0x0a8 */
  volatile unsigned int rslr0;         /* 0x0ac */
  volatile unsigned int rslr1;         /* 0x0b0 */
  volatile unsigned int rslr2;         /* 0x0b4 */
  volatile unsigned int rslr3;         /* 0x0b8 */
  volatile unsigned int rdgr0;         /* 0x0bc */
  volatile unsigned int rdgr1;         /* 0x0c0 */
  volatile unsigned int rdgr2;         /* 0x0c4 */
  volatile unsigned int rdgr3;         /* 0x0c8 */
  volatile unsigned int mxsl;          /* 0x0cc */
  volatile unsigned int bcr;           /* 0x0d0 */
  volatile unsigned int bst;           /* 0x0d4 */
  volatile unsigned int bsram0;        /* 0x0d8 */
  volatile unsigned int bsram1;        /* 0x0dc */
  volatile unsigned int bdr1;          /* 0x0e0 */
  volatile unsigned int bbr;           /* 0x0e4 */
  volatile unsigned int bsr;           /* 0x0e8 */
  volatile unsigned int byr;           /* 0x0ec */
  volatile unsigned int bfa;           /* 0x0f0 */
  volatile unsigned int pctl_svn;      /* 0x0f4 */
  volatile unsigned int pctl_idr;      /* 0x0f8 */
  volatile unsigned int err;           /* 0x0fc */

  // SDR_PHY CONTROL REGISTER
  volatile unsigned int phy_dly0;      /* 0x100 */
  volatile unsigned int phy_dly1_rst;  /* 0x104 */
  volatile unsigned int phy_dly_clk;   /* 0x108 */
  volatile unsigned int phy_dly_st;    /* 0x10c */
  volatile unsigned int phy_dly_num;   /* 0x110 */
  volatile unsigned int reserved0[68];

  // WRAP CONTROL REGISTER
  volatile unsigned int misc;          /* 0x224 */
  volatile unsigned int cq_ver;        /* 0x228 */
  volatile unsigned int cq_mon;        /* 0x22c */
  volatile unsigned int wq_ver;        /* 0x230 */
  volatile unsigned int wq_mon;        /* 0x234 */
  volatile unsigned int rq_ver;        /* 0x238 */
  volatile unsigned int rq_mon;        /* 0x23c */
  volatile unsigned int reserved1[8];          
  volatile unsigned int wrap_idr;      /* 0x2a0 */
  volatile unsigned int wrap_svn;      /* 0x2a4 */

}; //ms_rxi310_portmap
#else

struct ms_rxi310_portmap {
  volatile unsigned int ccr;           /* 0x000 */
  volatile unsigned int dcr;           /* 0x004 */
  volatile unsigned int iocr;          /* 0x008 */
  volatile unsigned int csr;           /* 0x00c */

  volatile unsigned int drr;           /* 0x010 */
  volatile unsigned int tpr0;          /* 0x014 */
  volatile unsigned int tpr1;          /* 0x018 */
  volatile unsigned int tpr2;          /* 0x01c */

  volatile unsigned int tpr3;          /* 0x020 */
  volatile unsigned int reserved_0x024;            /* 0x024 */
  volatile unsigned int cdpin;         /* 0x028 */
  volatile unsigned int tdpin;         /* 0x02c */

  volatile unsigned int mrinfo;         /* 0x030 */
  volatile unsigned int emr0;          /* 0x034 */
  volatile unsigned int emr1;          /* 0x038 */
  volatile unsigned int emr2;          /* 0x03c */

  volatile unsigned int emr3;          /* 0x040 */
  volatile unsigned int emr4;          /* 0x044 */
  volatile unsigned int emr5;          /* 0x048 */
  volatile unsigned int emr6;          /* 0x04c */

  /*Reserved*/
  volatile unsigned int dllcr3;        /* 0x050 */
  volatile unsigned int dllcr4;        /* 0x054 */
  volatile unsigned int dllcr5;        /* 0x058 */
  volatile unsigned int dllcr6;        /* 0x05c */

    /*Reserved*/
  volatile unsigned int dllcr7;        /* 0x060 */
  volatile unsigned int dllcr8;        /* 0x064 */
  volatile unsigned int dqtr0;         /* 0x068 */
  volatile unsigned int dqtr1;         /* 0x06c */


    /*Reserved*/
  volatile unsigned int dqtr2;         /* 0x070 */
  volatile unsigned int dqtr3;         /* 0x074 */
  volatile unsigned int dqtr4;         /* 0x078 */
  volatile unsigned int dqtr5;         /* 0x07c */

   /*Reserved*/
  volatile unsigned int dqtr6;         /* 0x080 */
  volatile unsigned int dqtr7;         /* 0x084 */
  volatile unsigned int dqstr;         /* 0x088 */
  volatile unsigned int dqsbtr;        /* 0x08c */


   /*Reserved*/
  volatile unsigned int odtcr;         /* 0x090 */
  volatile unsigned int dtr0;          /* 0x094 */
  volatile unsigned int dtr1;          /* 0x098 */
  volatile unsigned int dtar;          /* 0x09c */

  /*Reserved*/
  volatile unsigned int zqcr0;         /* 0x0a0 */
  volatile unsigned int zqcr1;         /* 0x0a4 */
  volatile unsigned int zqsr;          /* 0x0a8 */
  volatile unsigned int rslr0;         /* 0x0ac */

  /*Reserved*/
  volatile unsigned int rslr1;         /* 0x0b0 */
  volatile unsigned int rslr2;         /* 0x0b4 */
  volatile unsigned int rslr3;         /* 0x0b8 */
  volatile unsigned int rdgr0;         /* 0x0bc */

  /*Reserved*/
  volatile unsigned int rdgr1;         /* 0x0c0 */
  volatile unsigned int rdgr2;         /* 0x0c4 */
  volatile unsigned int rdgr3;         /* 0x0c8 */
  volatile unsigned int mxsl;          /* 0x0cc */


   volatile unsigned int bcr;           /* 0x0d0 */
  volatile unsigned int bct;           /* 0x0d4 */
  volatile unsigned int bcm;        /* 0x0d8 */
  volatile unsigned int bst;        /* 0x0dc */



  volatile unsigned int bsram0;        /* 0x0e0 */
  volatile unsigned int bsram1;        /* 0x0e4 */


  volatile unsigned int reserved_0x0e8;           /* 0x0e8 */
  volatile unsigned int reserved_0x0ec;           /* 0x0ec */


  volatile unsigned int  reserved_0x0f0;           /* 0x0f0 */
  volatile unsigned int pctl_svn;      /* 0x0f4 */
  volatile unsigned int pctl_idr;      /* 0x0f8 */
  
  volatile unsigned int reserved_0x0fc;           /* 0x0fc */


  // SDR_PHY CONTROL REGISTER
  volatile unsigned int phy_dly0;      /* 0x100 */
  volatile unsigned int phy_dly1_rst;  /* 0x104 */
  volatile unsigned int phy_dly_clk;   /* 0x108 */
  volatile unsigned int phy_dly_st;    /* 0x10c */
  volatile unsigned int phy_dly_num;   /* 0x110 */
  volatile unsigned int reserved0[68]; //offset + 0x110

  // WRAP CONTROL REGISTER
  volatile unsigned int misc;          /* 0x224 */
  volatile unsigned int cq_ver;        /* 0x228 */
  volatile unsigned int cq_mon;        /* 0x22c */
  volatile unsigned int wq_ver;        /* 0x230 */
  volatile unsigned int wq_mon;        /* 0x234 */
  volatile unsigned int rq_ver;        /* 0x240 */
  volatile unsigned int rq_mon;        /* 0x244 */
  volatile unsigned int reserved1[22];          
  volatile unsigned int wwrap_idr;     /* 0x2a0 */
  volatile unsigned int wrap_svn;      /* 0x2a4 */


}; //ms_rxi310_portmap

#endif


#define QFIFO_CMD_BANK_BFO             (35 - QFIFO_CMD_WRRD_BFO) // [38:35]
#define QFIFO_CMD_BANK_BFW              4                 
#define QFIFO_CMD_PAGE_BFO             (20 - QFIFO_CMD_WRRD_BFO) // [34:20]
#define QFIFO_CMD_PAGE_BFW              15 
#define QFIFO_CMD_COLU_BFO             (7  - QFIFO_CMD_WRRD_BFO) // [19: 7]
#define QFIFO_CMD_COLU_BFW              13 // [19: 7]
#define QFIFO_BST_LEN_BFO              (3  - QFIFO_CMD_WRRD_BFO) // [6:3]
#define QFIFO_BST_LEN_BFW               4  // [6:3]
#define QFIFO_CMD_WRRD_BFO              2                        // [2], remove bit[1:0]
#define QFIFO_CMD_WRRD_BFW              1  // [2], remove bit[1:0]

enum qfifo_wrrd {
  RD_CMD      = 0,
  WR_CMD      = 1 
};

struct bist_cmd_data_b37 {
  uint32_t        CMD_BANK; 
  uint32_t        CMD_PAGE; 
  uint32_t        CMD_COLU; 
  uint32_t        BST_LEN; 
  enum qfifo_wrrd WRRD;
   
};

struct bist_data_b128 {
  uint32_t        data_3; 
  uint32_t        data_2; 
  uint32_t        data_1; 
  uint32_t        data_0; 
};

#endif // not assembly

#endif // MS_RXI310_DEFS_H
