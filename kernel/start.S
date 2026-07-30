.section .text._start
.global _start
.type _start, @function
.type _syscall_start, @function
.type _syscall_exit, @function

.extern user_regs

_start:
    csrw sstatus, zero
    csrw sie, zero

    la t0, _syscall_start
    csrw stvec, t0
    
    la sp, __stack_top
    
    la t0, __bss_start
    la t1, __bss_end
    beq t0, t1, 2f
1:
    sw zero, 0(t0)
    addi t0, t0, 4
    bltu t0, t1, 1b
2:
    call kernel_main
    wfi

_syscall_start:
    csrw sscratch, x1
    la x1, user_regs

    sw x2, 4(x1)
    sw x3, 8(x1)
    sw x4, 12(x1)
    sw x5, 16(x1)
    sw x6, 20(x1)
    sw x7, 24(x1)
    sw x8, 28(x1)
    sw x9, 32(x1)
    sw x10, 36(x1)
    sw x11, 40(x1)
    sw x12, 44(x1)
    sw x13, 48(x1)
    sw x14, 52(x1)
    sw x15, 56(x1)
    sw x16, 60(x1)
    sw x17, 64(x1)
    sw x18, 68(x1)
    sw x19, 72(x1)
    sw x20, 76(x1)
    sw x21, 80(x1)
    sw x22, 84(x1)
    sw x23, 88(x1)
    sw x24, 92(x1)
    sw x25, 96(x1)
    sw x26, 100(x1)
    sw x27, 104(x1)
    sw x28, 108(x1)
    sw x29, 112(x1)
    sw x30, 116(x1)
    sw x31, 120(x1)

    csrr t0, sscratch
    sw t0, 0(x1)
    
    jal kernel_trap

_syscall_exit:
    csrw sscratch, x1
    la x1, user_regs

    lw x2, 4(x1)
    lw x3, 8(x1)
    lw x4, 12(x1)
    lw x5, 16(x1)
    lw x6, 20(x1)
    lw x7, 24(x1)
    lw x8, 28(x1)
    lw x9, 32(x1)
    lw x10, 36(x1)
    lw x11, 40(x1)
    lw x12, 44(x1)
    lw x13, 48(x1)
    lw x14, 52(x1)
    lw x15, 56(x1)
    lw x16, 60(x1)
    lw x17, 64(x1)
    lw x18, 68(x1)
    lw x19, 72(x1)
    lw x20, 76(x1)
    lw x21, 80(x1)
    lw x22, 84(x1)
    lw x23, 88(x1)
    lw x24, 92(x1)
    lw x25, 96(x1)
    lw x26, 100(x1)
    lw x27, 104(x1)
    lw x28, 108(x1)
    lw x29, 112(x1)
    lw x30, 116(x1)
    lw x31, 120(x1)

    csrr x1, sscratch

    sret
