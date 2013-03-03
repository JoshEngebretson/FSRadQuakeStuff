

include_directories(
	Include
)

set (FSRAD_SRC

    Source/3dutils.cpp
    Source/BeamTree.cpp
    Source/RadPrim.cpp
    Source/BSP.cpp

)

add_executable(fsrad ${FSRAD_SRC})