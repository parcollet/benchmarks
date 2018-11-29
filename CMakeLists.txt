
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

#=== build gtest as an external project ===

include(ExternalProject)
set(GTEST_ROOT ${CMAKE_BINARY_DIR}/gtest)
ExternalProject_Add(gtest_project
  PREFIX gtest
  GIT_REPOSITORY https://github.com/abseil/googletest
  GIT_TAG release-1.8.1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_ROOT}
)

# Create the imported gtest target
add_library(gtest STATIC IMPORTED)
add_dependencies(gtest gtest_project)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
 set(LIBTEST ${GTEST_ROOT}/lib/libgtest.a)
else ()
 set(LIBTEST ${GTEST_ROOT}/lib64/libgtest.a)
endif()

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION ${LIBTEST}
  INTERFACE_LINK_LIBRARIES pthread
  ## Populating the INTERFACE_INCLUDE_DIRECTORIES won't work
  ## as the directory does not yet exist
  INTERFACE_COMPILE_OPTIONS "-isystem;${GTEST_ROOT}/include"
)

#=== build google benchmark as an external project ===

include(ExternalProject)
set(GBENCH_ROOT ${CMAKE_BINARY_DIR}/gbenchmark)
ExternalProject_Add(gtest_project
  PREFIX gbench
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG release-v1.4.1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GBENCH_ROOT}
)

# Create the imported gtest target
add_library(gbench STATIC IMPORTED)
add_dependencies(gbench gbench_project)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
 set(LIBBENCH ${GBENCH_ROOT}/lib/libgbench.a)
else ()
 set(LIBBENCH ${GBENCH_ROOT}/lib64/libgbench.a)
endif()

set_target_properties(gbench PROPERTIES
  IMPORTED_LOCATION ${LIBBENCH}
  INTERFACE_LINK_LIBRARIES pthread
  ## Populating the INTERFACE_INCLUDE_DIRECTORIES won't work
  ## as the directory does not yet exist
  INTERFACE_COMPILE_OPTIONS "-isystem;${GBENCH_ROOT}/include"
)

add_subdirectory(benchs)


