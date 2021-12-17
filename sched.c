#include "os.h"
#include "sched.h"
#include "types.h"
#include "uart.h"
#include "riscv.h"

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct taskInfo * tasks_info[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _currentTask is used to point to the entry addr of current task
 */
static int _top = 0;
static int _totalTaskCounter = 0;
struct taskInfo * _currentTask = 0x0;
struct taskInfo * _KernelTask = 0x0;

void dumpTasksList();

struct taskInfo * task_create(void (*task)(void* param),
								void *param, uint8_t priority,
								uint32_t timeslice);

void sched_init(){
    w_mscratch(0);
	_KernelTask = task_create(kernel,NULL,0,1);
	/* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
}

int getTaskCounter(){
	return _top;
}

struct taskInfo * popTask(){
	struct taskInfo * task = tasks_info[0];
	if(task->priority != 0) 
		task->priority--;
	for(int i = 1; i < _top; i++){
		// skip if taskId is 0
		// kernel task should not be schedule
		if(tasks_info[i]->taskId == 0)
			continue;
		if(task->priority <= tasks_info[i]->priority){
			tasks_info[i - 1] = tasks_info[i];
		}
		else{
			tasks_info[i - 1] = task;
			return task;
		}
	}
	// skip the last one task
	// cause it is kernel task;
	tasks_info[_top - 2] = task;
	return task;
}

/*
 * DESCRIPTION
 * 	insert new task into task list.
 * 	- newTask
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int insertTask(struct taskInfo * newTask){
	if (_top < MAX_TASKS) {
		struct taskInfo * tmpTask ;
		for(int i = 0; i < _top; i++){
			if(newTask->priority > tasks_info[i]->priority){
				tmpTask = tasks_info[i];
				tasks_info[i] = newTask;
				newTask = tmpTask;
			}
		}
		tasks_info[_top++] = newTask;
		// dumpTasksList();
		return 0;
	} else {
		return -1;
	}
}

#ifdef RV32
static uint32_t tickCounter = 0;
#else
static uint64_t tickCounter = 0;
#endif
void tick_schedule(){
	if(_tick - tickCounter >= _currentTask->timeslice){
		tickCounter = _tick;
		schedule();
	}
}

void schedule(){
    _currentTask = popTask();
	if(_currentTask == NULL){
		panic("ERROR: There is no task to be scheduled.\n");
		return;
	}
	struct context * ctx = &(_currentTask->task_context);
    switch_to(ctx);
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- task: task entry
 *  - param: the parameter of the task
 *	- priority: priority of the task
 * 	- timeslice: timeslice for eact exec, min value is 1
 * RETURN VALUE
 * 	NULL error occured
 */
struct taskInfo * task_create(void (*task)(void* param),
						void *param, uint8_t priority, uint32_t timeslice) {
	struct taskInfo * newTask = malloc(sizeof(struct taskInfo));
	newTask->taskId = _totalTaskCounter++;
	newTask->priority = priority;
	newTask->timeslice = timeslice == 0 ? 1 : timeslice;
	newTask->task_context.sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
	newTask->task_context.pc = (reg_t) task;
	if(param != NULL)
		newTask->task_context.a0 = (reg_t) param;
	return insertTask(newTask) == 0 ? newTask : NULL;
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield(){
	int hartId = r_mhartid();
	*((uint32_t*)CLINT_MSIP(hartId)) = 1;
}

void task_exit(){
	for(int i = 0; i < _top;){
		if(tasks_info[i++] == _currentTask){
			while(i < _top){
				tasks_info[i - 1] = tasks_info[i];
				i++;
			}
		}
	}
	_top--;
	return;
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

// switch back to os
void task_os() {
	switch_to(&(_KernelTask->task_context));
}

void dumpTasksList(){
	printf("Task List: ");
	for(int i = 0; i < _top; i++){
		printf("[%d]{id:%d; priority:%d}\t",i,tasks_info[i]->taskId,tasks_info[i]->priority);
	}
	printf("\n");
}
