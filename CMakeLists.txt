cmake_minimum_required(VERSION 3.10)
project(polygongen)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Python
find_package(Python 3.10.12 COMPONENTS Interpreter Development REQUIRED)

# Add pybind11
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# Source files - note the genpoly-fpg prefix for submodule sources
set(SOURCES
    genpoly-fpg/src/eventQueue.cpp
    genpoly-fpg/src/initialPolygonGenerator.cpp
    genpoly-fpg/src/insertion.cpp
    genpoly-fpg/src/polygonTransformer.cpp
    genpoly-fpg/src/predicates.cpp
    genpoly-fpg/src/settings.cpp
    genpoly-fpg/src/statistics.cpp
    genpoly-fpg/src/tedge.cpp
    genpoly-fpg/src/tpolygon.cpp
    genpoly-fpg/src/translation.cpp
    genpoly-fpg/src/triangle.cpp
    genpoly-fpg/src/triangulation.cpp
    genpoly-fpg/src/vertex.cpp
    genpoly-fpg/src/translationKinetic.cpp
    genpoly-fpg/src/translationRetriangulation.cpp
    genpoly-fpg/src/polygon.cpp
    genpoly-fpg/src/holeInsertion.cpp
    src/bindings.cpp  # This is in our own src directory
)

# Include directories - include both our headers and genpoly-fpg headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/genpoly-fpg/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Create the Python module
pybind11_add_module(genpoly_fpg ${SOURCES})

# Compiler flags
target_compile_options(genpoly_fpg PRIVATE
    -Wall
    -Wextra
    -pedantic
    -Werror=switch-enum
)

# Special handling for predicates.cpp
set_source_files_properties(genpoly-fpg/src/predicates.cpp 
    PROPERTIES COMPILE_FLAGS "-Wno-float-equal -Wno-missing-declarations")