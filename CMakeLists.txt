cmake_minimum_required(VERSION 3.5)
project(ImageFrame)

# set variables
if(UNIX)
	set(IMAGE_FRAME_INSTALL_DIR /opt/ImageFrame)
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
elseif(WIN32)
	set(IMAGE_FRAME_INSTALL_DIR ${CMAKE_SOURCE_DIR})
	set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/bin)
	set(CMAKE_INSTALL_RPATH ${IMAGE_FRAME_INSTALL_DIR}/bin)
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)


find_package(Threads REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
#QT
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Gui Core Widgets)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
add_compile_options(-O4)


# set source files
set(INCLUDE_FILES
        include/ImageFrame/Gui.h
        include/ImageFrame/ImgProcessing.h)
set(SOURCE_FILES
        src/Gui.cpp
        src/ImgProcessing.cpp)

include_directories(include ${ImageMagick_INCLUDE_DIRS})

add_executable(ImageFrame
        src/main.cpp
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

#https://stackoverflow.com/questions/41912734/deploy-icu-libraries-when-linking-a-cmake-project-with-qt5
target_link_libraries(ImageFrame
        ${CMAKE_THREAD_LIBS_INIT}
        ${ImageMagick_LIBRARIES}
        Qt5::Core
        Qt5::Widgets)


if (UNIX)
	install(PROGRAMS bin/ImageFrame
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagickCore_LIBRARIES "${ImageMagick_LIBPATH}/libMagickCore-6.Q16.so*")
	file(GLOB ImageMagickWand_LIBRARIES "${ImageMagick_LIBPATH}/libMagickWand-6.Q16.so*")
	file(GLOB ImageMagick++_LIBRARIES "${ImageMagick_LIBPATH}/libMagick++-6.Q16.so*")
	install(FILES ${ImageMagickCore_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${ImageMagickWand_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
	install(FILES ${ImageMagick++_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)

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
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)

	install(FILES icons/icon.png
			DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)

	# install ImageMagick libraries
	get_filename_component(ImageMagick_LIBPATH ${ImageMagick_LIBRARIES} PATH)
	file(GLOB ImageMagickCore_LIBRARIES "${ImageMagick_LIBPATH}/../CORE_RL_Magick++_.dll")
	file(GLOB ImageMagickWand_LIBRARIES "${ImageMagick_LIBPATH}/../CORE_RL_MagickCore_.dll")
	file(GLOB ImageMagick++_LIBRARIES "${ImageMagick_LIBPATH}/../CORE_RL_MagickWand_.dll")
	install(FILES ${ImageMagickCore_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)
	install(FILES ${ImageMagickWand_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)
	install(FILES ${ImageMagick++_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)
	
	# install Qt libraries
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_filename_component(QT5_LIBPATH ${QtCore_location} PATH)
	file(GLOB Qt5Core_LIBRARIES "${QT5_LIBPATH}/Qt5Core.dll")
	file(GLOB Qt5Gui_LIBRARIES "${QT5_LIBPATH}/Qt5Gui.dll")
	file(GLOB Qt5Widgets_LIBRARIES "${QT5_LIBPATH}/Qt5Widgets.dll")
	install(FILES ${Qt5Core_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)
	install(FILES ${Qt5Gui_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)
	install(FILES ${Qt5Widgets_LIBRARIES} DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)

endif()





#fixup_boundle
#https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt

#deploy lib, environment variable
#https://lemirep.wordpress.com/2013/06/01/deploying-qt-applications-on-linux-and-windows-3/


#deploy QT under linux
# github linuxdeployqt


#deploy libraries WINDOWS
# https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building
