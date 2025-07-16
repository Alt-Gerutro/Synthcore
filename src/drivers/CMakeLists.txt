add_library(drivers STATIC
    interrupts/idt.c
    fs/ext4/ext4_io.c
    io/display.c
    io/input.c
)

target_include_directories(drivers PUBLIC ${CMAKE_SOURCE_DIR}/include)
