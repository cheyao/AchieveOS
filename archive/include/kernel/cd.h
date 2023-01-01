//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _CD_H
#define _CD_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <kernel/ata.h>

void identify(Disk *d);

void read_cdrom(Disk *d, uint32_t lba, uint32_t sectors, uint16_t *buffer);

void ls(void);

void cd(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  //_CD_H
