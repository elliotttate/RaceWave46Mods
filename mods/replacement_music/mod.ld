OUTPUT_ARCH(mips)
SECTIONS {
    . = 0x80800000;
    .text : { *(.text) *(.text.*) }
    .rodata : { *(.rodata) *(.rodata.*) }
    .data : { *(.data) *(.data.*) }
    .bss (NOLOAD) : { *(.bss) *(.bss.*) *(COMMON) }
    /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.pdr) *(.comment) *(.note*) }
}
