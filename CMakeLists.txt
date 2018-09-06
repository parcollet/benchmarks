
# Start configuration
cmake_minimum_required(VERSION 2.8.12)
project(benchs CXX)

# Default to Release build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Type of build" FORCE)
endif()
message( STATUS "-------- BUILD-TYPE: ${CMAKE_BUILD_TYPE} --------")

# Build static libraries
set(BUILD_SHARED_LIBS OFF)

# 
add_definitions(-std=c++17 )

# Load Dependencies
find_package(TRIQS 1.5 REQUIRED)
find_package(Cpp2Py 1.5 REQUIRED)

# Default Install directory to TRIQS_ROOT if not given or invalid.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR (NOT IS_ABSOLUTE ${CMAKE_INSTALL_PREFIX}))
  message(STATUS "No install prefix given (or invalid). Defaulting to TRIQS_ROOT")
  set(CMAKE_INSTALL_PREFIX ${TRIQS_ROOT} CACHE PATH "default install path" FORCE)
endif()
message(STATUS "-------- INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX} --------")

add_subdirectory(google_benchmark)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/google_benchmark/include ${TRIQS_INCLUDE_ALL})

add_subdirectory(benchs)


