cmake_minimum_required(VERSION 3.10)
project(ImageFrame)

# ---------------------------
# find packages
# ---------------------------
find_package(Threads REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
#QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Gui Core Widgets)

# ---------------------------
# set cmake variables
# ---------------------------
if(UNIX)
	set(IMAGE_FRAME_INSTALL_DIR /opt/ImageFrame)
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/lib)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/lib)
elseif(WIN32)
	set(IMAGE_FRAME_INSTALL_DIR ${CMAKE_SOURCE_DIR}/ImageFrame)
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/lib)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/lib)
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# ---------------------------
# set compiler variables
# ---------------------------
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
add_compile_options(-O4)

# ---------------------------
# build
# ---------------------------
configure_file(include/ImageFrame/build_defines.h.in include/ImageFrame/build_defines.h)

# set source files
set(INCLUDE_FILES include/ImageFrame/ui.h)

set(SOURCE_FILES
		src/ui.cpp)
set(UI_SOURCES
        ui/mainwindow.ui)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

include_directories(include
		${ImageMagick_INCLUDE_DIRS}
		${CMAKE_CURRENT_BINARY_DIR}/include/ImageFrame)

add_executable(ImageFrame
        src/main.cpp
        ${INCLUDE_FILES}
        ${SOURCE_FILES}
		${UI_GENERATED_HEADERS})

target_link_libraries(ImageFrame
        ${CMAKE_THREAD_LIBS_INIT}
        ${ImageMagick_LIBRARIES}
        Qt5::Core
        Qt5::Widgets
		Qt5::Gui)

# ---------------------------
# install
# ---------------------------
if (UNIX)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ImageFrame
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR})

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagick_LIB "${ImageMagick_LIBPATH}/libMagick*so*")
	install(FILES ${ImageMagick_LIB} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)

	# install Qt libraries
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_filename_component(QT5_LIBPATH ${QtCore_location} PATH)
	file(GLOB Qt5Core_LIBRARIES "${QT5_LIBPATH}/libQt5Core.so*")
	file(GLOB Qt5Gui_LIBRARIES "${QT5_LIBPATH}/libQt5Gui.so*")
	file(GLOB Qt5Widgets_LIBRARIES "${QT5_LIBPATH}/libQt5Widgets.so*")
	install(FILES ${Qt5Core_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES ${Qt5Gui_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES ${Qt5Widgets_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)

	# run post installation script
	install(CODE "execute_process(COMMAND sudo ln -sf /opt/ImageFrame/ImageFrame /usr/local/bin)")

elseif(WIN32)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ImageFrame.exe
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR})

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagick_LIBS "${ImageMagick_LIBPATH}/../CORE_RL*.dll")
	install(FILES ${ImageMagick_LIBS} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	
	# install Qt libraries
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_target_property(QtGui_location Qt5::Gui LOCATION)
	get_target_property(QtWidget_location Qt5::Widgets LOCATION)
	get_target_property(QWindowsPlugin Qt5::QWindowsIntegrationPlugin LOCATION)
	install(FILES ${QtCore_location} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES ${QtGui_location} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES ${QtWidget_location} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES ${QWindowsPlugin} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)

	# for 64bit
	install(FILES C:/Windows/System32/msvcp120.dll DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)
	install(FILES C:/Windows/System32/msvcr120.dll DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/lib)

	# for 32bit
	#install(FILES C:/Windows/SysWOW64/msvcp120.dll DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
	#install(FILES C:/Windows/SysWOW64/msvcr120.dll DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
endif()





#fixup_boundle
#https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt

#deploy lib, environment variable
#https://lemirep.wordpress.com/2013/06/01/deploying-qt-applications-on-linux-and-windows-3/


#deploy QT under linux
# github linuxdeployqt


#deploy libraries WINDOWS
# https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building

