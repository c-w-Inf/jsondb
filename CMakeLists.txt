cmake_minimum_required(VERSION 3.10)
project(jsondb)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(jsondb ${SOURCES})

target_include_directories(jsondb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(jsondb PRIVATE pthread)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(jsondb PRIVATE -g -O0 -Wall -Wextra)
    message(STATUS "Debug")
else()
    target_compile_options(jsondb PRIVATE -O2)
    message(STATUS "Release")
endif()
