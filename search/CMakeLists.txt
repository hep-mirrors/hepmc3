#--------
#-search
#--------
include_directories(${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR} )

set( hepmc3search_sources 
${PROJECT_SOURCE_DIR}/search/src/Relatives.cc 
${PROJECT_SOURCE_DIR}/search/src/Selector.cc)

add_library(objlibsearch OBJECT ${hepmc3search_sources})

set_property(TARGET objlibsearch PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlibsearch PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(HepMC3search SHARED $<TARGET_OBJECTS:objlibsearch>)
add_library(HepMC3search_static STATIC  $<TARGET_OBJECTS:objlibsearch>)

target_link_libraries(HepMC3search  HepMC3)
set_target_properties(HepMC3search PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
set_target_properties(HepMC3search PROPERTIES SOVERSION 1)
if (WIN32)
set_target_properties(HepMC3search_static PROPERTIES OUTPUT_NAME HepMC3search-static ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
elseif()
set_target_properties(HepMC3search_static PROPERTIES OUTPUT_NAME HepMC3search ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
endif()


# installs 
install(TARGETS HepMC3search DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT search-libs)
install(TARGETS HepMC3search_static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT search-libs)

install(DIRECTORY ${PROJECT_BINARY_DIR}/search/ DESTINATION  ${CMAKE_INSTALL_LIBDIR}  COMPONENT search-libs  FILES_MATCHING PATTERN "CMakeFiles"  EXCLUDE  PATTERN "src"  EXCLUDE PATTERN "include"  EXCLUDE)
install(DIRECTORY include/HepMC3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT search-devel)
