#include "os.h"
#include "types.h"

extern struct time now();

uint32_t _tick = 0;

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval){
    int hartId = r_mhartid();
    *((uint64_t*)CLINT_MTIMECMP(hartId)) = *(uint64_t*)CLINT_MTIME + (uint64_t)interval;
}

void timer_init(){
    /*
	 * On reset, mtime is cleared to zero, but the mtimecmp registers 
	 * are not reset. So we have to init the mtimecmp manually.
	 */
    timer_load(TIMER_INTERVAL);

    /* enable machine-mode timer interrupts. */
    w_mie(r_mie() | MIE_MTIE);
}

void timer_handler() 
{
    _tick++;
	// printf("tick: %d\n", _tick);
	timer_load(TIMER_INTERVAL);

    tick_schedule();
}