#ifndef __PORTS_H
#define __PORTS_H 1

#include <stdint.h>

static inline void outd(uint32_t port, uint64_t val) {
    __asm__ __volatile__("sd %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outw(uint32_t port, uint32_t val) {
    __asm__ __volatile__("sw %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outh(uint32_t port, uint16_t val) {
    __asm__ __volatile__("sh %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outb(uint32_t port, uint8_t val) {
    __asm__ __volatile__("sb %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline uint64_t ind(uint32_t port) {
    uint64_t data;
    __asm__ __volatile__("ld %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint32_t inw(uint32_t port) {
    uint32_t data;
    __asm__ __volatile__("lw %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint16_t inh(uint32_t port) {
    uint16_t data;
    __asm__ __volatile__("lh %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint8_t inb(uint32_t port) {
    uint16_t data;
    __asm__ __volatile__("lb %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

#define SD_DATA   0xC0000
#define SD_SEEK   0xC1000
#define UART      0xC1002
#define MEM_SIZE  0xC1003
#define DEBUG_INT 0xEFFFF

#endif
