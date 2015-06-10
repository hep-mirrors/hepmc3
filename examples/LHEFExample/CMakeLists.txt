
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/examples/LHEFExample)

add_executable(LHEF_example_cat.exe ${PROJECT_SOURCE_DIR}/examples/LHEFExample/LHEF_example_cat.cc)
target_link_libraries(LHEF_example_cat.exe HepMC ${ROOT_LIBRARIES})
set_target_properties(LHEF_example_cat.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

# create environment scripts

set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
set(libdir "${CMAKE_INSTALL_PREFIX}/lib")
 
configure_file(
${CMAKE_SOURCE_DIR}/examples/LHEFExample/LHEF_example_env.sh.in
	    ${PROJECT_BINARY_DIR}/outputs/bin/LHEF_example_env.sh
     	     @ONLY
      	     )

# installs 

install(TARGETS LHEF_example_cat.exe DESTINATION bin)
install(FILES LHEF_example.lhe LHEF_example_cmp.lhe DESTINATION bin)
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin/ DESTINATION bin
        FILES_MATCHING PATTERN "*.sh")
