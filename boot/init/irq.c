#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <string.h>
#include <bspchip.h>
//#ifdef CONFIG_RTL8198C
#include <asm/rtl8198c.h>
//#endif

unsigned long exception_handlers[32];
void set_except_vector(int n, void *addr);
asmlinkage void do_reserved(struct pt_regs *regs);
void __init exception_init(void);


static struct irqaction *irq_action[NR_IRQS] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};



//#define ALLINTS (IE_IRQ0|IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)	
#define ALLINTS (IE_IRQ0|IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)	
//------------------------------------------------------------------------------

inline unsigned int					
clear_cp0_status(unsigned int clear)				
{								
	unsigned int res;					
								
	res = read_32bit_cp0_register(CP0_STATUS);		
	res &= ~clear;						
	write_32bit_cp0_register(CP0_STATUS, res);		
	SPECIAL_EHB();	
}								
//------------------------------------------------------------------------------

inline unsigned int					
change_cp0_status(unsigned int change, unsigned int newvalue)	
{								
	unsigned int res;					
								
	res = read_32bit_cp0_register(CP0_STATUS);		
	res &= ~change;						
	res |= (newvalue & change);					
	write_32bit_cp0_register(CP0_STATUS, res);		
	SPECIAL_EHB();							
	return res;						
}


//------------------------------------------------------------------------------

static void  GIMR_enable_irq(unsigned int irq)
{
	REG32(BSP_GIMR) |= (1<<irq);
  
}
//------------------------------------------------------------------------------

static void  GIMR_disable_irq(unsigned int irq)
{

	REG32(BSP_GIMR) &= ~(1<<irq);	
}

extern  void do_IRQ(int irq);
//------------------------------------------------------------------------------

void irq_dispatch(int irq_nr)
{
	int i,irq=0;
	//prom_printf("irq.c : irq_nr=%x  irq_nr2=%x  \n",irq_nr, irq_nr2);
	
	//Low 32bit
    for (i=0; i<=31; i++)
    {
        if (irq_nr & 0x01)
		{
			//prom_printf("do irq=%x\n",irq);
			do_IRQ(irq);

		}  
        irq++;
        irq_nr = irq_nr >> 1;
    }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


void  init_IRQ(void)
{
	extern asmlinkage void IRQ_finder(void);

	unsigned int i;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);
	

	/* Set up the external interrupt exception vector */
	/* First exception is Interrupt*/
	set_except_vector(0, IRQ_finder);

	/* Enable all interrupts */
	change_cp0_status(ST0_IM, ALLINTS);
}


// below is adopted from kernel/irq.c

//------------------------------------------------------------------------------

int setup_IRQ(int irq, struct irqaction *new)
{
    int shared = 0;
    struct irqaction *old, **p;
    unsigned long flags;

    p = irq_action + irq;
//    prom_printf("IRQ action=%x,%x\n",irq_action,irq);
    save_and_cli(flags);
    *p = new;
    
    restore_flags(flags);
    
    return 0;
}

int request_IRQ(unsigned long irq, struct irqaction *action, void* dev_id)
{

    int retval;
      
 //   prom_printf("IRQ No=%x,%x\n",irq,NR_IRQS);
    if (irq >= NR_IRQS)
		return -EINVAL;

	action->dev_id = dev_id;
	
    retval = setup_IRQ(irq, action);
 //   prom_printf("devid & retval =%x,%x\n",dev_id,retval);
 	
	GIMR_enable_irq(irq);
	//GIC_GIMR_enable(irq);
	
    if (retval)

	    return retval;
}

//------------------------------------------------------------------------------

int	free_IRQ(unsigned long irq)
{
	GIMR_disable_irq(irq);	
	
}

//------------------------------------------------------------------------------

void do_IRQ(int irqnr)
{
    struct irqaction *action;
	unsigned long i;

	//printf("Got irq %d\n", irqnr);
	
    action = *(irqnr + irq_action);
        	
	if (action) 
    {
    	//printf("Do ISR=%x\n", action->handler);
	    action->handler(irqnr, action->dev_id, NULL);
    }
	else
	{    
		prom_printf("Fail, you got irq=%X, but not have ISR\n", irqnr);
		for(;;);
	}			
}	


//------------------------------------------------------------------------------
void set_except_vector(int n, void *addr)
{
	unsigned handler = (unsigned long) addr;
	exception_handlers[n] = handler;
}
//------------------------------------------------------------------------------

void do_reserved(struct pt_regs *regs)
{
	/*fatal hard/software error*/
	int i;
	prom_printf("Undefined Exception happen.");	
	for(;;);
	/*Just hang here.*/
}

unsigned long ebase_reg = 0;
//------------------------------------------------------------------------------
void  init_exception(void)
{
	extern char exception_matrix;

	unsigned long i;
	clear_cp0_status(ST0_BEV);

	for (i = 0; i <= 31; i++)
		set_except_vector(i, do_reserved);

	ebase_reg = 0x80000000;
	write_c0_ebase(ebase_reg);
	memcpy((void *)(KSEG0 + 0x180), &exception_matrix, 0x80);
    flush_cache();
}
//------------------------------------------------------------------------------

//============================================================================

// init interrupt 
void initInterrupt(void)
{
	//printf("=>init interrupt...\n");
	REG32(BSP_GIMR)=0x00;/*mask all interrupt*/

	REG32(0xb8003114)=0;  //disable timer interrupt
	REG32(0xb8000010)&=~(1<<11);

	/*setup the BEV0,and IRQ */
	init_exception();/*Copy handler to 0x80000080*/
	init_IRQ();      /*Allocate IRQfinder to Exception 0*/
	sti();
}
