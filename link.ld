ENTRY(_start)
OUTPUT_FORMAT(binary)

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
  PROVIDE(disk_start = . - 0x7E00);
}