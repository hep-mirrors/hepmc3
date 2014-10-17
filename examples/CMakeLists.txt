# building examples
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

find_package(Pythia8 QUIET)

if(PYTHIA8_FOUND)
  add_subdirectory(Pythia8Example)
else()
  message(STATUS "Examples: Pythia8 package not found. ") 
  message(STATUS "pythia8_example disabled")
endif()

find_package(ROOT REQUIRED)

if(ROOT_FOUND)
  add_subdirectory(RootIOExample)
else()
  message(STATUS "Examples: ROOT package not found. ") 
  message(STATUS "rootIO_example disabled")
endif()

