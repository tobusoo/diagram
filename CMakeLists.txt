cmake_minimum_required(VERSION 3.22)

project(
    Diagram
    VERSION 1.0
    LANGUAGES CXX   
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML COMPONENTS graphics system window REQUIRED)
include_directories( ${SFML_INCLUDE_DIRS} )

add_subdirectory(external)
add_subdirectory(src)
