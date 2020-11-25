cmake_minimum_required(VERSION 3.0)
project(42sh)

include_directories("${CMAKE_SOURCE_DIR}/src/include")

set(SRCS
    src/core/main.c
    src/core/custom_descriptor.c
    src/core/argument_handler.c
)

set (TESTSRC
    src/core/custom_descriptor.c
    src/core/argument_handler.c
)

set(HEADERS
    src/include/custom_descriptor.h
    src/include/argument_handler.h
)

add_compile_options(-g -Wall -Wextra -pedantic -Werror -std=c99 -lcriterion -fPIC)
add_executable(42sh ${SRCS} ${HEADERS})

add_executable(testsuite ${TESTSRC} ${HEADERS} tests/main.c)
target_link_libraries(testsuite PUBLIC criterion)