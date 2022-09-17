#include "idt.h"
#include "keyboard.h"
#include "screen.h"

__attribute__((unused)) unsigned char *VIDEO_BUFFER;

void main() {
    VIDEO_BUFFER = *((unsigned char**) 0x7FFF0);
    init_idt();
    init_keyboard();
    init_pallet();
}
