

macro( hepmc_test testname )

  message( STATUS "building ${testname} for ${testname}" )

  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR} )
  #ADD_EXECUTABLE(${testname} ${${testname}_source}  CompareGenEvent.cc ${ARGN} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}   ${ARGN} )
  target_include_directories(${testname} BEFORE
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/
        )
   target_link_libraries(${testname}
    PUBLIC        
    PRIVATE
        HepMC
)
    set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
    set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
    add_test( ${testname} ${CMAKE_CURRENT_BINARY_DIR}/${testname} )
 
endmacro( hepmc_test )

macro( hepmc_root_test testname )

  message( STATUS "building ROOT test ${testname} " )

  # include test in search path
#  include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}")

  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR} )
  #message(STATUS "building simple test ${testname} from ${${testname}_source}")
  #ADD_EXECUTABLE(${testname} ${${testname}_source}   CompareGenEvent.cc ${ARGN} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/ ${CMAKE_CURRENT_SOURCE_DIR}/../rootIO/include/
        )
   LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../outputs/lib64/  )
   target_link_libraries(${testname} HepMC HepMCrootIO)
   
       set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
    set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
   

  add_test( ${testname} ${testname} )

endmacro( hepmc_root_test )


#link_libraries( HepMC )
#list(APPEND INCLUDE_DIRS )
set( CMAKE_INCLUDE_DIRECTORIES_BEFORE ON )

set( HepMC_tests
		#testHepMCIteration
		testMass
		#testFlow
        testPrintBug
		testPolarization 
        #testSimpleVector
        testUnits
		#testMultipleCopies
		testWeights 
		testLoops
		testIO1
		testIO3
		 )
if(HEPMC_ENABLE_ROOTIO)
set( HepMC_root_tests 
		testIO2
		testIO4
		)
elseif()
set( HepMC_root_tests 
		)
endif()


# automake/autoconf variables for *.cc.in
set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )

FILE( GLOB process_source *.cc.in )
foreach( sfile ${process_source} )
  get_filename_component(basename ${sfile} NAME_WE )
  configure_file( ${sfile}
                  ${CMAKE_CURRENT_BINARY_DIR}/${basename}.cc  @ONLY )
endforeach( sfile ${process_source} )

#hepmc_test( testHepMC testHepMCMethods.cc )
#hepmc_test( testStreamIO testHepMCMethods.cc )

foreach ( test ${HepMC_tests} )
  hepmc_test( ${test} )
endforeach ( test ${HepMC_tests} )
set_tests_properties(testLoops PROPERTIES TIMEOUT 5)

foreach ( test ${HepMC_root_tests} )
  hepmc_root_test( ${test} )
endforeach ( test ${HepMC_root_tests} )
