OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ROM   (rx)  : ORIGIN = 0x80000000, LENGTH = 1M
    RAM   (rwx) : ORIGIN = 0x81000000, LENGTH = 1M
    LOGRAM (rw) : ORIGIN = 0x82000000, LENGTH = 1M
}

SECTIONS
{
    .text : {
        KEEP(*(.text._start))
        KEEP(*(.text.start))
        *(.text)
        *(.text.*)
    } > ROM

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > ROM

    .init_array : {
        __init_array_start = .;
        KEEP(*(.init_array))
        KEEP(*(.init_array.*))
        __init_array_end = .;
    } > ROM

    .data : {
        __data_start = .;
        *(.data)
        *(.data.*)
        __data_end = .;
    } > RAM AT> ROM
    __data_load = LOADADDR(.data);

    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) : {
        . = ALIGN(16);
        __stack_bottom = .;
        . += 8K;
        __stack_top = .;
    } > RAM

    .ringbuf (NOLOAD) : ALIGN(64)
    {
        __ringbuf_start = .;
        KEEP(*(.ringbuf))
        . = ALIGN(64);
        __ringbuf_end = .;
    } > LOGRAM

    /DISCARD/ : {
        *(.eh_frame)
        *(.eh_frame.*)
        *(.eh_frame_hdr)
        *(.comment)
        *(.note.*)
        *(.debug_*)
        *(.stab*)
        *(.llvm_*)
        *(.rela.*)
    }
    _end = .;
}
