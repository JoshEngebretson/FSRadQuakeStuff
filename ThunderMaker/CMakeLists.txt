
include (BundleUtilities)

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

set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Icon.icns)

add_executable(ThunderMaker MACOSX_BUNDLE
	${CONTROLLER_FILES} 
	${GL_FILES} 
	${IO_FILES} 
	${MODEL_FILES} 
	${RENDERER_FILES} 
	${UTILITY_FILES} 
	${VIEW_FILES} 
	${MAC_FILES} 
	${RESOURCES}
)

SET_SOURCE_FILES_PROPERTIES(${RESOURCES} PROPERTIES
                            MACOSX_PACKAGE_LOCATION Resources)

#SET_SOURCE_FILES_PROPERTIES(Mac/TrenchBroom/en.lproj/MainMenu.xib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )


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

# Use our own Info.plist.
set_target_properties(ThunderMaker PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Mac/TrenchBroom/TrenchBroom-Info.plist")

# Process the XIB wth ibtool.

# Make sure we can find the 'ibtool' program. If we can NOT find it we
# skip generation of this project
find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
  message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with 
                    the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
endif()

# Locate the .app and process the XIB into it.
get_target_property(THUNDERMAKER_BIN ThunderMaker LOCATION)
get_dotapp_dir(${THUNDERMAKER_BIN} APPLICATION_APP_LOCATION)

#add_custom_command (TARGET ThunderMaker POST_BUILD 
#                  COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text 
#                          --compile ${APPLICATION_APP_LOCATION}/Contents/Resources/MainMenu.nib 
#                         ${CMAKE_CURRENT_SOURCE_DIR}/Mac/TrenchBroom/en.lproj/MainMenu.xib
#                  COMMENT "Compiling MainMenu.xib")

# OSX app bundle
set_target_properties(ThunderMaker PROPERTIES
 	MACOSX_BUNDLE_GUI_IDENTIFIER "com.thunderbeast.thundermaker"
  	XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
)

set(RSYNC_CMD rsync -a --delete)

set (DESTAPPRESOURCES ${CMAKE_SOURCE_DIR}/Artifacts/Mac/ThunderMaker.app/Contents/Resources)

add_custom_target(CreateMacArtifact ALL
   #COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/Artifacts/Mac/ThunderMaker.app/Contents/Resources
   COMMAND ${RSYNC_CMD} ${APPLICATION_APP_LOCATION} ${CMAKE_SOURCE_DIR}/Artifacts/Mac/
   COMMAND ${RSYNC_CMD} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Help/Documentation ${DESTAPPRESOURCES}
   COMMAND ${RSYNC_CMD} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Defs ${DESTAPPRESOURCES}
   COMMAND ${RSYNC_CMD} ${CMAKE_CURRENT_SOURCE_DIR}/Mac/TrenchBroom/en.lproj ${DESTAPPRESOURCES}
   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Graphics/*.png ${DESTAPPRESOURCES}
   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Graphics/*.lmp ${DESTAPPRESOURCES}
   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Shader/*.vertsh ${DESTAPPRESOURCES}
   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Shader/*.fragsh ${DESTAPPRESOURCES}
   COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text 
                     --compile ${DESTAPPRESOURCES}/MainMenu.nib 
                     ${CMAKE_CURRENT_SOURCE_DIR}/Mac/TrenchBroom/en.lproj/MainMenu.xib  
)
		
add_dependencies(CreateMacArtifact ThunderMaker)
