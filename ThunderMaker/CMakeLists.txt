

include_directories(
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/wx/include/osx_cocoa-unicode-static-2.9
	/Users/josh/Dev/wxWidgets-2.9.4/include
	${CMAKE_CURRENT_SOURCE_DIR}/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Source
	${CMAKE_CURRENT_SOURCE_DIR}/Mac/TrenchBroom
)

add_definitions( -D_FILE_OFFSET_BITS=64 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__ )

file ( GLOB CONTROLLER_FILES Source/Controller/*.cpp )
file ( GLOB GL_FILES Source/GL/*.cpp Source/GL/*.c )
file ( GLOB IO_FILES Source/IO/*.cpp )
file ( GLOB MODEL_FILES Source/Model/*.cpp )
file ( GLOB RENDERER_FILES Source/Renderer/*.cpp Source/Renderer/Shader/*.cpp Source/Renderer/Text/*.cpp )
file ( GLOB UTILITY_FILES Source/Utility/*.cpp )
file ( GLOB VIEW_FILES Source/View/*.cpp )

file ( GLOB MAC_FILES Mac/TrenchBroom/*.cpp Mac/TrenchBroom/*.mm)

add_executable(ThunderMaker 
	${CONTROLLER_FILES} 
	${GL_FILES} 
	${IO_FILES} 
	${MODEL_FILES} 
	${RENDERER_FILES} 
	${UTILITY_FILES} 
	${VIEW_FILES} 
	${MAC_FILES} 
)

target_link_libraries(ThunderMaker 
	"-framework IOKit"
	"-framework Carbon"
	"-framework Cocoa"
	"-framework AudioToolbox"
	"-framework System"
	"-framework OpenGL"
	"-framework WebKit"
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_gl-2.9.a	
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_xrc-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_webview-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_qa-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_baseu_net-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_html-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_adv-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_osx_cocoau_core-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_baseu_xml-2.9.a 
	/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib/libwx_baseu-2.9.a  	
	-L/Users/josh/Dev/wxWidgets-2.9.4/build-cocoa-release/lib   	
	-lexpat 
	-lwxregexu-2.9 
	-lwxtiff-2.9 
	-lwxjpeg-2.9 
	-lwxpng-2.9 
	-lz 
	-lpthread 
	-liconv 	
	${CMAKE_CURRENT_SOURCE_DIR}/Mac/Lib/libbz2.a
	${CMAKE_CURRENT_SOURCE_DIR}/Mac/Lib/libfreetype.a

)


#get_target_property(THUNDERBINARY ThunderMaker LOCATION)

#set(RSYNC_CMD rsync -a --delete)

#add_custom_target(CreateMacArtifact ALL
#    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/Artifacts/Mac/
#    COMMAND ${RSYNC_CMD} ${CMAKE_CURRENT_SOURCE_DIR}/Application/Mac/ThunderMaker.app ${CMAKE_SOURCE_DIR}/Artifacts/Mac/
#    COMMAND cp ${THUNDERBINARY} ${CMAKE_SOURCE_DIR}/Artifacts/Mac/ThunderMaker.app/Contents/MacOS/ThunderMaker
#)
		
#add_dependencies(CreateMacArtifact ThunderMaker)
