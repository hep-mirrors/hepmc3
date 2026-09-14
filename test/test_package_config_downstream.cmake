set(_install_prefix "${HEPMC3_TEST_BINARY_DIR}/install")
set(_consumer_binary_dir "${HEPMC3_TEST_BINARY_DIR}/build")

file(REMOVE_RECURSE "${HEPMC3_TEST_BINARY_DIR}")

if(CMAKE_VERSION VERSION_LESS 3.15)
  set(_install_root "${HEPMC3_TEST_BINARY_DIR}/stage")
  set(_install_prefix "${_install_root}${HEPMC3_INSTALL_PREFIX}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E env "DESTDIR=${_install_root}"
      "${CMAKE_COMMAND}" --build "${HEPMC3_BINARY_DIR}" --target install
    RESULT_VARIABLE _install_result
    OUTPUT_VARIABLE _install_stdout
    ERROR_VARIABLE _install_stderr)
    if(NOT _install_result EQUAL 0)
      message(FATAL_ERROR
      "Could not install HepMC3 for the downstream package-config test:\n"
      "${_install_stdout}${_install_stderr}")
    endif()
else()
  foreach(_component IN LISTS HEPMC3_COMPONENTS)
    if(_component MATCHES "^python")  # skip Python as it has a different install directory
      continue()
    endif()
    execute_process(
      COMMAND "${CMAKE_COMMAND}" --install "${HEPMC3_BINARY_DIR}" --prefix "${_install_prefix}" --component "${_component}"
      RESULT_VARIABLE _install_result_component
      OUTPUT_VARIABLE _install_stdout_component
      ERROR_VARIABLE _install_stderr_component)
      if(NOT _install_result_component EQUAL 0)
        message(FATAL_ERROR
        "Could not install HepMC3 for the downstream package-config test:\n"
        "${_install_stdout_component}${_install_stderr_component} for ${component}")
      endif()
  endforeach()
endif()



if(CMAKE_VERSION VERSION_LESS 3.13)
  file(MAKE_DIRECTORY "${_consumer_binary_dir}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" "${HEPMC3_TEST_SOURCE_DIR}"
      "-DHepMC3_DIR=${_install_prefix}/${HEPMC3_INSTALL_DATADIR}/HepMC3/cmake"
    WORKING_DIRECTORY "${_consumer_binary_dir}"
    RESULT_VARIABLE _configure_result
    OUTPUT_VARIABLE _configure_stdout
    ERROR_VARIABLE _configure_stderr)
else()
  execute_process(
    COMMAND "${CMAKE_COMMAND}"
      -S "${HEPMC3_TEST_SOURCE_DIR}"
      -B "${_consumer_binary_dir}"
      "-DHepMC3_DIR=${_install_prefix}/${HEPMC3_INSTALL_DATADIR}/HepMC3/cmake"
    RESULT_VARIABLE _configure_result
    OUTPUT_VARIABLE _configure_stdout
    ERROR_VARIABLE _configure_stderr)
endif()
if(NOT _configure_result EQUAL 0)
  message(FATAL_ERROR
    "Installed HepMC3 could not be found by an old-policy downstream project:\n"
    "${_configure_stdout}${_configure_stderr}")
endif()
