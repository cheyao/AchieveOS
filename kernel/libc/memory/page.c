//
// Created by cheyao on 22.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define PAGE_LOCATION ((size_t)0x600000);

static inline void setBits(uint64_t *A, uint64_t k, uint64_t amount) {
	for (uint64_t i = 0; i < amount; i++)
		A[(k) / sizeof(A[0])] |= (1 << ((k) % sizeof(A[0])));
}

static inline void clearBits(uint64_t *A, uint64_t k, uint64_t amount) {
	for (uint64_t i = 0; i < amount; i++)
		A[(k) / sizeof(A[0])] &= ~(1 << ((k) % sizeof(A[0])));
}

static inline int testBits(const uint64_t *A, uint64_t k, uint64_t amount) {
	for (uint64_t i = 0; i < amount; i++)
		if ((bool) (A[(k) / sizeof(A[0])] & (1 << ((k) % sizeof(A[0])))))
			return 1;

	return 0;
}

#define PAGE_SIZE 4096

typedef struct SMAP_entry {
	uint64_t Base;
	uint64_t Length;
	uint32_t Type;  // entry Type
	uint32_t ACPI;  // extended
} __attribute__((packed)) entry_t;

uint64_t *buddy = (uint64_t *) 0xA00000;
size_t memory_start = 0;
uint64_t entries = 0;
uint64_t last_search = 0;

void init_memory(void) {
	uint16_t entry_num = *((uint16_t *) 0x2000) - 1;
	entry_t *entry = (entry_t *) 0x2004;
	size_t max_addr = 0;

	for (int i = 0; i < entry_num; i++) {
		// 128 Mib is allocated for the kernel
		if (entry->Type == 1 && entry->Base + entry->Length >= 0x8000000 && (void *) memory_start == NULL)
			memory_start = (entry->Base < 0x8000000 ? 0x8000000 : entry->Base);

		if (entry->Base + entry->Length > max_addr)
			max_addr = entry->Base + entry->Length;

		entry++;
	}

	if (max_addr < 512 * 1024 * 1024)
		error("Error! Not enough memory!\nYou have %dMib of memory, but we need at least 256Mib! (Maybe less but I hardcoded this :D)",
		      max_addr / 1024 / 1024);

	*((size_t *) (0x602000 + 8 * 5)) = (size_t) memory_start + 0x83;
	*((size_t *) (0x602000 + 8 * 6)) = (size_t) memory_start + 0x200000 + 0x83;

	buddy = (uint64_t *) ((size_t) buddy + sizeof(buddy) * 8);

	entries = max_addr / 2097152;

	entry = (entry_t *) 0x2004;
	for (int i = 0; i < entry_num; i++) {
		if (entry->Type != 1 && entry->Base >= memory_start) {
			setBits(buddy, (entry->Base - memory_start) / 2097152, entry->Length / 2097152);
		}

		entry++;
	}
}

void *alloc_page(size_t pages) {
	if (pages == 0)
		return NULL;

	uint64_t i = last_search;
	while (i <= entries - pages && testBits(buddy, i, pages))
		i++;

	if (i > entries - pages) {
		i = 0;
		while (i <= last_search && testBits(buddy, i, pages))
			i++;

		if (i <= last_search)
			goto _else;

		error("Out of memory!");
		return NULL;
	} else {
		_else:
		printf("Allocated %luk page (%#llx - %#llx)", pages * 4, (memory_start + i * PAGE_SIZE),
		       (memory_start + i * PAGE_SIZE + pages * PAGE_SIZE));
		setBits(buddy, i, pages);
		return (void *) (memory_start + i * PAGE_SIZE);
	}
}

void free_page(void *start, size_t pages) {
	printf("Freed %luk page at %#lx", pages * 4, (size_t) start);
	clearBits(buddy, ((size_t) start - memory_start) / PAGE_SIZE / 64, pages);
}
