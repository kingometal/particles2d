cmake_minimum_required(VERSION 3.10)
set (exec "particles2d.x")
#include_directories(source, source/interfaces)
file (GLOB SOURCES "source/*.cpp")
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
add_executable (${exec} ${SOURCES})
target_link_libraries(${exec} SDL2 SDL2_ttf config)
set_target_properties(${exec}
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

