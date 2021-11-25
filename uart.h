#include "types.h"

#ifndef __UART_H__
#define __UART_H__

int putc(char ch);
void puts(array s);
void print(array s);
void printf_str(array s, array* param, int parac);

char getc();

#endif /* __UART_H__ */