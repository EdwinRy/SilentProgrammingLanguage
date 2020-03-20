SET(HEADERS
    include/silent_files.h
    include/silent_compiler.h
    include/silent_lexer.h
    include/silent_parser.h
    include/silent_semantics.h
    include/silent_codegen.h
    include/silent_vm.h
    include/silent.h
)

SET(SOURCE
    src/main.c
    src/compiler.c
    src/lexer.c
    src/parser.c
    src/semantics.c
    src/codegen.c
    src/vm.c
    src/silent.c
)

add_executable(
    Silent
    ${HEADERS}
    ${SOURCE}
)

configure_file(tests/test.silent ${CMAKE_CURRENT_BINARY_DIR}/test.silent COPYONLY)

target_include_directories(
    Silent PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
