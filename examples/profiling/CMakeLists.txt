

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}  ${PYTHIA8_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/interfaces/pythia8/include)

# Generate dictionaries

ROOT_GENERATE_DICTIONARY(dict_Classes dict_Classes.h LINKDEF LinkDef.h OPTIONS -inlineInputHeader -DHEPMC_ROOTIO)

add_custom_target ( time_hepmc3_dictionary DEPENDS dict_Classes.cxx)


add_executable(time_hepmc3_write.exe
        ${CMAKE_CURRENT_SOURCE_DIR}/time_hepmc3_write.cc ${PROJECT_SOURCE_DIR}/interfaces/pythia8/src/Pythia8ToHepMC3.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/MyClass.cc dict_Classes.cxx)
target_link_libraries(time_hepmc3_write.exe ${PYTHIA8_LIBRARIES} ${ROOT_LIBRARIES} HepMC HepMCrootIO)
set_target_properties(time_hepmc3_write.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)
add_dependencies( time_hepmc3_write.exe time_hepmc3_dictionary )


add_executable(time_hepmc3_read.exe
        ${CMAKE_CURRENT_SOURCE_DIR}/time_hepmc3_read.cc ${PROJECT_SOURCE_DIR}/interfaces/pythia8/src/Pythia8ToHepMC3.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/MyClass.cc dict_Classes.cxx)
target_link_libraries(time_hepmc3_read.exe ${PYTHIA8_LIBRARIES} ${ROOT_LIBRARIES} HepMC HepMCrootIO)
set_target_properties(time_hepmc3_read.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)
add_dependencies( time_hepmc3_read.exe time_hepmc3_dictionary )



# create environment scripts

set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
set(libdir "${CMAKE_INSTALL_PREFIX}/lib:${ROOT_LIBRARY_DIR}")


configure_file(
${CMAKE_SOURCE_DIR}/examples/profiling/time_hepmc3_env.sh.in
            ${PROJECT_BINARY_DIR}/outputs/bin/time_hepmc3_env.sh
             @ONLY
             )


 
# installs 
install(TARGETS time_hepmc3_write.exe DESTINATION bin)
install(TARGETS time_hepmc3_read.exe DESTINATION bin)
install(DIRECTORY ${PROJECT_BINARY_DIR}/examples/profiling/ DESTINATION bin FILES_MATCHING PATTERN "*.pcm" PATTERN "CMakeFiles" EXCLUDE )




