# HepMC3 source-package structure

The package sources are organized as follows:

- The topmost directory contains documentation for users and developers,
  the contributing-author list, the main CMake build file `CMakeLists.txt`,
  the licence details, the change log, and a config-script template.

- The `src/` directory contains the core set of library sources
  while the corresponding headers are located in `include/HepMC3/`.

  The subdirectory `include/HepMC3/Data/` includes headers with
  definitions of passive-data (POD) structures used in the
  library. The `include/HepMC3/bxzstr/` contains a copy of the
  header-only `bxzstr` compression library.

- The `search/` directory contains the `search` sub-library, with
  source files in `search/src/` and the headers in
  `search/include/HepMC3/`. The `search/CMakeLists.txt` file is used
  by CMake to build the `search` sub-library.

- The `rootIO/` directory contains the `rootIO` sub-library, with
   source files in `rootIO/src/` and headers in `rootIO/include/` and
   `rootIO/include/HepMC3/`. The `rootIO/CMakeLists.txt` file is used
   by CMake to build the `rootIO` sublibrary.

- The `python/` directory contains the source files required for
  compilation of the Python bindings to HepMC3. These include the
  `pybind11` headers in `python/include/pybind11/`, and custom and
  automatically generated binding sources in `python/src/`. The
  automatic bindings are made using the `binder` configurations in the
  `python/src/*.binder` files and the header-file templates
  `python/*.hpp.in`. The file-sets `python/src/**/__init__.py` and
  `pyHepMC3.*egg-info.in` are used to build a Python package and
  installation, via the `python/CMakeLists.txt` CMake build
  configuration.

- The `interfaces/` directory contains subdirectories with interfaces
  (C++/Fortran source files, C++ headers) for legacy Monte Carlo event
  generators and event processing tools, e.g. HepMC2 and Pythia6.


- The `test/` and `python/test/` directories contain sets of files
  (source files, scripts, inputs) that are used in the unit tests of the
  library and the Python bindings respectively.  The files in `test/`
  can be split in two groups:

  - The tests of HepMC3 on itself with its inputs, e.g.
    ```
    ./test/testIO4.cc
    ./test/inputIO4.root
    ```
  - Tests in both directories which serve as examples for usage of HepMC3.


- The `cmake/Modules/` directory contains files needed for the CMake
  configuration. The subdirectory `cmake/Templates/` contains templates
  needed for generation of the library CMake configuration files.


- The subdirectory `examples/` contains several examples of using the
  HepMC3 library in standalone applications.

  Each example is located in its own directory and can be built using
  the `CMakeLists.txt` file in the same directory.

  - The `examples/ConvertExample/` subdirectory contains source code of
    utility that converts different types of event records into each other:

    - The files
      ```
      ./examples/ConvertExample/src/WriterDOT.cc
      ./examples/ConvertExample/include/WriterDOT.h
      ```
      are the source files for an event format that can be visualized with
      graphviz.

    - The files
      ```
      ./examples/ConvertExample/src/WriterHEPEVTZEUS.cc
      ./examples/ConvertExample/include/WriterHEPEVTZEUS.h
      ```
      contain an implementation of output format that can be used in the
      ZEUS experiment.

    - The files
      ```
      ./examples/ConvertExample/src/WriterRootTreeOPAL.cc
      ./examples/ConvertExample/include/WriterRootTreeOPAL.h
      ```
      contain an implementation of output format that can be used together
      with data from the OPAL experiment.

    - The files
      ```
      ./examples/ConvertExample/src/AnalysisExample.cc
      ./examples/ConvertExample/include/AnalysisExample.h
      ```
      illustrate an implementation of simple physics analysis using the
      HepMC3 library.

    - The files
      ```
      ./examples/ConvertExample/include/ReaderuprootTree.h
      ./examples/ConvertExample/src/ReaderuprootTree.cc
      ```
      implement an a reader for ROOT files based on
      [uproot](https://pypi.org/project/uproot/).

    - The `./examples/RootIOExample/` subdirectory contains source code of
      an utility that illustrates manipulations with LHEF event record.

    - The `./examples/RootIOExample/` subdirectory contains source code of
      an utility that reads HepMC3 events in ROOT format.

    - The `./examples/RootIOExample2/` subdirectory contains source code of
      an utility that reads HepMC3 events in ROOT TTree format.

    - The `./examples/RootIOExample2/` subdirectory contains source code
      of an utility that reads HepMC3 events and saves them using a
      custom ROOT-based class.

    - The `./examples/RootIOExample3/` subdirectory contains source code
      of an utility that reads HepMC3 events and saves them nto ROOT
      TTree. This is a simplified version of ConvertExample.

    - The `./examples/BasicExamples/` subdirectory contains source code
      of basic examples of HepMC3 usage, e.g. building of event from
      scratch, reading and writing files, usage of Fortran, etc.

    - The `./examples/Pythia6Example/` subdirectory contains source code
      of an utility that generates HepMC events with the Pythia6 Monte
      Carlo event generator.

    - The `./examples/Pythia8Example/` subdirectory contains source code
      of an utility that generates HepMC events with the Pythia8 Monte
      Carlo event generator.

    - The `./examples/LHEFExample/` subdirectory contains source code of
      an utility that illustrates manipulations with LHEF event record.

    - The `./examples/ViewerExample/` subdirectory contains source code
      of ROOT based GUI program that allows to visualize the HepMC3
      events.

    - The `./examples/SearchExample/` subdirectory contains source code
      example that deals with search of relations between particles in
      the event.

- The `doc/` directory contains files used for generation of library
  source code documentation with the Doxygen system. The
  `doc/CMakeLists.txt` file is used by CMake to build the
  documentation.
