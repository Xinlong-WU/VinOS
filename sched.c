#include "os.h"
#include "uart.h"

uint8_t task_stack[STACK_SIZE];
struct context ctx_task;

static void w_mscratch(reg_t x){
    asm volatile(" csrw mscratch, %0" :: "r" (x));
}

void user_task0(void){
    print("Task 0 is Created");
    while(1){
        print("Task 0 is running...");
        task_delay(1000);
    }
}

void sched_init(){
    w_mscratch(0);

    ctx_task.sp = (reg_t) &task_stack[STACK_SIZE - 1];
    ctx_task.ra = (reg_t) user_task0;
}

void schedule(){
    struct context *nextTask = &ctx_task;
    switch_to(nextTask);
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}