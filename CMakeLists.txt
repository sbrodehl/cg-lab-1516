cmake_minimum_required(VERSION 2.8.8)
project(cg_lab_1516)

# Add lokal CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

# Compiler checks
include(CXX11)
check_for_cxx11_compiler(CXX11_COMPILER)
if (CXX11_COMPILER)
    enable_cxx11()
else ()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} seems to have no C++11 support. \
    Please try again with a more recent compiler version.")
endif ()

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++ -lc++abi")
endif ()

# set CMAKE_PREFIX_PATH to Qt installation, e.g.
# set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\" ${CMAKE_PREFIX_PATH})
# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(OpenGL REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

qt5_add_resources(RESOURCES
        shaders.qrc
        )

set(SOURCE_FILES
        drawing.cpp
        StlReader.cpp
        StlReader.h
        Parametrics.h
        Torus.h
        Trefoil.h Zylinder.h)

include_directories(.)

add_executable(cg_lab_1516
        ${SOURCE_FILES}
        ${RESOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(cg_lab_1516
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        Qt5::OpenGL
        ${OPENGL_LIBRARY}
        )