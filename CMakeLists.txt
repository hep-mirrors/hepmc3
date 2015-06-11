cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#----------------------------------------------------------------------------
project(HepMC3)
#---Enable FORTRAN (unfortunatelly is not not possible in all cases)-------------------------------
if(NOT CMAKE_GENERATOR STREQUAL Xcode)
  enable_language(Fortran OPTIONAL)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

#---Setting HepMC Build type-----------------------------------------------
if(CMAKE_BUILD_TYPE)
  message(STATUS "HepMC3 Build type: ${CMAKE_BUILD_TYPE}")
else(CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "HepMC3 Build type not specified, use: ${CMAKE_BUILD_TYPE}")
endif(CMAKE_BUILD_TYPE)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

#-------------Options-------------------------------------------------------

option(HEPMC_ENABLE_CPP11 "Enables building with C++11." ON)
option(HEPMC_ENABLE_ROOTIO "Enables building with ROOT O/I." ON)
option(HEPMC_BUILD_EXAMPLES "Enables building of examples." OFF)

message(STATUS "HepMC: HEPMC_ENABLE_CPP11      ${HEPMC_ENABLE_CPP11}")
message(STATUS "HepMC: HEPMC_ENABLE_ROOTIO     ${HEPMC_ENABLE_ROOTIO}") 
message(STATUS "HepMC: HEPMC_BUILD_EXAMPLES    ${HEPMC_BUILD_EXAMPLES}") 


if(HEPMC_ENABLE_ROOTIO)
  if( EXISTS "$ENV{ROOTSYS}" )
    message(STATUS "HepMC: ROOTSYS provided $ENV{ROOTSYS}")
  elseif(ROOT_DIR)
    message(STATUS "HepMC: ROOT_DIR provided ${ROOT_DIR}")
  else() 
    message(STATUS "HepMC warning: Building of ROOTIO requested but no ROOTSYS or ROOT_DIR provided")
    message(STATUS "HepMC warning: Skipping HepMC ROOT IO libraries")
    set(HEPMC_ENABLE_ROOTIO OFF)
  endif()
endif()

# - Prepend our own CMake Modules to the search path
# This allows us to customize, add, and factor out functionality
# NB: if our custom modules include others that we don't supply, those in the
# base path will be used, so watch for incompatibilities!!
#
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/Modules
    ${CMAKE_MODULE_PATH})

#---Compilation and Link options---------------------------------------------

if(CMAKE_SYSTEM_NAME MATCHES Darwin)
  set(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS} -flat_namespace -single_module -undefined dynamic_lookup")
endif()

set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3 -Wall")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2")

# fortran flags needed for some tests and examples
set(CMAKE_Fortran_FLAGS            "-fno-automatic -fno-backslash -ffixed-line-length-132")

#----------------------------------------------------------------------------

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
  #--- buiding without C++11 and with Boost
  find_package(Boost 1.40 REQUIRED)
  message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()


message(STATUS "HepMC: Fortran FLAGS ${CMAKE_Fortran_FLAGS}")
message(STATUS "HepMC: CXX FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}  " )

#### RPATH handling #####

set(CMAKE_MACOSX_RPATH 1)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


#---Optional build of rootIO-------------------------------------------------

if(HEPMC_ENABLE_ROOTIO)
  set (ROOT_FIND_COMPONENTS RIO Core)
  # usage of ROOTConfig.cmake is disabled for the moment.
  find_file(ROOT_CONFIG 1ROOTConfig.cmake HINTS $ENV{ROOTSYS} ${ROOT_DIR} NO_DEFAULT_PATH PATH_SUFFIXES cmake)
  if (ROOT_CONFIG)
     message(STATUS "ROOTConfig.cmake found and will be used: ${ROOT_CONFIG}")
     include(${ROOT_CONFIG})
     message(STATUS "ROOT use file: ${ROOT_USE_FILE}")
     include(${ROOT_USE_FILE})
     if(ROOT_LIBRARIES AND ROOT_INCLUDE_DIRS)
       set(ROOT_FOUND TRUE)
     endif()
  else()
     message(STATUS "ROOTConfig.cmake not found, trying to use FindROOT.cmake...")
     find_package(ROOT QUIET REQUIRED COMPONENTS Core RIO)
  endif()

  message(STATUS "ROOT includes: ${ROOT_INCLUDE_DIRS}")
  message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")
  message(STATUS "ROOT_FOUND: ${ROOT_FOUND}")

  if(ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    add_definitions(-DHEPMC_ROOTIO)
  else()
    message(STATUS "HepMC warning: ROOT needed for ROOTIO but not found")
    message(STATUS "HepMC warning: Skipping HepMC ROOT IO libraries")
    set(HEPMC_ENABLE_ROOTIO OFF)
  endif()
endif()


#---Select sources for the various libraries---------------------------------
file(GLOB hepmc3_sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/Search/*.cc)

add_library(objlib OBJECT ${hepmc3_sources})
include_directories(include)
# shared libraries need PIC:
set_property(TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(HepMC SHARED $<TARGET_OBJECTS:objlib>)
add_library(HepMC_static STATIC $<TARGET_OBJECTS:objlib>)
target_link_libraries(HepMC_static ${ROOT_LIBRARIES} )
target_link_libraries(HepMC ${ROOT_LIBRARIES} )

set_target_properties(HepMC PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)
set_target_properties(HepMC_static PROPERTIES OUTPUT_NAME HepMC ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)

#---Install build products---------------------------------------------------
install(TARGETS HepMC DESTINATION lib)
install(TARGETS HepMC_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include COMPONENT headers)


if(ROOT_FOUND)
   add_subdirectory(rootIO)
endif()
# Add the examples subdirectories

if(HEPMC_BUILD_EXAMPLES)
  set(HepMC_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")
  add_subdirectory(examples)
endif()

add_custom_target(move_headers ALL ${CMAKE_COMMAND} -DPREFIX=${CMAKE_BINARY_DIR}/outputs
                                   -DCOMPONENTS="headers"
                                   -P ${CMAKE_SOURCE_DIR}/cmake/Modules/local_install.cmake )

#------------------------------------------------------------------------------
# Add the validation subdirectories
if(HEPMC_ENABLE_VALIDATION)
  add_subdirectory(validation)
endif()


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
