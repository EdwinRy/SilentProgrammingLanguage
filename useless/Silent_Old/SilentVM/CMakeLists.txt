SET(SOURCE
    src/main.c
    src/SilentLib.c
    src/SilentVM.c
)

SET(HEADERS
    src/SilentLib.h
    src/SilentVM.h
)

add_executable(
    SilentVM
    ${SOURCE}
    ${HEADERS}
)

