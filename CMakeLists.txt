cmake_minimum_required(VERSION 3.14)

project(LatticeExample LANGUAGES CXX)
set(CMAKE_PREFIX_PATH d:\\Cpp_Dev\\Qt\\5.15.2\\mingw81_64\\)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5Widgets REQUIRED)

set(SOURCES
        src/main.cpp src/objReader/ObjReader.cpp src/objReader/ObjReader.h src/geometry/Geometry.cpp src/geometry/Geometry.h src/lattice/LatticeDeform.cpp src/lattice/LatticeDeform.h src/geometry/GeoTools.cpp src/geometry/GeoTools.h src/objWriter/ObjWriter.cpp src/objWriter/ObjWriter.h src/geometry/kdTree/Node.h src/geometry/kdTree/NodeInner.cpp src/geometry/kdTree/NodeInner.h src/geometry/kdTree/NodeLeaf.cpp src/geometry/kdTree/NodeLeaf.h src/geometry/kdTree/KDTree.cpp src/geometry/kdTree/KDTree.h)

add_executable(LatticeExample ${SOURCES})
target_link_libraries(LatticeExample Qt5::Widgets)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET LatticeExample PROPERTY WIN32_EXECUTABLE true)
endif ()
