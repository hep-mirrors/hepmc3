#--------
#-root IO

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}  ${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(rootIO_Classes include/rootIO_Classes.hh LINKDEF include/LinkDef.hh OPTIONS -inlineInputHeader)

add_library(HepMCrootIO SHARED ${PROJECT_SOURCE_DIR}/rootIO/src/WriterRoot.cc ${PROJECT_SOURCE_DIR}/rootIO/src/ReaderRoot.cc rootIO_Classes.cxx)

target_link_libraries(HepMCrootIO ${ROOT_LIBRARIES} HepMC)
set_target_properties(HepMCrootIO PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)


# installs 
install(TARGETS HepMCrootIO DESTINATION lib)
install(DIRECTORY ${PROJECT_BINARY_DIR}/rootIO/ DESTINATION lib FILES_MATCHING PATTERN "*.pcm")
install(DIRECTORY include/HepMC DESTINATION include COMPONENT headers)
