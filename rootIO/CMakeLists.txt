#--------
#-root IO

include_directories(${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(HepMC3rootIO include/rootIO_Classes.hh LINKDEF include/LinkDef.hh OPTIONS -inlineInputHeader)

add_library(realHepMC3rootIO SHARED ${PROJECT_SOURCE_DIR}/rootIO/src/WriterRoot.cc ${PROJECT_SOURCE_DIR}/rootIO/src/ReaderRoot.cc ${PROJECT_SOURCE_DIR}/rootIO/src/WriterRootTree.cc ${PROJECT_SOURCE_DIR}/rootIO/src/ReaderRootTree.cc ${PROJECT_SOURCE_DIR}/rootIO/src/Streamers.cc HepMC3rootIO.cxx)
set_property(TARGET realHepMC3rootIO PROPERTY POSITION_INDEPENDENT_CODE 1)

target_link_libraries(realHepMC3rootIO ${ROOT_Tree_LIBRARY} ${ROOT_RIO_LIBRARY} ${ROOT_Core_LIBRARY} HepMC3)
set_target_properties(realHepMC3rootIO PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
set_target_properties(realHepMC3rootIO PROPERTIES SOVERSION 1)
set_target_properties(realHepMC3rootIO PROPERTIES OUTPUT_NAME "HepMC3rootIO")
# installs 
install(TARGETS realHepMC3rootIO DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT rootIO-libs)
install(DIRECTORY ${PROJECT_BINARY_DIR}/rootIO/ DESTINATION  ${CMAKE_INSTALL_LIBDIR}  COMPONENT rootIO-libs  FILES_MATCHING  PATTERN "*.pcm"  PATTERN "*.rootmap"  PATTERN "CMakeFiles" EXCLUDE  PATTERN "src" EXCLUDE  PATTERN "include" EXCLUDE)
install(DIRECTORY include/HepMC3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT rootIO-devel)
