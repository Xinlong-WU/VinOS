#ifndef __TIMER_H__
#define __TIMER_H__

#include "platform.h"
#include "types.h"

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

extern uint32_t _tick;

struct time{
    int hour;
    int minit;
    int second;
};


extern void timer_init();
extern void timer_load(int interval);
extern void timer_handler();

#endif //__TIMER_H__