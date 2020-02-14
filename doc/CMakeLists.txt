find_package(Doxygen)
if (DOXYGEN_FOUND)
find_program(DOXYGEN NAMES doxygen doxygen.exe HINTS ${DOXYGEN_EXECUTABLE})
else()
find_program(DOXYGEN NAMES doxygen doxygen.exe )
endif()
if (DOXYGEN)
message(STATUS "HepMC3 doc: doxygen found in ${DOXYGEN}. Generation of documentation is enabled.")
add_custom_target( doxydoc ALL
COMMAND ${DOXYGEN}  Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/html DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT doc)
else()
message(STATUS "HepMC3 doc: doxygen not found, generation of documentation is disabled.")
endif()
