#project(pyHepMC3 CXX)
#Should be fixed 
cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_VERBOSE_MAKEFILE ON)

option(USE_INSTALLED_HEPMC3 "Use pyhepmc as a standalone package" OFF)
option(RERUN_BINDER "Rerun binder" OFF)

if (USE_INSTALLED_HEPMC3)
set(${PROJECT_NAME}_VERSION 3.2.0)
set(THIS_PROJECT_NAME ${PROJECT_NAME})
set(THIS_PROJECT_NAME_VERSION 3.2.0)
option(HEPMC3_ENABLE_TEST "Enable tests" ON)

find_package(HepMC3  REQUIRED  )
include_directories(${HEPMC3_INCLUDE_DIR})
message(STATUS "HEPMC3_INCLUDE DIR: ${HEPMC3_INCLUDE_DIR}")
message(STATUS "HEPMC3_LIB: ${HEPMC3_LIB}")
include(GNUInstallDirs)
if (HEPMC3_ROOTIO_LIB)
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
    set(HEPMC3_ROOTIO_LIBRARY ${HEPMC3_ROOTIO_LIB} )
  endif()
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("/D _CRT_SECURE_NO_WARNINGS" COMPILER_SUPPORTS__CRT_SECURE_NO_WARNINGS)
if(COMPILER_SUPPORTS__CRT_SECURE_NO_WARNINGS)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS /wd4267 /wd4244 /wd4477")
endif()
CHECK_CXX_COMPILER_FLAG("/Zc:__cplusplus" COMPILER_SUPPORTS_ZCPLUSPLUS)
if(COMPILER_SUPPORTS_ZCPLUSPLUS)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /bigobj")
endif()
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11 AND NOT COMPILER_SUPPORTS_ZCPLUSPLUS)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
set ( HEPMC3_INCLUDE_DIRECTORIES ${HEPMC3_INCLUDE_DIR})
set (HEPMC3_INCLUDE_FLAGS -I${HEPMC3_INCLUDE_DIR})
message(STATUS "pyHepMC3 v${THIS_PROJECT_NAME_VERSION}")
else()
set(THIS_PROJECT_NAME pyHepMC3)
set(${THIS_PROJECT_NAME_VERSION} ${PROJECT_VERSION})
set (HEPMC3_INCLUDE_FLAGS -I${PROJECT_SOURCE_DIR}/include   -I${PROJECT_SOURCE_DIR}/rootIO/include  -I${PROJECT_SOURCE_DIR}/search/include)
set (HEPMC3_INCLUDE_DIRECTORIES  ${PROJECT_SOURCE_DIR}/include   ${PROJECT_SOURCE_DIR}/rootIO/include  ${PROJECT_SOURCE_DIR}/search/include)
set ( HEPMC3_LIB HepMC3)
set ( HEPMC3_SEARCH_LIB HepMC3search)
  if(ROOT_FOUND)
set (HEPMC3_ROOTIO_LIBRARY HepMC3rootIO)
  endif()
endif()
if(HEPMC3_ENABLE_SEARCH)
if (MSVC)
message(STATUS "HepMC3-search  python bindings are not compatible with MSVC")
else()
    add_definitions(-DHEPMC3_ENABLE_SEARCH)
endif()
endif()
set(BN src/pyHepMC3.cpp)
set(BNrootIO src/pyHepMC3rootIO.cpp)
set(BNsearch src/pyHepMC3search.cpp)
if (RERUN_BINDER)
find_program (BINDER binder HINTS $ENV{HOME}/binder/build/llvm-4.0.0/build_4.0.0.linux.mppui4.release/bin)
if(BINDER)
message(FATAL "HepMC3: binder found in ${BINDER}. Automatic generatin of code for python bindings is possible.")
configure_file("all_includes.hpp.in" "all_includes.hpp" IMMEDIATE @ONLY)
configure_file("root_includes.hpp.in" "root_includes.hpp" IMMEDIATE @ONLY)
configure_file("search_includes.hpp.in" "search_includes.hpp" IMMEDIATE @ONLY)
add_custom_command(
OUTPUT  src/pyHepMC3.cpp src/pyHepMC3search.cpp  src/pyHepMC3rootIO.cpp
DEPENDS all_includes.hpp root_includes.hpp search_includes.hpp src/pyHepMC3.binder
COMMAND ${BINDER} --single-file  --root-module pyHepMC3 --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" --config src/pyHepMC3.binder   all_includes.hpp -- -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS} -I${ROOT_INCLUDE_DIRS}
COMMAND ${BINDER} --single-file  --root-module pyHepMC3rootIO --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" --config src/pyHepMC3rootIO.binder   root_includes.hpp -- -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS} -I${ROOT_INCLUDE_DIRS}
COMMAND ${BINDER} --single-file  --root-module pyHepMC3search --prefix "${CMAKE_CURRENT_SOURCE_DIR}/src/" --config src/pyHepMC3search.binder   search_includes.hpp -- -std=c++11 -I. -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_FLAGS} 
WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
VERBATIM)
else()
message(FATAL "HepMC3: binder not found!  Automatic generatin of code for python bindings is not possible.")
endif()
endif()

macro(bindings ver verminor)
SET( Python_VERSION_MAJOR 0)
SET( Python_VERSION_MINOR 0)
if (${ver} STREQUAL 2)
find_package (Python ${ver} COMPONENTS  Development Interpreter)
endif()
if (${ver} STREQUAL 3)
find_package (Python3 ${ver} COMPONENTS  Development Interpreter)
if (Python3_VERSION AND Python3_LIBRARIES AND Python3_INCLUDE_DIRS AND Python3_EXECUTABLE  ) #something is set
SET( Python_VERSION ${Python3_VERSION})
SET( Python_VERSION_MAJOR ${Python3_VERSION_MAJOR})
SET( Python_VERSION_MINOR ${Python3_VERSION_MINOR})
SET( Python_LIBRARIES ${Python3_LIBRARIES})
SET( Python_SITEARCH ${Python3_SITEARCH})
SET( Python_EXECUTABLE ${Python3_EXECUTABLE})
SET( Python_INCLUDE_DIRS ${Python3_INCLUDE_DIRS})
endif()
endif()


if ( Python_LIBRARIES AND Python_INCLUDE_DIRS AND Python_EXECUTABLE AND ((${Python_VERSION_MAJOR} STREQUAL ${ver} AND ${verminor} STREQUAL "X")  OR "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}" STREQUAL "${ver}.${verminor}" ))
message(STATUS "HepMC3: Python found in ${Python_EXECUTABLE}. Python bindings generation is possible.")
if (${ver} STREQUAL 2)
Python_add_library(pyHepMC3${Python_VERSION} MODULE  ${BN}   )
if(ROOT_FOUND)
Python_add_library(pyHepMC3rootIO${Python_VERSION} MODULE  ${BNrootIO}   )
endif()
if(HEPMC3_ENABLE_SEARCH)
Python_add_library(pyHepMC3search${Python_VERSION} MODULE  ${BNsearch}   )
endif()
endif()
if (${ver} STREQUAL 3)
Python3_add_library(pyHepMC3${Python_VERSION} MODULE  ${BN}   )
if(ROOT_FOUND)
Python3_add_library(pyHepMC3rootIO${Python_VERSION} MODULE  ${BNrootIO}   )
endif()
if(HEPMC3_ENABLE_SEARCH)
Python3_add_library(pyHepMC3search${Python_VERSION} MODULE  ${BNsearch}   )
endif()
endif()
set_target_properties(pyHepMC3${Python_VERSION} PROPERTIES OUTPUT_NAME pyHepMC3 LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/$<0:> MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/$<0:> ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/$<0:>)
target_include_directories(pyHepMC3${Python_VERSION} PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(pyHepMC3${Python_VERSION} PUBLIC ${Python_LIBRARIES}  ${HEPMC3_LIB}  )
if(ROOT_FOUND)
set_target_properties(pyHepMC3rootIO${Python_VERSION} PROPERTIES OUTPUT_NAME pyHepMC3rootIO LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/rootIO/$<0:> MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/$<0:> ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/rootIO/$<0:>)
target_include_directories(pyHepMC3rootIO${Python_VERSION} PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(pyHepMC3rootIO${Python_VERSION} PUBLIC ${Python_LIBRARIES}  ${HEPMC3_LIB} ${HEPMC3_ROOTIO_LIBRARY} )
endif()
if(HEPMC3_ENABLE_SEARCH)
set_target_properties(pyHepMC3search${Python_VERSION} PROPERTIES OUTPUT_NAME pyHepMC3search LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/search/$<0:> MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/$<0:> ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/search/$<0:>)
target_include_directories(pyHepMC3search${Python_VERSION} PUBLIC ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${HEPMC3_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(pyHepMC3search${Python_VERSION} PUBLIC ${Python_LIBRARIES}  ${HEPMC3_LIB} ${HEPMC3_SEARCH_LIB} )
endif()

add_custom_target(
        foo${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/__init__.py ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/__init__.py
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
if(ROOT_FOUND)
add_custom_target(
        foorootIO${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/rootIO/__init__.py ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/rootIO/__init__.py
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
endif()
if(HEPMC3_ENABLE_SEARCH)
add_custom_target(
        foosearch${Python_VERSION} ALL
        COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/search/__init__.py ${CMAKE_CURRENT_SOURCE_DIR}/${Python_VERSION}/pyHepMC3/search/__init__.py
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
endif()

if (WIN32)
install(TARGETS pyHepMC3${Python_VERSION} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3  COMPONENT python)
install(FILES src/__init__.py  DESTINATION  ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3 COMPONENT python)
if(ROOT_FOUND)        
install(TARGETS pyHepMC3rootIO${Python_VERSION} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3/rootIO  COMPONENT pythonrootIO)
install(FILES src/rootIO/__init__.py  DESTINATION  ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3/rootIO COMPONENT pythonrootIO)
endif()
if(HEPMC3_ENABLE_SEARCH)
install(TARGETS pyHepMC3search${Python_VERSION} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3/search  COMPONENT pythonsearch)
install(FILES src/rootIO/__init__.py  DESTINATION  ${CMAKE_INSTALL_LIBDIR}/${Python_VERSION}/pyHepMC3/search COMPONENT pythonsearch)
endif()
else()
install(TARGETS pyHepMC3${Python_VERSION} DESTINATION ${Python_SITEARCH}/pyHepMC3  COMPONENT python)
install(FILES src/__init__.py  DESTINATION  ${Python_SITEARCH}/pyHepMC3 COMPONENT python)
if(ROOT_FOUND)        
install(TARGETS pyHepMC3rootIO${Python_VERSION} DESTINATION ${Python_SITEARCH}/pyHepMC3/rootIO  COMPONENT pythonrootIO)
install(FILES src/rootIO/__init__.py  DESTINATION  ${Python_SITEARCH}/pyHepMC3/rootIO COMPONENT pythonrootIO)
endif()
if(HEPMC3_ENABLE_SEARCH)
install(TARGETS pyHepMC3search${Python_VERSION} DESTINATION ${Python_SITEARCH}/pyHepMC3/search  COMPONENT pythonsearch)
install(FILES src/search/__init__.py  DESTINATION  ${Python_SITEARCH}/pyHepMC3/search COMPONENT pythonsearch)
endif()
endif()


set( HepMC_python_tests 
        test_IO1
        test_Boost
        test_Print
#        test_Polarization
        test_Pythonization_docs
#        test_Pythonization_FourVector
        test_Pythonization_GenEvent
        test_Pythonization_GenRunInfo
#BAD        test_Units
        )
if(ROOT_FOUND)        
list( APPEND HepMC_python_tests test_IO2)
endif()        
if(HEPMC3_ENABLE_TEST)
list( APPEND HepMC_python_tests test_Pythonization_Search)
endif()
if(HEPMC3_ENABLE_TEST)
if(USE_INSTALLED_HEPMC3)
ENABLE_TESTING()
endif()
foreach ( ptest ${HepMC_python_tests} )
  add_test(NAME python${Python_VERSION}_${ptest} COMMAND ${Python_EXECUTABLE} ${ptest}.py   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
endforeach ( ptest ${HepMC_python_tests} )
endif()

else()
  message(STATUS "HepMC3: Python not found. Python bindings for version ${ver}  disabled.")
endif()
endmacro(bindings)

bindings(2 X)
bindings(3 X)


