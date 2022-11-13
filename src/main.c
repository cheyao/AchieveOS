#include "types.h"
#include "keyboard.h"
#include "stdio.h"
#include "idt.h"
#include "time.h"

void mainC(void) {
    update_cursor(0);
    init_idt();

    printf("Hello formatted world!\n");
    printf("Percent: %5%\n");
    printf("Hello + %s\n", "world!");
    printf("H%c\n", 'i');
    printf("Five: %0d\n", 5);
    printf("Characters: %c %c \n", 'a', 65);
    printf("Decimals: %d %ld\n", 1977, 650000L);
    printf("Preceding with blanks: %10d \n", 1977);
    printf("Preceding with zeros: %010d \n", 1977);
    printf("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    printf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    printf("Width trick: %*d \n", 5, 10);
    printf("%s \n", "A string");
}

void tickC(void) {
    ticks++;
}