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
	schedule();

	printf("void -> %d\n", sizeof(void));
	printf("char-> %d\n", sizeof(char));
	printf("uint8_t -> %d\n", sizeof(uint8_t));
	printf("short int-> %d\n", sizeof(short));
	printf("int -> %d\n", sizeof(int));
	printf("long long int-> %d\n", sizeof(long long int));
	printf("float-> %d\n", sizeof(float));
	printf("double-> %d\n", sizeof(double));
	printf("long double-> %d\n", sizeof(long double));
	
	// char buffer[200];
	while (1) {
		getc();
		// getLine(buffer, 200);
		// if(strcpy(buffer,"shutdown") == 0){
		// 	print("ok, shutdown.\nbye~");
		// 	break;
		// }
		// char ch = getc();
		// itoa(ch, buffer);
		// print(buffer);
	}; // stop here!
}