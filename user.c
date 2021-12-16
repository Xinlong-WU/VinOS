#include "os.h"
#include "uart.h"

#define DELAY 1000

void user_task0(void * param)
{
	int i = 50;
	int * pa = (int *)param;
	printf("Task 0: Created!\n");
	printf("Param of Task 0: %d\n\n", *pa);
	while (i--) {
		printf("Task 0: Running...\n");
		printf("Param of Task 0: %d\n\n", *pa);
		*pa += 1;
		task_delay(DELAY);
	}
	printf("===================> user task 0 distoryed\n");
	return;
}

void user_task1(void * param)
{
	int i = 50;
	int * pa = (int *)param;
	printf("Task 1: Created!\n");
	printf("Param of Task 1: %d\n\n", *pa);
	while (i--) {
		printf("Task 1: Running...\n");
		printf("Param of Task 1: %d\n\n", *pa);
		*pa += 1;
		task_delay(DELAY);
	}
	printf("===================> user task 1 distoryed\n");
	return;
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void user_init()
{
	int * param1 = malloc(sizeof(int));
	*param1 = 1;
	int * param2 = malloc(sizeof(int));
	*param2 = 20;
	task_create(user_task0,param1,1,2);
	task_create(user_task1,param2,1,3);
}

