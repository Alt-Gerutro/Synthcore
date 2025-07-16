set_source_files_properties(boot.S PROPERTIES LANGUAGE ASM)

add_executable(kernel.elf
    boot.S
    kernel.c
)

set_target_properties(kernel.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    POSITION_INDEPENDENT_CODE OFF
    COMMENT "Building a kernel"
)

target_link_libraries(kernel.elf drivers)
