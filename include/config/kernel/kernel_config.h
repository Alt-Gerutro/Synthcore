#ifndef _CONFIG_SYNTHCORE_H
#define _CONFIG_SYNTHCORE_H

#define MULTIBOOT_MAGIC     0x1BADB002
#define MULTIBOOT_FLAGS     0b00000011
#define MULTIBOOT_CHECKSUM  -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((noreturn, used))
void kpanic();

#endif // _CONFIG_SYNTHCORE_H