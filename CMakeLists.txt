cmake_minimum_required(VERSION 3.0)
project(42sh)

include_directories("${CMAKE_SOURCE_DIR}/src/include")

set(SRCS
    src/core/main.c
    src/core/custom_descriptor.c
    src/core/argument_handler.c
    src/execution/execution.c
)

set (TESTSRC
    src/core/custom_descriptor.c
    src/core/argument_handler.c
    src/execution/execution.c
)

set(HEADERS
    src/include/custom_descriptor.h
    src/include/argument_handler.h
    src/include/lexer.h
    src/include/my_utils.h
    src/include/parser.h
    src/include/execution.h
    src/include/structures.h
    src/include/tokens.h
)

add_compile_options(-g -Wall -Wextra -pedantic -Werror -std=c99 -lcriterion)
add_executable(42sh ${SRCS} ${HEADERS})

add_executable(testsuite ${TESTSRC} ${HEADERS} tests/main.c)
target_link_libraries(testsuite PUBLIC criterion)