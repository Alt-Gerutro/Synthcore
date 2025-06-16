cmake_minimum_required(VERSION 3.12)
project(SynthLoader)

find_program(ASMC yasm)
if(NOT ASMC)
    message(FATAL_ERROR "yasm compiler is not found.")
endif()

set(BOOTLOADER_SOURCES
    bootsect.asm
    disk_load.asm
    gdt.asm
    print.asm
    switch.asm
)

add_custom_command(
    OUTPUT synthloader
    COMMAND ${ASMC} -f bin ${CMAKE_CURRENT_SOURCE_DIR}/bootsect.asm -o ${CMAKE_BINARY_DIR}/synthloader
    DEPENDS ${BOOTLOADER_SOURCES}
    COMMENT "Building a SynthLoader"
)

add_custom_command(
    OUTPUT kernel_entry.o
    COMMAND ${ASMC} -f elf ${CMAKE_CURRENT_SOURCE_DIR}/kernel_entry.asm -o kernel_entry.o
    DEPENDS kernel_entry.asm
    COMMENT "Building a kernel_entry for SynthLoader"
)

add_custom_target(build_synthloader DEPENDS synthloader kernel_entry.o)
set_target_properties(build_synthloader PROPERTIES EXCLUDE_FROM_ALL FALSE)
