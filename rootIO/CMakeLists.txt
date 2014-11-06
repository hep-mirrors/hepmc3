#--------
#-root IO

if(ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/rootIO/include ${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(rootIO_Classes include/rootIO_Classes.hh LINKDEF include/LinkDef.hh)

list(APPEND ROOT_FIND_COMPONENTS RIO)

add_library(rootIO ${PROJECT_SOURCE_DIR}/rootIO/src/IO_RootStreamer.cc rootIO_Classes.cxx)
target_link_libraries(rootIO ${ROOT_LIBRARIES} RIO HepMC)
set_target_properties(rootIO PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)


# installs 
install(TARGETS rootIO DESTINATION lib)
