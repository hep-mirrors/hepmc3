# building tests
#------------------
cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
#
if (${CMAKE_VERSION} VERSION_GREATER "3.8")
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
endif()

#---Add executables------------------------------------------------------------
add_executable(search_example.exe ${CMAKE_CURRENT_SOURCE_DIR}/src/search_example.cc)
target_link_libraries(search_example.exe HepMC3::HepMC3 HepMC3::search )
set_target_properties(search_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)
if (USE_INSTALLED_HEPMC3)
  install(TARGETS search_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
