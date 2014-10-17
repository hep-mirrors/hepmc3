
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(rootIO_example_Classes include/rootIO_example_Classes.hh LINKDEF  include/LinkDef.hh)

add_library(rootIO_example_ClassesDict SHARED rootIO_example_Classes.cxx ${sources})
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(rootIO_example_ClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})

target_link_libraries(rootIO_example_ClassesDict HepMC ${ROOT_LIBRARIES})
###

add_executable(rootIO_example ${PROJECT_SOURCE_DIR}/examples/RootIOExample/rootIO_example.cc)
target_link_libraries(rootIO_example ${ROOT_LIBRARIES} HepMC)
set_target_properties(rootIO_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

# create environment scripts

set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
set(libdir "${CMAKE_INSTALL_PREFIX}/lib:${ROOT_LIBRARY_DIR}")
 
configure_file(
${CMAKE_SOURCE_DIR}/examples/RootIOExample/rootIO_example_env.sh.in
	    ${PROJECT_BINARY_DIR}/outputs/bin/rootIO_example_env.sh
     	     @ONLY
      	     )

# installs 
install(TARGETS rootIO_example DESTINATION bin)
install(FILES hepmc.in DESTINATION bin)
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin/ DESTINATION bin
        FILES_MATCHING PATTERN "*.sh")
