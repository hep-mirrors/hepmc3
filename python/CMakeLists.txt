#project(pyHepMC3 CXX)
#Should be fixed
cmake_minimum_required(VERSION 3.0.0)
if(${CMAKE_VERSION} VERSION_LESS "3.14.00")
SET_PROPERTY (GLOBAL PROPERTY CMAKE_ROLE "PROJECT")
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)

option(USE_INSTALLED_HEPMC3 "Use HepMC3 bindings as a standalone package" OFF)
option(RERUN_BINDER "Rerun binder" OFF)
if (USE_INSTALLED_HEPMC3)
  set(${PROJECT_NAME}_VERSION 3.2.3)
  set(THIS_PROJECT_NAME ${PROJECT_NAME})
  set(THIS_PROJECT_NAME_VERSION 3.2.3)
  option(HEPMC3_ENABLE_TEST "Enable tests" ON)

  find_package(HepMC3  REQUIRED  )
  include_directories(${HEPMC3_INCLUDE_DIR})
  message(STATUS "HEPMC3_INCLUDE DIR: ${HEPMC3_INCLUDE_DIR}")
  message(STATUS "HEPMC3_LIB: ${HEPMC3_LIB}")
  include(GNUInstallDirs)
  if (HEPMC3_ROOTIO_LIB)
    message(STATUS "ROOT_FOUND:     ${ROOT_FOUND}")
    message(STATUS "ROOT includes:  ${ROOT_INCLUDE_DIRS}")
    message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")
    message(STATUS "ROOT definitions: ${ROOT_DEFINITIONS}")
    if(ROOT_FOUND)
      list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
      list(APPEND CMAKE_PREFIX_PATH ${ROOT_DIR})
      include_directories(${ROOT_INCLUDE_DIRS})
      link_directories(${ROOT_LIBRARY_DIR})
      add_definitions(${ROOT_DEFINITIONS})
      add_definitions(-DHEPMC3_ROOTIO)
      foreach(fl ${ROOT_CXX_FLAGS})
        CHECK_CXX_COMPILER_FLAG(${fl} COMPILER_SUPPORTS_${fl})
        if(COMPILER_SUPPORTS_${fl})
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
        endif()
      endforeach(fl ${ROOT_CXX_FLAGS})
      set(HEPMC3_ROOTIO_LIBRARY ${HEPMC3_ROOTIO_LIB} )
    endif()
  endif()

  include(CheckCXXCompilerFlag)
#This is a workaround for ROOT+Cmake problem.
  CHECK_CXX_COMPILER_FLAG("-Wno-unused-but-set-variable" COMPILER_SUPPORTS_-Wno-unused-but-set-variable)
  if (COMPILER_SUPPORTS_-Wno-unused-but-set-variable)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
  endif()
  CHECK_CXX_COMPILER_FLAG("-Wno-unused-value" COMPILER_SUPPORTS_-Wno-unused-value)
  if (COMPILER_SUPPORTS_-Wno-unused-value)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value")
  endif()
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    CHECK_CXX_COMPILER_FLAG("-fsized-deallocation" COMPILER_SUPPORTS_-fsized-deallocation)
    if (COMPILER_SUPPORTS_-fsized-deallocation)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsized-deallocation")
    endif()
  endif()
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_-std=c++11)
  if(COMPILER_SUPPORTS_-std=c++11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "PGI")
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
    set(MSVC_CXX_FLAGS_TO_CHECK "/bigobj" "/D _CRT_SECURE_NO_WARNINGS" "/wd4267" "/wd4244" "/wd4477")
    foreach(fl ${MSVC_CXX_FLAGS_TO_CHECK})
      CHECK_CXX_COMPILER_FLAG(${fl} COMPILER_SUPPORTS_${fl})
      if(COMPILER_SUPPORTS_${fl})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fl}")
      endif()
    endforeach(fl ${MSVC_CXX_FLAGS_TO_CHECK})
  endif()


  set (HEPMC3_INCLUDE_DIRECTORIES ${HEPMC3_INCLUDE_DIR})
  set (HEPMC3_INCLUDE_FLAGS -I${HEPMC3_INCLUDE_DIR})
  set (HEPMC3_LOADABLE_LIBRARIES_DIR ${HEPMC3_LIB_DIR})
  message(STATUS "pyHepMC3 v${THIS_PROJECT_NAME_VERSION}")
else()
  CHECK_CXX_COMPILER_FLAG("-Wno-unused-but-set-variable" COMPILER_SUPPORTS_-Wno-unused-but-set-variable)
  if (COMPILER_SUPPORTS_-Wno-unused-but-set-variable)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
  endif()
  CHECK_CXX_COMPILER_FLAG("-Wno-unused-value" COMPILER_SUPPORTS_-Wno-unused-value)
  if (COMPILER_SUPPORTS_-Wno-unused-value)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value")
  endif()
  set(THIS_PROJECT_NAME pyHepMC3)
  set(THIS_PROJECT_NAME_VERSION ${HEPMC3_VERSION_MAJOR}.${HEPMC3_VERSION_MINOR}.${HEPMC3_VERSION_PATCH})
  set (HEPMC3_LOADABLE_LIBRARIES_DIR ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>)
  set (HEPMC3_INCLUDE_FLAGS -I${PROJECT_SOURCE_DIR}/include   -I${PROJECT_SOURCE_DIR}/rootIO/include  -I${PROJECT_SOURCE_DIR}/search/include)
  set (HEPMC3_INCLUDE_DIRECTORIES  ${PROJECT_SOURCE_DIR}/include   ${PROJECT_SOURCE_DIR}/rootIO/include  ${PROJECT_SOURCE_DIR}/search/include)
  set ( HEPMC3_LIB HepMC3)
  set ( HEPMC3_SEARCH_LIB HepMC3search)
  #CMAKE_POSITION_INDEPENDENT_CODE should be ON or the static libraries should be compilled with -fPIC
  #This option should be documented
  if (HEPMC3_USE_STATIC_LIBS_FOR_PYTHON AND HEPMC3_BUILD_STATIC_LIBS)
   set ( HEPMC3_LIB HepMC3_static)
   set ( HEPMC3_SEARCH_LIB HepMC3search_static)
  endif()
  if(ROOT_FOUND)
    set (HEPMC3_ROOTIO_LIBRARY HepMC3rootIO)
  endif()
endif()

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Sun*")
    message(STATUS "HepMC3 python: WARNING: you are trying to use Oracle Studio compiller that is not officially supported by pybind11.\
 This compiler does not have the needed functionality to build the HepMC3 python modules using pybind11. Most likely the build fill fail.\
 To disable the build of python bindings use -DHEPMC3_ENABLE_PYTHON=OFF with cmake.")
endif()

#Workaround for nvc++ for this particular code
if (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI")
    message(STATUS "HepMC3 python: WARNING: you are using PGI/NVidia compiller that is not officially supported by pybind11.\
 The optimization and debug for the python modules will be disabled.")
    string(REPLACE "-g"    ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-fast" ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-O4"   ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-O3"   ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-O2"   ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-O1"   ""  CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-g"    ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-fast" ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O4"   ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O3"   ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O2"   ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O1"   ""  CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -O0")
endif()

configure_file("pyHepMC3.egg-info.in" "pyHepMC3.egg-info" IMMEDIATE @ONLY)
if(HEPMC3_ENABLE_SEARCH)
  add_definitions(-DHEPMC3_ENABLE_SEARCH)
  configure_file("pyHepMC3.search.egg-info.in" "pyHepMC3.search.egg-info" IMMEDIATE @ONLY)
endif()
if(ROOT_FOUND)
  configure_file("pyHepMC3.rootIO.egg-info.in" "pyHepMC3.rootIO.egg-info" IMMEDIATE @ONLY)
endif()

set(autoBN       src/pyHepMC3.cpp src/pyHepMC3_0.cpp src/pyHepMC3_1.cpp
                 src/pyHepMC3_2.cpp src/pyHepMC3_3.cpp src/pyHepMC3_4.cpp
                 src/pyHepMC3_5.cpp src/pyHepMC3_6.cpp src/pyHepMC3_7.cpp
                 src/pyHepMC3_8.cpp src/pyHepMC3_9.cpp src/pyHepMC3_10.cpp
                 src/pyHepMC3_11.cpp src/pyHepMC3_12.cpp src/pyHepMC3_13.cpp
                 src/pyHepMC3_14.cpp src/pyHepMC3_15.cpp src/pyHepMC3_16.cpp
                 src/pyHepMC3_17.cpp src/pyHepMC3_18.cpp 
)
set(autoBNrootIO src/pyHepMC3rootIO.cpp src/pyHepMC3rootIO_0.cpp src/pyHepMC3rootIO_1.cpp src/pyHepMC3rootIO_2.cpp)
set(autoBNsearch src/pyHepMC3search.cpp src/pyHepMC3search_0.cpp src/pyHepMC3search_1.cpp)
set(BN       ${autoBN}       src/binders.cpp)
set(BNrootIO ${autoBNrootIO} src/root_binders.cpp)
set(BNsearch ${autoBNsearch} src/search_binders.cpp)
if (RERUN_BINDER)
  find_program (BINDER binder HINTS $ENV{HOME}/bin /usr/bin)
  if(BINDER)
    message(STATUS "HepMC3 python: binder found in ${BINDER}. Automatic generatin of code for python bindings is possible.")

    configure_file("all_includes.hpp.in" "all_includes.hpp" IMMEDIATE @ONLY)
    add_custom_command(
      OUTPUT  ${autoBN}
      DEPENDS all_includes.hpp src/pyHepMC3.binder
      COMMAND ${BINDER} --flat --max-file-size=20000  
                        --root-module pyHepMC3 
                        --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" 
                        --config src/pyHepMC3.binder   all_includes.hpp 
                        --  -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM
    )
    if(ROOT_FOUND)
      configure_file("root_includes.hpp.in" "root_includes.hpp" IMMEDIATE @ONLY)
      add_custom_command(
        OUTPUT  ${autoBNrootIO}
        DEPENDS root_includes.hpp src/pyHepMC3rootIO.binder
        COMMAND ${BINDER} --flat --max-file-size=5000  
                          --root-module pyHepMC3rootIO 
                          --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" 
                          --config src/pyHepMC3rootIO.binder   root_includes.hpp 
                          --  -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS} -I${ROOT_INCLUDE_DIRS}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM
      )
    endif()
    if(HEPMC3_ENABLE_SEARCH)
      configure_file("search_includes.hpp.in" "search_includes.hpp" IMMEDIATE @ONLY)
      add_custom_command(
        OUTPUT  ${autoBNsearch}
        DEPENDS search_includes.hpp src/pyHepMC3search.binder
        COMMAND ${BINDER} --flat --max-file-size=5000  
                          --root-module pyHepMC3search 
                          --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" 
                          --config src/pyHepMC3search.binder   search_includes.hpp 
                          -- -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS} 
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM)
    endif()
  else()
    message(FATAL_ERROR "HepMC3  python: binder not found!  Automatic generatin of code for python bindings is not possible.")
  endif()
endif()

macro(bindings ver verminor)
SET( Python_VERSION_MAJOR "0")
SET( Python_VERSION_MINOR "0")

#Compilation with PyPy
if (${ver} MATCHES pypy* )
  unset(Python_FOUND CACHE)
  unset(Python_VERSION CACHE)
  unset(Python_Interpreter_FOUND CACHE)
  unset(Python_EXECUTABLE CACHE)
  unset(Python_INCLUDE_DIR CACHE)
  unset(Python_INCLUDE_DIRS CACHE)
  unset(Python_LIBRARIES CACHE)
  unset(_Python_EXECUTABLE CACHE)
  string(REPLACE "pypy" "" forpypy "${ver}")
  find_package(PyPy  ${forpypy})
  set(Python_EXECUTABLE        ${PyPy_EXE})
  set(Python_Interpreter       ${PyPy_EXE})
  set(Python_VERSION           pypy${PyPy_Python_VERSION})
  set(Python_VERSION_MAJOR     pypy${PyPy_Python_VERSION_MAJOR})
  set(Python_VERSION_MINOR     ${PyPy_Python_VERSION_MINOR})
  set(Python_LIBRARIES         ${PyPy_LIBRARIES})
  set(Python_INCLUDE_DIRS      ${PyPy_INCLUDE_DIRS})
  set(Python_SITEARCH          ${PyPy_SITEARCH})
endif()

#Compilation with CPython2
if (${ver} STREQUAL 2)
  unset(Python2_FOUND CACHE)
  unset(Python2_VERSION CACHE)
  unset(Python2_Interpreter_FOUND CACHE)
  unset(Python2_EXECUTABLE CACHE)
  unset(Python2_INCLUDE_DIR CACHE)
  unset(Python2_INCLUDE_DIRS CACHE)
  unset(Python2_LIBRARIES CACHE)
  unset(_Python2_EXECUTABLE CACHE) # Attempt to make FindPython reenterant
  unset(_Python2_INCLUDE_DIR CACHE) # Attempt to make FindPython reenterant
  if (${verminor} STREQUAL X)
    find_package (Python2 ${ver} COMPONENTS  Interpreter Development.Module)
  else()
    find_package (Python2 ${ver}.${verminor} EXACT COMPONENTS  Interpreter Development.Module)
  endif()
  if (Python2_VERSION AND Python2_INCLUDE_DIRS AND Python2_EXECUTABLE  ) #something is set
    SET( Python_VERSION       ${Python2_VERSION})
    SET( Python_VERSION_MAJOR ${Python2_VERSION_MAJOR})
    SET( Python_VERSION_MINOR ${Python2_VERSION_MINOR})
    SET( Python_LIBRARIES     ${Python2_LIBRARIES})
    SET( Python_SITEARCH      ${Python2_SITEARCH})
    SET( Python_EXECUTABLE    ${Python2_EXECUTABLE})
    SET( Python_INCLUDE_DIRS  ${Python2_INCLUDE_DIRS})
    unset(_Python2_EXECUTABLE CACHE) # Attempt to make FindPython reenterant
    unset(_Python2_INCLUDE_DIR CACHE) # Attempt to make FindPython reenterant
  endif()
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
  if (${verminor} STREQUAL X)
    find_package (Python3 ${ver} COMPONENTS  Interpreter Development.Module )
  else()
    find_package (Python3 ${ver}.${verminor} EXACT COMPONENTS  Interpreter Development.Module)
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
  endif()
endif()


if ( Python_INCLUDE_DIRS AND Python_EXECUTABLE
    AND ((${Python_VERSION_MAJOR} STREQUAL ${ver} AND ${verminor} STREQUAL "X")  
        OR "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}" STREQUAL "${ver}.${verminor}" ))
  message(STATUS "HepMC3 python: Python verson ${ver}.${verminor} found in ${Python_EXECUTABLE}. Python bindings generation is possible.")

#Compilation with PyPy
  if (${ver} MATCHES pypy* )
    add_library(pyHepMC3${Python_VERSION} MODULE  ${BN}   )
    set_target_properties(pyHepMC3${Python_VERSION}
      PROPERTIES
        PREFIX ""
        OUTPUT_NAME pyHepMC3.pypy-${PyPy_PyPy_VERSION_MAJOR}${PyPy_PyPy_VERSION_MINOR})
    if(ROOT_FOUND)
      add_library(pyHepMC3rootIO${Python_VERSION} MODULE  ${BNrootIO} )
      set_target_properties(pyHepMC3rootIO${Python_VERSION}
        PROPERTIES
          PREFIX ""
          OUTPUT_NAME pyHepMC3rootIO.pypy-${PyPy_PyPy_VERSION_MAJOR}${PyPy_PyPy_VERSION_MINOR})
    endif()
    if(HEPMC3_ENABLE_SEARCH)
      add_library(pyHepMC3search${Python_VERSION} MODULE  ${BNsearch} )
      set_target_properties(pyHepMC3search${Python_VERSION}
        PROPERTIES
          PREFIX ""
          OUTPUT_NAME pyHepMC3search.pypy-${PyPy_PyPy_VERSION_MAJOR}${PyPy_PyPy_VERSION_MINOR})
    endif()
  endif()
  
#Compilation with CPython2
  if (${ver} STREQUAL 2)
    Python2_add_library(pyHepMC3${Python_VERSION} MODULE  ${BN}   )
    set_target_properties(pyHepMC3${Python_VERSION}
      PROPERTIES  OUTPUT_NAME pyHepMC3  )
    if(ROOT_FOUND)
      Python2_add_library(pyHepMC3rootIO${Python_VERSION} MODULE  ${BNrootIO}   )
      set_target_properties(pyHepMC3rootIO${Python_VERSION}
        PROPERTIES  OUTPUT_NAME pyHepMC3rootIO  )
    endif()
    if(HEPMC3_ENABLE_SEARCH)
      Python2_add_library(pyHepMC3search${Python_VERSION} MODULE  ${BNsearch}   )
      set_target_properties(pyHepMC3search${Python_VERSION}
        PROPERTIES  OUTPUT_NAME pyHepMC3search  )
    endif()
  endif()

#Compilation with CPython3
  if (${ver} STREQUAL 3)
    Python3_add_library(pyHepMC3${Python_VERSION} MODULE  ${BN} )
    set_target_properties(pyHepMC3${Python_VERSION}
      PROPERTIES  OUTPUT_NAME pyHepMC3  )
    if(ROOT_FOUND)
      Python3_add_library(pyHepMC3rootIO${Python_VERSION} MODULE  ${BNrootIO} )
      set_target_properties(pyHepMC3rootIO${Python_VERSION}
        PROPERTIES  OUTPUT_NAME pyHepMC3rootIO  )
    endif()
    if(HEPMC3_ENABLE_SEARCH)
      Python3_add_library(pyHepMC3search${Python_VERSION} MODULE  ${BNsearch})
      set_target_properties(pyHepMC3search${Python_VERSION}
        PROPERTIES  OUTPUT_NAME pyHepMC3search  )
    endif()
  endif()
  set_target_properties(pyHepMC3${Python_VERSION}
    PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/$<0:>
      MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/$<0:>
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/$<0:>)
  target_link_libraries(pyHepMC3${Python_VERSION}
    PUBLIC  ${HEPMC3_LIB} )

  target_include_directories(pyHepMC3${Python_VERSION}
    PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )
  if(ROOT_FOUND)
    set_target_properties(pyHepMC3rootIO${Python_VERSION}
      PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/rootIO/$<0:>
        MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/rootIO/$<0:>
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/rootIO/$<0:>)
    target_link_libraries(pyHepMC3rootIO${Python_VERSION}
      PUBLIC  ${HEPMC3_LIB} ${HEPMC3_ROOTIO_LIBRARY} )
    target_include_directories(pyHepMC3rootIO${Python_VERSION}
      PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )

  endif()
  if(HEPMC3_ENABLE_SEARCH)
    set_target_properties(pyHepMC3search${Python_VERSION}
      PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/$<0:>
        MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/$<0:>
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/$<0:>)
    target_link_libraries(pyHepMC3search${Python_VERSION}
      PUBLIC  ${HEPMC3_LIB} ${HEPMC3_SEARCH_LIB} )
    target_include_directories(pyHepMC3search${Python_VERSION}
      PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )
  endif()

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/)
  add_custom_target(
        foo${Python_VERSION} ALL
    COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/__init__.py
                )
  if(ROOT_FOUND)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/rootIO/)
    add_custom_target(
        foorootIO${Python_VERSION} ALL
      COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/rootIO/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/rootIO/__init__.py
                )
  endif()
  if(HEPMC3_ENABLE_SEARCH)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/)
    add_custom_target(
        foosearch${Python_VERSION} ALL
      COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/search/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/__init__.py
                )
  endif()

  if (WIN32)
#Python is insane
    if(HEPMC3_ENABLE_TEST)
      add_custom_target(
        copy${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND} -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/HepMC3.dll ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/HepMC3.dll
        COMMAND ${CMAKE_COMMAND} -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/HepMC3.dll ${CMAKE_CURRENT_SOURCE_DIR}/test/HepMC3.dll
        DEPENDS HepMC3
      )
      if(HEPMC3_ENABLE_SEARCH)
        add_custom_target(
        copysearch${Python_VERSION} ALL
          COMMAND ${CMAKE_COMMAND} -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/HepMC3search.dll ${CMAKE_CURRENT_BINARY_DIR}/${Python_VERSION}/pyHepMC3/search/HepMC3search.dll
        COMMAND ${CMAKE_COMMAND} -E copy  ${HEPMC3_LOADABLE_LIBRARIES_DIR}/HepMC3search.dll ${CMAKE_CURRENT_SOURCE_DIR}/test/HepMC3search.dll
        DEPENDS HepMC3search
                )
      endif()
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

  install(TARGETS pyHepMC3${Python_VERSION}
    DESTINATION ${HEPMC3_Python_SITEARCH}/pyHepMC3
    COMPONENT python)
  install(FILES src/__init__.py
    DESTINATION  ${HEPMC3_Python_SITEARCH}/pyHepMC3
    COMPONENT python)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyHepMC3.egg-info
    DESTINATION  ${HEPMC3_Python_SITEARCH} 
    RENAME pyHepMC3-${THIS_PROJECT_NAME_VERSION}-py${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}.egg-info
    COMPONENT python)
  if(ROOT_FOUND)
    install(TARGETS pyHepMC3rootIO${Python_VERSION}
      DESTINATION ${HEPMC3_Python_SITEARCH}/pyHepMC3/rootIO
      COMPONENT pythonrootIO)
    install(FILES src/rootIO/__init__.py
      DESTINATION  ${HEPMC3_Python_SITEARCH}/pyHepMC3/rootIO
      COMPONENT pythonrootIO)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyHepMC3.rootIO.egg-info
      DESTINATION  ${HEPMC3_Python_SITEARCH} 
      RENAME pyHepMC3.rootIO-${THIS_PROJECT_NAME_VERSION}-py${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}.egg-info
      COMPONENT pythonrootIO)
  endif()
  if(HEPMC3_ENABLE_SEARCH)
    install(TARGETS pyHepMC3search${Python_VERSION}
      DESTINATION ${HEPMC3_Python_SITEARCH}/pyHepMC3/search
      COMPONENT pythonsearch)
    install(FILES src/search/__init__.py
      DESTINATION  ${HEPMC3_Python_SITEARCH}/pyHepMC3/search
      COMPONENT pythonsearch)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyHepMC3.search.egg-info
      DESTINATION  ${HEPMC3_Python_SITEARCH} 
      RENAME pyHepMC3.search-${THIS_PROJECT_NAME_VERSION}-py${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}.egg-info
      COMPONENT pythonsearch)
  endif()

  set( HepMC_python_tests
        test_IO1
        test_Boost
        test_Print
#        test_Polarization
        test_Pythonization_docs
        test_Pythonization_FourVector
        test_Pythonization_GenEvent
        test_Pythonization_GenRunInfo
        test_Units
        test_Attribute
  )
#Workaround for nvc++ for this particular test
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI")  
    list(REMOVE_ITEM HepMC_python_tests  test_Print)
  endif()
  if(ROOT_FOUND)
    list( APPEND HepMC_python_tests test_IO2)
  endif()
  if(HEPMC3_ENABLE_SEARCH)
    list( APPEND HepMC_python_tests test_Pythonization_Search)
  endif()
  if(HEPMC3_ENABLE_TEST)
    if(USE_INSTALLED_HEPMC3)
      ENABLE_TESTING()
    endif()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test/Pythia8ToHepMC3.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test/pyHepMC3TestUtils.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/")
    foreach ( ptest ${HepMC_python_tests} )
      file(GLOB INPUT${ptest}  "${CMAKE_CURRENT_SOURCE_DIR}/test/input${testname}*" )
      file(COPY ${INPUT${ptest}} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test")
      file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test/${ptest}.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/")
      add_test(NAME python${Python_VERSION}_${ptest}
        COMMAND ${Python_EXECUTABLE} ${ptest}.py 
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test"
      )
    endforeach ( ptest ${HepMC_python_tests} )
  endif()
else()
  message(STATUS "HepMC3 python: Python version ${ver}.${verminor} not found. Python bindings for version ${ver}.${verminor} disabled.")
endif()
unset(HEPMC3_Python_SITEARCH)
endmacro(bindings)

if (HEPMC3_PYTHON_VERSIONS)
  string(REPLACE "," ";" TEMPVERSIONS ${HEPMC3_PYTHON_VERSIONS})
  foreach( tempver ${TEMPVERSIONS} )
    string(REPLACE "." ";" MAJMIN "${tempver}.X.Y")
    list(GET MAJMIN 0 MAJ)
    list(GET MAJMIN 1 MIN)
    bindings(${MAJ} ${MIN})
  endforeach ()
else()
  bindings(2 X)
  bindings(3 X)
endif()
