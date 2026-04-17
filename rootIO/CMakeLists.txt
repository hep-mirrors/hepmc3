#project(pyHepMC3 CXX)
#Should be fixed
cmake_minimum_required(VERSION 3.10.0...4.0.3)
if(${CMAKE_VERSION} VERSION_LESS "3.14.00")
  SET_PROPERTY (GLOBAL PROPERTY CMAKE_ROLE "PROJECT")
endif()
include(CheckIPOSupported)
set(CMAKE_VERBOSE_MAKEFILE ON)

option(USE_INSTALLED_HEPMC3 "Use HepMC3 rootIO as a standalone package" OFF)
if (USE_INSTALLED_HEPMC3)
  set(${PROJECT_NAME}_VERSION 3.3.1)
  set(THIS_PROJECT_NAME ${PROJECT_NAME})
  set(THIS_PROJECT_NAME_VERSION 3.3.1)
  option(HEPMC3_ENABLE_TEST "Enable tests" ON)


  find_package(HepMC3 ${${PROJECT_NAME}_VERSION} REQUIRED  )
  include(CheckCXXCompilerFlag)

  message(STATUS "HEPMC3_LIB: ${HEPMC3_LIB}")
  include(GNUInstallDirs)
  if (HEPMC3_ROOTIO_LIB)
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
    list(APPEND CMAKE_PREFIX_PATH ${ROOT_DIR})
    find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)
    if(ROOT_FOUND)  
      message(STATUS "HepMC3 rootIO: ROOT_FOUND=${ROOT_FOUND}")
      message(STATUS "HepMC3 rootIO: ROOT_USE_FILE=${ROOT_USE_FILE}")
      message(STATUS "HepMC3 rootIO: ROOT_INCLUDE_DIRS=${ROOT_INCLUDE_DIRS}")
      message(STATUS "HepMC3 rootIO: ROOT_LIBRARIES=${ROOT_LIBRARIES}")
      message(STATUS "HepMC3 rootIO: ROOT_DEFINITIONS=${ROOT_DEFINITIONS}")
      message(STATUS "HepMC3 rootIO: ROOT_CXX_FLAGS=${ROOT_CXX_FLAGS}")
      string(REPLACE "ROOTUseFile.cmake" "modules/RootNewMacros.cmake" ROOT_NEWMACROS_FILE "${ROOT_USE_FILE}")
      if(EXISTS "${ROOT_NEWMACROS_FILE}")
        include(${ROOT_NEWMACROS_FILE})
      endif()
      add_definitions(-DHEPMC3_ROOTIO)
# Use ROOT_CXX_STANDARD if defined, otherwise deduce from ROOT_CXX_FLAGS
      if(DEFINED ROOT_CXX_STANDARD)
        set(HEPMC3_CXX_STANDARD ${ROOT_CXX_STANDARD})
      else()
        string(REPLACE " " ";" ROOT_CXX_FLAGS_LIST ${ROOT_CXX_FLAGS})
        foreach(fl ${ROOT_CXX_FLAGS_LIST})
          if ("${fl}" MATCHES   ".*\\+\\+14$" OR "${fl}" MATCHES   ".*\\+\\+1y$" )
            set(HEPMC3_CXX_STANDARD 14)
          endif()
          if ("${fl}" MATCHES   ".*\\+\\+17$" OR "${fl}" MATCHES   ".*\\+\\+1z$" )
            set(HEPMC3_CXX_STANDARD 17)
          endif()
          if ("${fl}" MATCHES   ".*\\+\\+20$" OR "${fl}" MATCHES   ".*\\+\\+2a$" )
            set(HEPMC3_CXX_STANDARD 20)
          endif()
          if ("${fl}" MATCHES   ".*\\+\\+23$" OR "${fl}" MATCHES   ".*\\+\\+2b$" )
            set(HEPMC3_CXX_STANDARD 23)
          endif()
          if ("${fl}" MATCHES   ".*\\+\\+26$" OR "${fl}" MATCHES   ".*\\+\\+2c$" )
            set(HEPMC3_CXX_STANDARD 26)
          endif()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
        endforeach(fl ${ROOT_CXX_FLAGS_LIST})
      endif()
    endif()
  endif()
  if (NOT HEPMC3_CXX_STANDARD)
    set(HEPMC3_CXX_STANDARD 11)
  endif()
#Memory problem and false positive warnings.
  set(${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "")
  set(MSVC_CXX_FLAGS_TO_CHECK "/bigobj;/D_CRT_SECURE_NO_WARNINGS;/wd4267;/wd4244;/wd4477")
  set(PGI_CXX_FLAGS_TO_CHECK "-Wc,--pending_instantiations=0;-noswitcherror;--diag_suppress=code_is_unreachable;--diag_suppress=inline_gnu_noinline_conflict;--diag_suppress=declared_but_not_referenced")
  set(NVHPC_CXX_FLAGS_TO_CHECK "-Wc,--pending_instantiations=0;-noswitcherror;--diag_suppress=code_is_unreachable;--diag_suppress=inline_gnu_noinline_conflict;--diag_suppress=declared_but_not_referenced")
  set(Intel_CXX_FLAGS_TO_CHECK "-diag-disable=2196")
  set(IntelLLVM_CXX_FLAGS_TO_CHECK "-diag-disable=2196,-Wno-gnu-zero-variadic-macro-arguments,-Wno-unused-variable")
  set(GNU_CXX_FLAGS_TO_CHECK "-Wno-unused-value;-Wno-unused-but-set-variable;-fvisibility=hidden")
  set(Clang_CXX_FLAGS_TO_CHECK "-fsized-deallocation;-Wno-unused-value;-Wno-unused-but-set-variable;-fvisibility=hidden")
  set (HEPMC3_LOADABLE_LIBRARIES_DIR ${HEPMC3_LIB_DIR})
  message(STATUS "rootIOHepMC3 v${THIS_PROJECT_NAME_VERSION}")
else()
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-fvisibility=hidden")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-but-set-variable")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-variable")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-value")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "--diag_suppress=inline_gnu_noinline_conflict")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "--diag_suppress=declared_but_not_referenced")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-delete-abstract-non-virtual-dtor")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-gnu-zero-variadic-macro-arguments")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-old-style-cast")
  set(THIS_PROJECT_NAME rootIOHepMC3)
  set(THIS_PROJECT_NAME_VERSION ${HEPMC3_VERSION_MAJOR}.${HEPMC3_VERSION_MINOR}.${HEPMC3_VERSION_PATCH})
  set (HEPMC3_LOADABLE_LIBRARIES_DIR ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
endif()



if (MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
endif()
ROOT_GENERATE_DICTIONARY(G__HepMC3rootIO include/rootIO_Classes.hh LINKDEF include/LinkDef.hh NOINSTALL OPTIONS -inlineInputHeader)
set_source_files_properties(G__HepMC3rootIO.cxx PROPERTIES SKIP_LINTING ON)
add_library(HepMC3rootIO SHARED ${PROJECT_SOURCE_DIR}/rootIO/src/WriterRoot.cc ${PROJECT_SOURCE_DIR}/rootIO/src/ReaderRoot.cc ${PROJECT_SOURCE_DIR}/rootIO/src/WriterRootTree.cc ${PROJECT_SOURCE_DIR}/rootIO/src/ReaderRootTree.cc ${PROJECT_SOURCE_DIR}/rootIO/src/Streamers.cc G__HepMC3rootIO.cxx)
target_compile_definitions(HepMC3rootIO PUBLIC HEPMC3_ROOTIO)
set_property(TARGET HepMC3rootIO PROPERTY POSITION_INDEPENDENT_CODE 1)

target_link_libraries(HepMC3rootIO PUBLIC ROOT::Tree ROOT::RIO ROOT::Core HepMC3::HepMC3)

target_include_directories(HepMC3rootIO PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${ROOT_INCLUDE_DIRS})

set_target_properties(HepMC3rootIO PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
  EXPORT_NAME HepMC3::rootIO)
if (MSVC)
  set_target_properties(HepMC3rootIO PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>)
endif()
set_target_properties(HepMC3rootIO PROPERTIES SOVERSION 3)
# installs
install(TARGETS HepMC3rootIO
  EXPORT HepMC3rootIOTargets
  DESTINATION ${HEPMC3_ROOTIO_INSTALL_LIBDIR}
  COMPONENT rootIOlibs)
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/ DESTINATION  ${HEPMC3_ROOTIO_INSTALL_LIBDIR}  COMPONENT rootIOlibs  FILES_MATCHING  PATTERN "*.pcm"  PATTERN "*.rootmap" PATTERN "CMakeFiles" EXCLUDE   PATTERN "src" EXCLUDE  PATTERN "include" EXCLUDE)
install(DIRECTORY include/HepMC3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT rootIOdevel)

# Alias for packages that include this as a sub-project but
# want to use the namespaced target names
add_library(HepMC3::rootIO ALIAS HepMC3rootIO)

set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/HepMC3/cmake)
install(EXPORT HepMC3rootIOTargets
  DESTINATION ${CONFIG_INSTALL_DIR}
  COMPONENT rootIOdevel)
