BOOTLOADER_DIR = bootloader
DRIVERS_DIR  = drivers
KERNEL_DIR = kernel

all:
	cd $(BOOTLOADER_DIR) && make
	cd $(DRIVERS_DIR) && make
	cd $(KERNEL_DIR) && make

clean:
	cd $(DRIVERS_DIR) && make clean
	cd $(KERNEL_DIR) && make clean
