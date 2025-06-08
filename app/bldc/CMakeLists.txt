add_library(bldc STATIC)

target_sources(bldc PRIVATE 
    "bldc.c"
)

target_include_directories(bldc PRIVATE 
)

target_link_libraries(bldc PRIVATE
)

target_compile_options(bldc PUBLIC
    -std=c23
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Waddress
    -pedantic
    -Wdeprecated
    -Wsign-conversion
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wcast-align
)
