#include "idt.h"
#include "keyboard.h"
#include "screen.h"

__attribute__((unused)) unsigned char *VIDEO_BUFFER;

void main() {
    VIDEO_BUFFER = *((unsigned char**) 0x7FFF0);
    VIDEO_BUFFER[0] = 0x12;
    VIDEO_BUFFER[1] = 0x12;
    init_idt();
    init_keyboard();
    // init_pallet();
}
