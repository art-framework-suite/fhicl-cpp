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

cmake_minimum_required(VERSION 3.12...3.14 FATAL_ERROR)

project(fhiclcpp VERSION 4.11.01 LANGUAGES CXX)

# cetmodules contains our cmake modules
find_package(cetmodules 1.01.01 REQUIRED)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -DBOOST_TEST_NO_OLD_TOOLS
  )

cet_report_compiler_flags()


# Both of these are needed to ensure sane and consistent Python
# location, even though we don't actually need the interpreter.
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

# Other required components.
find_package(hep_concurrency REQUIRED)
find_package(cetlib_except REQUIRED)
find_package(cetlib REQUIRED)
find_package(Boost 1.50.0 REQUIRED COMPONENTS program_options)
find_library(SQLITE3 NAMES sqlite3 REQUIRED)

# source code
add_subdirectory(fhiclcpp)
add_subdirectory(python)
add_subdirectory(tools)
add_subdirectory(cmakeConfig)
