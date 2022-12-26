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

uint64_t *_buddy = (uint64_t *) 0xA00000;
size_t _memory_start = 0;
uint64_t _entries = 0;
uint64_t _last_search = 0;

void __init_memory(void) {
	uint16_t entry_num = *((uint16_t *) 0x2000) - 1;
	entry_t *entry = (entry_t *) 0x2004;
	size_t max_addr = 0;

	for (int i = 0; i < entry_num; i++) {
		// 128 Mib is allocated for the kernel
		if (entry->Type == 1 && entry->Base + entry->Length >= 0x8000000 && (void *) _memory_start == NULL)
			_memory_start = (entry->Base < 0x8000000 ? 0x8000000 : entry->Base);

		if (entry->Base + entry->Length > max_addr)
			max_addr = entry->Base + entry->Length;

		entry++;
	}

	if (max_addr < 512 * 1024 * 1024)
		_error("Error! Not enough memory!\nYou have %dMib of memory, but the kernel needs at least 256Mib!",
		       max_addr / 1024 / 1024);

	*((size_t *) (0x602000 + 8 * 5)) = (size_t) _memory_start + 0x83;
	*((size_t *) (0x602000 + 8 * 6)) = (size_t) _memory_start + 0x200000 + 0x83;

	_buddy = (uint64_t *) ((size_t) _buddy + sizeof(_buddy) * 8);

	_entries = max_addr / 4096;

	entry = (entry_t *) 0x2004;
	for (int i = 0; i < entry_num; i++) {
		if (entry->Type != 1 && entry->Base >= _memory_start) {
			setBits(_buddy, (entry->Base - _memory_start) / 4096, entry->Length / 4096);
		}

		entry++;
	}
}

void *__alloc_page(size_t pages) {
	if (pages == 0)
		return NULL;

	uint64_t i = _last_search;
	while (i <= _entries - pages && testBits(_buddy, i, pages))
		i++;

	if (i > _entries - pages) {
		i = 0;
		while (i <= _last_search && testBits(_buddy, i, pages))
			i++;

		if (i <= _last_search)
			goto _else;

		_error("Out of memory! ");
		return NULL;
	} else {
		_else:
		printf("Allocated %luk page (%#lx - %#lx)", pages * 4, (_memory_start + i * PAGE_SIZE),
		       (_memory_start + i * PAGE_SIZE + pages * PAGE_SIZE));
		setBits(_buddy, i, pages);
		return (void *) (_memory_start + i * PAGE_SIZE);
	}
}

void __free_page(void *start, size_t pages) {
	printf("Freed %luk page at %#lx", pages * 4, (size_t) start);
	clearBits(_buddy, ((size_t) start - _memory_start) / PAGE_SIZE / 64, pages);
}

void *__alloc_virt_page(size_t pages) {
	return NULL;
}

void __free_virt_page(void *start, size_t pages) {

}
