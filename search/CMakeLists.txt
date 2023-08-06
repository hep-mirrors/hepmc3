#--------
#-search
#--------

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set( hepmc3search_sources
${PROJECT_SOURCE_DIR}/search/src/Relatives.cc
${PROJECT_SOURCE_DIR}/search/src/Selector.cc)

add_library(HepMC3search SHARED ${hepmc3search_sources})
target_link_libraries(HepMC3search HepMC3)
set_target_properties(HepMC3search PROPERTIES OUTPUT_NAME HepMC3search   SOVERSION 5
  ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
  LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
  EXPORT_NAME HepMC3::search)

target_include_directories(HepMC3search PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# installs
install(TARGETS HepMC3search
  EXPORT HepMC3searchTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT searchlibs)

# Alias for packages that include this as a sub-project but
# want to use the namespaced target names
add_library(HepMC3::search ALIAS HepMC3search)

if (HEPMC3_BUILD_STATIC_LIBS)
  add_library(HepMC3search_static STATIC ${hepmc3search_sources})

  target_include_directories(HepMC3search_static PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

  target_compile_definitions(HepMC3search_static PRIVATE HEPMC3search_NO_StandardSelector_EXPORTS HEPMC3search_NO_Relatives_EXPORTS)
  set_target_properties(HepMC3search_static PROPERTIES OUTPUT_NAME HepMC3search-static
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    EXPORT_NAME HepMC3::search_static)
  install(TARGETS HepMC3search_static
    EXPORT HepMC3searchTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT searchlibs)

  # Alias for packages that include this as a sub-project but
  # want to use the namespaced target names
  add_library(HepMC3::search_static ALIAS HepMC3search_static)
endif()

install(DIRECTORY ${PROJECT_BINARY_DIR}/search/ DESTINATION  ${CMAKE_INSTALL_LIBDIR}  COMPONENT searchlibs  FILES_MATCHING PATTERN "CMakeFiles"  EXCLUDE  PATTERN "src"  EXCLUDE PATTERN "include"  EXCLUDE)
install(DIRECTORY include/HepMC3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT searchdevel)

set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/HepMC3/cmake)
install(EXPORT HepMC3searchTargets
  DESTINATION ${CONFIG_INSTALL_DIR}
  COMPONENT searchdevel)
