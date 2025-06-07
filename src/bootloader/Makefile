include ../../build/build.mk

TEMP_DIR = ../../build/temp

BOOTLOADER_TARGET = $(TEMP_DIR)/bootloader.bin
KERNEL_TARGET = $(TEMP_DIR)/kernel_entry.o

all: $(BOOTLOADER_TARGET) $(KERNEL_TARGET)

$(BOOTLOADER_TARGET):
	$(ASMC) -f bin bootsect.asm -o $(BOOTLOADER_TARGET)

$(KERNEL_TARGET):
	$(ASMC) -f elf kernel_entry.asm -o $(KERNEL_TARGET)
