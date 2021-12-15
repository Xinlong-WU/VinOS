#include <stddef.h>

#include "os.h"

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

	print("-> Now goto Kernel...");
	task_os();
}

void kernel(){
	user_init();
	
	while(1){
		printf("-> OS: Activate next task\n");
		schedule();
		printf("-> OS: Back to os now\n\n");
	}
}