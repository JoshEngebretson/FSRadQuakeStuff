
#set(CMAKE_OSX_ARCHITECTURES "i386")

include_directories(
	qcommon
)

add_definitions( -DDOUBLEVEC_T=1 )

set (QCOMMON_SRC

    qcommon/dyna1.c
    qcommon/bspfile.c
    qcommon/cmdlib.c
    qcommon/lbmlib.c
    qcommon//mathlib.c
    qcommon/polylib.c
    qcommon/scriplib.c
    qcommon/threads.c
    qcommon/trilib.c
    qcommon/wadlib.c

)

set (QBSP_SRC

    qbsp/brush.c
    qbsp/csg4.c    
    qbsp/map.c
    qbsp/merge.c
    qbsp/nodraw.c
    qbsp/outside.c
    qbsp/portals.c
    qbsp/qbsp.c
    qbsp/region.c
    qbsp/solidbsp.c
    qbsp/surfaces.c
    qbsp/tjunc.c
    qbsp/writebsp.c

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


