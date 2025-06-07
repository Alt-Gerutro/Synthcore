include build.mk

DISK = synthcore.iso
OBJS = $(filter-out kernel_entry.o, $(wildcard temp/*.o))

all: clean
	mkdir -p temp
	cd ../src && make
	make $(DISK)

$(DISK): temp/bootloader.bin temp/kernel.bin
	dd if=/dev/zero of=$@ bs=1M count=64 status=none
	parted -s $@ mklabel msdos
	parted -s $@ mkpart primary ext4 1 100%

	dd if=temp/bootloader.bin of=$@ conv=notrunc

	dd if=temp/kernel.bin of=$@ seek=1 conv=notrunc

	sudo losetup -fP $@
	LOOP=$$(losetup -j $@ | cut -d: -f1); \
	sudo mkfs.ext4 -L OS_ROOT $${LOOP}p1; \
	sudo mount $${LOOP}p1 /mnt; \
	sudo mkdir -p /mnt/bin; \
	sudo umount /mnt; \
	sudo losetup -d $${LOOP}

	echo -ne '\x55\xaa' | dd of=$@ bs=1 seek=510 conv=notrunc

temp/kernel.bin: $(OBJS)
	$(LD) -o $@ -Ttext 0x1000 temp/kernel_entry.o $(OBJS) --oformat binary

run: all
	$(QEMU_CMD) -drive file=$(DISK),format=raw

debug: all
	$(QEMU_CMD) -drive file=$(DISK),format=raw -s -S &
	$(TERM) $(TERM_COMMANDS) &

clean:
	cd ../src && make clean
	rm -rf temp disk $(DISK)
	clear

.SILENT: clean
.PHONY: clean run
