#Beacause of LZMA targets
if (HEPMC3_TEST_LZMA)
  cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
endif()
if (HEPMC3_TEST_BZIP2)
  cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
endif()


macro( hepmc_test testname )
  message( STATUS "HepMC3 test: building ${testname} for ${testname}" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}   ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src )
  target_link_libraries(${testname} PUBLIC  PRIVATE HepMC3::HepMC3 )
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${CMAKE_CURRENT_BINARY_DIR}/${testname} )
  SET_TESTS_PROPERTIES( ${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUT${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUT${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_test )

macro( hepmc_root_test testname )
  message( STATUS "HepMC3 test: building ROOT test ${testname} " )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src )
  target_link_libraries(${testname} PRIVATE HepMC3::HepMC3 HepMC3::rootIO)

  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  SET_TESTS_PROPERTIES( ${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}"
  ENVIRONMENT "LD_LIBRARY_PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{LD_LIBRARY_PATH}"
  ENVIRONMENT "DYLD_LIBRARY_PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{DYLD_LIBRARY_PATH}"
  )
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUT${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUT${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_root_test )


macro( hepmc_protobuf_test testname )
  message( STATUS "HepMC3 test: building protobuf test ${testname} " )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src  )
  target_link_libraries(${testname} PRIVATE HepMC3::HepMC3 HepMC3::protobufIO)

  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  SET_TESTS_PROPERTIES( ${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUT${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUT${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_protobuf_test )


set( CMAKE_INCLUDE_DIRECTORIES_BEFORE ON )
cmake_policy(SET CMP0057 NEW)

option(HEPMC3_TEST_THREADS   "Use Threads in tests if available" ON)
option(HEPMC3_TEST_HEPMC2    "Use HepMC2 in tests if available" ON)
option(HEPMC3_TEST_VALGRIND  "Use valgrind in tests if available" OFF)
option(HEPMC3_TEST_ZLIB      "Use zlib in tests if available" ON)
option(HEPMC3_TEST_LZMA      "Use lzma in tests if available" ON)
option(HEPMC3_TEST_BZIP2     "Use bzip2 in tests if available" ON)
option(HEPMC3_TEST_ZSTD      "Use zstd in tests if available" ON)

set(HEPMC3_TEST_PACKAGES_LIST "")
if (HEPMC3_TEST_THREADS)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "Threads")
endif()
if (HEPMC3_TEST_HEPMC2)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "HepMC2")
endif()
if (HEPMC3_TEST_VALGRIND)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "valgrind")
endif()
if (HEPMC3_TEST_ZLIB)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "zlib")
endif()
if (HEPMC3_TEST_LZMA)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "lzma")
endif()
if (HEPMC3_TEST_BZIP2)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "bzip2")
endif()
if (HEPMC3_TEST_ZSTD)
 LIST(APPEND HEPMC3_TEST_PACKAGES_LIST "zstd")
endif()
message(STATUS "HepMC3 test: The following packages are enabled for tests HEPMC3_TEST_PACKAGES_LIST=${HEPMC3_TEST_PACKAGES_LIST}" )

file(GLOB INPUTFILES  "${CMAKE_CURRENT_SOURCE_DIR}/*.input" )
file(COPY ${INPUTFILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set( HepMC_tests
        testBoost
        testFourVector
        testFourVectorSafe
        testMass
        testPrint
        testPrintBug
        testPolarization
        testUnits
        testMultipleCopies
        testWeights
        testLoops
        testDelete
        testSkip1
        testIO1
        testIO3
        testIO6
        testIO7
        testIO8
        testIO11
        testIO12
        testIO13
        testReaderFactory1
        testSingleVertexHepMC2
        testAttributes
        testHEPEVTWrapper1
        testOrder
        testVertexAttributes
         )
set( HepMC_root_tests
        testIO2
        testIO4
        testReaderFactory2
        testRoot300
        testRootTree300
        )

set( HepMC_search_tests
        testSearch1
        )
if (WIN32)
  set( HepMC_protobuf_tests )
else()
  set( HepMC_protobuf_tests
        testIO20
        testIO21
        testIO22
        testIO23
        testIO24
        testIO25
        testIO26
        testIO27
        testIO28
        testIO29
        )
endif()

set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
if ("Threads" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads)
endif()
if("Threads" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND Threads_FOUND AND NOT (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI") AND NOT (${CMAKE_CXX_COMPILER_ID} MATCHES "NVHPC"))
  message(STATUS "HepMC3 test: Threads library found.  Thread safety tests enabled")
  list( APPEND HepMC_tests "testThreads1" )
  list( APPEND HepMC_tests "testIO10" )
  list( APPEND HepMC_tests "testReaderFactory3" )
  if (HEPMC3_ENABLE_SEARCH)
    list( APPEND HepMC_search_tests "testThreadssearch" )
  endif()
else()
  message(STATUS "HepMC3 test: No threads library found or disabled for compiller with ID ${CMAKE_CXX_COMPILER_ID}. Thread safety tests are disabled")
endif()

set(compress_tests testIO9)
if(HEPMC3_ENABLE_PROTOBUFIO AND NOT WIN32)
  LIST(APPEND compress_tests testReaderFactory4)
endif()

if ("zlib" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(ZLIB)
  if(ZLIB_FOUND)
    message(STATUS "HepMC3 test: ZLIB library found. zlib tests enabled")
    list( APPEND HepMC_tests ${compress_tests} )
  endif()
endif()
if ("lzma" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(LibLZMA)
  if(LIBLZMA_FOUND)
    message(STATUS "HepMC3 test: LZMA library found.  lzma tests enabled")
    list( APPEND HepMC_tests ${compress_tests} )
  endif()
endif()
if ("bzip2" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(BZip2)
  if(LIBLZMA_FOUND)
    message(STATUS "HepMC3 test: BZip2 library found.  BZip2 tests enabled")
    list( APPEND HepMC_tests ${compress_tests} )
  endif()
endif()
if ("zstd" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(zstd 1.4.0)
  if(zstd_FOUND)
    message(STATUS "HepMC3 test: Zstd library version ${zstd_VERSION} found.  Zstd tests enabled")
    list( APPEND HepMC_tests ${compress_tests} )
  endif()
endif()

## Here we add the tests
list(REMOVE_DUPLICATES HepMC_tests)
foreach ( test ${HepMC_tests} )
  hepmc_test( ${test} )
  set_tests_properties(${test} PROPERTIES TIMEOUT 2000)  #Relevant for testLoops
endforeach ( test ${HepMC_tests} )

if(HEPMC3_ENABLE_SEARCH )
  foreach ( test ${HepMC_search_tests} )
    hepmc_test( ${test} )
    target_link_libraries(${test} PRIVATE HepMC3::search)
  endforeach ( test ${HepMC_search_tests} )
endif()

if(HEPMC3_ENABLE_PROTOBUFIO )
  foreach ( test ${HepMC_protobuf_tests} )
    hepmc_protobuf_test( ${test} )
  target_link_libraries(${test} PRIVATE HepMC3::protobufIO)
  endforeach ( test ${HepMC_protobuf_tests} )
endif()


## Treatment of special needs in the HepMC tests
if(Threads_FOUND AND NOT (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI") AND NOT (${CMAKE_CXX_COMPILER_ID} MATCHES "NVHPC"))
    target_link_libraries(testThreads1 PUBLIC Threads::Threads)
    target_link_libraries(testIO10 PUBLIC Threads::Threads)
    target_link_libraries(testReaderFactory3 PUBLIC Threads::Threads)
    if (HEPMC3_ENABLE_SEARCH)
     target_link_libraries(testThreadssearch PUBLIC Threads::Threads)
    endif()
endif()
if (WIN32)
  if ("zlib" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND ZLIB_FOUND)
     get_filename_component(z ${ZLIB_LIBRARIES} DIRECTORY)
     find_file(zlibdll NAME zlib.dll PATHS ${z} ${z}/../bin/)
     if (zlibdll)
       file(COPY ${zlibdll} DESTINATION ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
     endif()
   endif()
   if ("lzma" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND LIBLZMA_FOUND)
     get_filename_component(l ${LIBLZMA_LIBRARIES} DIRECTORY)
     find_file(lzmadll NAME lzma.dll PATHS ${l} ${l}/../bin/)
     if (lzmadll)
       file(COPY ${lzmadll} DESTINATION ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
     endif()
   endif()
   if ("bzip2" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND BZIP2_FOUND)
     get_filename_component(b ${BZIP2_LIBRARIES} DIRECTORY)
     find_file(libbz2dll NAME libbz2.dll PATHS ${b} ${b}/../bin/)
     if (libbz2dll)
       file(COPY ${libbz2dll} DESTINATION ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
     endif()
   endif()
   if ("zstd" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND zstd_FOUND)
     get_filename_component(b ${zstd_LIBRARY} DIRECTORY)
     find_file(zstddll NAME libzstd.dll PATHS ${b} ${b}/../bin/)
     if (zstddll)
       file(COPY ${zstddll} DESTINATION ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/)
     endif()
   endif()
endif()

foreach ( ctest ${compress_tests} )
  if ("zlib" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND ZLIB_FOUND)
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_USE_COMPRESSION")
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_Z_SUPPORT=1")
    target_link_libraries(${ctest} PRIVATE ZLIB::ZLIB )
  endif()
  if ("lzma" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND LIBLZMA_FOUND)
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_USE_COMPRESSION")
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_LZMA_SUPPORT=1")
    target_link_libraries(${ctest} PRIVATE LibLZMA::LibLZMA )
  endif()
  if ("bzip2" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND BZIP2_FOUND)
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_USE_COMPRESSION")
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_BZ2_SUPPORT=1")
    target_link_libraries(${ctest} PRIVATE BZip2::BZip2 )
  endif()
  if ("zstd" IN_LIST HEPMC3_TEST_PACKAGES_LIST AND zstd_FOUND)
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_USE_COMPRESSION")
    target_compile_options(${ctest} PUBLIC "-DHEPMC3_ZSTD_SUPPORT=1")
    target_link_libraries(${ctest} PRIVATE zstd::libzstd_shared )
  endif()
endforeach ( ctest ${compress_tests} )

if(HEPMC3_ENABLE_PROTOBUFIO AND "testReaderFactory4" IN_LIST compress_tests)
    target_link_libraries(testReaderFactory4 PRIVATE HepMC3::protobufIO)
endif()

#So far the test with ROOT have issues on WIN32
if(HEPMC3_ENABLE_ROOTIO AND (NOT WIN32) )
  foreach ( test ${HepMC_root_tests} )
    hepmc_root_test( ${test} )
  endforeach ( test ${HepMC_root_tests} )
  target_link_libraries(testIO4 PRIVATE ROOT::Hist)
endif()


set( convert_tests )


if ("HepMC2" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(HepMC2 QUIET)
endif()

if(NOT HepMC2_FOUND)
  message(STATUS "HepMC3 test: HEPMC2 package not found or disabled. Please check if HEPMC2_ROOT_DIR is set properly.")
else()
  list( APPEND convert_tests "testConvert1" )
  list( APPEND convert_tests "testConvert2" )
endif()


macro( hepmc_convert_test testname )
  message( STATUS "HepMC3 test: building Convert test ${testname} with HEPMC2" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  message( STATUS "HepMC3 test: building Convert test ${testname} ${${testname}_source} with HEPMC2" )
  ADD_EXECUTABLE(convert_${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(convert_${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src "${PROJECT_SOURCE_DIR}/interfaces/HepMCCompatibility/include" )
  target_link_libraries(convert_${testname} PRIVATE HepMC2::HepMC2  HepMC3::HepMC3  )
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( convert_${testname} convert_${testname} )
  SET_TESTS_PROPERTIES(  convert_${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUTconvert_${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUTconvert_${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUTconvert_${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUTconvert_${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_convert_test )

foreach ( test ${convert_tests} )
  hepmc_convert_test( ${test} )
endforeach ( test ${convert_tests} )


#One should use something else for Windows/BSD/MAC


if ("valgrind" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_program( MEMORYCHECK_COMMAND valgrind )
endif()
if(MEMORYCHECK_COMMAND)
  message(STATUS "HepMC3 test: valgrind package found in ${MEMORYCHECK_COMMAND}.  memory check tests enabled.")
  set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --error-exitcode=1 --default-suppressions=yes --gen-suppressions=all" )
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "Intel")
     file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/intelHepMC3.supp" "{\n<HepMC3_at_Linux_with_Intel_1>\nMemcheck:Addr32\nfun:__intel_*\n}\n{\n<HepMC3_at_Linux_with_Intel_2>\nMemcheck:Cond\nfun:*strchr\n}\n"  )
     set( MEMORYCHECK_COMMAND_OPTIONS  "${MEMORYCHECK_COMMAND_OPTIONS} --suppressions=${CMAKE_CURRENT_BINARY_DIR}/IntelHepMC3.supp")
  endif()
elseif()
  message(STATUS "HepMC3 test: valgrind package not found or disabled.  memory check tests disabled.")
endif()


macro(add_memcheck_test binaryname )
  set(memcheck_command "${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}")
  separate_arguments(memcheck_command)
  add_test(memcheck_${binaryname} ${memcheck_command} ./${binaryname} ${ARGN})
  SET_TESTS_PROPERTIES( memcheck_${binaryname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUTmemcheck_${binaryname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${binaryname}*" )
  file(COPY ${INPUTmemcheck_${binaryname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${binaryname} name)
  file(GLOB INPUTmemcheck_${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUTmemcheck_${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro(add_memcheck_test)


if(MEMORYCHECK_COMMAND)
  set( HepMC_memcheck_tests ${HepMC_tests} )
  if(HEPMC3_ENABLE_SEARCH )
    set( HepMC_memcheck_tests ${HepMC_memcheck_tests} ${HepMC_search_tests} )
  endif()
  if(HEPMC3_ENABLE_PROTOBUFIO )
    set( HepMC_memcheck_tests ${HepMC_memcheck_tests} ${HepMC_protobuf_tests} )
  endif()
  list(REMOVE_ITEM HepMC_memcheck_tests testFourVector )
#The story behind is complicated. It seems that on some platforms valgind dislikes the comparison of infinities.
  foreach ( test ${HepMC_memcheck_tests} )
    add_memcheck_test( ${test} )
    set_tests_properties(memcheck_${test} PROPERTIES DEPENDS ${test})
  endforeach ( test ${HepMC_memcheck_tests} )
endif()

