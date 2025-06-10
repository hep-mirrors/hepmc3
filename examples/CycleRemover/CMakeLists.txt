# building tests
#------------------
cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
#
if (${CMAKE_VERSION} VERSION_GREATER "3.8")
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
endif()

#---Add executables------------------------------------------------------------
add_executable(cycle_remover.exe ${CMAKE_CURRENT_SOURCE_DIR}/cycle_remover.cc ${CMAKE_CURRENT_SOURCE_DIR}/CycleRemover.cc ${CMAKE_CURRENT_SOURCE_DIR}/WriterDOTCycleRemover.cc)
target_link_libraries(cycle_remover.exe HepMC3::HepMC3 )
set_target_properties(cycle_remover.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)
if (USE_INSTALLED_HEPMC3)
  install(TARGETS cycle_remover.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
