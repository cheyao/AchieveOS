ENTRY(_start)

SECTIONS {
	. = 0x8000;
	phys = .;

	.start ALIGN(0x1000): {
		code = .;
		*(.start)
	}

	.text : {
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