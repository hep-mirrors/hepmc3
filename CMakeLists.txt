cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
# earlier cmake versions are unable to find all Boost configurations
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

#---Find Boost---------------------------------------------------------------

find_package(Boost 1.40 REQUIRED)
message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")

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

set(CMAKE_CXX_FLAGS                "-fPIC")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2")

# fortran flags needed for some tests and examples
set(CMAKE_Fortran_FLAGS            "-fno-automatic -fno-backslash -ffixed-line-length-132")

# Other options eventually used:
# -pg
# -DBOOST_DISABLE_ASSERTS   
# -DBOOST_SP_DISABLE_THREADS

# The way to add an option to all build types:
#add_definitions(-DBOOST_DISABLE_ASSERTS)

#--------------------------------------------------------------------------

include_directories(include ${Boost_INCLUDE_DIRS})

#---Select sources for the various libraries---------------------------------
file(GLOB hepmc3_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB hepmc3_sources_IO ${PROJECT_SOURCE_DIR}/src/IO/*.cc)
file(GLOB hepmc3_sources_Search ${PROJECT_SOURCE_DIR}/src/Search/*.cc)

#---Add libraries------------------------------------------------------------

add_library(HepMC SHARED ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
add_library(HepMC_static STATIC ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
set_target_properties(HepMC PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/library)
set_target_properties(HepMC_static PROPERTIES OUTPUT_NAME HepMC ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/library)

#---Install build products---------------------------------------------------
install(TARGETS HepMC DESTINATION lib)
install(TARGETS HepMC_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

#------------------------------------------------------------------------------
# Add the examples and tests subdirectories
if(HEPMC_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

if(HEPMC_BUILD_EXAMPLES)
  set(HepMC_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")
  add_subdirectory(examples)
endif()

if(HEPMC_BUILD_ROOTIO)
  add_subdirectory(rootio)
endif()
