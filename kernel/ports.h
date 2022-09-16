//
// Created by cheyao on 13.09.2022.
//

#ifndef PACMAN_PORTS_H
#define PACMAN_PORTS_H

#include "types.h"

unsigned char inb (u16int port);
void outb (u16int port, u8int data);

#endif //PACMAN_PORTS_H
