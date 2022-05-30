cmake_minimum_required(VERSION 3.10)

# set the project name
project(Test CXX)

# Set C++ version.
set(CMAKE_CXX_STANDARD 20)

# Add source files.
set(
    SourceFiles
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Profiler.cpp"
)

# Add includes.
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# add the executable
add_executable(Test ${SourceFiles})

# Build settings.
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")