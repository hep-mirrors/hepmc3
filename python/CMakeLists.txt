#project(pyHepMC3 CXX)
#Should be fixed
cmake_minimum_required(VERSION 3.5.0)
if(${CMAKE_VERSION} VERSION_LESS "3.14.00")
SET_PROPERTY (GLOBAL PROPERTY CMAKE_ROLE "PROJECT")
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)

option(USE_INSTALLED_HEPMC3 "Use HepMC3 bindings as a standalone package" OFF)
option(RERUN_BINDER "Rerun binder" OFF)
if (RERUN_BINDER)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
if (USE_INSTALLED_HEPMC3)
  set(${PROJECT_NAME}_VERSION 3.3.0)
  set(THIS_PROJECT_NAME ${PROJECT_NAME})
  set(THIS_PROJECT_NAME_VERSION 3.3.0)
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
      message(STATUS "HepMC3 python: ROOT_FOUND=${ROOT_FOUND}")
      message(STATUS "HepMC3 python: ROOT_USE_FILE=${ROOT_USE_FILE}")
      message(STATUS "HepMC3 python: ROOT_INCLUDE_DIRS=${ROOT_INCLUDE_DIRS}")
      message(STATUS "HepMC3 python: ROOT_LIBRARIES=${ROOT_LIBRARIES}")
      message(STATUS "HepMC3 python: ROOT_DEFINITIONS=${ROOT_DEFINITIONS}")
      message(STATUS "HepMC3 python: ROOT_CXX_FLAGS=${ROOT_CXX_FLAGS}")
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
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
        endforeach(fl ${ROOT_CXX_FLAGS_LIST})
      endif()
    endif()
  endif()
  if (HEPMC3_PROTOBUFIO_LIB)
    find_package(Protobuf 2.4 REQUIRED)
    add_definitions(-DHEPMC3_PROTOBUFIO)
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
  message(STATUS "pyHepMC3 v${THIS_PROJECT_NAME_VERSION}")
else()
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-fvisibility=hidden")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-but-set-variable")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-variable")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-unused-value")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "--diag_suppress=inline_gnu_noinline_conflict")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "--diag_suppress=declared_but_not_referenced")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-delete-abstract-non-virtual-dtor")
  list(APPEND ${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK "-Wno-gnu-zero-variadic-macro-arguments")
  set(THIS_PROJECT_NAME pyHepMC3)
  set(THIS_PROJECT_NAME_VERSION ${HEPMC3_VERSION_MAJOR}.${HEPMC3_VERSION_MINOR}.${HEPMC3_VERSION_PATCH})
  set (HEPMC3_LOADABLE_LIBRARIES_DIR ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
endif()
foreach(fl ${${CMAKE_CXX_COMPILER_ID}_CXX_FLAGS_TO_CHECK})
  CHECK_CXX_COMPILER_FLAG(${fl} CXX_COMPILER_SUPPORTS_${fl})
    if(CXX_COMPILER_SUPPORTS_${fl})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
    endif()
endforeach()

if(HEPMC3_ENABLE_SEARCH)
  add_definitions(-DHEPMC3_ENABLE_SEARCH)
endif()

set(autoBN       ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_0.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_1.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_2.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_4.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_5.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_6.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_7.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_8.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_9.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_10.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_11.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_12.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_13.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_14.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_15.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_16.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_17.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3_18.cpp
)
set(autoBNrootIO ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3rootIO.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3rootIO_0.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3rootIO_1.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3rootIO_2.cpp)
set(autoBNsearch ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3search.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3search_0.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3search_1.cpp)
set(autoBNprotobufIO ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3protobufIO.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3protobufIO_0.cpp )
set(BN       ${autoBN}       ${CMAKE_CURRENT_SOURCE_DIR}/src/binders.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ReaderuprootTree.cc)
set(BNrootIO ${autoBNrootIO} ${CMAKE_CURRENT_SOURCE_DIR}/src/rootIObinders.cpp)
set(BNsearch ${autoBNsearch} ${CMAKE_CURRENT_SOURCE_DIR}/src/searchbinders.cpp)
set(BNprotobufIO ${autoBNprotobufIO} ${CMAKE_CURRENT_SOURCE_DIR}/src/protobufIObinders.cpp)

foreach(_comp IN ITEMS " " ${HEPMC3_COMPONENTS})
  if (${_comp} STREQUAL " ")
    set(_dotcomp "") 
  else()
    set(_dotcomp ".${_comp}")   
  endif()
  configure_file("pyHepMC3${_dotcomp}.egg-info.in" "pyHepMC3${_dotcomp}.egg-info" IMMEDIATE @ONLY)
endforeach()
if (RERUN_BINDER AND ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  find_program (BINDER binder HINTS $ENV{HOME}/bin /usr/bin)

  execute_process(COMMAND ${BINDER} --version OUTPUT_VARIABLE BINDER_VERSION_STRING OUTPUT_STRIP_TRAILING_WHITESPACE) 
  string(REGEX REPLACE "[\r\n ]" ";" BINDER_VERSION_LIST "${BINDER_VERSION_STRING} foo 0.0.0")
  list(GET BINDER_VERSION_LIST 1 BINDER_VERSION)
  if (BINDER_VERSION VERSION_GREATER 1.4.0)
    message(STATUS "HepMC3 python: BINDER_VERSION=${BINDER_VERSION} is sufficient for the generation of bindings.")
  else()
    message(FATAL_ERROR "HepMC3 python: BINDER_VERSION=${BINDER_VERSION}<=1.4.0 is not sufficient for the generation of bindings.")
  endif()
  set(INCARG "")
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -v /dev/null -S ERROR_VARIABLE GCCSPEC OUTPUT_QUIET)
  STRING(REGEX REPLACE "\r?\n" ";" GCCSPECV2 "${GCCSPEC}")
  FOREACH(line ${GCCSPECV2})
    string(REGEX MATCH "^LIBRARY_PATH.*"  GCCSPECV3 "${line}")
    string(REPLACE "LIBRARY_PATH=/"  "/" GCCSPECV4 "${GCCSPECV3}")
    if (GCCSPECV4)
      STRING(REPLACE ":" ";--extra-arg=-I" INCARG "--extra-arg=-I${GCCSPECV4}")
      message(STATUS "HepMC3 python: INCARG=${INCARG}")
    endif()
  endforeach()
  if(BINDER)
    message(STATUS "HepMC3 python: binder found in ${BINDER}. Automatic generation of code for python bindings is possible.")
    foreach(_comp IN ITEMS " " ${HEPMC3_COMPONENTS} )
      set(EARG "")
      if (${_comp} STREQUAL " ")
        set(_comp "") 
      else()
        STRING(TOUPPER ${_comp} _COMP)
        set(EARG "--extra-arg=-DHEPMC3_${_COMP}")
      endif()
      configure_file("${_comp}includes.hpp.in" "${_comp}includes.hpp" IMMEDIATE @ONLY)
      add_custom_command(
        OUTPUT  ${autoBN${_comp}}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_comp}includes.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3${_comp}.binder
        COMMAND ${BINDER} --flat --max-file-size=20000
                          --skip-line-number
                          --root-module pyHepMC3${_comp}
                          --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/"
                          --config ${CMAKE_CURRENT_SOURCE_DIR}/src/pyHepMC3${_comp}.binder   
                            ${CMAKE_CURRENT_BINARY_DIR}/${_comp}includes.hpp
                          --extra-arg=-std=c++${HEPMC3_CXX_STANDARD}
                          --extra-arg=-I${PROJECT_SOURCE_DIR}/${_comp}/include
                          --extra-arg=-I${PROJECT_BINARY_DIR}/${_comp}
                          --extra-arg=-I${CMAKE_CURRENT_SOURCE_DIR}
                          --extra-arg=-I${CMAKE_CURRENT_SOURCE_DIR}/include
                          --extra-arg=-I${ROOT_INCLUDE_DIRS}
                           ${INCARG}
                           ${EARG}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM
        COMMENT "Generating ${autoBN${_comp}}"
      )
      add_custom_target(generate_${_comp} DEPENDS ${autoBN${_comp}})
    endforeach()
  else()
    message(FATAL_ERROR "HepMC3  python: binder not found and/or the C++ is not GNU!  Automatic generatin of code for python bindings is not possible.")
  endif()
endif()

macro(bindings verin verminor)
SET( Python_VERSION_MAJOR "0")
SET( Python_VERSION_MINOR "0")
string(REPLACE "pypy" "" ver "${verin}")
if (${verin} MATCHES pypy* )
  set(Python3_FIND_IMPLEMENTATIONS PyPy)
  set(IMPL PyPy)
  if(NOT (${CMAKE_VERSION} VERSION_GREATER "3.17"))
    message(STATUS "HepMC3 python: WARNING: Version of cmake ${CMAKE_VERSION} is too low to support PyPy. at least version 3.18 is required") 
  endif()
else()
  set(Python3_FIND_IMPLEMENTATIONS CPython)
  set(IMPL CPython)
endif()

#Compilation with CPython3
if (${ver} STREQUAL 3)
  unset(Python3_FOUND CACHE)
  unset(Python3_VERSION CACHE)
  unset(Python3_Interpreter_FOUND CACHE)
  unset(Python3_EXECUTABLE CACHE)
  unset(Python3_INCLUDE_DIR CACHE)
  unset(Python3_INCLUDE_DIRS CACHE)
  unset(Python3_LIBRARIES CACHE)
  unset(_Python3_EXECUTABLE CACHE) # Attempt to make FindPython reenterant
  unset(_Python3_INCLUDE_DIR CACHE) # Attempt to make FindPython reenterant
  unset(_Python3_INTERPRETER_PROPERTIES CACHE) # Attempt to make FindPython reenterant
  if (${verminor} STREQUAL X)
    find_package (Python3 3.6 COMPONENTS  Interpreter Development.Module)
  else()
    find_package (Python3 3.${verminor} EXACT COMPONENTS  Interpreter Development.Module)
  endif()
  if (Python3_VERSION AND Python3_INCLUDE_DIRS AND Python3_EXECUTABLE  ) #something is set
    SET( Python_VERSION       ${Python3_VERSION})
    SET( Python_VERSION_MAJOR ${Python3_VERSION_MAJOR})
    SET( Python_VERSION_MINOR ${Python3_VERSION_MINOR})
    SET( Python_LIBRARIES     ${Python3_LIBRARIES})
    SET( Python_SITEARCH      ${Python3_SITEARCH})
    SET( Python_EXECUTABLE    ${Python3_EXECUTABLE})
    SET( Python_INCLUDE_DIRS  ${Python3_INCLUDE_DIRS})
    unset(_Python3_EXECUTABLE CACHE) # Attempt to make FindPython reenterant
    unset(_Python3_INCLUDE_DIR CACHE) # Attempt to make FindPython reenterant
  unset(_Python3_INTERPRETER_PROPERTIES CACHE) # Attempt to make FindPython reenterant

  endif()
endif()


if ( Python_INCLUDE_DIRS AND Python_EXECUTABLE
    AND ((${Python_VERSION_MAJOR} STREQUAL ${ver} AND ${verminor} STREQUAL "X")
        OR "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}" STREQUAL "${ver}.${verminor}" ))
  message(STATUS "HepMC3 python: Python verson ${ver}.${verminor} found in ${Python_EXECUTABLE}. Python bindings generation is possible.")

  foreach(_comp IN ITEMS " " ${HEPMC3_COMPONENTS} )
    if (${_comp} STREQUAL " ")
      set(_comp "") 
      set(__comp "") 
    else()
      set(__comp HepMC3::${_comp})   
    endif()
    if (${ver} STREQUAL 3)
      Python3_add_library(${IMPL}pyHepMC3${_comp}${Python_VERSION} MODULE ${BN${_comp}})
      set_target_properties(${IMPL}pyHepMC3${_comp}${Python_VERSION} PROPERTIES  OUTPUT_NAME pyHepMC3${_comp})
    endif()
    if (${verin} MATCHES pypy* )
      execute_process( COMMAND ${Python_EXECUTABLE} -c "import  sys;v=sys.pypy_version_info;print(str(v[0])+str(v[1]))" 
      OUTPUT_VARIABLE PyPy_PyPy_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
      set_target_properties(${IMPL}pyHepMC3${_comp}${Python_VERSION}
      PROPERTIES PREFIX "" OUTPUT_NAME pyHepMC3${_comp}.pypy-${PyPy_PyPy_VERSION})
    endif()
    if (RERUN_BINDER)
      add_dependencies(${IMPL}pyHepMC3${_comp}${Python_VERSION} generate_${_comp})
    endif()
  
    set_target_properties(${IMPL}pyHepMC3${_comp}${Python_VERSION}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/$<0:>
        MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/$<0:>
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/$<0:>)
    target_link_libraries(${IMPL}pyHepMC3${_comp}${Python_VERSION}
        PUBLIC  HepMC3::HepMC3 ${__comp} )
    target_include_directories(${IMPL}pyHepMC3${_comp}${Python_VERSION}
        PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../include  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../ )
    add_custom_target(
        ${IMPL}foo${_comp}${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/
        COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/../src/${_comp}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/__init__.py
                )
  endforeach()

  message(STATUS "HepMC3 python: For this platforrm CMAKE_SHARED_LIBRARY_PREFIX=${CMAKE_SHARED_LIBRARY_PREFIX}, CMAKE_SHARED_LIBRARY_SUFFIX=${CMAKE_SHARED_LIBRARY_SUFFIX}")
  message(STATUS "HepMC3 python: For this platforrm CMAKE_IMPORT_LIBRARY_PREFIX=${CMAKE_IMPORT_LIBRARY_PREFIX}, CMAKE_IMPORT_LIBRARY_SUFFIX=${CMAKE_IMPORT_LIBRARY_SUFFIX}")

  if (WIN32)
#Python is insane
    if(HEPMC3_ENABLE_TEST)
    foreach(_comp IN ITEMS " " ${HEPMC3_COMPONENTS} )
       if (${_comp} STREQUAL " ")
         set(_comp "") 
       endif()
        message(STATUS "HepMC3 python: copying ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        message(STATUS "HepMC3 python: copying ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/test/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        message(STATUS "HepMC3 python: copying ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX}")
        message(STATUS "HepMC3 python: copying ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/test/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX}")
        add_custom_target(
        copy${_comp}${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND}  -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX}
        COMMAND ${CMAKE_COMMAND}  -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/test/${CMAKE_SHARED_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_SHARED_LIBRARY_SUFFIX}
        COMMAND ${CMAKE_COMMAND}  -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/${_comp}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX}
        COMMAND ${CMAKE_COMMAND}  -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX} ${CMAKE_CURRENT_SBINARY_DIR}/test/${CMAKE_IMPORT_LIBRARY_PREFIX}HepMC3${_comp}${CMAKE_IMPORT_LIBRARY_SUFFIX}        DEPENDS HepMC3${_comp}
                )
      endforeach()
    endif()
  endif()
  message(STATUS "HepMC3 python: Tweak HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} option to set the installation path for the python${Python_VERSION_MAJOR}${Python_VERSION_MINOR} bindings.")
  if (WIN32)
    if (NOT HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
      set (HEPMC3_Python_SITEARCH ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION})
      message(STATUS "HepMC3 python: HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} defaults to ${HEPMC3_Python_SITEARCH}")
    else()
      set (HEPMC3_Python_SITEARCH ${HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR}})
      message(STATUS "HepMC3 python: HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} is set to to ${HEPMC3_Python_SITEARCH}")
    endif()
  else()
    if (NOT HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
      set (HEPMC3_Python_SITEARCH ${Python_SITEARCH})
      message(STATUS "HepMC3 python: HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} defaults to ${HEPMC3_Python_SITEARCH}")
    else()
      set (HEPMC3_Python_SITEARCH ${HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR}})
      message(STATUS "HepMC3 python: HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} is set to to ${HEPMC3_Python_SITEARCH}")
    endif()
  endif()
  if(NOT (HEPMC3_Python_SITEARCH  MATCHES "^${CMAKE_INSTALL_PREFIX}(.*)"))
    message(STATUS "HepMC3 python: WARNING: The installation path of the python modules is HEPMC3_Python_SITEARCH=${HEPMC3_Python_SITEARCH}.")
    message(STATUS "HepMC3 python: WARNING: The installation path of the python modules is outside of the global instalation path CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}.")
    message(STATUS "HepMC3 python: WARNING: You can use the HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR} variable to set the desired installation path for the Python${Python_VERSION_MAJOR}${Python_VERSION_MINOR} modules.")
  endif()

    foreach(_comp IN ITEMS " " ${HEPMC3_COMPONENTS} )
       if (${_comp} STREQUAL " ")
         set(_comp "") 
         set(_dotcomp "") 
       else()
         set(_dotcomp ".${_comp}")   
       endif()
    install(TARGETS ${IMPL}pyHepMC3${_comp}${Python_VERSION}
      DESTINATION ${HEPMC3_Python_SITEARCH}/pyHepMC3/${_comp}
      COMPONENT python${_comp})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src/${_comp}/__init__.py
      DESTINATION  ${HEPMC3_Python_SITEARCH}/pyHepMC3/${_comp}
      COMPONENT python${_comp})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../pyHepMC3${_dotcomp}.egg-info
      DESTINATION  ${HEPMC3_Python_SITEARCH}
      RENAME pyHepMC3${_dotcomp}-${THIS_PROJECT_NAME_VERSION}-py${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}.egg-info
      COMPONENT python${_comp})
  endforeach()
  set( HepMC_python_tests
        test_IO1
        test_IO3
        test_IO4
        test_Boost
        test_Print
        test_Polarization
        test_Pythonization_docs
        test_Pythonization_FourVector
        test_Pythonization_GenEvent
        test_Pythonization_GenRunInfo
        test_Units
        test_Attribute
        test_HEPEVT
  )
#Workaround for nvc++ for this particular test
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI")
    list(REMOVE_ITEM HepMC_python_tests  test_Print)
  endif()
  if(ROOT_FOUND)
    list( APPEND HepMC_python_tests test_IO2 test_IO7)
  endif()
  if(HEPMC3_ENABLE_SEARCH)
    list( APPEND HepMC_python_tests test_Pythonization_Search)
  endif()
  if(HEPMC3_ENABLE_PROTOBUFIO AND Protobuf_FOUND AND NOT WIN32)
    list( APPEND HepMC_python_tests test_Protobuf)
  endif()
  if(HEPMC3_ENABLE_TEST)
    if(USE_INSTALLED_HEPMC3)
      ENABLE_TESTING()
    endif()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../test/Pythia8ToHepMC3.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../test/pyHepMC3TestUtils.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/")
    foreach ( ptest ${HepMC_python_tests} )
      file(GLOB INPUT${ptest}  "${CMAKE_CURRENT_SOURCE_DIR}/../test/input${testname}*" )
      file(COPY ${INPUT${ptest}} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test")
      configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../test/${ptest}.py" "${CMAKE_CURRENT_BINARY_DIR}/test/${ptest}.py" COPYONLY)
      add_test(NAME ${IMPL}${Python_VERSION}_${ptest}
        COMMAND ${Python_EXECUTABLE} ${ptest}.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test"
      )
      if(HEPMC3_ENABLE_PROTOBUFIO AND Protobuf_FOUND AND NOT WIN32)
        get_filename_component(pcdir ${Protobuf_PROTOC_EXECUTABLE} DIRECTORY)
        SET_TESTS_PROPERTIES(  ${IMPL}${Python_VERSION}_${ptest}
                  PROPERTIES
                  ENVIRONMENT
                  "PATH=${pcdir};${Protobuf_INCLUDE_DIR}/../lib;${Protobuf_INCLUDE_DIR}/../lib64;${Protobuf_INCLUDE_DIR}/../bin;$ENV{PATH}"
                  )
      endif()
    endforeach ( ptest ${HepMC_python_tests} )
  endif()
else()
  message(STATUS "HepMC3 python: Python version ${ver}.${verminor} not found. Python bindings for version ${ver}.${verminor} disabled.")
endif()
unset(HEPMC3_Python_SITEARCH)
endmacro(bindings)


set(NN  "A" "B" "C" "D")
if (NOT HEPMC3_PYTHON_VERSIONS)
  set(HEPMC3_PYTHON_VERSIONS "3")
endif()
string(REPLACE "," ";" TEMPVERSIONS ${HEPMC3_PYTHON_VERSIONS})
list(SORT TEMPVERSIONS)

list(LENGTH TEMPVERSIONS LL)
MATH(EXPR LL "${LL}-1")

foreach( ind RANGE 0 ${LL})
    LIST(GET TEMPVERSIONS ${ind} tempver_0) 
    if (tempver_0)
    LIST(GET NN ${ind} tempver_1)
    string(REPLACE "." ";" MAJMIN "${tempver_0}.X.Y")
    list(GET MAJMIN 0 MAJ)
    list(GET MAJMIN 1 MIN)
    message(STATUS "HepMC3 python: Using directory ${tempver_1} to build bindings for Python version ${tempver_0}")
    add_subdirectory(${tempver_1})
    endif()
endforeach()
