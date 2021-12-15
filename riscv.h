#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

#define MXLEN 32

/*  Machine-mode scratch register */
static inline void w_mscratch(reg_t x){
    asm volatile(" csrw mscratch, %0" :: "r" (x));
}

/* Machine-mode interrupt vector */
static inline void w_mtvec(reg_t x)
{
	asm volatile("csrw mtvec, %0" : : "r" (x));
}


#endif //__RISCV_H__