#ifndef _IO_BYTE_SYNTHCORE
#define _IO_BYTE_SYNTHCORE

#include <utils/types.h>

static inline u8 inB(u16 port) {
    u8 v;
    __asm__ volatile("inb %1, %0"
                : "=a"(v)      // выход в AL
                : "Nd"(port)   // порт в DX
                );
    return v;
}

static inline u16 inW(u16 port) {
    u16 v;
    __asm__ volatile("inw %1, %0"
                : "=a"(v)       // выход в AL
                : "Nd"(port)    // порт в DX
                );
    return v;
}

static inline u32 inDW(u16 port) {
    u32 v;
    __asm__ volatile("inw %1, %0"
                : "=a"(v)       // выход в AL
                : "Nd"(port)    // порт в DX
                );
    return v;
}

// OUT

static inline void outB(u16 port, u8 byte) {
    __asm__ volatile("outb %0, %1"
                :               // Без входных операндов
                : "a"(byte),    // Входной операнд значения
                "Nd"(port)      // Входной операнд порта
                );
}

static inline void outW(u16 port, u16 word) {
    __asm__ volatile("outw %0, %1"
                :               // Без входных операндов
                : "a"(word),    // Входной операнд значения
                "Nd"(port)      // Входной операнд порта
                );
}

static inline void outDW(u16 port, u32 dword) {
    __asm__ volatile("outl %0, %1"
                :               // Без входных операндов
                : "a"(dword),   // Входной операнд значения
                "Nd"(port)      // Входной операнд порта
                );
}

static inline void mem_copy(u8 *src, u8 *dest, u32 bytes) {
    u32 i = 0;
    while (i < bytes) {
        dest[i] = src[i];
        i++;
    }
}

#endif // _IO_BYTE_SYNTHCORE
