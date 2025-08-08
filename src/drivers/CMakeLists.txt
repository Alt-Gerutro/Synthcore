add_library(drivers STATIC
    int/gdt_d.c
    int/idt_d.c
    int/pic_d.c
    int/interrupts_h.c
    io/screen_d.c
    io/input_d.c
)

target_include_directories(drivers PUBLIC ${CMAKE_SOURCE_DIR}/include)
