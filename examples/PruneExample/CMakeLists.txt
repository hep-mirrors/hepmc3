set(basic_sources   ${CMAKE_CURRENT_SOURCE_DIR}/prune_example.cc)
set(basic_includes ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR} )
set(basic_libraries HepMC3::HepMC3)
set(basic_defines )
set(basic_dependencies )
set(basic_libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" )
find_package(ZLIB)

add_executable(prune_example.exe  ${basic_sources} )
include_directories(${basic_includes})
target_link_libraries(prune_example.exe ${basic_libraries} )
target_compile_definitions(prune_example.exe PRIVATE ${basic_defines} )
if (basic_dependencies)
  add_dependencies( prune_example.exe ${basic_dependencies} )
endif()
# create environment scripts
set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(libdir ${basic_libdir})
set_target_properties(prune_example.exe
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)

# installs
if (USE_INSTALLED_HEPMC3)
  install(TARGETS prune_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()



