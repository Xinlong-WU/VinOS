#ifndef __UART_H__
#define __UART_H__

#include "types.h"

/* uart */
extern void uart_init();
extern int putc(char ch);
extern void puts(string s);
extern void print(string s);

extern char getc();
extern void getLine(string strBuffer, int bufferSize);

/* printf */
extern int  printf(const string s, ...);
extern void panic(string s);

#endif /* __UART_H__ */