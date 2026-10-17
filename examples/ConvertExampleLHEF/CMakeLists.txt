
set(basic_sources   ${CMAKE_CURRENT_SOURCE_DIR}/convert_example_LHEF.cc
                    ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c )
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c PROPERTIES LANGUAGE CXX SKIP_LINTING ON)

add_executable(convert_example_LHEF.exe  ${basic_sources} )
target_include_directories(convert_example_LHEF.exe  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(convert_example_LHEF.exe PRIVATE cxx_std_11)

set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(libdir ${basic_libdir})
set_target_properties(convert_example_LHEF.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)

# installs
if (USE_INSTALLED_HEPMC3)
  install(TARGETS convert_example_LHEF.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

#This option is for developers only
if ( HEPMC3_USE_GENGETOPT )
  find_program(GENGETOPT gengetopt)
  if( NOT GENGETOPT )
    message( STATUS  "HepMC3 ConvertExampleLHEF: gengetopt not found. Old scripts for options parsing will be used." )
  else()
    message( STATUS  "HepMC3 ConvertExampleLHEF: gengetopt found in ${GENGETOPT}. New scripts for options parsing can be generated." )
    add_custom_command(
      OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.h
      COMMAND ${GENGETOPT} -N --no-version --input=${CMAKE_CURRENT_SOURCE_DIR}/cmdline.ggo
      --file-name cmdline --output-dir=${CMAKE_CURRENT_SOURCE_DIR}
      --include-getopt --show-required --unnamed-opts=\"inputfile outputfile\"
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.ggo
      COMMENT        "HepMC3 ConvertExampleLHEF: Generating getopt parser code..."
    )
  endif()
  add_custom_target( cmdlineoptionsLHEF DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.h)
  add_dependencies(  convert_example_LHEF.exe cmdlineoptionsLHEF )
endif()


