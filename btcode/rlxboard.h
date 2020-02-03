/*
 * Realtek Semiconductor Corp.
 *
 * Copyright 2012  Jethro Hsu (jethro@realtek.com)
 * Copyright 2012  Tony Wu (tonywu@realtek.com)
 */

//#include <linux/autoconf.h>
#ifndef __RLXBOARD_H__
#define __RLXBOARD_H__

#define CONFIG_MIPS32		1  /* MIPS32 CPU core	*/
#define CONFIG_CPU_HAS_WBC	1

#define CONFIG_ETHADDR		DE:AD:BE:EF:01:01    /* Ethernet address */
//wei del
//#define CONFIG_BOOTDELAY	5			/* autoboot after 2 seconds */
//#define CONFIG_BAUDRATE		57600
#define CONFIG_BAUDRATE		38400

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define	CONFIG_TIMESTAMP		/* Print image info with timestamp */
#undef	CONFIG_BOOTARGS

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"addmisc=setenv bootargs ${bootargs} "				\
		"console=ttyS0,${baudrate} "				\
		"panic=1\0"						\
	"bootfile=/vmlinux.elf\0"					\
	"load=tftp 80500000 ${u-boot}\0"				\
	""
/* Boot from NFS root */
//#define CONFIG_BOOTCOMMAND	"bootp; setenv bootargs root=/dev/nfs rw nfsroot=${serverip}:${rootpath} ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}::off; bootm 0x80001000"
#define CONFIG_BOOTCOMMAND	"bootm 0x81000000"

/*
 * Miscellaneous configurable options
 */
#undef CONFIG_SYS_LONGHELP				/* undef to save memory      */
#define	CONFIG_SYS_PROMPT		"rlxboot# "	/* Monitor Command Prompt    */
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size   */

/* Print Buffer Size */
#define	CONFIG_SYS_PBSIZE 		(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args*/

#define CONFIG_SYS_MALLOC_LEN		(1 << 20)
#define CONFIG_SYS_BOOTPARAMS_LEN	(32 << 10)
#define CONFIG_SYS_HZ			    1000

#define CONFIG_SYS_SDRAM_BASE		0x80000000     /* Cached addr */
#define CONFIG_SYS_SDRAM_BASE_UNCACHE  0xa0000000    /*uncached addr*/
#define CONFIG_SYS_SDRAM_SIZE		(32 << 20)

//wei add
#define	CONFIG_SYS_TEXT_BASE	    0xa0500000  


#define	CONFIG_SYS_LOAD_ADDR		0x80001000     /* default load address	*/

#define CONFIG_SYS_MEMTEST_START	0xA0400000  // 0x80400000
#define CONFIG_SYS_MEMTEST_END		0xA0500000  // 0x80500000

/*-----------------------------------------------------------------------
 * SMP Configuration
 */
//#define CONFIG_SMP              /* undefine this if it is not a SMP */  //wei del, because 96F is single core.
#define CORENLOOP               0xa0000e00
#define SMPBOOT                 0xa0000f00
#define CONFIG_BOOT_ADDR        0xbfb070f4

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_FLASH_BOOT		0       /* set '1' to boot from flash */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */

#define PHYS_FLASH_1			0xbfc00000 /* Flash Bank #1 */

/* The following #defines are needed to get flash environment right */
#define	CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define	CONFIG_SYS_MONITOR_LEN		(128 << 10)

//#define CONFIG_SYS_INIT_SP_OFFSET	0x1000000
#define CONFIG_SYS_INIT_SP_SIZE		4096

/* We boot from this flash, selected with dip switch */
#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(2 * CONFIG_SYS_HZ) /* Timeout for Flash Erase */
#define CONFIG_SYS_FLASH_WRITE_TOUT	(2 * CONFIG_SYS_HZ) /* Timeout for Flash Write */

#define	CONFIG_ENV_IS_NOWHERE		1

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_ADDR			0x80800000
#define CONFIG_ENV_SIZE			0x10000
#define CONFIG_FLASH_16BIT
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_NET_MULTI
#define CONFIG_MEMSIZE_IN_BYTES


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
//#define CONFIG_SYS_DCACHE_SIZE		32768
//#define CONFIG_SYS_ICACHE_SIZE		65536
//#define CONFIG_SYS_CACHELINE_SIZE	32

/*-----------------------------------------------------------------------
 * uboot Configuration
 */

/* Support bootm-ing different OSes */
#define CONFIG_BOOTM_LINUX	1
#define CONFIG_BOOTM_NETBSD	0
#define CONFIG_BOOTM_RTEMS	0

#define CONFIG_GZIP		1
#define CONFIG_ZLIB		1
#define CONFIG_PARTITIONS	1

#define CONFIG_CMD_BOOTM	1
#define CONFIG_CMD_CRC32	1
#define CONFIG_CMD_EXPORTENV	1
#define CONFIG_CMD_IMPORTENV	1
#define CONFIG_CMD_SAVEENV	/* saveenv                      */
#define CONFIG_CMD_EDITENV	/* editenv                      */
#define CONFIG_CMD_GO		1

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/*
 * Command line configuration.
 */
#define CONFIG_CMD_CONSOLE	/* coninfo                      */
#define CONFIG_CMD_ECHO		/* echo arguments               */
//#define CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot    */  //wei del
#define CONFIG_CMD_SOURCE	/* "source" command support     */
#define CONFIG_CMD_XIMG		/* Load part of Multi Image     */

#undef CONFIG_CMD_AMBAPP
#define CONFIG_CMD_BDI		/* bdinfo                       */
#undef CONFIG_CMD_BEDBUG
#undef CONFIG_CMD_BOOTD		/* bootd                        */
#undef CONFIG_CMD_IMI		/* iminfo                       */
#undef CONFIG_CMD_ITEST		/* Integer (and string) test    */
#undef CONFIG_CMD_I2C
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support   */
#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect       */
#undef CONFIG_CMD_IMLS		/* List all found images        */
#undef CONFIG_CMD_LOADB		/* loadb                        */
#undef CONFIG_CMD_LOADS		/* loads                        */
#undef CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#undef CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#undef CONFIG_CMD_NFS		/* NFS support                  */
#undef CONFIG_CMD_PXE
#undef CONFIG_CMD_UBI
#undef CONFIG_CMD_UBIFS
#undef CONFIG_CMD_RUN          /* run command in env variable  */
#undef CONFIG_CMD_SETGETDCR    /* DCR support on 4xx           */

#undef CONFIG_CMD_ELF
#undef CONFIG_CMD_IDE
#undef CONFIG_CMD_FAT


//#define CONFIG_USE_IRQ   //wei del

//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CONFIG_MODEL_MIPS
#define PHY_2_NONCACHE_ADDR(addr)   ((addr) | 0x80000000)
#define PHY_2_CACHE_ADDR(addr)      ((addr) | 0xA0000000)
#define CACHE_2_NONCACHE_ADDR(addr) ((addr) | 0x20000000)
#define VIR_2_PHY_ADDR(addr)        ((addr) & (~0xA0000000))
#endif  // CONFIG_MODEL_MIPS

#ifdef CONFIG_RTL8197F

#define CONFIG_SYS_ROM_CACHE_BASE_ADDR      0x9FD00000
#define CONFIG_SYS_SRAM_CACHE_BASE_ADDR     0x9FE00000
#define CONFIG_SYS_ROM_NONCACHE_BASE_ADDR   0xBFD00000
#define CONFIG_SYS_SRAM_NONCACHE_BASE_ADDR  0xBFE00000
#define CONFIG_SYS_ROM_PHY_BASE_ADDR        0x1FD00000
#define CONFIG_SYS_SRAM_PHY_BASE_ADDR       0x1FE00000

#define CONFIG_SYS_INIT_SP_OFFSET	        0x4000
#define CONFIG_SYS_INIT_SP_SIZE		        4096

// each IP base addr
#define SPI_NOR_BASE_ADDR                   0xB0000000
#define EFUSE_BASE_ADDR                     0xB8000700
//#define SWITCH_BASE_ADDR                    0xB8010000    // in switch/include/rtl8196x/asicregs.h
#define EMMC_BASE_ADDR                      0xB8015000
#define PARALLEL_NAND_BASE_ADDR             0xB801A000
#define SPI_NAND_BASE_ADDR                  0xB801A400
#define UART0_BASE_ADDR                     0xB8147000
#define UART1_BASE_ADDR                     0xB8147400
#define DW_TIMER_BASE_ADDR                  0xb8148000

// optional features
#define FPGA_VERIFY_EFUSE                   0
#define EFUSE_WRITE_CMD                     0
#define XMODEM_CMD                          0

#if defined(CONFIG_VERIFY_FPGA) || defined(CONFIG_VERIFY_FPGA_BITFILE)
#define CONFIG_OCP_CLOCK_FREQ               66000000
#else   // CONFIG_VERIFY_ASIC || CONFIG_VERIFY_DV
#define CONFIG_OCP_CLOCK_FREQ               800000000
#endif
#define CONFIG_UART_FREQ                    (CONFIG_OCP_CLOCK_FREQ/8)

#define CPU_FREQ_BASE                       (CONFIG_OCP_CLOCK_FREQ/1000000)
#define UBOOT_ADDR                          0xA0500000

#endif  // CONFIG_RTL8197F

// #define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_OCP_CLOCK_FREQ/2)

#endif	/* __CONFIG_H */
