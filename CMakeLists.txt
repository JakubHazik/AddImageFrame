cmake_minimum_required(VERSION 3.5)
project(ImageFrame)


# set paths
set(IMAGE_FRAME_INSTALL_DIR /opt/ImageFrame)
set(CMAKE_INSTALL_RPATH "${IMAGE_FRAME_INSTALL_DIR}/bin/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_PREFIX ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)

#message(${CMAKE_INSTALL_RPATH})
set(LIB_DIR ${CMAKE_SOURCE_DIR}/bin/lib)




find_package(Threads REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)
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


#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBRARIES OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")
#set_target_properties(ImageFrame PROPERTIES LINK_FLAGS "-static")

add_executable(ImageFrame
        src/main.cpp
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

#add_custom_target(createLibFolder ALL
#        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_DIR})
#add_custom_target(copyLibraries
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        $<TARGET_FILE:Qt5::Widgets>
#        $<TARGET_FILE:Qt5::Gui>
#        $<TARGET_FILE:Qt5::Core>
#        ${ImageMagick_LIBRARIES}
#        ${ImageMagick_INCLUDE_DIRS}
#        ${LIB_DIR})
#add_dependencies(ImageFrame createLibFolder copyLibraries)


#link_directories(${CMAKE_INSTALL_RPATH})
#add_library(l SHARED ${ImageMagick_LIBRARIES} )

#https://stackoverflow.com/questions/41912734/deploy-icu-libraries-when-linking-a-cmake-project-with-qt5
target_link_libraries(ImageFrame
        ${CMAKE_THREAD_LIBS_INIT}
        ${ImageMagick_LIBRARIES}
        Qt5::Core
        Qt5::Widgets)

#message(${ImageMagick_LIBRARIES})
#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

#install(CODE "
#    include(BundleUtilities)
#    fixup_bundle(${CMAKE_SOURCE_DIR}/bin/ImageFrame ${ImageMagick_LIBRARIES} \"\" )
#    " COMPONENT Runtime)



if (UNIX)
#        install(
#                TARGETS ImageFrame
#                RUNTIME DESTINATION bin
#                LIBRARY DESTINATION lib
#        )

   # install(CODE ${CMAKE_COMMAND} -E copy_if_different ${var} ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)


#
#add_custom_command(TARGET ImageFrame POST_BUILD
#        COMMAND ${CMAKE_COMMAND}
#        -E copy_if_different ${var} ${CMAKE_SOURCE_DIR}/bin/lib)
#
#
install(CODE "
        include(GetPrerequisites)
        get_prerequisites(${CMAKE_SOURCE_DIR}/bin/ImageFrame var 0 1 \"\" \"\")
        ")

    #message(${var})

    install(PROGRAMS ${var}
            DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib
            )

#
#file(INSTALL ${var}
#        DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
#

install(PROGRAMS bin/ImageFrame
        DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin)

install(FILES icons/icon.png
        DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/icons)





#        install(FILES
#                $<TARGET_FILE:Qt5::Widgets>
#                $<TARGET_FILE:Qt5::Core>
#                $<TARGET_FILE:Qt5::Gui>
#                ${ImageMagick_LIBRARIES}
#                DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib)
#
#        install(TARGETS ImageFrame
#                RUNTIME DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin COMPONENT ${ImageMagick_LIBRARIES} #libraries
#                LIBRARY DESTINATION ${IMAGE_FRAME_INSTALL_DIR}/bin/lib COMPONENT ${ImageMagick_LIBRARIES}
#                )
endif (UNIX)

#fixup_boundle
#https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt

#deploy lib, environment variable
#https://lemirep.wordpress.com/2013/06/01/deploying-qt-applications-on-linux-and-windows-3/


#deploy QT under linux
# github linuxdeployqt


#deploy libraries WINDOWS
# https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building