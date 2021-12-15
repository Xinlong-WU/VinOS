#ifndef __PLIC_H__
#define __PLIC_H__

extern void plic_init();
extern int plic_claim();
extern void plic_complete(int irq);

#endif // __PLIC_H__