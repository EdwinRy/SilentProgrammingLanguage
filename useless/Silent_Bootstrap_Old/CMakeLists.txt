SET(HEADERS
    include/Silent.hpp
    include/lexer.hpp
    include/parser.hpp
    include/compiler.hpp
)

SET(SOURCE
    src/main.cpp
    src/compiler.cpp
    src/lexer.cpp
    src/parser.cpp
    src/silent.cpp
)

add_executable(
    Silent
    ${HEADERS}
    ${SOURCE}
)


#[[configure_file(tests/test.silent ${CMAKE_CURRENT_BINARY_DIR}/test.silent COPYONLY)]]

target_include_directories(
    Silent PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

