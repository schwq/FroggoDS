set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.20)

set(projectName FroggoDS)

project(${projectName})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source/NDS)

add_executable(${projectName} ${SOURCE_FILES})

add_definitions(-w -DDEBUG_DS)