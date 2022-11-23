//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _CD_H
#define _CD_H

#include <kernel/ata.h>

void atapi(Disk *d);

void read_cdrom(Disk *d, uint32_t lba, uint32_t sectors);

#endif  //_CD_H
