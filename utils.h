#ifndef __UART_H__
#define __UART_H__

#include "types.h"

extern string itoa(int num,string arr);
extern int strcpy(string str1,string str2);
extern struct time now();
extern char* timefmt(struct time T);

#endif /* __UART_H__ */