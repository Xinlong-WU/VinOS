#ifndef __TRAP_H__
#define __TRAP_H__

extern void trap_init();
extern void trap_test();

enum Interrupt_type{
    U_SoftWare = 0,
    S_SoftWare,
    M_SoftWare = 3,
    U_Timer,
    S_Timer,
    M_Timer = 7,
    U_External,
    S_External,
    M_External = 11,
};

enum Exception_type{
    Inst_addr_misaligned = 0,
    Inst_acc_Fault,
    Illegal_inst,
    Break_point,
    Load_addr_misaligned,
    Load_acc_Fault,
    Store_addr_misaligned,
    Store_acc_Fault,
};

#endif //__TRAP_H__