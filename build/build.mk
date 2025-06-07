TERM = alacritty
TERM_COMMANDS = -e "gdb"

CC = i386-elf-gcc
LD = i386-elf-ld
ASMC = yasm

CFLAGS = -Wall -Wextra -ffreestanding -fno-pie

QEMU_CMD = qemu-system-i386
