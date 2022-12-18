ENTRY(_start)

SECTIONS { /* 0xC0000000 */
	. = 0xC0000000;
	phys = .;

	.text ALIGN(0x1000) : {
		code = .;
		*(.text)
		. = ALIGN(0x1000);
	}

	.data ALIGN(0x1000): {
		data = .;
		*(.data)
		. = ALIGN(0x1000);
	}

	.rodata ALIGN(0x1000): {
		*(.rodata*)
		. = ALIGN(0x1000);
	}

	.bss ALIGN(0x1000): {
		bss = .;
		*(.bss)
		. = ALIGN(0x1000);
	}

	end = .; _end = .; __end = .;

	/*DISCARD/ : {
		*(.comment)
		*(.note.gnu.build-id)
		*(.shstrtab)
		*(.strtab)
		*(.symtab)
		*(.shstrtab)
	}*/
} 