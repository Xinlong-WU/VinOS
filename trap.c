#include "trap.h"
#include "uart.h"
#include "riscv.h"
#include "plic.h"
#include "platform.h"
#include "timer.h"
#include "sched.h"

extern void trap_vector();
void external_interrupt_handler();

void trap_init(){
    /*
	 * set the trap-vector base-address for machine-mode
	 */
    w_mtvec((reg_t) trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & (~(1 << (MXLEN - 1)));
	
	if (cause & (1 << (MXLEN - 1))) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case M_SoftWare:{
			// printf("software interruption!\n");
			/*
			 * acknowledge the software interrupt by clearing
    		 * the MSIP bit in mip.
			 */
			reg_t id = r_mhartid();
			*(uint32_t*)CLINT_MSIP(id) = 0;
			schedule();
			break;
		}
		case M_Timer:
			// printf("timer interruption!\n");
			timer_handler();
			break;
		case M_External:
			// printf("external interruption!\n");
			external_interrupt_handler();
			break;
		default:
			printf("unknown async exception!\n");
			break;
		}
	} else {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case Load_acc_Fault:{
			// printf("software interruption!\n");
			/*
			 * acknowledge the software interrupt by clearing
    		 * the MSIP bit in mip.
			 */
			reg_t id = r_mhartid();
			*(uint32_t*)CLINT_MSIP(id) = 0;
			task_exit();
			schedule();
			break;
		}
		default:
			/* Synchronous trap - exception */
			printf("Sync exceptions!, code = %d\n", cause_code);
			panic("OOPS! What can I do!");
			//return_pc += 4;
			break;
		}
	}

	return return_pc;
}

void external_interrupt_handler(){
	int irq = plic_claim();
	switch(irq){
		default:
			printf("unexpected interrupt irq = %d\n", irq);
			break;
		case UART0_IRQ:{
			uart_isr();
			plic_complete(irq);
			break;
		}
	}
}

void trap_test()
{
	/*
	 * Synchronous exception code = 7
	 * Store/AMO access fault
	 */
	*(int *)0x00000000 = 100;

	/*
	 * Synchronous exception code = 5
	 * Load access fault
	 */
	//int a = *(int *)0x00000000;

	print("Yeah! I'm return back from trap!");
}
