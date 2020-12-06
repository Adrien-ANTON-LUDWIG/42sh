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
    src/execution/pipe.c
    src/execution/redir.c
    src/lexer/lexer_utils.c
    src/lexer/lexer_cmd.c
    src/lexer/lexer_redir.c
    src/lexer/lexer_in.c
    src/lexer/lexer_operator.c
    src/lexer/lexer.c
    src/parser/ast.c
    src/parser/parser.c
    src/parser/parser_cpdlist.c
    src/parser/parser_if.c
    src/parser/parser_while.c
    src/parser/parser_for.c
    src/parser/parser_operator.c
    src/tokens/execution/command_execution.c
    src/tokens/tokens.c
    src/utils/list.c
    src/utils/major.c
    src/utils/my_err.c
    src/utils/my_xmalloc.c
    tests/printer.c
)

file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/include/*.h")

message("DBG:" $ENV{DBG})
if (DEFINED ENV{DBG})
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c99 -fsanitize=address -g)
    add_link_options(-Wall -Wextra -pedantic -Werror -std=c99 -fsanitize=address -g)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c99)
    add_link_options(-Wall -Wextra -pedantic -Werror -std=c99)
endif()

add_library(42 STATIC ${SRC})

add_executable(42sh ${HEADERS} src/main.c)
target_link_libraries(42sh 42)

add_executable(testsuite EXCLUDE_FROM_ALL ${HEADERS} tests/main.c)
target_link_libraries(testsuite PUBLIC criterion 42)

add_executable(lexer_test EXCLUDE_FROM_ALL tests/lexer_tests/lexer_test.c ${HEADERS})
target_link_libraries(lexer_test 42)

add_custom_target(debug
    COMMAND export DBG=DEBUG && cmake .. && make)

add_custom_target(release
    COMMAND unset DBG && make clean && cmake .. && make)

add_custom_target(lt
    DEPENDS lexer_test)

add_custom_target(check
    DEPENDS testsuite
    COMMAND ./testsuite --verbose)

add_custom_target(doc
    COMMAND doxygen ${CMAKE_SOURCE_DIR}/doc/doxconf ${CMAKE_SOURCE_DIR}/doc
    BYPRODUCTS ./doc/)

add_custom_target(script_test
    DEPENDS 42sh
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/tests/scripts ./
    COMMAND bash ./scripts/tests_script.sh
    BYPRODUCTS ./scripts/ actual expected)