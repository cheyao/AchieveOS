ENTRY(main)

SECTIONS {
    . = 0x7E00;
  .text : { *(.text)
            *(.text.*) }
  .rodata : {
          *(.rodata)
          *(.rodata.*) }
  .data : { *(.data)
            *(.data.*) }
  .bss : { *(.bss)
           *(COMMON)
           *(.bss.*) }
}