cmake_minimum_required(VERSION 3.10)
project(ImageFrame)

# set variables
if(UNIX)
	set(IMAGE_FRAME_INSTALL_DIR /opt/ImageFrame)
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
elseif(WIN32)
	set(IMAGE_FRAME_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install)
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/bin)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/bin)
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)


find_package(Threads REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
#QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Gui Core Widgets)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
add_compile_options(-O4)

# set source files
set(INCLUDE_FILES
		include/ImageFrame/ui.h)
set(SOURCE_FILES
		src/ui.cpp)
set(UI_SOURCES
        ui/mainwindow.ui)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

include_directories(include ${ImageMagick_INCLUDE_DIRS})

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

if (UNIX)
	install(PROGRAMS bin/ImageFrame
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagickCore_LIB "${ImageMagick_LIBPATH}/libMagickCore-6.Q16.so*")
	file(GLOB ImageMagickWand_LIB "${ImageMagick_LIBPATH}/libMagickWand-6.Q16.so*")
	file(GLOB ImageMagick++_LIB "${ImageMagick_LIBPATH}/libMagick++-6.Q16.so*")
	install(FILES ${ImageMagickCore_LIB} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${ImageMagickWand_LIB} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${ImageMagick++_LIB} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)

	# install Qt libraries
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_filename_component(QT5_LIBPATH ${QtCore_location} PATH)
	file(GLOB Qt5Core_LIBRARIES "${QT5_LIBPATH}/libQt5Core.so*")
	file(GLOB Qt5Gui_LIBRARIES "${QT5_LIBPATH}/libQt5Gui.so*")
	file(GLOB Qt5Widgets_LIBRARIES "${QT5_LIBPATH}/libQt5Widgets.so*")
	install(FILES ${Qt5Core_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${Qt5Gui_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${Qt5Widgets_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)

elseif(WIN32)
	install(PROGRAMS bin/Release/ImageFrame.exe
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR})

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR})

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagick_LIBS "${ImageMagick_LIBPATH}/../CORE_RL*.dll")
	install(FILES ${ImageMagick_LIBS} DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
	
	# install Qt libraries
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_filename_component(QT5_LIBPATH ${QtCore_location} PATH)
	file(GLOB Qt5Core_LIBRARIES "${QT5_LIBPATH}/Qt5Core.dll")
	file(GLOB Qt5Gui_LIBRARIES "${QT5_LIBPATH}/Qt5Gui.dll")
	file(GLOB Qt5Widgets_LIBRARIES "${QT5_LIBPATH}/Qt5Widgets.dll")
	install(FILES ${Qt5Core_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
	install(FILES ${Qt5Gui_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
	install(FILES ${Qt5Widgets_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR})
endif()





#fixup_boundle
#https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt

#deploy lib, environment variable
#https://lemirep.wordpress.com/2013/06/01/deploying-qt-applications-on-linux-and-windows-3/


#deploy QT under linux
# github linuxdeployqt


#deploy libraries WINDOWS
# https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building

