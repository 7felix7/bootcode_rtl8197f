#include <linux/types.h>
#include "rtk.h"
#include <bspchip.h>
#include "etherboot.h"


/* Setting image header */
typedef struct _setting_header_ {
	unsigned char  Tag[2];
	unsigned char  Version[2];
	unsigned short len;
} SETTING_HEADER_T, *SETTING_HEADER_Tp;

//------------------------------------------------------------------------------------------
#define BAUD_RATE	  		(38400)  
#define ACCCNT_TOCHKKEY 	(128*1024)  //128K
unsigned long return_addr;
#define WAIT_TIME_USER_INTERRUPT	(3*BSP_CPU0_FREQ)

#define LOCALSTART_MODE 0
#define DOWN_MODE 1
#define DEBUG_LOCALSTART_MODE 3

/*Cyrus Tsai*/
unsigned long kernelsp;
#if  defined(RTL8198)
#ifdef CONFIG_NAND_FLASH
	#define _SYSTEM_HEAP_SIZE	1024*64*4*16	//wei add
#else
	#define _SYSTEM_HEAP_SIZE	1024*64	//wei add
#endif
	char dl_heap[_SYSTEM_HEAP_SIZE];	//wei add
#endif


//check
#define HS_IMAGE_OFFSET		(24*1024)	//0x6000
#define DS_IMAGE_OFFSET		(25*1024)	//0x6400
#define CS_IMAGE_OFFSET		(32*1024)	//0x8000

#define CODE_IMAGE_OFFSET		(64*1024)	//0x10000
#define CODE_IMAGE_OFFSET2	(128*1024)	//0x20000
#define CODE_IMAGE_OFFSET3	(192*1024)	//0x30000
#define CODE_IMAGE_OFFSET4	(0x8000)

//flash mapping
#define ROOT_FS_OFFSET		(0x260000)
#define ROOT_FS_OFFSET_OP1		(0x10000)
#define ROOT_FS_OFFSET_OP2		(0x40000)

#if defined(CONFIG_WEBPAGE_BACKUP)
#define WEBPAGE_OFFSET		(0x10000)
#define WEBPAGE_OFFSET_OP1	(0x20000)
#define WEBPAGE_OFFSET_OP2	(0x30000)
#endif



int enable_10M_power_saving(int phyid , int regnum,int data);
int user_interrupt(unsigned long time);

#if defined( CONFIG_NFBI) || defined(CONFIG_NONE_FLASH)
// return,  0: not found, 1: linux found, 2:linux with root found
int check_system_image(unsigned long addr,IMG_HEADER_Tp pHeader);
#else
int check_system_image(unsigned long addr,IMG_HEADER_Tp pHeader,SETTING_HEADER_Tp setting_header);
int check_rootfs_image(unsigned long addr);
#endif
//------------------------------------------------------------------------------------------

//ddr
#define DDR_DBG 0
#define PROMOS_DDR_CHIP 1
#define IMEM_DDR_CALI_LIMITS 60 //1sec=30 times
#define __IRAM_IN_865X      __attribute__ ((section(".iram-rtkwlan")))
#define __IRAM_FASTEXTDEV        __IRAM_IN_865X

#define RTL_W32(addr, l)			((*(volatile unsigned long*)(addr)) = (l))
#define RTL_R32(addr)				(*(volatile unsigned long *)(addr))

#define Check_UART_DataReady()	(REG32(BSP_UART0_LSR) & 0x01)
#define Get_UART_Data()			(REG32(BSP_UART0_RBR) & 0xff)
#define Get_GPIO_SW_IN()		(!(RTL_R32(BSP_GPIO_DAT_REG(BSP_RESET_BTN_PIN)) & (1<<BSP_GPIO_BIT(BSP_RESET_BTN_PIN))))
#define Set_GPIO_LED_ON()		(REG32(BSP_GPIO_DAT_REG(BSP_RESET_LED_PIN)) =  REG32(BSP_GPIO_DAT_REG(BSP_RESET_LED_PIN)) & (~(1<<BSP_GPIO_BIT(BSP_RESET_LED_PIN))))
#define Set_GPIO_LED_OFF()      (REG32(BSP_GPIO_DAT_REG(BSP_RESET_LED_PIN)) =  REG32(BSP_GPIO_DAT_REG(BSP_RESET_LED_PIN)) | (1<<BSP_GPIO_BIT(BSP_RESET_LED_PIN)))

void Init_GPIO();

