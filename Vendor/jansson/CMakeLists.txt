
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set (JANSSON_SRC

    src/dump.c
    src/error.c
    src/hashtable.c
    src/load.c
    src/memory.c
    src/pack_unpack.c
    src/strbuffer.c
    src/strconv.c
    src/utf.c
    src/value.c

)

add_library (jansson STATIC ${JANSSON_SRC})