//
// Created by cheyao on 18.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _CD_H
#define _CD_H

#include <kernel/ata.h>

void atapi(Disk d);

Disk makedisk(uint16_t port, uint8_t ds);

#endif //_CD_H
