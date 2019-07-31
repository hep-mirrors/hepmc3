
set(basic_sources   ${CMAKE_CURRENT_SOURCE_DIR}/convert_example.cc
                    ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterDOT.cc 
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterHEPEVTZEUS.cc )
set(basic_includes ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR} )
set(basic_libraries HepMC3)
set(basic_defines   HEPMCCONVERT_EXTENSION_HEPEVTZEUS=1   HEPMCCONVERT_EXTENSION_DOT=1)
set(basic_dependencies )
set(basic_libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" )
find_package(ZLIB)
if(ZLIB_FOUND)
list(APPEND basic_includes ${ZLIB_INCLUDE_DIR})
list(APPEND basic_sources  ${CMAKE_CURRENT_SOURCE_DIR}/src/gzstream.C )
list(APPEND basic_libraries ${ZLIB_LIBRARIES})
list(APPEND basic_defines HEPMCCONVERT_EXTENSION_GZ=1)
# create environment scripts
set(basic_libdir "${basic_libdir}:${ZLIB_ROOT}")
endif()

if(ROOT_FOUND)
list(APPEND basic_includes ${PROJECT_SOURCE_DIR}/rootIO/include  ${ROOT_INCLUDE_DIRS})
list(APPEND basic_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterRootTreeOPAL.cc )
list(APPEND basic_libraries ${ROOT_LIBRARIES} HepMC3rootIO)
list(APPEND basic_defines HEPMCCONVERT_EXTENSION_ROOTTREEOPAL=1)
list(APPEND basic_dependencies dictionary)
# create environment scripts
set(basic_libdir "${basic_libdir}:${ROOT_LIBRARY_DIR}")
endif()


add_executable(convert_example.exe  ${basic_sources} )
include_directories(${basic_includes})
target_link_libraries(convert_example.exe ${basic_libraries} )
target_compile_definitions(convert_example.exe PRIVATE ${basic_defines} )
if (basic_dependencies)
add_dependencies( convert_example.exe ${basic_dependencies} )
endif()
# create environment scripts
set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(libdir ${basic_libdir})
set_target_properties(convert_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR})
 
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/convert_example_env.sh.in
	    ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/convert_example_env.sh @ONLY)

# installs 
install(TARGETS convert_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${CMAKE_INSTALL_BINDIR} FILES_MATCHING PATTERN "*.pcm" PATTERN "*config"  PATTERN "CMakeFiles" EXCLUDE )
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR}  FILES_MATCHING PATTERN "*env.sh*")






#The cmdline.c and cmdline.h for this example are generated from file below
#gengetopt -N  --no-version --input=hepmcconvert.ggo --include-getopt --show-required -u"inputfile outputfile"
#version "3.1"
#package "hepmcconvert"
#purpose "Convert between different file formats of Monte Carlo event record.
#Example:
#         hepmcconvert -i hepmc2 -o treeroot input.hepmc output.root
#"
#option "input-format" i "Input format" string required values="hepmc2", "hepmc3", "hpe" ,"root", "treeroot","lhef","gz"
#option "output-format" o "Output format" string required values="hepmc2", "hepmc3", "hpe" ,"root", "treeroot" ,"treerootopal","hpezeus", "dump", "dot"
#option "extensions" e "Extensions, in a form extension=value, could be passed to readers/writers" multiple optional string
#option "events-limit" - "Limit of events to read from input" optional long default="100000000"
#option "first-event-number" -       "Lowest allowed event number" optional long default="-100000000"
#option "last-event-number" -        "Highest allowed event number" optional long default="100000000"
#option "print-every-events-parsed" - "Frequency of parsing information printouts" optional long default="100"
