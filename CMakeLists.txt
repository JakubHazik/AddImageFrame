cmake_minimum_required(VERSION 3.10)
project(ImageFrame)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

#find_program(MAGICK_CONFIG "Magick++-config")
find_package(ImageMagick COMPONENTS Magick++)
#find_package(ImageMagick COMPONENTS convert)
#QT
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Gui Core Widgets)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
#add_compile_options(-O4)

include_directories(include)
include_directories(${ImageMagick_INCLUDE_DIRS})

set(INCLUDE_FILES
        include/ImageFrame/Gui.h
        include/ImageFrame/ImgProcessing.h)
set(SOURCE_FILES
        src/Gui.cpp
        src/ImgProcessing.cpp)

add_executable(ImageFrame
        src/main.cpp
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

set(EXECUTABLE_OUTPUT_PATH bin)

target_link_libraries(ImageFrame
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${ImageMagick_LIBRARIES}
        Qt5::Core
        Qt5::Widgets)
