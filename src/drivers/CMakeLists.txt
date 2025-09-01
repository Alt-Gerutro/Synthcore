add_library(drivers STATIC
    video/screen.c
    keyboard/input.c
)

target_include_directories(drivers PUBLIC ${CMAKE_SOURCE_DIR}/include)