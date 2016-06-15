# Append triqs installed files to the cmake load path
list(APPEND CMAKE_MODULE_PATH ${TRIQS_PATH}/share/triqs/cmake)

# start configuration 
cmake_minimum_required(VERSION 2.8)
project(triqs_benchs CXX)
set(CMAKE_BUILD_TYPE Release)
enable_testing()

# We use shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Load TRIQS, including all predefined variables from TRIQS installation
find_package(TRIQS REQUIRED)
add_definitions(${TRIQS_CXX_DEFINITIONS})

add_subdirectory(google_benchmark)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/google_benchmark/include ${TRIQS_INCLUDE_ALL})

add_subdirectory(benchs)


