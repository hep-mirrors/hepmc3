
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/examples/RootIOExample3 ${PROJECT_SOURCE_DIR}/rootIO/include ${ROOT_INCLUDE_DIR})

add_executable(rootIOTree_example_read.exe ${PROJECT_SOURCE_DIR}/examples/RootIOExample3/rootIOTree_example_read.cc)
target_link_libraries(rootIOTree_example_read.exe ${ROOT_LIBRARIES}  HepMC HepMCrootIO )
set_target_properties(rootIOTree_example_read.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

add_executable(rootIOTree_example_write.exe ${PROJECT_SOURCE_DIR}/examples/RootIOExample3/rootIOTree_example_write.cc)
target_link_libraries(rootIOTree_example_write.exe ${ROOT_LIBRARIES}  HepMC HepMCrootIO )
set_target_properties(rootIOTree_example_write.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

# create environment scripts

set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
set(libdir "${CMAKE_INSTALL_PREFIX}/lib:${ROOT_LIBRARY_DIR}")
 
configure_file(
${CMAKE_SOURCE_DIR}/examples/RootIOExample3/rootIOTree_example_env.sh.in
	    ${PROJECT_BINARY_DIR}/outputs/bin/rootIOTree_example_env.sh
     	     @ONLY
      	     )

# installs 
install(TARGETS rootIOTree_example_write.exe rootIOTree_example_read.exe DESTINATION bin)
#install(FILES example.hepmc3 DESTINATION bin)
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin/ DESTINATION bin
        FILES_MATCHING PATTERN "*.sh")
