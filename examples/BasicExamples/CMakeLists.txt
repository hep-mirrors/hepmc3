# building tests
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

#---Add executables------------------------------------------------------------

add_executable(basic_tree.exe ${PROJECT_SOURCE_DIR}/examples/BasicExamples/basic_tree.cc)
target_link_libraries(basic_tree.exe HepMC)
set_target_properties(basic_tree.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

install(TARGETS basic_tree.exe DESTINATION bin)

if(CMAKE_Fortran_COMPILER)
 add_executable(hepevt_wrapper_example.exe ${PROJECT_SOURCE_DIR}/examples/BasicExamples/hepevt_wrapper_example_main.cc ${PROJECT_SOURCE_DIR}/examples/BasicExamples/hepevt_wrapper_example_fortran.f)
 target_link_libraries(hepevt_wrapper_example.exe HepMC_static)
 set_target_properties(hepevt_wrapper_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

install(TARGETS hepevt_wrapper_example.exe DESTINATION bin)

else()
 message(STATUS "No Fortran compiler - dissabling HepEvt example.")
endif()

add_executable(HepMC2_adapter_example.exe ${PROJECT_SOURCE_DIR}/examples/BasicExamples/HepMC2_adapter_example.cc)
target_link_libraries(HepMC2_adapter_example.exe HepMC_static)
set_target_properties(HepMC2_adapter_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

install(TARGETS HepMC2_adapter_example.exe DESTINATION bin)

add_executable(HepMC3_fileIO_example.exe ${PROJECT_SOURCE_DIR}/examples/BasicExamples/HepMC3_fileIO_example.cc)
target_link_libraries(HepMC3_fileIO_example.exe HepMC_static)
set_target_properties(HepMC3_fileIO_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

install(TARGETS HepMC3_fileIO_example.exe DESTINATION bin)
