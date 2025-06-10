# building tests
#------------------
cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
#
if (${CMAKE_VERSION} VERSION_GREATER "3.8")
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
endif()

#---Add executables------------------------------------------------------------
add_executable(basic_tree.exe ${CMAKE_CURRENT_SOURCE_DIR}/basic_tree.cc)
target_link_libraries(basic_tree.exe HepMC3::HepMC3 HepMC3::search ROOT::Tree ROOT::RIO ROOT::Core)
set_target_properties(basic_tree.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)
if (USE_INSTALLED_HEPMC3)
  install(TARGETS basic_tree.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
