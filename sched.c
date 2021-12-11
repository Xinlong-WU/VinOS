#include "os.h"
#include "uart.h"

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct taskInfo * tasks_info[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;

void dumpTasksList();

static void w_mscratch(reg_t x){
    asm volatile(" csrw mscratch, %0" :: "r" (x));
}

void sched_init(){
    w_mscratch(0);
}


struct taskInfo * popTask(){
	struct taskInfo * task = tasks_info[0];
	if(task->priority != 0) 
		task->priority--;
	for(int i = 1; i < _top; i++){
		if(task->priority <= tasks_info[i]->priority){
			tasks_info[i - 1] = tasks_info[i];
		}
		else{
			tasks_info[i - 1] = task;
			return task;
		}
	}
	tasks_info[_top - 1] = task;
	return task;
}

void schedule(){
    if(_top <= 0){
        panic("Num of task should be greater than zero!");
        return;
    }
    struct taskInfo * nextTask = popTask();
	// dumpTasksList();
	struct context * ctx = &(nextTask->task_context);
	// printf("switch to task 0x%x\n",ctx);
    switch_to(ctx);
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

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- task: task entry
 *  - param: the parameter of the task
 *	- priority: priority of the task
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
extern int  task_create(void (*task)(void* param),
                 void *param, uint8_t priority) {
	struct taskInfo * newTask = malloc(sizeof(struct taskInfo));
	newTask->taskId = _top;
	newTask->priority = priority;
	newTask->task_context.sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
	newTask->task_context.ra = (reg_t) task;
	if(param != NULL)
		newTask->task_context.a0 = (reg_t) param;
	return insertTask(newTask);
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
	schedule();
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

void dumpTasksList(){
	printf("Task List: ");
	for(int i = 0; i < _top; i++){
		printf("[%d]{id:%d; priority:%d}\t",i,tasks_info[i]->taskId,tasks_info[i]->priority);
	}
	printf("\n");
}
