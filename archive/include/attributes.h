#ifndef _KERNEL_ATTRIBUTES_H_
#define _KERNEL_ATTRIBUTES_H_

#ifdef _MSC_VER
#error "\033[91;40mSTOP! Uninstall msvc and get clang now!"
#endif

#if __STDC_VERSION__ > 201710L
// Finally C23 made this!
#define USED                [[gnu::used]]
#define DEPRECATED(reason)  [[deprecated(reason)]]
#define NO_RETURN           [[noreturn]]
#define PRINTF_FORMAT(f, a) [[gnu::format(__printf__, f, a)]]
#define SCANF_FORMAT(f, a)  [[gnu::format(__scanf__, f, a)]]
#else
#warning "Please upgrade to C23!"
#define USED                __attribute__((used))
#define DEPRECATED(reason)  __attribute__((deprecated(reason)))
#define NO_RETURN           _Noreturn
#define PRINTF_FORMAT(f, a) __attribute__((format(__printf__, f, a)))
#define SCANF_FORMAT(f, a)  __attribute__((format(__scanf__, f, a)))
#endif

// This line might cause problems with non-clang/gnu compilers
#define BUILTIN_ALLOC(n) __builtin_alloca(n)

#endif
