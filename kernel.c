#include "kernel.h"
#include "uart.h"
#include "utils.h"
void start_kernel(void)
{
	print("=========================================================================================");
	print("Author: Vincent");
	print("Github: https://github.com/Xinlong-WU/VinOS");
	print("License: BSD-2-Clause License");
	print("Date: 2021/11/25");
	char *params[1] = {VERSION};
	printf_str("Version: %s", params, 1);
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
	
	char buffer[200];
	while (1) {
		getLine(buffer, 200);
		print(buffer);
		if(strcpy(buffer,"shutdown") == 0){
			print("ok, shutdown.\nbye~");
			break;
		}
		// char ch = getc();
		// itoa(ch, buffer);
		// print(buffer);
	}; // stop here!
}