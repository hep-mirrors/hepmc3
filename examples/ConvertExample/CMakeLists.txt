set(basic_sources   ${CMAKE_CURRENT_SOURCE_DIR}/convert_example.cc
                    ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterDOT.cc
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterHEPEVTZEUS.cc )
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c PROPERTIES LANGUAGE CXX SKIP_LINTING ON)

set(basic_libraries HepMC3::HepMC3)
set(basic_defines HEPMCCONVERT_EXTENSION_HEPEVTZEUS=1 HEPMCCONVERT_EXTENSION_DOT=1)
set(basic_dependencies )
set(basic_libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" )
find_package(ZLIB)
if(ZLIB_FOUND)
  message(STATUS "HepMC3 ConvertExample: zlib found. zlib compressed files support enabled. ${ZLIB_INCLUDE_DIR} ${ZLIB_LIBRARIES} ")
  list(APPEND basic_libraries ZLIB::ZLIB)
  list(APPEND basic_defines HEPMC3_USE_COMPRESSION)
  list(APPEND basic_defines HEPMC3_Z_SUPPORT=1)
  # create environment scripts
  #set(basic_libdir "${basic_libdir}:${ZLIB_ROOT}")
elseif()
  message(STATUS "HepMC3 ConvertExample: zlib not found. zlib compressed files support disabled.")
endif()
if(${CMAKE_VERSION} VERSION_GREATER "3.14")
 find_package(LibLZMA)
endif()
if(LIBLZMA_FOUND)
  message(STATUS "HepMC3 ConvertExample: lzma found. lzma compressed files support enabled. ${LIBLZMA_INCLUDE_DIRS} ${LIBLZMA_LIBRARIES} ")
  list(APPEND basic_libraries LibLZMA::LibLZMA)
  list(APPEND basic_defines HEPMC3_USE_COMPRESSION)
  list(APPEND basic_defines HEPMC3_LZMA_SUPPORT=1)
elseif()
  message(STATUS "HepMC3 ConvertExample: lzma not found. lzma compressed files support disabled. Note: CMake>3.14 is required")
endif()
if(${CMAKE_VERSION} VERSION_GREATER "3.10")
  find_package(BZip2)
endif()
if(BZIP2_FOUND)
  message(STATUS "HepMC3 ConvertExample: bzip2 found. bzip2 compressed files support enabled. ${BZIP2_INCLUDE_DIRS} ${BZIP2_LIBRARIES} ")
  list(APPEND basic_libraries BZip2::BZip2)
  list(APPEND basic_defines HEPMC3_USE_COMPRESSION)
  list(APPEND basic_defines HEPMC3_BZ2_SUPPORT=1)
elseif()
  message(STATUS "HepMC3 ConvertExample: bzip not found. bzip2 compressed files support disabled. Note: CMake>3.10 is required")
endif()
if(ROOT_FOUND)
  list(APPEND basic_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/WriterRootTreeOPAL.cc )
  list(APPEND basic_libraries ROOT::Tree ROOT::RIO ROOT::Core HepMC3::rootIO)
  list(APPEND basic_defines HEPMCCONVERT_EXTENSION_ROOTTREEOPAL=1)
  # create environment scripts
  set(basic_libdir "${basic_libdir}:${ROOT_LIBRARY_DIR}")
endif()
if(Protobuf_FOUND)
  list(APPEND basic_libraries protobuf::libprotobuf HepMC3::protobufIO)
  # create environment scripts
  set(basic_libdir "${basic_libdir}:${Protobuf_LIBRARY_DIR}")
endif()


add_executable(convert_example.exe  ${basic_sources} )
add_library(Analysis SHARED src/AnalysisExample.cc)
target_link_libraries(Analysis HepMC3::HepMC3 )
target_include_directories(Analysis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )

if (MSVC)
  set(  MSVCLINKFLAGS "/WHOLEARCHIVE:${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/HepMC3-static.lib")
  if(ZLIB_FOUND)
    set(  MSVCLINKFLAGS "${MSVCLINKFLAGS} /WHOLEARCHIVE:${ZLIB_LIBRARIES}")
  endif()
  set_target_properties(convert_example.exe PROPERTIES LINK_FLAGS ${MSVCLINKFLAGS})
endif()
target_include_directories(convert_example.exe  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(convert_example.exe ${basic_libraries} )
target_compile_definitions(convert_example.exe PRIVATE ${basic_defines} )
if (basic_dependencies)
  add_dependencies( convert_example.exe ${basic_dependencies} )
endif()
# create environment scripts
set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(libdir ${basic_libdir})
set_target_properties(convert_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)
set_target_properties(Analysis PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/$<0:>)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/convert_example_env.sh.in
        ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/convert_example_env.sh @ONLY)

# installs
if (USE_INSTALLED_HEPMC3)
  install(TARGETS convert_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS Analysis DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR}  FILES_MATCHING PATTERN "*env.sh*" )
endif()

#This option is for developers only
if ( HEPMC3_USE_GENGETOPT )
  find_program(GENGETOPT gengetopt)
  if( NOT GENGETOPT )
    message( STATUS  "HepMC3 examples: gengetopt not found. Old scripts for options parsing will be used." )
  else()
    message( STATUS  "HepMC3 examples: gengetopt found in ${GENGETOPT}. New scripts for options parsing can be generated." )
    add_custom_command(
      OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.h
      COMMAND ${GENGETOPT} -N --no-version --input=${CMAKE_CURRENT_SOURCE_DIR}/cmdline.ggo
      --file-name cmdline --output-dir=${CMAKE_CURRENT_SOURCE_DIR}
      --include-getopt --show-required --unnamed-opts=\"inputfile outputfile\"
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.ggo
      COMMENT        "HepMC3 examples: Generating getopt parser code..."
    )
  endif()
  add_custom_target( cmdlineoptions DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.c ${CMAKE_CURRENT_SOURCE_DIR}/cmdline.h)
  add_dependencies(  convert_example.exe cmdlineoptions )
endif()


