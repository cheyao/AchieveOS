ENTRY(_start)

SECTIONS {
	. = 0xC0000000;

	kernel_start = .;

	.text ALIGN(0x1000) : {
		*(.text)
		. = ALIGN(0x1000);
	}

	.data ALIGN(0x1000): {
		*(.data)
		. = ALIGN(0x1000);
	}

	.rodata ALIGN(0x1000): {
		*(.rodata*)
		. = ALIGN(0x1000);
	}

	.bss ALIGN(0x1000): {
		*(.bss)
		. = ALIGN(0x1000);
	}

	kernel_end = .;

	/*DISCARD/ : {
		*(.comment)
		*(.note.gnu.build-id)
	}*/
} 