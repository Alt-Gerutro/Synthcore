cmake_minimum_required(VERSION 3.20)
project(SynthCore C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_LINKER ld.lld)

set(CMAKE_C_FLAGS "-target i386-pc-none-elf -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-PIC -fno-PIE -nostdlib -O0 -Wall -Wextra -Wno-unused-command-line-argument -gdwarf-4 -g")
set(LINKER_FLAGS "-fuse-ld=lld -T${CMAKE_SOURCE_DIR}/include/config/kernel/linker.ld -nostdlib -static -z max-page-size=0x1000 -g")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/kernel)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers)

add_custom_target(iso ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/boot/grub
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel.elf> iso_root/boot
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/include/config/grub/grub.cfg iso_root/boot/grub
    COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso iso_root
    DEPENDS kernel.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building .iso file"
)

find_program(QEMU qemu-system-x86_64)

add_custom_target(run
    COMMAND ${QEMU}
        -cdrom ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso
        -no-reboot
    DEPENDS iso
    COMMENT "Starting ReSynthCore QEMU"
)

add_custom_target(run_debug
    COMMAND ${QEMU}
        -cdrom ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso
        -no-reboot
        -no-shutdown
        -serial file:serial.log
        -d int,cpu_reset
    DEPENDS iso
    COMMENT "Starting ReSynthCore QEMU in DEBUG MODE"
)
