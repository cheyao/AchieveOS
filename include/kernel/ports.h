//
// Created by cheyao on 08.10.2022.
// Huge thanks to SOSO for the code :)
// https://github.com/ozkl/soso
//

#ifndef _PORTS_H
#define _PORTS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

static __inline__ void outb(uint16_t __port, uint8_t __val) {
	__asm__ __volatile__("outb %0,%1"::"a"(__val), "dN"(__port));
}

static __inline__ void outw(uint16_t __port, uint16_t __val) {
	__asm__ __volatile__("outw %0,%1"
			:
			: "a"(__val), "dN"(__port));
}

static __inline__ void outl(uint16_t __port, uint32_t __val) {
	__asm__ __volatile__("outl %0,%1"
			:
			: "a"(__val), "dN"(__port));
}

static __inline__ uint8_t inb(uint16_t __port) {
	uint8_t __val;
	__asm__ __volatile__("inb %1,%0"
			: "=a"(__val)
			: "dN"(__port));
	return __val;
}

static __inline__ uint16_t inw(uint16_t __port) {
	uint16_t __val;
	__asm__ __volatile__("inw %1,%0"
			: "=a"(__val)
			: "dN"(__port));
	return __val;
}

static __inline__ uint32_t inl(uint16_t __port) {
	uint32_t __val;
	__asm__ __volatile__("inl %1,%0"
			: "=a"(__val)
			: "dN"(__port));
	return __val;
}

static __inline__ void outsb(uint16_t __port, const void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; outsb"
			: "+S"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline__ void outsw(uint16_t __port, const void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; outsw"
			: "+S"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline void outsl(uint16_t __port, const void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; outsl"
			: "+S"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline__ void insb(uint16_t __port, void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; insb"
			: "+D"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline void insw(uint16_t __port, void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; insw"
			: "+D"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline void insl(uint16_t __port, void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; insl"
			: "+D"(__buf), "+c"(__n)
			: "d"(__port));
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif  // _PORTS_H
