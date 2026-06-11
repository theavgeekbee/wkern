.section .text._start
.global _start
.type _start, @function

_start:
    # csrw mstatus, zero
    # csrw mie, zero
    
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
    j .
