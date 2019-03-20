# building examples
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
include_directories(${PROJECT_SOURCE_DIR}/include )


#This is needed for standalone compilation
if (USE_INSTALLED_HEPMC3)
find_package(HepMC3 REQUIRED)
include_directories(${HEPMC3_INCLUDE_DIR})
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
endif()
#<-This is needed for standalone compilation 





include(CheckLanguage)
check_language(Fortran)
if (CMAKE_Fortran_COMPILER)
enable_language(Fortran)
get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)
message(STATUS "Fortran compiler : ${Fortran_COMPILER_NAME}")
if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
set( CMAKE_Fortran_FLAGS  "${CMAKE_Fortran_FLAGS} -fno-automatic -fno-backslash -ffixed-line-length-132")
elseif (CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
set( CMAKE_Fortran_FLAGS  "${CMAKE_Fortran_FLAGS} -fno-automatic -fno-backslash -extend-source 132")
elseif()
message(STATUS "HepMC3: Don't know how to set Fortran FLAGS for  ${Fortran_COMPILER_NAME} with ID ${Fortran_COMPILER_ID}")
endif ()
if(CMAKE_SYSTEM_NAME MATCHES Darwin)
  set(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS} -flat_namespace -single_module -undefined dynamic_lookup")
else()
endif()
message(STATUS "HepMC3: Fortran FLAGS ${CMAKE_Fortran_FLAGS}")
add_subdirectory(Pythia6Example)
else()
message(STATUS "HepMC3: Fortran compiler not found")
endif()



add_subdirectory(BasicExamples)

add_subdirectory(LHEFExample)

add_subdirectory(ConvertExample)

add_subdirectory(Pythia8Example)

if(ROOT_FOUND)
  add_subdirectory(RootIOExample)
  add_subdirectory(RootIOExample2)
  add_subdirectory(RootIOExample3)
  add_subdirectory(ViewerExample)
else()
  message(STATUS "Examples: ROOT package not found. ")
  message(STATUS "rootIO_example disabled")
endif()
