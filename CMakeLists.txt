cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#----------------------------------------------------------------------------
project(HepMC3)
set(PROJECT_VERSION 3.02.03)
SET(HEPMC3_VERSION 3.02.03)
SET(HEPMC3_VERSION_MAJOR  3)
SET(HEPMC3_VERSION_MINOR  2)
SET(HEPMC3_VERSION_PATCH  3)

set(CMAKE_VERBOSE_MAKEFILE ON)
#This module respects HFS, e.g. defines lib or lib64 when it is needed.
include("GNUInstallDirs")
#---Setting HepMC3 Build type-----------------------------------------------
if(CMAKE_BUILD_TYPE)
  message(STATUS "HepMC3 Build type: ${CMAKE_BUILD_TYPE}")
else(CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "HepMC3 Build type not specified, use: ${CMAKE_BUILD_TYPE}")
endif(CMAKE_BUILD_TYPE)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

#-------------Options-------------------------------------------------------

option(HEPMC3_ENABLE_SEARCH      "Enables building of search library" ON)
option(HEPMC3_ENABLE_ROOTIO      "Enables building with ROOT O/I." ON)
option(HEPMC3_ENABLE_PYTHON      "Enables building of Python  bindings, master switch" ON)
option(HEPMC3_BUILD_DOCS         "Enables building of documentation" OFF)
option(HEPMC3_BUILD_EXAMPLES     "Enables building of examples." OFF)
option(HEPMC3_INSTALL_EXAMPLES   "Enables installation of examples." ON)
option(HEPMC3_ENABLE_TEST        "Enables building of tests." OFF)
option(HEPMC3_INSTALL_INTERFACES "Enables installation of HepMC3 interfaces to MC tools." OFF)
option(HEPMC3_BUILD_STATIC_LIBS  "Enables building of static libraties." ON)

if (NOT HEPMC3_PYTHON_VERSIONS)
  set(HEPMC3_PYTHON_VERSIONS "2.X,3.X")
endif()

if (NOT HEPMC3_ROOTIO_INSTALL_LIBDIR)
  set(HEPMC3_ROOTIO_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
endif()

message(STATUS "HepMC3: HEPMC3_ENABLE_SEARCH         ${HEPMC3_ENABLE_SEARCH}")
message(STATUS "HepMC3: HEPMC3_ENABLE_ROOTIO         ${HEPMC3_ENABLE_ROOTIO}")
message(STATUS "HepMC3: HEPMC3_ENABLE_PYTHON         ${HEPMC3_ENABLE_PYTHON}")
message(STATUS "HepMC3: HEPMC3_PYTHON_VERSIONS       ${HEPMC3_PYTHON_VERSIONS}")
message(STATUS "HepMC3: HEPMC3_ENABLE_TEST           ${HEPMC3_ENABLE_TEST}")
message(STATUS "HepMC3: HEPMC3_BUILD_DOCS            ${HEPMC3_BUILD_DOCS}")
message(STATUS "HepMC3: HEPMC3_BUILD_EXAMPLES        ${HEPMC3_BUILD_EXAMPLES}")
message(STATUS "HepMC3: HEPMC3_INSTALL_INTERFACES    ${HEPMC3_INSTALL_INTERFACES}")
message(STATUS "HepMC3: HEPMC3_ROOTIO_INSTALL_LIBDIR ${HEPMC3_ROOTIO_INSTALL_LIBDIR}")



if(${CMAKE_VERSION} VERSION_LESS "3.0.0")
  message(STATUS "HepMC3: You are using CMake of version<3.0.0. The support for old CMake versions will be removed soon.")
if(HEPMC3_ENABLE_TEST)
  message(STATUS "HepMC3: HEPMC3_ENABLE_TEST is enabled. This requires CMake of version 3.0.0 or higher")
  message(STATUS "HepMC3: Please use a newer CMake or disable the tests setting -DHEPMC3_ENABLE_TEST:BOOL=OFF")
endif()
if(HEPMC3_ENABLE_PYTHON)
  message(STATUS "HepMC3: HEPMC3_ENABLE_PYTHON is enabled. This requires CMake of version 3.0.0 or higher.")
  message(STATUS "HepMC3: Please use a newer CMake or disable the python bindings setting -DHEPMC3_ENABLE_PYTHON:BOOL=OFF")
endif()
if(HEPMC3_ENABLE_ROOTIO)
  message(STATUS "HepMC3: HEPMC3_ENABLE_ROOTIO is enabled. This requires CMake of version 3 or higher, depending on the used ROOT version.")
  message(STATUS "HepMC3: Please use a newer CMake or disable the ROOT I/O interface setting -DHEPMC3_ENABLE_ROOTIO:BOOL=OFF")
endif()
if(HEPMC3_BUILD_EXAMPLES)
  message(STATUS "HepMC3: HEPMC3_BUILD_EXAMPLES is enabled. This requires CMake of version 3 or higher.")
  message(STATUS "HepMC3: Please use a newer CMake or disable the building of examples setting -DHEPMC3_BUILD_EXAMPLES:BOOL=OFF")
endif()
if(HEPMC3_ENABLE_TEST OR HEPMC3_ENABLE_PYTHON OR HEPMC3_ENABLE_ROOTIO OR HEPMC3_BUILD_EXAMPLES)
  message(FATAL_ERROR "HepMC3: The CMake is too old for the selected configuration.  Exit.")
endif()
endif()


if(HEPMC3_ENABLE_ROOTIO)
  if( EXISTS "$ENV{ROOTSYS}" )
    message(STATUS "HepMC3: ROOTSYS provided $ENV{ROOTSYS}")
  elseif(ROOT_DIR)
    message(STATUS "HepMC3: ROOT_DIR provided ${ROOT_DIR}")
  else()
    message(STATUS "HepMC3: Warning: Building of ROOTIO requested but no ROOTSYS or ROOT_DIR provided")
    message(STATUS "HepMC3: Warning: Build will use ROOTConfig.cmake or FindROOT.cmake")
  endif()
endif()

# - Prepend our own CMake Modules to the search path
# This allows us to customize, add, and factor out functionality
# NB: if our custom modules include others that we don't supply, those in the
# base path will be used, so watch for incompatibilities!!
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

include(CheckCXXCompilerFlag)
#This is a workaround for ROOT+Cmake problem.
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_-std=c++11)
if(COMPILER_SUPPORTS_-std=c++11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
#This is a workaround for wrong flag.
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI")
    set(CMAKE_CXX_EXTENSIONS ON)
  endif()
endif()

#Workarounds for nvc++
if (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI")
  #Templates
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc,--pending_instantiations=0 ")
  if(UNIX AND CMAKE_CXX_COMPILER_VERSION GREATER 20 )
    # exception_ptr on old gcc
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GCC_ATOMIC_TEST_AND_SET_TRUEVAL=1 -D__GCC_ATOMIC_INT_LOCK_FREE=3")
  endif()
endif()

if (MSVC)
#This is a workaround for VC memory problem and false positive warnings.
set(MSVC_CXX_FLAGS_TO_CHECK "/bigobj" "/EHsc" "/D_CRT_SECURE_NO_WARNINGS" "/wd4267" "/wd4244" "/wd4477")
foreach(fl ${MSVC_CXX_FLAGS_TO_CHECK})
  CHECK_CXX_COMPILER_FLAG(${fl} COMPILER_SUPPORTS_${fl})
  if(COMPILER_SUPPORTS_${fl})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
  endif()
endforeach(fl ${MSVC_CXX_FLAGS_TO_CHECK})
endif()


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
  set(ROOT_CXX_FLAGS_BEFORE_ROOT "${ROOT_CXX_FLAGS}")
  find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)
  if(ROOT_FOUND)
    set(CMAKE_CXX_FLAGS_BEFORE_ROOT "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_BEFORE_ROOT "${CMAKE_C_FLAGS}")
    set(CMAKE_Fortran_FLAGS_BEFORE_ROOT "${CMAKE_Fortran_FLAGS}")
    include(${ROOT_USE_FILE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_BEFORE_ROOT}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_BEFORE_ROOT}")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS_BEFORE_ROOT}")
    if (ROOT_CXX_FLAGS_BEFORE_ROOT)
    message(STATUS "Manualy set ROOT flags ${ROOT_CXX_FLAGS_BEFORE_ROOT}")
    set( ROOT_CXX_FLAGS "${ROOT_CXX_FLAGS_BEFORE_ROOT}")
    endif()
    if (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
    add_definitions(-DANSICPP)
    endif()
    message(STATUS "ROOT_FOUND:     ${ROOT_FOUND}")
    message(STATUS "ROOT_USE_FILE ${ROOT_USE_FILE}")
    message(STATUS "ROOT includes:  ${ROOT_INCLUDE_DIRS}")
    message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")
    message(STATUS "ROOT definitions: ${ROOT_DEFINITIONS}")
    include_directories(${ROOT_INCLUDE_DIRS})
    add_definitions(-DHEPMC3_ROOTIO)
    string(REPLACE " " ";" ROOT_CXX_FLAGS_LIST ${ROOT_CXX_FLAGS})
    foreach(fl ${ROOT_CXX_FLAGS_LIST})
    CHECK_CXX_COMPILER_FLAG(${fl} COMPILER_SUPPORTS_${fl})
    if(COMPILER_SUPPORTS_${fl})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
    endif()
    endforeach(fl ${ROOT_CXX_FLAGS_LIST})
    set(HEPMC3_ROOTIO_LIBRARY "-lHepMC3rootIO" )
  else()
    message(STATUS "HepMC3 warning: ROOT needed for ROOTIO but not found")
    message(STATUS "HepMC3 warning: Skipping HepMC ROOT IO libraries")
    set(HEPMC3_ENABLE_ROOTIO OFF)
  endif()
endif()



message(STATUS "HepMC3: CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID} " )
message(STATUS "HepMC3: CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}  " )
message(STATUS "HepMC3: CMAKE_C_COMPILER_ID ${CMAKE_C_COMPILER_ID} " )

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
  ${PROJECT_SOURCE_DIR}/src/ReaderPlugin.cc
  ${PROJECT_SOURCE_DIR}/src/WriterPlugin.cc
)

#We need to export all symbols on WIn to build .lib files
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(HepMC3 SHARED ${hepmc3core_sources})
target_include_directories(HepMC3 PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(${CMAKE_VERSION} VERSION_LESS "3.0.0")
#Problems with generator expressions for cmake2
  set_target_properties(HepMC3 PROPERTIES OUTPUT_NAME HepMC3  SOVERSION 3
                                               ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/
                                               LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/
                                               RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
else()
  set_target_properties(HepMC3 PROPERTIES OUTPUT_NAME HepMC3  SOVERSION 3
                                               ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
                                               LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
                                               RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>)
endif()
if (UNIX)
  target_link_libraries(HepMC3 PRIVATE dl)
endif()
#---Install build products---------------------------------------------------
install(TARGETS HepMC3 DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)

if (HEPMC3_BUILD_STATIC_LIBS)
  add_library(HepMC3_static STATIC  ${hepmc3core_sources})
  target_include_directories(HepMC3_static PUBLIC ${PROJECT_SOURCE_DIR}/include)
  target_compile_definitions(HepMC3_static PRIVATE NO_DECLSPEC_hepevtptr)
  if(${CMAKE_VERSION} VERSION_LESS "3.0.0")
#Problems with generator expressions for cmake2
    set_target_properties(HepMC3_static PROPERTIES OUTPUT_NAME HepMC3-static
                                               ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/
                                               LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/
                                               RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
  else()
    set_target_properties(HepMC3_static PROPERTIES OUTPUT_NAME HepMC3-static
                                               ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
                                               LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
                                               RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>)
  endif()
  if (UNIX)
    target_link_libraries(HepMC3_static PRIVATE dl)
  endif()
#---Install build products---------------------------------------------------
  install(TARGETS HepMC3_static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT devel)

# Generate config CMake files to be used by other projects
#

include(CMakePackageConfigHelpers)

set(INTERFACES_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/HepMC3/interfaces)
set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/HepMC3/cmake)

configure_package_config_file(cmake/Templates/HepMC3Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config.cmake
  INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
            CMAKE_INSTALL_LIBDIR
            HEPMC3_ROOTIO_INSTALL_LIBDIR
            INTERFACES_INSTALL_DIR)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config-version.cmake COMPATIBILITY SameMajorVersion VERSION ${PROJECT_VERSION})

configure_package_config_file(HepMC3-config.in
  ${CMAKE_CURRENT_BINARY_DIR}/outputs/bin/HepMC3-config
  INSTALL_DESTINATION ${CMAKE_INSTALL_BINDIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
            CMAKE_INSTALL_LIBDIR
            HEPMC3_ROOTIO_INSTALL_LIBDIR
            INTERFACES_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/outputs/share/HepMC3/cmake/HepMC3Config-version.cmake
        DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT devel)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/outputs/bin/HepMC3-config
        DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libs
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

########################################################################
## Optional packages
########################################################################
#Search engine
if(HEPMC3_ENABLE_SEARCH)
  add_subdirectory(search)
endif()

#Interfaces for some MCEGs
if (HEPMC3_INSTALL_INTERFACES)
  install(DIRECTORY interfaces/ DESTINATION ${INTERFACES_INSTALL_DIR} COMPONENT devel)
endif()

# ROOT support
if(ROOT_FOUND)
   add_subdirectory(rootIO)
endif()

option(USE_INTERFACE_FROM_PYTHIA8 "Use Pythia8ToHepMC3.h from Pythia8" OFF)
# Add the examples subdirectories
if(HEPMC3_BUILD_EXAMPLES)
  set(HepMC_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")
  add_subdirectory(examples)
endif()
if (HEPMC3_INSTALL_EXAMPLES)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ DESTINATION  ${CMAKE_INSTALL_DOCDIR}/examples COMPONENT doc  FILES_MATCHING PATTERN "*"   PATTERN "*CMakeFiles*" EXCLUDE    PATTERN "Makefile" EXCLUDE  PATTERN "*cmake_install.cmake*" EXCLUDE  PATTERN "*libAnalysis*" EXCLUDE )
endif()

if(HEPMC3_BUILD_DOCS)
  add_subdirectory(doc)
endif()

#test suite
if(HEPMC3_ENABLE_TEST)
  ENABLE_TESTING()
  add_subdirectory(test)
endif()

#Python bindings
if(HEPMC3_ENABLE_PYTHON)
  add_subdirectory(python)
endif()

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "cmake/Templates/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)
  add_custom_target(uninstall  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
#This is for developers only
find_program (ASTYLE NAMES astyle)
if (ASTYLE)
  message(STATUS "HepMC3: Developers only: astyle found in ${ASTYLE}. Formating of C++ code is possible.")
  ADD_CUSTOM_TARGET(beauty
    COMMAND ${ASTYLE}  --keep-one-line-blocks  
    src/*cc  
    rootIO/src/*cc 
    search/src/*cc  
    examples/*/src/*cc 
    examples/*/include/*h
    test/*cc  
    test/*h 
    interfaces/*/include/*.h 
    interfaces/*/include/*/*.h 
    interfaces/*/src/*.cxx
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()
find_program (BLACK NAMES black)
if (BLACK)
  message(STATUS "HepMC3: Developers only: black found in ${BLACK}. Formating of Python code is possible.")
  ADD_CUSTOM_TARGET(beautypython COMMAND ${BLACK} -l 120  python/src/*py python/test/*py WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
endif()
find_program (CPPCHECK NAMES cppcheck)
if(CPPCHECK)
  message(STATUS "HepMC3: Developers only: cppcheck found in ${CPPCHECK}. Static analysis of C++ code is possible.")
  ADD_CUSTOM_TARGET(staticcheck
    COMMAND ${CPPCHECK} --force  --suppress=unusedFunction -v -x c++ --std=c++11 --enable=all
      -I./include -I./search/include -I./rootIO/include -I${ROOT_INCLUDE_DIRS}
      src/*.cc 
      search/src/*.cc 
      rootIO/src/*.cc 
      examples/*/src/*cc
      interfaces/*/include/*h
      interfaces/*/src/*cxx
      interfaces/*/src/*cc
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
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
set(CPACK_PACKAGE_VERSION_MINOR 2)
set(CPACK_PACKAGE_VERSION_PATCH 3)
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_FILE_NAME "HepMC3")
set(CPACK_PACKAGE_DESCRIPTION "Event record library for high energy physics Monte Carlo")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
SET(CPACK_INSTALL_PREFIX "/usr")
SET(CPACK_OUTPUT_FILE_PREFIX binpackages)

#NSIS-specific settings
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING)

set(CPACK_COMPONENTS_ALL   devel libs   )

if (HEPMC3_ENABLE_SEARCH)
  list(APPEND CPACK_COMPONENTS_ALL   searchlibs   searchdevel)
endif()

if (HEPMC3_ENABLE_PYTHON)
  list(APPEND CPACK_COMPONENTS_ALL   python)
  if (HEPMC3_ENABLE_SEARCH)
    list(APPEND CPACK_COMPONENTS_ALL   pythonsearch)
  endif()
endif()
if (HEPMC3_BUILD_DOCS)
  list(APPEND CPACK_COMPONENTS_ALL   doc)
endif()
if(ROOT_FOUND)
  list(APPEND CPACK_COMPONENTS_ALL   rootIOdevel rootIOlibs)
  if (HEPMC3_ENABLE_PYTHON)
    list(APPEND CPACK_COMPONENTS_ALL   pythonrootIO)
  endif()
endif()
INCLUDE(CPack)
