# building tests
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

#---Add executables------------------------------------------------------------

add_executable(basic_tree ${PROJECT_SOURCE_DIR}/tests/basic_tree.cc)
target_link_libraries(basic_tree HepMC)
set_target_properties(basic_tree PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS basic_tree DESTINATION bin)

if(CMAKE_Fortran_COMPILER)
 add_executable(hepevt_wrapper_test ${PROJECT_SOURCE_DIR}/tests/hepevt_wrapper_test_main.cc ${PROJECT_SOURCE_DIR}/tests/hepevt_wrapper_test_fortran.f)
 target_link_libraries(hepevt_wrapper_test HepMC_static)
 set_target_properties(hepevt_wrapper_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS hepevt_wrapper_test DESTINATION bin)

else()
 message(STATUS "No Fortran compiler - dissabling HepEvt test")
endif()

add_executable(HepMC2_to_3_converter_test ${PROJECT_SOURCE_DIR}/tests/HepMC2_to_3_converter_test.cc)
target_link_libraries(HepMC2_to_3_converter_test HepMC_static)
set_target_properties(HepMC2_to_3_converter_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)

install(TARGETS HepMC2_to_3_converter_test DESTINATION bin)
