cmake_minimum_required(VERSION 3.20)
project(SynthCore C ASM)

enable_language(C ASM)

# Build configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile process configuration
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_LINKER ld.lld)

# Flags
set(CMAKE_C_FLAGS "-target i386-pc-none-elf -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -m32 -O2 -Wall -Wextra -Wno-unused-parameter -fno-function-sections -fno-data-sections")
set(CMAKE_ASM_FLAGS "-target i386-pc-none-elf -ffreestanding -nostdlib -fno-pic -m32")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -T${CMAKE_SOURCE_DIR}/include/config/kernel/linker.ld")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

add_custom_target(iso ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/boot/grub
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel.elf ${CMAKE_BINARY_DIR}/iso_root/boot
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/include/config/grub/grub.cfg ${CMAKE_BINARY_DIR}/iso_root/boot/grub
    COMMAND grub-mkrescue -o "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso" "iso_root/" -- -volid "SYNTHCORE"
    DEPENDS kernel.elf
    COMMENT "Generating bootable ISO image..."
)

find_program(QEMU qemu-system-x86_64)
if(QEMU)
    add_custom_target(run
        COMMAND ${QEMU}
            -cdrom "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso"
            -machine q35
            -m 2048
            -serial stdio
            -no-reboot
            "${QEMU_EXTRA_ARGS}"
        DEPENDS iso
        COMMENT "Running SynthCore in QEMU..."
    )
else()
    message(WARNING "qemu x86_64 not found - `run` target disabled")
endif()