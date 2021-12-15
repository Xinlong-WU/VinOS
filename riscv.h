#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

/*  Machine-mode scratch register */
static void w_mscratch(reg_t x){
    asm volatile(" csrw mscratch, %0" :: "r" (x));
}

#endif //__RISCV_H__