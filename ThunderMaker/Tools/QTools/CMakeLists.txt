
#set(CMAKE_OSX_ARCHITECTURES "i386")

include_directories(
	Include
)

add_definitions( -DDOUBLEVEC_T=1 )

set (TXQBSP_SRC

    brush.c
    bspfile.c
    cmdlib.c
    csg4.c
    dyna1.c
    map.c
    merge.c
    nodraw.c
    outside.c
    portals.c
    qbsp.c
    region.c
    solidbsp.c
    surfaces.c
    tjunc.c
    writebsp.c

)

add_executable(txqbsp ${TXQBSP_SRC})

get_target_property(TXQBSPBIN txqbsp LOCATION)
 
add_custom_command(TARGET txqbsp
    POST_BUILD
    COMMAND cp ${TXQBSPBIN} ${CMAKE_SOURCE_DIR}/Artifacts/txqbsp
)


