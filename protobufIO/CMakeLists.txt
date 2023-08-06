protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS HepMC3.proto)

set(HepMC3protobufIO_sources
  src/Writerprotobuf.cc
  src/Readerprotobuf.cc
  ${PROTO_SRCS})

#---Define dynamic library---------------------------------------------------
add_library(HepMC3protobufIO SHARED ${HepMC3protobufIO_sources})

target_include_directories(HepMC3protobufIO PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>)

target_include_directories(HepMC3protobufIO PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(HepMC3protobufIO HepMC3 protobuf::libprotobuf)

set_target_properties(HepMC3protobufIO
  PROPERTIES
    SOVERSION 1
    POSITION_INDEPENDENT_CODE 1
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
    EXPORT_NAME HepMC3::protobufIO)

#---Install shared build products---------------------------------------------------
install(TARGETS HepMC3protobufIO
  EXPORT HepMC3protobufIOTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT protobufIOlibs)

# Alias for packages that include this as a sub-project but
# want to use the namespaced target names
add_library(HepMC3::protobufIO ALIAS HepMC3protobufIO)

install(FILES
          ${CMAKE_CURRENT_SOURCE_DIR}/include/HepMC3/Writerprotobuf.h
          ${CMAKE_CURRENT_SOURCE_DIR}/include/HepMC3/Readerprotobuf.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/HepMC3
  COMPONENT protobufIOdevel)

if (HEPMC3_BUILD_STATIC_LIBS)
#---Define static library---------------------------------------------------
  add_library(HepMC3protobufIO_static STATIC ${HepMC3protobufIO_sources})
  target_compile_definitions(HepMC3protobufIO_static PRIVATE HEPMC3_NO_EXPORTS)

  target_include_directories(HepMC3protobufIO_static PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>)

  target_include_directories(HepMC3protobufIO_static PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

  set_target_properties(HepMC3protobufIO_static
    PROPERTIES
      OUTPUT_NAME HepMC3protobufIO-static
      ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_LIBDIR}/$<0:>
      EXPORT_NAME HepMC3::protobufIO_static)

#---Install static build products---------------------------------------------------
  install(TARGETS HepMC3protobufIO_static
    EXPORT HepMC3protobufIOTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT protobufIOlibs)

  # Alias for packages that include this as a sub-project but
  # want to use the namespaced target names
  add_library(HepMC3::protobufIO_static ALIAS HepMC3protobufIO_static)
endif()

set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/HepMC3/cmake)
install(EXPORT HepMC3protobufIOTargets
  DESTINATION ${CONFIG_INSTALL_DIR}
  COMPONENT protobufIOlibs)
