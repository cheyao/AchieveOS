ENTRY(_start)

SECTIONS {
	. = 0; /* Achieve-BIOS is gonna load the OS into addr 0 */

	.text ALIGN(0x8) : {
		*(.text)
		. = ALIGN(0x8);
	}

	.data ALIGN(0x8): {
		*(.data)
		. = ALIGN(0x8);
	}

	.rodata ALIGN(0x8): {
		*(.rodata*)
		. = ALIGN(0x8);
	}

	.bss ALIGN(0x8): {
		*(.bss)
		. = ALIGN(0x8);
	}
} 
