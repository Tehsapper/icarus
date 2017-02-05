project( icarus )
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-O2")
set(CONFIG_PATH_VALUE "${CMAKE_INSTALL_PREFIX}/etc/${PROJECT_NAME}")

find_package( OpenCV REQUIRED )

include_directories(include)

list(APPEND SRC_FILES 
		src/main.cpp src/video.cpp src/config.cpp src/sentry.cpp src/motiondetector.cpp src/imageframe.cpp src/logger.cpp )


add_executable( icarus ${SRC_FILES} )
target_compile_definitions(icarus PRIVATE CONFIG_FILE_PATH=\"${CONFIG_PATH_VALUE}/config.ini\")
target_link_libraries( icarus ${OpenCV_LIBS} )

install(TARGETS icarus DESTINATION "bin" COMPONENT binaries )
install(FILES config.ini DESTINATION ${CONFIG_PATH_VALUE} COMPONENT config)