
#set(CMAKE_OSX_ARCHITECTURES "i386")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Vendor/jansson/src
)

add_definitions( -DDOUBLEVEC_T -DQBSP_VERSION=0.5 -DUSE_PTHREADS)

set (QCOMMON_SRC

    common/bspfile.c
    common/cmdlib.c
    common/lbmlib.c
    common/log.c
    common/mathlib.c
    common/polylib.c
    common/scriplib.c
    common/threads.c
    common/trilib.c
    common/wadlib.c
)

set (QBSP_SRC

    qbsp/brush.c
    qbsp/bspfile.c
    qbsp/cmdlib.c
    qbsp/csg4.c
    qbsp/file.c
    qbsp/globals.c
    qbsp/map.c
    qbsp/mathlib.c
    qbsp/merge.c
    qbsp/outside.c
    qbsp/parser.c
    qbsp/portals.c
    qbsp/qbsp.c
    qbsp/solidbsp.c
    qbsp/surfaces.c
    qbsp/tjunc.c
    qbsp/util.c
    qbsp/wad.c
    qbsp/warnerr.h
    qbsp/winding.c
    qbsp/writebsp.c

)

set (QVIS_SRC

    vis/flow.c
    vis/soundpvs.c
    vis/state.c
    vis/vis.c
)

set (QTHUNDER_SRC

    qthunder/qentities.c
    qthunder/qthunder.c
)


add_library (qcommon STATIC ${QCOMMON_SRC})

add_executable(qbsp ${QBSP_SRC})

target_link_libraries(qbsp  
    qcommon        
)

get_target_property(QBSPBIN qbsp LOCATION)
 
add_custom_command(TARGET qbsp
    POST_BUILD
    COMMAND cp ${QBSPBIN} ${CMAKE_SOURCE_DIR}/Artifacts/qbsp
)

add_executable(qvis ${QVIS_SRC})

target_link_libraries(qvis  
    qcommon        
)

get_target_property(QVISBIN qvis LOCATION)
 
add_custom_command(TARGET qvis
    POST_BUILD
    COMMAND cp ${QVISBIN} ${CMAKE_SOURCE_DIR}/Artifacts/qvis
)

add_executable(qthunder ${QTHUNDER_SRC})

target_link_libraries(qthunder  
    qcommon
    jansson        
)

get_target_property(QTHUNDERBIN qthunder LOCATION)
 
add_custom_command(TARGET qthunder
    POST_BUILD
    COMMAND cp ${QTHUNDERBIN} ${CMAKE_SOURCE_DIR}/Artifacts/qthunder
)

