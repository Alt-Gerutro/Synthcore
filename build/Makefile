include build.mk

DISK = synthcore.img
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
