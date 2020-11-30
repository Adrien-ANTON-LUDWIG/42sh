cmake_minimum_required(VERSION 3.0)
project(42sh)

include_directories("src/include/core"
                    "src/include/execution"
                    "src/include/lexer"
                    "src/include/parser"
                    "src/include/tokens"
                    "src/include/tokens/lexing"
                    "src/include/"
                    "src/include/tokens/execution"
                    "src/include/tests"
                    "src/include/utils")

set (SRC
    src/core/custom_descriptor.c
    src/core/argument_handler.c
    src/execution/execution.c
    src/execution/exec_ast.c
    src/lexer/lexer_list.c
    src/lexer/lexer.c
    src/parser/ast.c
    src/parser/parser.c
    src/parser/parser_if.c
    src/tokens/execution/command_execution.c
    src/tokens/execution/if_execution.c
    src/tokens/lexing/redirection.c
    src/tokens/tokens.c
    src/utils/list.c
    src/utils/major.c
    src/utils/my_err.c
    src/utils/my_xmalloc.c
    tests/printer.c
)

add_library(42 STATIC ${SRC})

set(CMAKE_LINKER_FLAGS_DEBUG "-fsanitize=address")

file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/include/*.h")

add_compile_options(-Wall -Wextra -pedantic -Werror -std=c99 -lcriterion)

add_executable(42sh ${HEADERS} src/main.c)
target_link_libraries(42sh 42)

add_executable(testsuite EXCLUDE_FROM_ALL ${HEADERS} tests/main.c)
target_link_libraries(testsuite PUBLIC criterion 42)

add_executable(lexer_test EXCLUDE_FROM_ALL tests/lexer_tests/lexer_test.c ${HEADERS})
target_link_libraries(lexer_test 42)

add_custom_target(lt
    DEPENDS lexer_test)

add_custom_target(check
    DEPENDS testsuite
    COMMAND ./testsuite --verbose)