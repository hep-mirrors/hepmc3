macro( hepmc_test testname )
  message( STATUS "HepMC3 test: building ${testname} for ${testname}" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}   ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
  target_link_libraries(${testname} PUBLIC  PRIVATE HepMC3 )
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
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/ ${CMAKE_CURRENT_SOURCE_DIR}/../rootIO/include/)
  target_link_libraries(${testname} PRIVATE HepMC3 HepMC3rootIO)
   
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  SET_TESTS_PROPERTIES( ${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUT${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUT${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_root_test )










set( CMAKE_INCLUDE_DIRECTORIES_BEFORE ON )
cmake_policy(SET CMP0057 NEW)
if (NOT HEPMC3_TEST_PACKAGES)
  if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(HEPMC3_TEST_PACKAGES "Threads,HepMC2,Pythia8,Photospp,Tauolapp,MCTester,valgrind")
  else()
    set(HEPMC3_TEST_PACKAGES "Threads,HepMC2,Pythia8,Photospp,Tauolapp,MCTester")
  endif()
endif()
message(STATUS "HepMC3 test: The following packages are enabled for tests HEPMC3_TEST_PACKAGES=${HEPMC3_TEST_PACKAGES}" )
string(REPLACE "," ";" HEPMC3_TEST_PACKAGES_LIST ${HEPMC3_TEST_PACKAGES})




file(GLOB INPUTFILES  "${CMAKE_CURRENT_SOURCE_DIR}/*.input" )
file(COPY ${INPUTFILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set( HepMC_tests
        testBoost
        testMass
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
        testReaderFactory1
        testSingleVertexHepMC2
         )

set( HepMC_root_tests 
        testIO2
        testIO4
        testReaderFactory2
        testRoot300
        testRootTree300
        )


set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
if ("Threads" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads)
endif()
if(Threads_FOUND AND NOT (${CMAKE_CXX_COMPILER_ID} MATCHES "PGI"))
  message(STATUS "HepMC3 test: Threads library found.  Thread safety tests enabled")
  list( APPEND HepMC_tests "testThreads1" )   
else()
  message(STATUS "HepMC3 test: No threads library found or disabled for compiller with ID ${CMAKE_CXX_COMPILER_ID}. Thread safety tests are disabled")
endif()

foreach ( test ${HepMC_tests} )
  hepmc_test( ${test} )
  set_tests_properties(${test} PROPERTIES TIMEOUT 2000)  #Relevant for testLoops
endforeach ( test ${HepMC_tests} )

if(Threads_FOUND)
  if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(testThreads1 PUBLIC "-pthread")
  endif()
  if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(testThreads1 PUBLIC "${CMAKE_THREAD_LIBS_INIT}")
  endif()
endif()
#So far the test with ROOT have issues on WIN32
if(HEPMC3_ENABLE_ROOTIO AND (NOT WIN32) )
  foreach ( test ${HepMC_root_tests} )
    hepmc_root_test( ${test} )
  endforeach ( test ${HepMC_root_tests} )
  target_link_libraries(testIO4 PRIVATE ${ROOT_Hist_LIBRARY})
endif()



set( libMCEG_sources ValidationControl.cc   SimpleEventTool.cc OutputValidationTool.cc)
set( libMCEG_sources_HEPMC3  )
set( libMCEG_link_libraries_HEPMC2  )
set( libMCEG_link_libraries_HEPMC3  )
set( libMCEG_include_directories   ${CMAKE_CURRENT_SOURCE_DIR}/../include/  )
set( MCEG_tests )

set( convert_tests )


if ("Pythia8" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(Pythia8 8.220)
endif()
if(NOT PYTHIA8_FOUND)
  message(STATUS "HepMC3 test: Pythia8 package not found, was disabled or too old. The minimal supported Pythia8 version is 8.220. Please check if PYTHIA8_ROOT_DIR is set properly.") 
else()
  message(STATUS "HepMC3 test: Pythia8 package found. ${PYTHIA8_LIBRARIES} ${PYTHIA8_VERSION}") 
  list( APPEND libMCEG_sources    "PythiaValidationTool.cc" )
  list( APPEND libMCEG_link_libraries_HEPMC2  "${PYTHIA8_LIBRARIES}")
  list( APPEND libMCEG_link_libraries_HEPMC3  "${PYTHIA8_LIBRARIES}")
  SET(interfacelocation ${PROJECT_SOURCE_DIR}/interfaces/pythia8/include/Pythia8)
  if (USE_INTERFACE_FROM_PYTHIA)
    find_file(interfacefrompythia8 ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugins/Pythia8ToHepMC3.h)
    if (interfacefrompythia8) 
      message(STATUS "HepMC3 test: Found Pythia8Plugins/Pythia8ToHepMC3.h  in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
    else()
      message(FATAL "HepMC3 test: Pythia8Plugins/Pythia8ToHepMC3.h  is absent in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
    endif()
    SET(interfacelocation ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugings)
  endif()
  list( APPEND libMCEG_include_directories  "${interfacelocation}"   "${PYTHIA8_INCLUDE_DIRS}")
  list( APPEND libMCEG_compile_definitions  "-DPYTHIA8")

  list( APPEND MCEG_tests "testPythia1" )   
  list( APPEND MCEG_tests "testPythia2" )   
  list( APPEND MCEG_tests "testPythia3" )   
endif()

if ("Photospp" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(Photospp )
endif()
if(NOT PHOTOSPP_FOUND)
  message(STATUS "HepMC3 test: Photospp package not found or disabled. Please check if PHOTOSPP_ROOT_DIR is set properly.") 
else()
  message(STATUS "HepMC3 test: Photospp package found. Building Photospp tests. PHOTOSPP_LIBRARIES ${PHOTOSPP_LIBRARIES} PHOTOSPP_HEPMC2_LIBRARIES ${PHOTOSPP_HEPMC2_LIBRARIES} PHOTOSPP_INCLUDE_DIRS ${PHOTOSPP_INCLUDE_DIRS}")
  list( APPEND  libMCEG_sources   "PhotosValidationTool.cc" )
  list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/Photospp/src/PhotosHepMC3Event.cxx" "${PROJECT_SOURCE_DIR}/interfaces/Photospp/src/PhotosHepMC3Particle.cxx"   )
  list( APPEND  libMCEG_link_libraries_HEPMC2  "${PHOTOSPP_LIBRARIES}"  "${PHOTOSPP_HEPMC2_LIBRARIES}")
  list( APPEND  libMCEG_link_libraries_HEPMC3  "${PHOTOSPP_LIBRARIES}" )
  list( APPEND  libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/Photospp/include" "${PROJECT_SOURCE_DIR}/interfaces/Photospp/include/Photos"  "${PHOTOSPP_INCLUDE_DIRS}" "${PHOTOSPP_INCLUDE_DIR}/Photos" )
  list( APPEND  libMCEG_compile_definitions  "-DPHOTOSPP")

  list( APPEND  MCEG_tests "testPhotos1" )   
endif()

if ("Tauolapp" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_package(Tauolapp )
endif()
if(NOT TAUOLAPP_FOUND)
  message(STATUS "HepMC3 test: Tauolapp package not found or disabled. Please check if TAUOLA_ROOT_DIR is set properly.") 
else()
  message(STATUS "HepMC3 test: Tauolapp package found. Building Tauolapp tests. TAUOLAPP_LIBRARIES ${TAUOLAPP_LIBRARIES} TAUOLAPP_HepMC_LIBRARY ${TAUOLAPP_HepMC_LIBRARY} TAUOLAPP_INCLUDE_DIRS ${TAUOLAPP_INCLUDE_DIRS}")
  list( APPEND  libMCEG_sources "TauolaValidationTool.cc" )
  list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/src/TauolaHepMC3Event.cxx" "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/src/TauolaHepMC3Particle.cxx"  )
  if ( TAUOLAPP_HepMC_LIBRARY )
    list( APPEND  libMCEG_link_libraries_HEPMC2  "${TAUOLAPP_CxxInterface_LIBRARY}" "${TAUOLAPP_Fortran_LIBRARY}"  "${TAUOLAPP_HepMC_LIBRARY}" )
  else()
    list( APPEND  libMCEG_link_libraries_HEPMC2  "${TAUOLAPP_CxxInterface_LIBRARY}" "${TAUOLAPP_Fortran_LIBRARY}" )
  endif()
  list( APPEND  libMCEG_link_libraries_HEPMC3  "${TAUOLAPP_CxxInterface_LIBRARY}" "${TAUOLAPP_Fortran_LIBRARY}" )
  list( APPEND  libMCEG_include_directories    "${TAUOLAPP_INCLUDE_DIRS}" "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/include") 
  list( APPEND  libMCEG_compile_definitions  "-DTAUOLAPP")
  list( APPEND  MCEG_tests "testTauola1" )   
endif()



if(HEPMC3_ENABLE_ROOTIO)
  if ("MCTester" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
    find_package(MCTester )
  endif()
  if(NOT MCTESTER_FOUND)
    message(STATUS "HepMC3 test: MCTester package not found or disabled. Please check if MCTESTER_ROOT_DIR is set properly.") 
  else()
    message(STATUS "HepMC3 test: MCTester package found. Building MCTester tests. MCTESTER_LIBRARIES ${MCTESTER_LIBRARIES} MCTESTER_INCLUDE_DIRS ${MCTESTER_INCLUDE_DIRS}")
    list( APPEND  libMCEG_sources "McTesterValidationTool.cc" )
    list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/src/HepMC3Event.cxx" "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/src/HepMC3Particle.cxx"   )
    list( APPEND  libMCEG_link_libraries_HEPMC2  "${MCTESTER_LIBRARIES}" "${ROOT_LIBRARIES}" )
    list( APPEND  libMCEG_link_libraries_HEPMC3  "${MCTESTER_LIBRARIES}" "${ROOT_LIBRARIES}" )
    list( APPEND  libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/include" "${MCTESTER_INCLUDE_DIRS}"  "${ROOT_INCLUDE_DIRS}")
    list( APPEND  libMCEG_compile_definitions  "-DMCTESTER")
    list( APPEND  MCEG_tests "testMCTester1" )   
  endif()
endif()


add_library(MCEG3 SHARED ${libMCEG_sources} ${libMCEG_sources_HEPMC3})
target_link_libraries(MCEG3  PRIVATE HepMC3 PUBLIC ${libMCEG_link_libraries_HEPMC3} )
target_compile_definitions(MCEG3  PRIVATE ${libMCEG_compile_definitions})
target_include_directories(MCEG3   BEFORE
PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
PRIVATE ${libMCEG_include_directories}
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/)



macro( hepmc_mceghepmc3_test testname )
  message( STATUS "HepMC3 test: building MCEG3 test ${testname} with HEPMC3" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/   )
  #The flag is needed for Taoula that conflates generic C++ interface with the HepMC2 interface
  #FIXME put here Visual studio flags when needed
  #For Darwin target_link_libraries(${testname}  "-undefined dynamic_lookup"  HepMC3  MCEG3)   
if (APPLE)  
  target_link_libraries(${testname}  "-undefined dynamic_lookup"  HepMC3  MCEG3)   
else()
  target_link_libraries(${testname}  "-Wl,--allow-shlib-undefined"  HepMC3  MCEG3)   
endif()
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  SET_TESTS_PROPERTIES( ${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUT${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUT${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_mceghepmc3_test )


foreach ( test ${MCEG_tests} )
  hepmc_mceghepmc3_test( ${test} )
endforeach ( test ${MCEG_tests} )




if ("HepMC2" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
find_package(HepMC2 QUIET)
endif()

if(NOT HEPMC2_FOUND)
  message(STATUS "HepMC3 test: HEPMC2 package not found or disabled. Please check if HEPMC2_ROOT_DIR is set properly.") 
else()
  message(STATUS "HepMC3 test: HEPMC2 package found. ${HEPMC2_LIBRARIES}") 
  add_library(MCEG2 SHARED ${libMCEG_sources})
  target_link_libraries(MCEG2  PRIVATE ${HEPMC2_LIBRARIES} PUBLIC ${libMCEG_link_libraries_HEPMC2} )
  target_compile_definitions(MCEG2  PRIVATE ${libMCEG_compile_definitions} -DHEPMC2)
  target_include_directories(MCEG2   BEFORE
  PUBLIC ${HEPMC2_INCLUDE_DIRS}
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
  PRIVATE ${libMCEG_include_directories}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   )

  list( APPEND convert_tests "testConvert1" )  
  list( APPEND convert_tests "testConvert2" )  

macro( hepmc_mceghepmc2_test testname )
  message( STATUS "HepMC3 test: building MCEG2 test ${testname} with HEPMC2" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(hepmc2_${testname} ${${testname}_source}  ${ARGN} )
  target_compile_definitions(hepmc2_${testname}  PRIVATE  -DHEPMC2)
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/   )  
  target_link_libraries(hepmc2_${testname} PRIVATE ${HEPMC2_LIBRARIES}   MCEG2   )
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( hepmc2_${testname} hepmc2_${testname} )
  SET_TESTS_PROPERTIES(  hepmc2_${testname} PROPERTIES ENVIRONMENT "PATH=${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/;$ENV{PATH}")
  file(GLOB INPUThepmc2_${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUThepmc2_${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  string(REPLACE "test" "" ${testname} name)
  file(GLOB INPUThepmc2_${name}  "${CMAKE_CURRENT_SOURCE_DIR}/input${name}*" )
  file(COPY ${INPUThepmc2_${name}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_mceghepmc2_test )



foreach ( test ${MCEG_tests} )
  hepmc_mceghepmc2_test( ${test} )
endforeach ( test ${MCEG_tests} )

macro( hepmc_convert_test testname )
  message( STATUS "HepMC3 test: building Convert test ${testname} with HEPMC2" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  message( STATUS "HepMC3 test: building Convert test ${testname} ${${testname}_source} with HEPMC2" )
  ADD_EXECUTABLE(convert_${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(convert_${testname} BEFORE
  PUBLIC ${HEPMC2_INCLUDE_DIRS}
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/   "${PROJECT_SOURCE_DIR}/interfaces/HepMCCompatibility/include" )  
  target_link_libraries(convert_${testname} PRIVATE ${HEPMC2_LIBRARIES}   HepMC3  )
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

endif()

#One should use something else for Windows/BSD/MAC


if ("valgrind" IN_LIST HEPMC3_TEST_PACKAGES_LIST)
  find_program( MEMORYCHECK_COMMAND valgrind )
endif()
if(MEMORYCHECK_COMMAND)
  message(STATUS "HepMC3 test: valgrind package found in ${MEMORYCHECK_COMMAND}.  memory check tests enabled.") 
  set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --error-exitcode=1" )
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
  foreach ( test ${HepMC_tests} )
    add_memcheck_test( ${test} )
    set_tests_properties(memcheck_${test} PROPERTIES DEPENDS ${test})
  endforeach ( test ${HepMC_tests} )
endif()

