#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#  cd .../path/to/build/directory
#  source .../path/to/source/code/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)

# Required to keep CMake happy.
cmake_minimum_required(VERSION ${CMAKE_VERSION} FATAL_ERROR)
# Actual version requirement.
cmake_policy(VERSION 3.4)

# Project information.
project(fhiclcpp VERSION 4.06.08 LANGUAGES CXX)

# cetmodules contains our cmake modules
find_package(cetmodules REQUIRED)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor
  )
cet_report_compiler_flags()

# Find dependencies
if(NOT APPLE)
  find_package(OpenSSL REQUIRED )
endif()
find_package(PythonLibs REQUIRED)
find_package(cetlib REQUIRED)
find_package(Boost 1.50.0 COMPONENTS program_options REQUIRED)

# SQLite
find_library(SQLITE3 NAMES sqlite3 REQUIRED)

# source code
add_subdirectory(fhiclcpp)
add_subdirectory(python)
add_subdirectory(tools)
add_subdirectory(ups)
