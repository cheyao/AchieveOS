//
// Created by cheyao on 15.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _FS_H
#define _FS_H

#include "types.h"

void read(u16int io, u16int control);

#define BUS_PRIMARY 0x1F0
#define BUS_SECONDARY 0x170
#define CONTROL_PRIMARY 0x3f0
#define CONTROL_SECONDARY 0x370

#define DR 0
#define ER 1
#define SCR 2
#define SNR 3
#define CLR 4
#define CHR 5
#define DS 6
#define CR 7

#define IDENTIFY_TABLE 0x7B00

#endif //_FS_H
