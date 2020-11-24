cmake_minimum_required(VERSION 3.0)
project(test)

set(SRCS
    src/core/main.c
    src/core/custom_descriptor.c
)

set(HEADERS
    src/core/custom_descriptor.h
)

add_compile_options(-g -Wall -Wextra -pedantic -Werror -std=c99)
add_executable(42sh ${SRCS} ${HEADERS})