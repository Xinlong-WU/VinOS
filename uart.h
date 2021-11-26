#include "types.h"

#ifndef __UART_H__
#define __UART_H__

int putc(char ch);
void puts(string s);
void print(string s);
void printf_str(string s, string* param, int parac);

char getc();

#endif /* __UART_H__ */