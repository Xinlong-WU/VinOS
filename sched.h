#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"

#define MAX_TASKS 10
#define STACK_SIZE 1024

/* task management */
struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;

	// save the pc to run in next schedule cycle
	reg_t pc; // offset: 31 *4 = 124
};

struct taskInfo{
	uint8_t taskId;
	uint8_t priority;
	uint32_t timeslice;
	struct context task_context;
};

extern struct taskInfo * _currentTask;
extern struct taskInfo * _KernelTask;

extern struct taskInfo * task_create(void (*task)(void* param),
							void *param, uint8_t priority, uint32_t timeslice);
extern void task_delay(volatile int count);
extern void task_yield();
extern void task_exit();
extern void schedule();
extern void tick_schedule();
extern void sched_init();
/* defined in entry.S */
extern void switch_to(struct context *next);

#endif /* __SCHED_H__ */