cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#----------------------------------------------------------------------------
project(HepMC3)
set(PROJECT_VERSION 3.01.01)


set(CMAKE_VERBOSE_MAKEFILE ON)

#---Setting HepMC3 Build type-----------------------------------------------
if(CMAKE_BUILD_TYPE)
  message(STATUS "HepMC3 Build type: ${CMAKE_BUILD_TYPE}")
else(CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "HepMC3 Build type not specified, use: ${CMAKE_BUILD_TYPE}")
endif(CMAKE_BUILD_TYPE)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

#-------------Options-------------------------------------------------------

option(HEPMC3_ENABLE_SEARCH "Enables building of search library" ON)
option(HEPMC3_ENABLE_ROOTIO "Enables building with ROOT O/I." ON)
option(HEPMC3_BUILD_EXAMPLES "Enables building of examples." OFF)
option(HEPMC3_ENABLE_TEST "Enables building of tests." OFF)
option(HEPMC3_INSTALL_INTERFACES "Enables installation of HepMC3 interfaces." OFF)


message(STATUS "HepMC3: HEPMC3_ENABLE_SEARCH         ${HEPMC3_ENABLE_SEARCH}") 
message(STATUS "HepMC3: HEPMC3_ENABLE_ROOTIO         ${HEPMC3_ENABLE_ROOTIO}") 
message(STATUS "HepMC3: HEPMC3_BUILD_EXAMPLES        ${HEPMC3_BUILD_EXAMPLES}") 
message(STATUS "HepMC3: HEPMC3_ENABLE_TEST           ${HEPMC3_ENABLE_TEST}") 
message(STATUS "HepMC3: HEPMC3_INSTALL_INTERFACES    ${HEPMC3_INSTALL_INTERFACES}")

if(HEPMC3_ENABLE_ROOTIO)
  if( EXISTS "$ENV{ROOTSYS}" )
    message(STATUS "HepMC3: ROOTSYS provided $ENV{ROOTSYS}")
  elseif(ROOT_DIR)
    message(STATUS "HepMC3: ROOT_DIR provided ${ROOT_DIR}")
  else() 
    message(STATUS "HepMC3 warning: Building of ROOTIO requested but no ROOTSYS or ROOT_DIR provided")
    message(STATUS "HepMC3 warning: Build will use ROOTConfig.cmake or FindROOT.cmake")
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

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("/Zc:__cplusplus" COMPILER_SUPPORTS_ZCPLUSPLUS)
if(COMPILER_SUPPORTS_ZCPLUSPLUS)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()
endif()

if ( "${CMAKE_CXX_COMPILE_FEATURES}"  MATCHES "cxx_range_for" )
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has cxx_range_for support.")
endif()



message(STATUS "HepMC3: CXX FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}  " )


#This module respects HFS, e.g. defines lib or lib64 when it is needed. 
include("GNUInstallDirs")
#### RPATH handling #####

set(CMAKE_MACOSX_RPATH 1)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
ENDIF("${isSystemDir}" STREQUAL "-1")


#---Optional build of rootIO-------------------------------------------------

if(HEPMC3_ENABLE_ROOTIO)
  set (ROOT_FIND_COMPONENTS Core RIO Tree)
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  list(APPEND CMAKE_PREFIX_PATH ${ROOT_DIR})
  find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)
  message(STATUS "ROOT_USE_FILE ${ROOT_USE_FILE}")
  include(${ROOT_USE_FILE})

  message(STATUS "ROOT includes: ${ROOT_INCLUDE_DIRS}")
  message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")
  message(STATUS "ROOT_FOUND: ${ROOT_FOUND}")

  if(ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    add_definitions(-DHEPMC3_ROOTIO)
    set(HEPMC3_ROOTIO_LIBRARY "-lHepMC3rootIO" )
  else()
    message(STATUS "HepMC3 warning: ROOT needed for ROOTIO but not found")
    message(STATUS "HepMC3 warning: Skipping HepMC ROOT IO libraries")
    set(HEPMC3_ENABLE_ROOTIO OFF)
  endif()
endif()



########################################################################
## Core
########################################################################
set( hepmc3core_sources 
${PROJECT_SOURCE_DIR}/src/GenCrossSection.cc 
${PROJECT_SOURCE_DIR}/src/GenParticle.cc  
${PROJECT_SOURCE_DIR}/src/GenVertex.cc      
${PROJECT_SOURCE_DIR}/src/Print.cc             
${PROJECT_SOURCE_DIR}/src/Setup.cc             
${PROJECT_SOURCE_DIR}/src/WriterHEPEVT.cc
${PROJECT_SOURCE_DIR}/src/GenEvent.cc         
${PROJECT_SOURCE_DIR}/src/GenPdfInfo.cc   
${PROJECT_SOURCE_DIR}/src/HEPEVT_Wrapper.cc  
${PROJECT_SOURCE_DIR}/src/ReaderAscii.cc        
${PROJECT_SOURCE_DIR}/src/ReaderHEPEVT.cc   
${PROJECT_SOURCE_DIR}/src/WriterAscii.cc
${PROJECT_SOURCE_DIR}/src/GenHeavyIon.cc      
${PROJECT_SOURCE_DIR}/src/GenRunInfo.cc   
${PROJECT_SOURCE_DIR}/src/LHEFAttributes.cc  
${PROJECT_SOURCE_DIR}/src/ReaderAsciiHepMC2.cc  
${PROJECT_SOURCE_DIR}/src/ReaderLHEF.cc     
${PROJECT_SOURCE_DIR}/src/WriterAsciiHepMC2.cc
)

#We need to export all symbols on WIn to build .lib files
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(objlibcore OBJECT ${hepmc3core_sources})
# shared libraries need PIC
set_property(TARGET objlibcore PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlibcore PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(HepMC3 SHARED $<TARGET_OBJECTS:objlibcore>)
add_library(HepMC3_static STATIC  $<TARGET_OBJECTS:objlibcore>)

set_target_properties(HepMC3 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
set_target_properties(HepMC3 PROPERTIES SOVERSION 1)
if (WIN32)
set_target_properties(HepMC3_static PROPERTIES OUTPUT_NAME HepMC3-static ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
elseif()
set_target_properties(HepMC3_static PROPERTIES OUTPUT_NAME HepMC3 ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
endif()
configure_file(HepMC3-config.in  ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/HepMC3-config )

#---Install build products---------------------------------------------------
install(TARGETS HepMC3 DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)
install(TARGETS HepMC3_static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT devel)

install(FILES  ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/HepMC3-config  DESTINATION  ${CMAKE_INSTALL_BINDIR} COMPONENT libs   PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Generate config CMake files to be used by other projects
#

include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR include )
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
set(CONFIG_INSTALL_DIR share/HepMC3/cmake )

configure_package_config_file(cmake/Templates/HepMC3Config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config.cmake
                                 INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
                                 PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config.cmake  DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT devel)

########################################################################
## Optional packages
########################################################################
#Search engine
if(HEPMC3_ENABLE_SEARCH)
  add_subdirectory(search)
endif()

#Interfaces for some MCEGs
if (HEPMC3_INSTALL_INTERFACES)
  install(DIRECTORY interfaces DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/../ COMPONENT devel)
endif()

# ROOT support
if(ROOT_FOUND)
   add_subdirectory(rootIO)
endif()

# Add the examples subdirectories
if(HEPMC3_BUILD_EXAMPLES)
  set(HepMC_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")
  add_subdirectory(examples)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT examples  PATTERN  "*config"  EXCLUDE)
endif()

#test suite
if(HEPMC3_ENABLE_TEST)
  ENABLE_TESTING()
  add_subdirectory(test)
endif()


########################################################################
## CPACK
########################################################################
#General CPACK settings
INCLUDE(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "HepMC3")
set(CPACK_PACKAGE_VENDOR "HepMC3 Developers")
#Should the version be defined above?
set(CPACK_PACKAGE_VERSION 3)
set(CPACK_PACKAGE_VERSION_MAJOR 3)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_FILE_NAME "HepMC3")
set(CPACK_PACKAGE_DESCRIPTION "Event record library for high energy physics Monte Carlo")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
SET(CPACK_INSTALL_PREFIX "/usr")
SET(CPACK_OUTPUT_FILE_PREFIX binpackages)


SET(CPACK_BINARY_RPM "ON")
#RPM-specific settings
set(CPACK_RPM_PACKAGE_DEBUG ON)
set(CPACK_RPM_PACKAGE_NAME "HepMC3")
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_LICENSE "GPL")
set(CPACK_RPM_PACKAGE_GROUP "General")
set(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${CPACK_PACKAGE_RELEASE}")
set(CPACK_RPM_libs_PACKAGE_SUMMARY  "HepMC3 libraries")
set(CPACK_RPM_devel_PACKAGE_SUMMARY "Headers for HepMC3 libraries")
set(CPACK_RPM_search_libs_PACKAGE_SUMMARY  "HepMC3 headers for event navigation")
set(CPACK_RPM_search_devel_PACKAGE_SUMMARY  "HepMC3 libraries for event navigation")
set(CPACK_RPM_examples_PACKAGE_SUMMARY "Examples for HepMC3 library")
set(CPACK_RPM_libs_PACKAGE_DESCRIPTION  ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_devel_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_search_libs_PACKAGE_DESCRIPTION  ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_search_devel_PACKAGE_DESCRIPTION  ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_examples_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_devel_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_NAME}-libs")
set(CPACK_RPM_search_devel_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_NAME}-search-libs")
set(CPACK_RPM_examples_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_NAME}-libs, ${CPACK_RPM_PACKAGE_NAME}-rootIO-libs, ${CPACK_RPM_PACKAGE_NAME}-search-libs")
#Everything that goes before  HepMC , i.e. top level directories,should be excluded
#This feature is working fine only in the recent CPackRPM module -- see cmake/modules directory.
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CPACK_PACKAGING_INSTALL_PREFIX} ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}  ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}  ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR} )



SET(CPACK_BINARY_DEB "ON")
#DEB-specific settings
set(CPACK_DEBIAN_PACKAGE_NAME "HepMC3")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_LICENSE "GPL")
set(CPACK_DEBIAN_PACKAGE_GROUP "General")
set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${CPACK_PACKAGE_RELEASE}")
set(CPACK_DEBIAN_devel_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}-libs")
set(CPACK_DEBIAN_search_devel_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}-search-libs")
set(CPACK_DEBIAN_examples_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}-libs, ${CPACK_DEBIAN_PACKAGE_NAME}-rootIO-libs")
set(CPACK_DEBIAN_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CPACK_PACKAGING_INSTALL_PREFIX} ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}  ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}  ${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR} )
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "HepMC3 Developers")


#NSIS-specific settings
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING)

if(ROOT_FOUND)
set(CPACK_COMPONENTS_ALL   devel libs rootIO-devel rootIO-libs   search-libs   search-devel  examples)
set(CPACK_RPM_rootIO-libs_PACKAGE_SUMMARY  "HepMC3 libraries with ROOT I/O support")
set(CPACK_RPM_rootIO-devel_PACKAGE_SUMMARY "Headers for HepMC3 libraries with ROOT I/O support")
set(CPACK_RPM_rootIO-libs_PACKAGE_DESCRIPTION  ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_rootIO-devel_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_rootIO-libs_PACKAGE_REQUIRES   "root, ${CPACK_RPM_PACKAGE_NAME}-libs")
set(CPACK_RPM_rootIO-devel_PACKAGE_REQUIRES   "${CPACK_RPM_PACKAGE_NAME}-devel, ${CPACK_RPM_PACKAGE_NAME}-rootIO-libs")
SET(CPACK_DEBIAN_rootIO-libs_PACKAGE_DEPENDS "libroot, ${CPACK_DEBIAN_PACKAGE_NAME}-libs")
SET(CPACK_DEBIAN_rootIO-devel_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}-devel, ${CPACK_DEBIAN_PACKAGE_NAME}-rootIO-libs")
else()
set(CPACK_COMPONENTS_ALL   devel libs  search-libs   search-devel )
endif()
INCLUDE(CPack)
