#include "../autoconf.h"
#include "start.h"

#include <bspchip.h>
#include <rom_def.h>
#include <efuse.h>
#include <sys_reg.h>
#include <rlxboard.h>

#define HZ 100

#define REG32(reg)	(*(volatile unsigned int *)(reg))
#define REG(reg)                      (*((volatile unsigned int *)(reg)))
//#define DBG 1

#ifndef CONFIG_IRAM_IMAGE
#undef printf 
#define printf      btprintf
#else
#define printf
#endif
//#define _memctl_debug_printf btprintf
#define _memctl_debug_printf(...)

//joyce test
#ifdef CONFIG_DRAM_BOOT_TEST
extern void * memset(void * s,int c, unsigned int  count);
#else
void * memset(void * s,int c, unsigned int  count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}
#endif

#if 0
#define JUMPADDR 0xa0100000

#define _WDTCNR_			BSP_WDTCNR
#define _WDTKICK_			(1 << 23)
#define _WDTSTOP_			(0xA5f00000)

static __inline__ void watchdog_stop(void)
{
	*((volatile unsigned long *)_WDTCNR_) |= _WDTKICK_;

	*((volatile unsigned long *)_WDTCNR_) = _WDTSTOP_;

}

static __inline__ void watchdog_resume()
{
	*((volatile unsigned long *)_WDTCNR_) |= _WDTKICK_;
}


static __inline__ void watchdog_kick(void)
{
	*((volatile unsigned long *)_WDTCNR_) |= _WDTKICK_;
}
//---------------------------------------------------

#if DBG
void uart_outc(char c)
{
#define UART_WAIT_BREAK 0
#if UART_WAIT_BREAK
  	int i=0;
#endif

    while (1)
	{
#if UART_WAIT_BREAK	
		i++;		
		if (i >=3210)
			break;
#endif	
		if 	(REG32(BSP_UART0_LSR) & 0x60)
			break;	
	}

	//for(i=0; i<0xff00;i++);
 	REG32(BSP_UART0_THR)= (unsigned int)(c);  

	if (c == 0x0a)
		REG32(BSP_UART0_THR)= (unsigned int)(0x0d);  
}
#endif
#endif

void serial_outc(char c)
{
	unsigned int busy_cnt = 0;

	do
	{
		/* Prevent Hanging */
		if (busy_cnt++ >= 30000)
		{
			/* Reset Tx FIFO */
			REG8(BSP_UART0_FCR) = BSP_TXRST | BSP_CHAR_TRIGGER_14;
			return;
		}
	} while ((REG8(BSP_UART0_LSR) & BSP_LSR_THRE) == BSP_TxCHAR_AVAIL);

	/* Send Character */
	REG8(BSP_UART0_THR) = c;
	if (c == 0x0a)
		REG8(BSP_UART0_THR) = 0x0d;
	return;
}

#if 0
//-----------------------------------------------------
inline unsigned char uart_inc(void)
{
#define UART_WAIT_BREAK 0
#if UART_WAIT_BREAK
  	unsigned int i=0;
#endif
	unsigned register  ch;

    while (1)
	{
		if 	(REG32(BSP_UART0_LSR) & 0x01)
			break;	
		
#if UART_WAIT_BREAK	
		i++;		
		if (i >=6540)
			break;
#endif			
	}	
	ch=REG32(BSP_UART0_RBR);
	return ch;
}
//-----------------------------------------------------
unsigned int kbhit(unsigned int loops)
{	unsigned int i=loops;
	while(loops--)
	{
		if(REG32(BSP_UART0_LSR) & 0x01 )
			return 1;
	}
	return 0;
}

#define Put_UartData uart_outc

inline volatile unsigned int Check_UartRxDataRdy()
{
	if 	(REG32(BSP_UART0_LSR) & 0x01)
		return 1;
	else
		return 0;
}

inline volatile unsigned int Get_UartData()
{
	return REG32(BSP_UART0_RBR);
}

inline volatile int Get_UartData_timeout(unsigned char *c, unsigned int  timeout)
{	
	unsigned int t=timeout;

	while(t--)
	{
		if(Check_UartRxDataRdy())
		{	*c=Get_UartData();
			return 1;
		}

	}
	return 0;
}
#endif

//-----------------------------------------------------
int vsprintf(char *buf, const char *fmt, const int *dp)
{
//	#define putchar uart_outc
	#define putchar(x)	serial_outc(x)
	char *p, *s;

	s = buf;
	for ( ; *fmt != '\0'; ++fmt) 
	{
		if (*fmt != '%') 
		{
			buf ? *s++ = *fmt : putchar(*fmt);
			continue;
		}
		if (*++fmt == 's') 
		{
			for (p = (char *)*dp++; *p != '\0'; p++)
				buf ? *s++ = *p : putchar(*p);
		}
		else 
		{	/* Length of item is bounded */
			char tmp[20], *q = tmp;
			int alt = 0;
			int shift = 28;

#if 1   //wei patch for %02x
			if ((*fmt  >= '0') && (*fmt  <= '9'))
			{
				int width;
				unsigned char fch = *fmt;
		                for (width=0; (fch>='0') && (fch<='9'); fch=*++fmt)
		                {    width = width * 10 + fch - '0';
		                }
				  shift=(width-1)*4;
			}
#endif
		
			if (*fmt  == 'x') 
			{
				/* With x86 gcc, sizeof(long) == sizeof(int) */
				const long *lp = (const long *)dp;
				long h = *lp++;
				int ncase = (*fmt & 0x20);
				dp = (const int *)lp;
#if 0				
				if (alt) 
				{
					*q++ = '0';
					*q++ = 'x' | ncase;
				}
#endif				
				for ( ; shift >= 0; shift -= 4)					
					*q++ = "0123456789ABCDEF"[(h >> shift) & 0xF] | ncase;
				
			}
			else if (*fmt == 'd') 
			{
				int i = *dp++;
				char *r;
				if (i < 0) 
				{
					*q++ = '-';
					i = -i;
				}
				p = q;		/* save beginning of digits */
				do 
				{
					*q++ = '0' + (i % 10);
					i /= 10;
				} while (i);
				/* reverse digits, stop in middle */
				r = q;		/* don't alter q */
				while (--r > p) 
				{
					i = *r;
					*r = *p;
					*p++ = i;
				}
			}			
			else if (*fmt == 'c')
				*q++ = *dp++;
			else
				*q++ = *fmt;
			/* now output the saved string */
			for (p = tmp; p < q; ++p)
				buf ? *s++ = *p : putchar(*p);
		}
	}
	if (buf)
		*s = '\0';
	return (s - buf);
}
//--------------------------------------------------
void btprintf(const char *fmt, ...)
{	
	(void)vsprintf(0, fmt, ((const int *)&fmt)+1);	
}

//--------------------------------------------------

//==========================================

#define CYGNUM_HAL_RTC_NUMERATOR 1000000000
#define CYGNUM_HAL_RTC_DENOMINATOR 100
#define CYGNUM_HAL_RTC_DIV_FACTOR BSP_DIVISOR
#define CYGNUM_HAL_RTC_PERIOD ((BSP_SYS_CLK_RATE / CYGNUM_HAL_RTC_DIV_FACTOR) / CYGNUM_HAL_RTC_DENOMINATOR)

/* how many counter cycles in a jiffy */
#define CYCLES_PER_JIFFY	CYGNUM_HAL_RTC_PERIOD

#define HAL_CLOCK_READ( _pvalue_ )					\
{									\
	*(_pvalue_) = REG32(BSP_TC0CNT);				\
	*(_pvalue_) = (REG32(BSP_TC0CNT) >> 4) & 0x0fffffff;		\
}

/*------------------------------------------------------------------------*/
/* Delay for some number of useconds.                                     */
void hal_delay_us(int us)
{
    unsigned int val1, val2;
    int diff;
    long usticks;
    long ticks;

    // Calculate the number of counter register ticks per microsecond.
    
    usticks = (CYGNUM_HAL_RTC_PERIOD * CYGNUM_HAL_RTC_DENOMINATOR) / 1000000;

    // Make sure that the value is not zero. This will only happen if the
    // CPU is running at < 2MHz.
    if( usticks == 0 ) usticks = 1;
    
    while( us > 0 )
    {
        int us1 = us;

        // Wait in bursts of less than 10000us to avoid any overflow
        // problems in the multiply.
        if( us1 > 10000 )
            us1 = 10000;

        us -= us1;

        ticks = us1 * usticks;

        HAL_CLOCK_READ(&val1);
        while (ticks > 0) {
            do {
                HAL_CLOCK_READ(&val2);
            } while (val1 == val2);
            diff = val2 - val1;
            if (diff < 0) diff += CYGNUM_HAL_RTC_PERIOD;
            ticks -= diff;
            val1 = val2;
        }
    }
}

int timer_init(void)
{
	REG32(BSP_TCIR) = (BSP_TC0IP | BSP_TC1IP);	
	REG32(BSP_TCCNR) = 0;
	REG32(BSP_CDBR)=(CYGNUM_HAL_RTC_DIV_FACTOR) << BSP_DIVF_OFFSET;
	//REG32(BSP_TC0DATA) = (CYGNUM_HAL_RTC_PERIOD) << BSP_TCD_OFFSET;
	REG32(BSP_TC0DATA) = 0xfffffff0; // 31:4
	REG32(BSP_TCCNR) = BSP_TC0EN | BSP_TC0MODE_TIMER;
	//REG32(BSP_TCIR) = BSP_TC0IE;
	return 0;
}

//==============================================================
#if 0
unsigned int uart_rx8b_val()  //user input 
{
	unsigned int val=0;
	unsigned char ch;
	int i;

  for(i=28; i>=0; i=i-4)
  {
	ch=uart_inc();
	_memctl_debug_printf("%c",ch);
	ch= (ch<':')? ch-'0'  :   (ch<'Z') ?  ch-'A'+10 : ch-'a'+10;
	val+=ch<<i;
	
  }	
	return val;
}
//==============================================================
unsigned int uart_rx4b_val()    //transfer binary
{
	unsigned int val;
	unsigned char ch;
	
	ch=uart_inc();
	val=ch<<24;
	
	ch=uart_inc();
	val+=ch<<16;

	ch=uart_inc();
	val+=ch<<8;

	ch=uart_inc();
	val+=ch;
	
	return val;
}
//==============================================================


#define putc(x)	Put_UartData(x)
#define getc_timeout  Get_UartData_timeout


int xmodem_receive()
{

//uart1_init(1, 38400);
#define xprintf(x,...) 

	//xmodem state machine
	#define	XMODEM_RX_READY 1
	#define	XMODEM_WAIT_HEADER 2
	#define	XMODEM_RX_PACKET 3
	#define	XMODEM_CANCEL 4
	#define	XMODEM_EOT 5

	//xmodem protocol header
	#define SOH				0x01	/* start of header */
	#define STX 			       0x02	/* start of header */
	#define EOT				0x04	/* end of text */
	#define ACK				0x06	/* acknowledge */
	#define NAK				0x15	/* negative acknowledge */
	#define CAN				0x18	/* cancel */
	#define CRC				0x43	/* crc acknowledge */

	#define RETRY			(20)
	
	volatile unsigned int state = XMODEM_RX_READY;

	unsigned int retry = RETRY;
	unsigned int timeout=50000;
	unsigned int i;
	unsigned char  c;
	
	#define packet_size  128
	unsigned char  block_index = 1;

	unsigned char head1,head2,head_csum;
	unsigned char csum;
	unsigned char *ptr=JUMPADDR;
	//unsigned char *ptr=0x80100000;
	_memctl_debug_printf("Loading...");
#if 0	
	if(!buf )
		return false;
#endif	

	
	/* received buffer size to zero */
	unsigned int size = 0;
	
	while(retry > 0)
	{

		switch(state)
		{	
			case XMODEM_RX_READY:				
				putc(NAK);	
				if(getc_timeout(&c, timeout))
				{
					if(c == SOH)
					{	state = XMODEM_RX_PACKET;
					}			
					else if(c == 3)  //Ctrl+C
					{	putc(CAN);	putc(CAN);
						putc(CAN);	putc(CAN);						
						return 0;
					}
				}	
				break;
				
			case XMODEM_WAIT_HEADER:			
				if(getc_timeout(&c, timeout))
				{	if(c == SOH)
					{	state = XMODEM_RX_PACKET;						
					}				
					else if(c == CAN)
					{	state = XMODEM_CANCEL;					
					}
					else if(c == EOT)
					{	state = XMODEM_EOT;
					}
				}
				else
				{	/* timed out, try again */
					retry--;
				}
				break;
				
			case XMODEM_RX_PACKET:		
				if(getc_timeout(&c, timeout))
				{	xprintf("c1=%x\n", c);
					head1=c;
				}
				else goto failx;
				
				if(getc_timeout(&c, timeout))
				{	xprintf("c2=%x\n", c);
					head2=c;				
				}
				else goto failx;

				csum=0;
				for(i = 0; i < packet_size  ; i++)
				{
					if(getc_timeout(&c, timeout))
					{	xprintf("c3=%x\n", c);
						ptr[i] = c;
						csum+=c;

					}
					else goto failx;
				}

				if(getc_timeout(&c, timeout))
				{	xprintf("c4=%x\n", c);
					head_csum=c;	
				}
				else goto failx;
		
				
				state = XMODEM_WAIT_HEADER;
				
				/* packet was too small, retry */
				if(i < (packet_size))
				{	goto failx;
				}

				/* check validity of packet */
				if( (head1 == (255-head2)) && (head_csum==csum) )
				{	xprintf("hok\n");
					/* is this the packet we were waiting for? */
					if(head1== block_index)
					{	xprintf("aok\n");
						ptr+=packet_size;
						size += packet_size;						
						block_index++;
						retry = RETRY;
						putc(ACK);
						continue;
					}
					else if(head1 == (unsigned char )(block_index-1))
					{
						/* this is a retransmission of the last packet */
						putc(ACK);
						 continue;
					}
				}	
failx:				
				retry--;
				putc(NAK);
				break;
				
			case XMODEM_CANCEL:
				putc(ACK);				
				return 0;
				
			case XMODEM_EOT:
				putc(ACK);
				//mdelay(100);				
				return size;
							
			default:
				break;
		}
	}
	
	/* retry too much, fail */
	putc(CAN);	putc(CAN);
	putc(CAN);	putc(CAN);
	
	
	return 0;
}


//==========================================================

#if 1
unsigned int rand2(void)
{
    static unsigned int x = 123456789;
    static unsigned int y = 362436;
    static unsigned int z = 521288629;
    static unsigned int c = 7654321;

    unsigned long long t, a= 698769069;

    x = 69069 * x + 12345;
    y ^= (y << 13); y ^= (y >> 17); y ^= (y << 5);
    t = a * z + c; c = (t >> 32); z = t;

    return x + y + z;
}
#endif
#endif

//------------------------------------------------------------------
//u4Byte config_sys_mips_timer_fre;   // for udelay function

u4Byte get_cpu_freq(u4Byte strap_pin)
{
    u1Byte cpu_freq_sel, cpu_div_sel;
    u4Byte cpu_freq_base = CPU_FREQ_BASE, cpu_freq_step = 50;
    u4Byte cpu_freq;

    if ((strap_pin & BIT_STARP_PIN_CLKCPU_FROM_CLK200) == FALSE) {
        cpu_freq_sel = BIT_GET_STRAP_PIN_CK_CPU_FREQ_SEL(strap_pin);

    	/* default: 0111 = 0x7 */
    	if(cpu_freq_sel < STRAP_PIN_CK_CPU_FREQ_SEL_DEFAULT) {
            printf("err: ck_cpu_freq_sel(0x%x)\n", cpu_freq_sel);
            // Error case, set default CPU to the lowest speed
            cpu_freq_sel = 0;
    	} else {
    		cpu_freq_sel -= STRAP_PIN_CK_CPU_FREQ_SEL_DEFAULT;
    	}
        cpu_freq = cpu_freq_base + cpu_freq_step * cpu_freq_sel;
    } else {
        cpu_freq = 200;
    }

    cpu_div_sel = BIT_GET_STRAP_PIN_CK_CPU_DIV_SEL(strap_pin);
    cpu_freq = cpu_freq >> cpu_div_sel;

    //config_sys_mips_timer_fre = (cpu_freq/2)*1000000;   // #define CONFIG_SYS_MIPS_TIMER_FREQ   (CONFIG_OCP_CLOCK_FREQ/2)

    return cpu_freq;
}

void show_board_info(u4Byte strap_pin, pu1Byte pdram_type_idx, pu1Byte pdram_freq_idx)
{
    u4Byte dram_val, cpu_freq;

    // (dram_type, dram_freq)
    // 0,0 => DDR2 400 MHz
    // 0,1 => DDR2 533 MHz
    // 1,0 => DDR1 200 MHz
    // 1,1 => DDR1 250 MHz
    u4Byte dram_table[2][2] = {{400, 533}, {200, 250}};

    cpu_freq = get_cpu_freq(strap_pin);
    // dram_type:  0: DDR2, 1:DDR1
    *pdram_type_idx = BIT_GET_STRAP_PIN_DRAM_TYPE_INV(strap_pin);

 ////   printf("*pdram_type_idx:%d\n",*pdram_type_idx);



    if ((strap_pin & BIT_STRAP_PIN_HW_DBG_DISABLE) == BIT_STRAP_PIN_HW_DBG_DISABLE) {


        *pdram_freq_idx = BIT_GET_STRAP_PIN_DRAM_FEQ(strap_pin);
  ////      printf("1111 *pdram_freq_idx:%d\n",*pdram_freq_idx);


    } else { // HW_DBG_DISABLE == 0
        *pdram_freq_idx = 0;
    }

    ////printf("2222 *pdram_freq_idx:%d\n",*pdram_freq_idx);


    //dram_val = dram_table[*pdram_type_idx][*pdram_freq_idx];
    ////printf("3333 dram_val:%d\n",dram_val);

	//printf("CPU(%d MHz), DDR%d(%d MHz)\n", cpu_freq, ((*pdram_type_idx == 0) ? 2 : 1), dram_val);
}

void init_ram(u4Byte strap_pin, u1Byte dram_type_idx, u1Byte dram_freq_idx, PEFUSE_DATA pefuse_data)
{
    s4Byte ret_val;
    printf("init_ram\n");

#ifdef CONFIG_RAM_SDR
    ret_val = ss_init_rxi310_sdr(strap_pin);
#endif
#ifndef CONFIG_ROM_BOOT //def CONFIG_RAM_DDR
    //printf("init_ram strap_pin:0x%x dram_type_idx:0x%x dram_freq_idx:0x%x pefuse_data:0x%x\n",strap_pin, dram_type_idx, dram_freq_idx, pefuse_data);
    ret_val = ss_init_rxi310();
#endif
    if (ret_val < 0) {
        printf("init ddr fail\n");
    } else {
        printf("init ddr ok\n");
    }
}

void start_c(void)
{
	EFUSE_DATA efuse_data;
	u1Byte dram_type_idx, dram_freq_idx;
	u4Byte strap_pin;
	u4Byte dram_val, cpu_freq;
	// (dram_type, dram_freq)
	// 0,0 => DDR2 400 MHz
	// 0,1 => DDR2 533 MHz
	// 1,0 => DDR1 200 MHz
	// 1,1 => DDR1 250 MHz
	u4Byte dram_table[2][2] = {{400, 533}, {200, 250}};

#ifdef CONFIG_ESD_SUPPORT
	REG32(BSP_CDBR)=(BSP_DIVISOR) << BSP_DIVF_OFFSET;
        REG32(BSP_TC0DATA) = (((BSP_SYS_CLK_RATE/BSP_DIVISOR)/HZ)) << 4;
        REG32(0xb800311c)=0x00600000;
	/*For DDR3 must , for DDR2 safe*/
#endif
#ifdef CONFIG_SW_8367R
       REG32(BSP_PIN_MUX_SEL3) = (REG32(BSP_PIN_MUX_SEL3) & ~(3<<30)) | (0x3<<30);
        REG32(BSP_PEFGH_CNR) &= (~(0x00000040));
        REG32(BSP_PEFGH_DIR) |= (0x00000040);
                        REG32(BSP_PEFGH_DAT) &= (~(0x00000040));
#endif

	_memctl_debug_printf("\n8197F Cstart\n");

	timer_init();
	strap_pin = REG32(REG_HW_STRAP);
	cpu_freq = get_cpu_freq(strap_pin);
	// dram_type:  0: DDR2, 1:DDR1
	dram_type_idx = BIT_GET_STRAP_PIN_DRAM_TYPE_INV(strap_pin);
	//printf("*pdram_type_idx:%d\n",*pdram_type_idx);
	if ((strap_pin & BIT_STRAP_PIN_HW_DBG_DISABLE) == BIT_STRAP_PIN_HW_DBG_DISABLE) {
		dram_freq_idx = BIT_GET_STRAP_PIN_DRAM_FEQ(strap_pin);
		//printf("1111 *pdram_freq_idx:%d\n",*pdram_freq_idx);
	} else { // HW_DBG_DISABLE == 0
		dram_freq_idx = 0;
	}
	//printf("2222 *pdram_freq_idx:%d\n",*pdram_freq_idx);
	//dram_val = dram_table[*pdram_type_idx][*pdram_freq_idx];
	//printf("3333 dram_val:%d\n",dram_val);
	//printf("CPU(%d MHz), DDR%d(%d MHz)\n", cpu_freq, ((*pdram_type_idx == 0) ? 2 : 1), dram_val);

	switch (REG32(0xB800000C) & 0x0F) {
                case 0x06:
                case 0x0C:
                        dram_val =  32;
                        break;
                case 0x04:
                case 0x0A:
                        dram_val =  64;
                        break;
                case 0x05:
                case 0x0B:
                        dram_val =  128;
                        break;
                default:
#ifdef CONFIG_DRAM_SIZE
                        dram_val = CONFIG_DRAM_SIZE_IN_MB;
#else
                        //dram_val = 32;
        //                dram_val = REG32(0xB8000F00); // auto detect
			dram_val = 0;
#endif
		break;
        }
        REG32(0xB8000F00) = dram_val;


	memset(&efuse_data, 0, sizeof(EFUSE_DATA));
	init_ram(strap_pin, dram_type_idx, dram_freq_idx, &efuse_data);

	dram_val = REG32(0xB8000F00);

	if (dram_type_idx == 0)
		printf("\nDRAM Type: DDR2\n");
	else
		printf("\nDRAM Type: DDR1\n");

	/*#ifdef CONFIG_CPU_1G_DDR2_533M
	printf("\tDRAM frequency: 533MHz\n");
	#elif defined(CONFIG_CPU_800M_DDR2_400M)
	printf("\tDRAM frequency: 400MHz\n");
	#elif defined(CONFIG_CPU_600M_DDR2_200M)
	printf("\tDRAM frequency: 200MHz\n");
	#else*/
	printf("\tDRAM frequency: %dMHz\n", dram_table[dram_type_idx][dram_freq_idx]);
	//#endif

	

	//printf("\tDRAM Size: %dMB (auto detect)\n", REG32(0xB8000F00));
/*	switch (REG32(0xB800000C) & 0x0F) {
		case 0x06:
		case 0x0C:
			dram_val =  32;
			break;
		case 0x04:
		case 0x0A:
			dram_val =  64;
			break;
		case 0x05:
		case 0x0B:
			dram_val =  128;
			break;
		default:
#ifdef CONFIG_DRAM_SIZE
			dram_val = CONFIG_DRAM_SIZE_IN_MB;
#else
			//dram_val = 32;
			dram_val = REG32(0xB8000F00); // auto detect
#endif
	}
	REG32(0xB8000F00) = dram_val;*/
	printf("\tDRAM Size: %dMB\n", dram_val);
}
//==============================================================
#ifdef CONFIG_IRAM_IMAGE
#if 1
void iram_jump_to_load_image()
{	
	unsigned int addr;
    unsigned char sys_boot_type;
    unsigned int strap_pin;
    EFUSE_DATA efuse_data;

	s4Byte  (*iram_jump_to_load_image_func)(u1Byte sys_boot_type, PEFUSE_DATA pefuse_data, u4Byte addr, pu1Byte sig);

	strap_pin = REG32(REG_HW_STRAP);
    sys_boot_type = BIT_GET_STRAP_PIN_BOOT_SEL(strap_pin);
    addr = (unsigned int)UBOOT_ADDR;  
    memset(&efuse_data, 0, sizeof(EFUSE_DATA));

	iram_jump_to_load_image_func = (void*)(0xbfd0ecd0);

	iram_jump_to_load_image_func(sys_boot_type, &efuse_data, addr, UBOOT_SIGNATURE);

}
#endif

/*
SECTIONS
{
    init_ram = 0xbfd0f328;
}
*/
#ifdef CONFIG_CUSTOMER_BOOT_SIGNATURE_SUPPORT
void iram_do_ddr_init()
{
	unsigned char sys_boot_type;
    unsigned int strap_pin;
    EFUSE_DATA efuse_data;
    char dram_type_idx, dram_freq_idx;

	void (*init_ram_func)(u4Byte strap_pin, u1Byte dram_type_idx, u1Byte dram_freq_idx, PEFUSE_DATA pefuse_data);

    strap_pin = REG32(REG_HW_STRAP);
    sys_boot_type = BIT_GET_STRAP_PIN_BOOT_SEL(strap_pin);
    memset(&efuse_data, 0, sizeof(EFUSE_DATA));
	init_ram_func = (void*)(0xbfd0f328);
    // (dram_type, dram_freq)
	// 0,0 => DDR2 400 MHz
	// 0,1 => DDR2 533 MHz
	// 1,0 => DDR1 200 MHz
	// 1,1 => DDR1 250 MHz

	/* ddr type */
	#ifdef CONFIG_DDR2_SDRAM
	dram_type_idx = 0;
		#ifdef CONFIG_DDR2_400MHZ
		dram_freq_idx = 0;
		#endif
		#ifdef CONFIG_DDR2_533MHZ
		dram_freq_idx = 1;
		#endif
	#endif
	#ifdef CONFIG_DDR1_SDRAM
	dram_type_idx = 1;
		#ifdef CONFIG_DDR1_200MHZ
		dram_freq_idx = 0;
		#endif
		#ifdef CONFIG_DDR1_250MHZ
		dram_freq_idx = 1;
		#endif
	#endif

	/* ddr freq */	
    init_ram_func(strap_pin, dram_type_idx, dram_freq_idx, &efuse_data);
}
#endif
#endif

