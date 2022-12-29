#include <kernel/ata.h>  // for OTHER, UNKNOWN, BUS_PRIMARY, BUS_SECONDARY
#include <kernel/idt.h>  // for Vector2, rgb, circle, line, quadratic_bez...
#include <stdbool.h>     // for false
#include <stdint.h>      // for uint16_t, uint8_t
#include <stdio.h>       // for memset
#include <kernel/memory.h>
#include <assert.h>
#include <stdlib.h>
#include <kernel/screen.h>

typedef struct SMAP_entry {
	uint64_t Base;
	uint64_t Length;
	uint32_t Type;  // entry Type
	uint32_t ACPI;  // extended
} __attribute__((packed)) SMAP_entry_t;

static const char *restrict types[] = {
		"Error! (0)",
		"Available memory (1)",
		"Reserved memory (2)",
		"ACPI reclaim memory (3)",
		"ACPI NVS memory (4)",
};

void main(void) {
	assert(0);
	init_idt();
	init_memory();

	line((Vector2) {0, 0}, (Vector2) {WIDTH, HEIGHT}, 0xFFFF);

	// ____SMAP_entry_t *entry = (____SMAP_entry_t *) 0x2004;

	// puts("| Base             |Length            |Type                   |");
	// puts("|------------------|------------------|-----------------------|");
	// for (int i = 0; i < *((uint16_t *) 0x2000) - 1; i++) {
	// 	printf("|%#018lx|%#018lx|%-23s|\n", entry->Base, entry->Length, _types[entry->Type > 4 ? 2 : entry->Type]);
	// 	entry++;
	// }
}
