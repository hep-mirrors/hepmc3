# building examples
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

find_package(Pythia8 QUIET)

if(PYTHIA8_FOUND)

  include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/interfaces/pythia8/include
                    ${PYTHIA8_INCLUDE_DIR})
  add_executable(pythia8_example ${PROJECT_SOURCE_DIR}/examples/pythia8_example.cc ${PROJECT_SOURCE_DIR}/interfaces/pythia8/src/Pythia8ToHepMC3.cc)
  target_link_libraries(pythia8_example ${PYTHIA8_LIBRARIES} HepMC)
  set_target_properties(pythia8_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

  # create environment scripts

  set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
  set(libdir "${CMAKE_INSTALL_PREFIX}/lib:${PYTHIA8_ROOT_DIR}/lib")
  set(pythia8data "${PYTHIA8_ROOT_DIR}/xmldoc")

  configure_file(
  ${CMAKE_SOURCE_DIR}/examples/pythia8_example_env.sh.in
	    ${PROJECT_BINARY_DIR}/outputs/bin/pythia8_example_env.sh
     	     @ONLY
      	     )

  # installs 

  install(TARGETS pythia8_example DESTINATION bin)
  install(FILES pythia8_ee_to_Z_to_tautau.conf DESTINATION bin)
  install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin/ DESTINATION bin
          FILES_MATCHING PATTERN "*.sh")
else()
  message(STATUS "Examples: Pythia8 package not found. ") 
  message(STATUS "pythia8_example disabled")
endif()
