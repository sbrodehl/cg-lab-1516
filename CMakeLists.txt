cmake_minimum_required(VERSION 2.8.8)
project(cg_lab_1516)

set(PROGNAME "cg_lab_1516")

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

qt5_add_resources(RESOURCES shader/shaders.qrc)

set(SOURCE_FILES
        drawing.cpp
        ParametricsViewer.cpp
        ParametricsViewer.h
        Mesh.cpp
        Mesh.h
        ParametricWindow.cpp
        ParametricWindow.h
        Parametrics.h
        Parametrics.cpp
        parametrics/ParameterTriangle.h
        parametrics/Torus.h
        parametrics/Trefoil.h
        parametrics/Tube.h
        parametrics/Cone.h
        parametrics/Disc.h
        Part.h
        parts/CylinderPart.h
        parts/ConePart.h
        parts/TorusPart.h
        parts/TrefoilPart.h
        UserInteractions.cpp
        ParamRenderArea.cpp
        ParamRenderArea.h
        PolygonTriangulationC/TriangulationAlgorithm.cpp
        PolygonTriangulationC/TriangulationAlgorithm.h
        PolygonTriangulationC/Edge.h
        PolygonTriangulationC/Vertex.h
        PolygonTriangulationC/Vertex.cpp
        PolygonTriangulationC/Line.h)

include_directories(.)
include_directories(parametrics)
include_directories(parts)
include_directories(thirdparty)
include_directories(PolygonTriangulationC)

if (APPLE)
    add_executable(${PROGNAME}
            MACOSX_BUNDLE
            ${SOURCE_FILES}
            ${RESOURCES})
    target_link_libraries(${PROGNAME}
            Qt5::Core
            Qt5::Widgets
            Qt5::Gui
            Qt5::OpenGL
            ${OPENGL_LIBRARY})
    qt5_use_modules(${PROGNAME}
            Widgets
            Gui
            OpenGL)
else ()
    add_executable(${PROGNAME}
            ${SOURCE_FILES}
            ${RESOURCES})
    target_link_libraries(${PROGNAME}
            Qt5::Core
            Qt5::Widgets
            Qt5::Gui
            Qt5::OpenGL
            ${OPENGL_LIBRARY})
    qt5_use_modules(${PROGNAME}
            Widgets
            Gui
            OpenGL)
endif()