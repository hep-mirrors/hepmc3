# building examples
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

find_package(Pythia8 QUIET)

if(PYTHIA8_FOUND)

  message(STATUS "pythia8 inc ${PYTHIA8_INCLUDE_DIR}")
 include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/interfaces/pythia8/include
                    ${PYTHIA8_INCLUDE_DIR})
  add_executable(pythia8_example ${PROJECT_SOURCE_DIR}/examples/pythia8_example.cc ${PROJECT_SOURCE_DIR}/interfaces/pythia8/src/Pythia8ToHepMC3.cc)
  target_link_libraries(pythia8_example ${PYTHIA8_LIBRARIES} HepMC)
  set_target_properties(pythia8_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)
else()
  message(STATUS "Examples: Pythia8 package not found. ") 
  message(STATUS "pythia8_example disabled")
endif()
