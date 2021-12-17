#ifndef __TYPES_H__
#define __TYPES_H__
#include <stddef.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned long long uint64_t;
typedef char* string;

/*
 * RISCV32: register is 32bits width
 */ 
#ifdef RV32
typedef uint32_t reg_t;
#else
typedef uint64_t reg_t;
#endif

#endif /* __TYPES_H__ */