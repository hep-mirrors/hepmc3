#This version is for standalone compilation of examples.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
find_package(Pythia8 8.300 COMPONENTS HepMC3)
if(Pythia8_FOUND)
  message(STATUS "HepMC3 Pythia8Example: Pythia8 package found. pythia8_example enabled")
  add_executable(pythia8_example.exe ${CMAKE_CURRENT_SOURCE_DIR}/pythia8_example.cc )
  target_link_libraries(pythia8_example.exe Pythia8::Pythia8 HepMC3::HepMC3)
  set_target_properties(pythia8_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR})

# create environment scripts

  set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
  get_filename_component(PYTHIA8_LIB_DIR ${PYTHIA8_LIBRARY} PATH)
  set(libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}:${PYTHIA8_LIB_DIR}")
  set(pythia8data "${PYTHIA8_XMLDOC_DIR}")

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pythia8_example_env.sh.in
               ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/pythia8_example_env.sh
               @ONLY
               )
# installs
  if (USE_INSTALLED_HEPMC3)
    install(TARGETS pythia8_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES pythia8_ee_to_Z_to_tautau.conf DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR}
           FILES_MATCHING PATTERN "*env.sh*")
  endif()
else()
  message(STATUS "HepMC3 examples: Pythia8 package not found. Please check if PYTHIA8_ROOT_DIR is set properly. pythia8_example disabled")
endif()
