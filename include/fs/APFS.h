//
// Created by cheyao on 20.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef ASMOS_APFS_H
#define ASMOS_APFS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint-gcc.h>

typedef int64_t paddr_t; // A physical address of an on-disk block.

// A range of physical addresses.
struct prange {
	paddr_t pr_start_paddr;
	uint64_t pr_block_count;
};
typedef struct prange prange_t;
typedef paddr_t pr_start_paddr;
typedef uint64_t pr_block_count;
typedef unsigned char uuid_t[16];

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //ASMOS_APFS_H
