#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "uart.h"
#include "utils.h"
#include "platform.h"
#include "memory.h"
#include "sched.h"
#include "trap.h"
#include "timer.h"
#include "riscv.h"
#include "plic.h"


#include <stddef.h>
#include <stdarg.h>

#define KERNEL_VERSION "0.5.1"

extern void user_init();

extern void kernel();

#endif /* __OS_H__ */
