cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# project

project(Voxie)
set(APP_NAME "voxie")
set(APP_NAME_FULL "Voxie")

set(ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(SRC_DIR "${ROOT_DIR}/src")
set(BUILD_DIR "${PROJECT_BINARY_DIR}")
set(TEMP_DIR "${BUILD_DIR}/tmp")

# compiler & linker options

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if(WIN32)
    # Tell glew we're building an executable.
    add_definitions(-DGLEW_STATIC)
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-std=c99 -Wall -pedantic ${CMAKE_C_FLAGS}")
endif()

set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# sources

set(EDITORSRCS
    ${SRC_DIR}/globals.cpp
    ${SRC_DIR}/ReferencePoint.cpp
    ${SRC_DIR}/VoxelModel.cpp
    ${SRC_DIR}/VoxelFile.cpp
    ${SRC_DIR}/RGBColor.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/MainWindow.cpp
    ${SRC_DIR}/VoxelEditor.cpp
    ${SRC_DIR}/ColorSpace.cpp
    ${SRC_DIR}/ColorSlider.cpp
    ${SRC_DIR}/PaletteGrid.cpp
    ${SRC_DIR}/PaletteEditor.cpp
    ${SRC_DIR}/ModelProperties.cpp
    ${SRC_DIR}/PositionArrows.cpp
    ${SRC_DIR}/draw.cpp
    ${SRC_DIR}/glew.c
)

# dependencies

set(CMAKE_LIBRARY_PATH "${ROOT_DIR}/lib" ${CMAKE_LIBRARY_PATH})
if(WIN32)
    set(CMAKE_LIBRARY_PATH "${ROOT_DIR}/lib/win32"
        ${CMAKE_LIBRARY_PATH})
endif()

set(CMAKE_INCLUDE_PATH ${SRC_DIR} "${ROOT_DIR}/include"
    ${CMAKE_INCLUDE_PATH})
if(WIN32)
    set(CMAKE_INCLUDE_PATH "${ROOT_DIR}/include/win32"
        ${CMAKE_INCLUDE_PATH})
endif()

find_package(Bullet REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

set(EDITOR_LIBS
    ${BULLET_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
    ${OPENGL_LIBRARY}
)

if(LINUX)
    set(CLIENT_LIBS
        ${CLIENT_LIBS}
        ${X11_Xrandr_LIB}
        ${X11_Xinput_LIB}
    )
elseif(OSX)
    set(CLIENT_LIBS
        ${CLIENT_LIBS}
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${CORE_FOUNDATION_FRAMEWORK}
    )
endif()

# build

include_directories(
    ${BULLET_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

# editor build
add_executable(voxie ${EDITORSRCS})
set_target_properties(voxie PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_RPATH}"
    AUTOMOC TRUE
)
include_directories(EDITOR_INCLUDES)
target_link_libraries(voxie ${EDITOR_LIBS})
