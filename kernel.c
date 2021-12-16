#include <stddef.h>

#include "os.h"

extern char* timefmt(struct time T);
extern struct time now();
extern void task_os();

void start_kernel(void)
{
	uart_init();
	print("-> init uart OK");

	print("=========================================================================================");
	print("Author: Vincent");
	print("Github: https://github.com/Xinlong-WU/VinOS");
	print("License: BSD-2-Clause License");
	print("Date: 2021/11/25");
	printf("Version: %s\n", KERNEL_VERSION);
	print("=========================================================================================");
	print("                                 ___           ___           ___     					");
	print("      ___            ___        /  /\\         /  /\\         /  /\\    					");
	print("     /  /\\          /__/\\      /  /::|       /  /::\\       /  /::\\   				");
	print("    /  /:/          \\__\\:\\    /  /:|:|      /  /:/\\:\\     /__/:/\\:\\  				");
	print("   /  /:/           /  /::\\  /  /:/|:|__   /  /:/  \\:\\   _\\_ \\:\\ \\:\\ 			");
	print("  /__/:/  ___    __/  /:/\\/ /__/:/ |:| /\\ /__/:/ \\__\\:\\ /__/\\ \\:\\ \\:\\			");
	print("  |  |:| /  /\\  /__/\\/:/~~  \\__\\/  |:|/:/ \\  \\:\\ /  /:/ \\  \\:\\ \\:\\_\\/		");
	print("  |  |:|/  /:/  \\  \\::/         |  |:/:/   \\  \\:\\  /:/   \\  \\:\\_\\:\\  			");
	print("  |__|:|__/:/    \\  \\:\\         |__|::/     \\  \\:\\/:/     \\  \\:\\/:/  			");
	print("   \\__\\::::/      \\__\\/         /__/:/       \\  \\::/       \\  \\::/   			");
	print("       ~~~~                     \\__\\/         \\__\\/         \\__\\/    				");

	print("-> init Memoey...");
	page_init();
	page_test();
	molloc_test();
	print("-> init Memoey...OK");

	print("-> init Multi Task...");
	sched_init();
	print("-> init Memoey...OK");

	print("-> init Tarp...");
	trap_init();
	print("-> init Tarp...OK");

	print("-> init plic...");
	plic_init();
	print("-> init plic...OK");

	print("-> init Timer...");
	timer_init();
	print("-> init Timer...OK");

	print("-> Now goto Kernel...");
	task_os();
}

extern int getTaskCounter();
void kernel(){
	user_init();
	
	while(1){
		printf("-> OS: Activate next task\n");
		task_yield();
		printf("-> OS: Back to os now\n\n");
		if(getTaskCounter() == 1){
			printf("ERROR: there is no more task\n");
			break;
		}
	}

	printf("Show local time:\n");
	while (1)
	{
		struct time T = now();
		printf("\r");
		printf("%s", timefmt(T));
		
		task_delay(10000);
	}
	
}