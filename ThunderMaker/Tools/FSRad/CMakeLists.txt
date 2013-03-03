

include_directories(
	Include
)

set (FSRAD_SRC

    Source/3dutils.cpp

)

add_executable(fsrad ${FSRAD_SRC})