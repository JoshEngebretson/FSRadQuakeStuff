
#set(CMAKE_OSX_ARCHITECTURES "i386")

include_directories(
	Include
    ${CMAKE_SOURCE_DIR}/Vendor/jansson/src
)

set (FSRAD_SRC

    Source/3dutils.cpp
    Source/primitive.cpp
    Source/BeamTree.cpp
    Source/RadPrim.cpp
    Source/PolarQuadtree.cpp    
    Source/BSP.cpp
    Source/GeomDB.cpp
    Source/ASEfile.cpp
    Source/Ent.cpp
    Source/RadLMap.cpp
    Source/SOctree.cpp
    Source/LMapGen.cpp
    Source/RadGen.cpp
    Source/stb_image_write.c

)

add_executable(fsrad ${FSRAD_SRC})

target_link_libraries(fsrad  
    jansson        
)


get_target_property(FSRADBIN fsrad LOCATION)
 
add_custom_command(TARGET fsrad
    POST_BUILD
    COMMAND cp ${FSRADBIN} ${CMAKE_SOURCE_DIR}/Artifacts/fsrad
)
