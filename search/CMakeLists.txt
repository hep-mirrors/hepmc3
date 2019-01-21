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

add_library(HepMCsearch SHARED $<TARGET_OBJECTS:objlibsearch>)
add_library(HepMCsearch_static STATIC  $<TARGET_OBJECTS:objlibsearch>)

target_link_libraries(HepMCsearch  HepMC)
set_target_properties(HepMCsearch PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})

if (WIN32)
set_target_properties(HepMCsearch_static PROPERTIES OUTPUT_NAME HepMCsearch-static ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
elseif()
set_target_properties(HepMCsearch_static PROPERTIES OUTPUT_NAME HepMCsearch ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR})
endif()


# installs 
install(TARGETS HepMCsearch DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)
install(TARGETS HepMCsearch_static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)

install(DIRECTORY ${PROJECT_BINARY_DIR}/search/ DESTINATION  ${CMAKE_INSTALL_LIBDIR}  COMPONENT search-libs  FILES_MATCHING PATTERN "CMakeFiles"  EXCLUDE  PATTERN "src"  EXCLUDE PATTERN "include"  EXCLUDE)
install(DIRECTORY include/HepMC DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT search-devel)
add_custom_target(move_search_headers ALL ${CMAKE_COMMAND} -DPREFIX=${CMAKE_BINARY_DIR}/outputs
                                   -DCOMPONENTS="search-devel"
                                   -P ${CMAKE_SOURCE_DIR}/cmake/Modules/local_install.cmake )
