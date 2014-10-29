# building tests
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

#---Add executables------------------------------------------------------------

add_executable(basic_tree ${PROJECT_SOURCE_DIR}/examples/BasicExamples/basic_tree.cc)
target_link_libraries(basic_tree HepMC)
set_target_properties(basic_tree PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS basic_tree DESTINATION bin)

if(CMAKE_Fortran_COMPILER)
 add_executable(hepevt_wrapper_example ${PROJECT_SOURCE_DIR}/examples/BasicExamples/hepevt_wrapper_example_main.cc ${PROJECT_SOURCE_DIR}/examples/BasicExamples/hepevt_wrapper_example_fortran.f)
 target_link_libraries(hepevt_wrapper_example HepMC_static)
 set_target_properties(hepevt_wrapper_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS hepevt_wrapper_example DESTINATION bin)

else()
 message(STATUS "No Fortran compiler - dissabling HepEvt example.")
endif()

add_executable(HepMC2_adapter_example ${PROJECT_SOURCE_DIR}/examples/BasicExamples/HepMC2_adapter_example.cc)
target_link_libraries(HepMC2_adapter_example HepMC_static)
set_target_properties(HepMC2_adapter_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS HepMC2_adapter_example DESTINATION bin)

add_executable(HepMC3_fileIO_example ${PROJECT_SOURCE_DIR}/examples/BasicExamples/HepMC3_fileIO_example.cc)
target_link_libraries(HepMC3_fileIO_example HepMC_static)
set_target_properties(HepMC3_fileIO_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS HepMC3_fileIO_example DESTINATION bin)
