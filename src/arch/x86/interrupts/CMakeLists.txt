add_library(interrupts STATIC
    gdt.c
    idt.c
    pic.c
    isr.c
    interrupts.c
)

target_include_directories(interrupts PUBLIC ${CMAKE_SOURCE_DIR}/include)