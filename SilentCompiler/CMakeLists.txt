SET(SOURCE
    src/SCodeGen.cpp
    src/Scompiler.cpp
    src/SParser.cpp
    src/STokenizer.cpp
)

SET(HEADERS
    include/SCodeGen.hpp
    include/SCompiler.hpp
    include/SFiles.hpp
    include/SParser.hpp
    include/STokenizer.hpp

)

add_executable(
    SilentCompiler
    ${SOURCE}
    ${HEADERS}
)

target_include_directories(
    SilentCompiler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
