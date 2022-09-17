ENTRY(main)
OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x1000;
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