//
// Created by cheyao on 08.10.2022.
//

#ifndef PACMAN_PORTS_H
#define PACMAN_PORTS_H

unsigned char inb(unsigned short port);

void outb(unsigned short port, unsigned char data);

unsigned short inw(unsigned short port);

void outw(unsigned short port, unsigned short data);

#endif  // PACMAN_PORTS_H
