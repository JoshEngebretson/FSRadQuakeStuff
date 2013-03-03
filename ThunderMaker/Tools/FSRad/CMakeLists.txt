

include_directories(
	Include
)

set (FSRAD_SRC

    Source/3dutils.cpp
    Source/primitive.cpp
    Source/BeamTree.cpp
    Source/PolarQuadtree.cpp
    Source/RadPrim.cpp
    Source/BSP.cpp
    Source/GeomDB.cpp
    Source/ASEfile.cpp
    Source/Ent.cpp
    Source/RadLMap.cpp
    Source/SOctree.cpp

)

add_executable(fsrad ${FSRAD_SRC})