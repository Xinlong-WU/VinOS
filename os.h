#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "uart.h"
#include "utils.h"
#include "platform.h"
#include "memory.h"
#include "sched.h"


#include <stddef.h>
#include <stdarg.h>

#define KERNEL_VERSION "0.2.7"

extern void user_init();

extern void kernel();

#endif /* __OS_H__ */
