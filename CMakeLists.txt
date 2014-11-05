cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#
#----------------------------------------------------------------------------
project(HepMC3)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# - Prepend our own CMake Modules to the search path
# This allows us to customize, add, and factor out functionality
# NB: if our custom modules include others that we don't supply, those in the
# base path will be used, so watch for incompatibilities!!
#
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/Modules
    ${CMAKE_MODULE_PATH})

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------

option(HEPMC_ENABLE_CPP11 "Enables building with C++11.")

if(HEPMC_ENABLE_CPP11)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. We need to use BOOST.")
    
    find_package(Boost 1.40 REQUIRED)
    message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})

  endif()
else()
  #--- buiding without C++11
  #
  #---Find Boost---------------------------------------------------------------
  find_package(Boost 1.40 REQUIRED)
  message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()

#---Enable FORTRAN (unfortunatelly is not not possible in all cases)-------------------------------
if(NOT CMAKE_GENERATOR STREQUAL Xcode)
enable_language(Fortran OPTIONAL)
endif()


#---Compilation and Link options---------------------------------------------

if(${CMAKE_BUILD_TYPE})
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
else()
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified, use: ${CMAKE_BUILD_TYPE}")
endif()

if(CMAKE_SYSTEM_NAME MATCHES Darwin)
  set(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS} -flat_namespace -single_module -undefined dynamic_lookup")
endif()

set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2")

# fortran flags needed for some tests and examples
set(CMAKE_Fortran_FLAGS            "-fno-automatic -fno-backslash -ffixed-line-length-132")

#--------------------------------------------------------------------------
include_directories(include)

#---Select sources for the various libraries---------------------------------
file(GLOB hepmc3_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB hepmc3_sources_IO ${PROJECT_SOURCE_DIR}/src/IO/*.cc)
file(GLOB hepmc3_sources_Search ${PROJECT_SOURCE_DIR}/src/Search/*.cc)

#---Add libraries------------------------------------------------------------

add_library(HepMC SHARED ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
add_library(HepMC_static STATIC ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
set_target_properties(HepMC PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)
set_target_properties(HepMC_static PROPERTIES OUTPUT_NAME HepMC ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)

#---Install build products---------------------------------------------------
install(TARGETS HepMC DESTINATION lib)
install(TARGETS HepMC_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include COMPONENT headers)

#---Optional build of rootIO-------------------------------------------------

find_package(ROOT QUIET)

if(ROOT_FOUND)
  add_subdirectory(rootIO)
endif()

#---Options
#option(HEPMC_ENABLE_TESTING "Enables building of tests.")
option(HEPMC_BUILD_EXAMPLES "Enables building of examples.")

#------------------------------------------------------------------------------
# Add the examples and tests subdirectories
#if(HEPMC_ENABLE_TESTING)
#  add_subdirectory(tests)
#endif()

if(HEPMC_BUILD_EXAMPLES)
  set(HepMC_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")
  add_subdirectory(examples)
endif()

add_custom_target(move_headers ALL ${CMAKE_COMMAND} -DPREFIX=${CMAKE_BINARY_DIR}/outputs
                                   -DCOMPONENTS="headers"
                                   -P ${CMAKE_SOURCE_DIR}/cmake/Modules/local_install.cmake )

# Generate config CMake files to be used by other projects
#

include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR include )
set(LIB_INSTALL_DIR lib)
set(CONFIG_INSTALL_DIR cmake )

configure_package_config_file(cmake/Templates/HepMCConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/outputs/cmake/HepMCConfig.cmake
                                 INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
                                 PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/outputs/cmake/HepMCConfig.cmake 
           DESTINATION ${CONFIG_INSTALL_DIR} )
