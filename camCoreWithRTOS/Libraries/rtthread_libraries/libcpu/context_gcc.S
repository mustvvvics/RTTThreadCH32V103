/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018/10/28     Bernard      The unify RISC-V porting implementation
 * 2018/12/27     Jesven       Add SMP support
 */

#include "cpuport.h"
#include "riscv_encoding.h"



#define RT_SAVED_REGNUM         32
#define RT_CONTEXT_SIZE         (RT_SAVED_REGNUM * REGBYTES)

    .extern rt_interrupt_from_thread
    .extern rt_interrupt_to_thread

#ifdef RT_USING_SMP
#define rt_hw_interrupt_disable rt_hw_local_irq_disable
#define rt_hw_interrupt_enable  rt_hw_local_irq_enable
#endif

/*
 * rt_base_t rt_hw_interrupt_disable(void);
 */
    .globl rt_hw_interrupt_disable
rt_hw_interrupt_disable:
    csrrci a0, mstatus, 8
    ret

/*
 * void rt_hw_interrupt_enable(rt_base_t level);
 */
    .globl rt_hw_interrupt_enable
rt_hw_interrupt_enable:
    csrw mstatus, a0
    ret


.globl rt_hw_context_switch_to

/* Start the first task.  This also clears the bit that indicates the FPU is
	in use in case the FPU was used before the scheduler was started - which
	would otherwise result in the unnecessary leaving of space in the stack
	for lazy saving of FPU registers. */
.align 2
rt_hw_context_switch_to:
    /* Setup Interrupt Stack using
       The stack that was used by main()
       before the scheduler is started is
       no longer required after the scheduler is started.
       Interrupt stack pointer is stored in CSR_MSCRATCH */
    la t0, _eusrstack
    csrw CSR_MSCRATCH, t0
    LOAD sp, 0x0(a0)                /* Read sp from first TCB member(a0) */

    j rt_hw_context_switch_exit


    .align 2
.global SW_handler
SW_handler:
    addi sp, sp, -RT_CONTEXT_SIZE
    STORE x1,  1  * REGBYTES(sp)    /* RA */

    STORE x4,  4  * REGBYTES(sp)
    STORE x5,  5  * REGBYTES(sp)
    STORE x6,  6  * REGBYTES(sp)
    STORE x7,  7  * REGBYTES(sp)
    STORE x8,  8  * REGBYTES(sp)
    STORE x9,  9  * REGBYTES(sp)
    STORE x10, 10 * REGBYTES(sp)
    STORE x11, 11 * REGBYTES(sp)
    STORE x12, 12 * REGBYTES(sp)
    STORE x13, 13 * REGBYTES(sp)
    STORE x14, 14 * REGBYTES(sp)
    STORE x15, 15 * REGBYTES(sp)
    STORE x16, 16 * REGBYTES(sp)
    STORE x17, 17 * REGBYTES(sp)
    STORE x18, 18 * REGBYTES(sp)
    STORE x19, 19 * REGBYTES(sp)
    STORE x20, 20 * REGBYTES(sp)
    STORE x21, 21 * REGBYTES(sp)
    STORE x22, 22 * REGBYTES(sp)
    STORE x23, 23 * REGBYTES(sp)
    STORE x24, 24 * REGBYTES(sp)
    STORE x25, 25 * REGBYTES(sp)
    STORE x26, 26 * REGBYTES(sp)
    STORE x27, 27 * REGBYTES(sp)
    STORE x28, 28 * REGBYTES(sp)
    STORE x29, 29 * REGBYTES(sp)
    STORE x30, 30 * REGBYTES(sp)
    STORE x31, 31 * REGBYTES(sp)

    /* Push mstatus to stack */
    csrr t0, CSR_MSTATUS
    STORE t0,  2  * REGBYTES(sp)

    /* Push additional registers */

    /* Store sp to task stack */
    LOAD t0, rt_interrupt_from_thread
    STORE sp, 0(t0)

    csrr t0, CSR_MEPC
    STORE t0, 0(sp)

    jal rt_hw_taskswitch

    /* Switch task context */
    LOAD t0, rt_interrupt_to_thread
    LOAD sp, 0x0(t0)

	j rt_hw_context_switch_exit



.align 2
    .global rt_hw_context_switch_exit
rt_hw_context_switch_exit:

	LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    /* Pop mstatus from stack and set it */
    LOAD t0,  2  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    LOAD x1,  1  * REGBYTES(sp)    /* RA */

    LOAD x4,  4  * REGBYTES(sp)
    LOAD x5,  5  * REGBYTES(sp)
    LOAD x6,  6  * REGBYTES(sp)
    LOAD x7,  7  * REGBYTES(sp)
    LOAD x8,  8  * REGBYTES(sp)
    LOAD x9,  9  * REGBYTES(sp)
    LOAD x10, 10 * REGBYTES(sp)
    LOAD x11, 11 * REGBYTES(sp)
    LOAD x12, 12 * REGBYTES(sp)
    LOAD x13, 13 * REGBYTES(sp)
    LOAD x14, 14 * REGBYTES(sp)
    LOAD x15, 15 * REGBYTES(sp)
    LOAD x16, 16 * REGBYTES(sp)
    LOAD x17, 17 * REGBYTES(sp)
    LOAD x18, 18 * REGBYTES(sp)
    LOAD x19, 19 * REGBYTES(sp)
    LOAD x20, 20 * REGBYTES(sp)
    LOAD x21, 21 * REGBYTES(sp)
    LOAD x22, 22 * REGBYTES(sp)
    LOAD x23, 23 * REGBYTES(sp)
    LOAD x24, 24 * REGBYTES(sp)
    LOAD x25, 25 * REGBYTES(sp)
    LOAD x26, 26 * REGBYTES(sp)
    LOAD x27, 27 * REGBYTES(sp)
    LOAD x28, 28 * REGBYTES(sp)
    LOAD x29, 29 * REGBYTES(sp)
    LOAD x30, 30 * REGBYTES(sp)
    LOAD x31, 31 * REGBYTES(sp)

    addi sp, sp, RT_CONTEXT_SIZE

    mret
