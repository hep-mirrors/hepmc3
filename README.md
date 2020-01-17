# Introduction
HepMC3 is a new rewrite of HepMC event record. It uses shared pointers for in-memory navigation and the POD concept for the persistency. 

# A Quick Start:

1. a)Checkout the HepMC from GIT repository:
 
  ```
  git clone https://gitlab.cern.ch/hepmc/HepMC3.git
  ```
  or b) download a tarball 
  ```
  wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.1.0.tar.gz
  tar -xzf HepMC3-3.1.0.tar.gz  
  ```
2. Create a workspace area on which to perform the builds 
  ```
  mkdir hepmc3-build
  cd hepmc3-build
  ```
  
3. Configure the build of the HepMC3
  a)
  ```
  cmake -DHEPMC3_ENABLE_ROOTIO=OFF -DCMAKE_INSTALL_PREFIX=../hepmc3-install ../HepMC3 
  ```
  or b)
  ```
  cmake -DHEPMC3_ENABLE_ROOTIO=OFF -DCMAKE_INSTALL_PREFIX=../hepmc3-install ../HepMC3-3.1.0 
  ```
  to disable compilation of search engine (enabled by default) add
  
  ```
  -DHEPMC3_ENABLE_SEARCH=OFF
  ```
  Please note, in case of using the search engine, the easiest way to link against 
  HepMC3search could be to use static version of the library.
  
4. In order to build with ROOTIO put the following flags and define LD_LIBRARY_PATH: 
  ```
  -DHEPMC3_ENABLE_ROOTIO=ON -DROOT_DIR=path_to_ROOT6_installation
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:path_to_ROOT6_libraries
  ```
  
5. In order to build with HepMC3 example put
  ```
  -DHEPMC3_BUILD_EXAMPLES=ON 
  ``` 
  
  Additional flags control the location installation of Photos, Tauola MC-TESTER, HepMC2 
  and Pythia8  that are used for tests. Please note that some examples are disabled  on Windows.

  For tests and for development purposes  HepMC3 uses extra packages, e.g. ZLIB, GrpahViz, binder.
  On Windows, most extra packages (e.g. ZLIB) have no default location, therefore
  one should specify their location manualy, e.g. 
```
cmake CMakeLists.txt -DZLIB_ROOT=C:\lib -DZLIB_INCLUDE_DIR=C:\include 
```

  
6. In order to build and run tests put
  ```
  -DHEPMC3_ENABLE_TEST=ON
  ```
  and run 
  ```
  make test
  ```
  after compilation is completed

  
7. Build and install HepMC3
  ```
  make -jN install

  ```
where N denotes a number of desired parallel jobs.
On Windows (in Unix-compatible shell) one can use 
```
cmake --build ./
```
instead

8. To run the tests do
```
ctest . 
```
On Windows/MSVC the build type should be specified
```
ctest . -C Debug
```
9. HepMC3 packages are available from standard repositories of Debian, 
Redhat/Fedora, Arch, hep-homebrew and others. 
To build Windows installer use NSIS and run
```
cpack.exe -G NSIS -C Debug
```
The primary supported development chains are 
gcc@Linux, clang@Linux, gcc@OSX, clang@OSX and  MSVC@Windows10, however
the code has been tested on other compilers and systems, e.g. clang@FreeBSD,
Sun@Solaris11, gcc@Solaris11, Sun@Linux, PGC@Linux etc.
Only a limiter support can be offered for these systems.



10. HepMC3 is supplied with Python bindings codes that can be build on multiple systems.
The number of potential combinations of compiler suites, Python versions and operationg systems is quite large,
therefore it is recommended to install the HepMC3 form the default repositories of the used operating system.
If not possible, the following options are available 
- compile the python package from sources
- use the precompiled binaries from pypi: https://pypi.org/project/HepMC3/
- use the https://anaconda.org/conda-forge/hepmc3
In addition to that part of the bindings are implemented in the pyhepmc project 

https://github.com/HDembinski/pyhepmc.


#  Examples
HepMC3 is shipped with multiple examples. These can be compiled during installation as descibed above  
or after the installation (for HepMC3>3.1.0). 
To compile the examples after the installation  copy the installed derectory with examples to desired directory 
and run cmake, e.g. 

```
mkdir -p myexamples
cd myexamples
cp -r /usr/share/doc/HepMC3-3.1.1/examples ./
cd examples
cmake -DUSE_INSTALLED_HEPMC3=ON CMakeLists.txt
make
```


# Source package stucture

The package sources are organized as follows:

0. The topmost directory contains the
README and AUTHORS files, short design description, the main cmake file CMakeLists.txt, the package license, 
change log and a template for configuration file.
```
./DESIGN
./README-for-developers
./COPYING
./AUTHORS
./LICENCE
./HepMC3-config.in
./README.md
./ChangeLog
```

1. The subrirectory src/ contains the core set of library sources while 
the corresponding headers are located in include/HepMC3 directory.
The subdirectory include/HepMC3/Data includes headers with definitions of 
POD structures used in the library. 

2. The subdirectory search/src contains a set of source files of the search 
sublibrary while the corresponding headers are located in 
the search/include/HepMC3 directory. The search/CMakeLists.txt file is used by cmake
to build  the search sublibrary.

3. The subdirectory rootIO/src contains a set of source files of the rootIO 
sublibrary while the corresponding sheaders are located in 
the search/include/HepMC3 and search/include/ directories. The 
rootIO/CMakeLists.txt file is used by cmake to build  the search sublibrary. 

4. The subdirectory python contains a set of source files required for compilation
of the python bindings to the library. These include the python/include/pybind11 
directory with the pybind11 headers.  The python/src/, 
 directory contains the  genuine 
 
```
python/src/root_binders.hpp
python/src/binders.hpp
python/src/stl_binders.hpp
```

and automatically generated  binding sources

```
python/src/pyHepMC3.cpp
python/src/pyHepMC3rootIO.cpp
python/src/pyHepMC3search.cpp
```

The automatically sources were generated using the binder configurations

```
python/src/pyHepMC3.binder
python/src/pyHepMC3rootIO.binder
python/src/pyHepMC3search.binder
```

and the header files generated from the templates

```
python/all_includes.hpp.in
python/root_includes.hpp.in
python/search_includes.hpp.in
```
The direcrory contain the files that are used to build a python package and installation:
```
python/src/__init__.py
python/src/search/__init__.py
python/src/rootIO/__init__.py
pyHepMC3.egg-info.in
pyHepMC3-rootIO.egg-info.in
pyHepMC3-search.egg-info.in
```
The python/CMakeLists.txt file is used by cmake
to build  the binding.

5. The subdirectory interfaces/ contains subdirectories with interfaces 
(C++/Fortran source files, C++ headrs) to 
Monte Carlo event generators and event processing tools
mc-tester,  Photospp,  pythia6,  pythia8 and  Tauolapp.

6. The subdirectories test/ and python/test contain sets of files (source files, scripts, inputs)
that are used in the unit tests of the library and the python bindings respectively.

7. The subdirectory cmake/Modules contains files needed for the cmake configuration
The subdirectory cmake/Templates contains templates needed for generation 
of the library cmake configuration files.  

8. The subdirectory examples contains several examples of usage of HepMC3 library 
in standalone applications.

9. The doc/ directory contains files used for generation of library source code documentation with
the Doxygen system. The dco/CMakeLists.txt file is used by cmake
to build  the documantion.


```
./examples
./examples/ConvertExample
./examples/ConvertExample/cmdline.h
./examples/ConvertExample/src
./examples/ConvertExample/src/WriterDOT.cc
./examples/ConvertExample/src/AnalysisExample.cc
./examples/ConvertExample/src/WriterHEPEVTZEUS.cc
./examples/ConvertExample/src/gzstream.C
./examples/ConvertExample/src/WriterRootTreeOPAL.cc
./examples/ConvertExample/convert_example_env.sh.in
./examples/ConvertExample/CMakeLists.txt
./examples/ConvertExample/include
./examples/ConvertExample/include/AnalysisExample.h
./examples/ConvertExample/include/WriterDOT.h
./examples/ConvertExample/include/gzstream.h
./examples/ConvertExample/include/WriterRootTreeOPAL.h
./examples/ConvertExample/include/ReaderGZ.h
./examples/ConvertExample/include/WriterHEPEVTZEUS.h
./examples/ConvertExample/cmdline.c
./examples/ConvertExample/convert_example.cc
./examples/CMakeLists.txt
./examples/RootIOExample
./examples/RootIOExample/rootIO_example_write.cc
./examples/RootIOExample/rootIO_example_env.sh.in
./examples/RootIOExample/CMakeLists.txt
./examples/RootIOExample/rootIO_example_read.cc
./examples/RootIOExample/example.hepmc3
./examples/RootIOExample3
./examples/RootIOExample3/rootIOTree_example_write.cc
./examples/RootIOExample3/CMakeLists.txt
./examples/RootIOExample3/rootIOTree_example_read.cc
./examples/RootIOExample3/rootIOTree_example_env.sh.in
./examples/BasicExamples
./examples/BasicExamples/basic_tree.cc
./examples/BasicExamples/CMakeLists.txt
./examples/BasicExamples/HepMC3_fileIO_example.cc
./examples/BasicExamples/hepevt_wrapper_example_fortran.f
./examples/BasicExamples/HepMC2_reader_example.cc
./examples/BasicExamples/hepevt_wrapper_example_main.cc
./examples/Pythia6Example
./examples/Pythia6Example/main63.f
./examples/Pythia6Example/CMakeLists.txt
./examples/Pythia6Example/pythia-6.4.28.f
./examples/Pythia8Example
./examples/Pythia8Example/CMakeLists.txt
./examples/Pythia8Example/cmake
./examples/Pythia8Example/cmake/Modules
./examples/Pythia8Example/cmake/Modules/FindPythia8.cmake
./examples/Pythia8Example/pythia8_example.cc
./examples/Pythia8Example/pythia8_example_env.sh.in
./examples/Pythia8Example/pythia8_pp_minbias.conf
./examples/Pythia8Example/pythia8_ee_to_Z_to_tautau.conf
./examples/LHEFExample
./examples/LHEFExample/LHEF_example_1.plhe
./examples/LHEFExample/LHEF_example_2_cmp.plhe
./examples/LHEFExample/CMakeLists.txt
./examples/LHEFExample/LHEF_example_cat.cc
./examples/LHEFExample/LHEF_example_cmp.lhe
./examples/LHEFExample/LHEF_example_2_out.plhe
./examples/LHEFExample/LHEF_example_1_cmp.plhe
./examples/LHEFExample/LHEF_example.lhe
./examples/LHEFExample/LHEF_example_1_out.plhe
./examples/LHEFExample/LHEF_example_env.sh.in
./examples/LHEFExample/LHEF_example_2.plhe
./examples/RootIOExample2
./examples/RootIOExample2/class_example_read.cc
./examples/RootIOExample2/src
./examples/RootIOExample2/src/MyClass.cc
./examples/RootIOExample2/src/MyRunClass.cc
./examples/RootIOExample2/CMakeLists.txt
./examples/RootIOExample2/class_example_env.sh.in
./examples/RootIOExample2/include
./examples/RootIOExample2/include/MyClass.h
./examples/RootIOExample2/include/LinkDef.h
./examples/RootIOExample2/include/myclass_Classes.h
./examples/RootIOExample2/include/MyRunClass.h
./examples/RootIOExample2/example.hepmc3
./examples/RootIOExample2/class_example_write.cc
./examples/ViewerExample
./examples/ViewerExample/src
./examples/ViewerExample/src/HepMC3ViewerFrame.cc
./examples/ViewerExample/src/main.cc
./examples/ViewerExample/CMakeLists.txt
./examples/ViewerExample/include
./examples/ViewerExample/include/LinkDef.h
./examples/ViewerExample/include/HepMC3ViewerFrame.h
./examples/ViewerExample/cmake
./examples/ViewerExample/cmake/Modules
./examples/ViewerExample/cmake/Modules/FindGraphviz.cmake
```

```
./test
./test/McTesterValidationTool.cc
./test/testBoost.cc
./test/inputIO5.hepmc
./test/PhotosValidationTool.h
./test/testMCTester1.cc
./test/inputReaderFactory1.hepmc
./test/inputIO1.hepmc
./test/McTesterValidationTool.h
./test/testWeights.cc
./test/inputThreads1.hepmc
./test/inputDelete.hepmc
./test/HepMC3TestUtils.h
./test/inputMultipleCopies1.hepmc
./test/testDelete2.cc
./test/CMakeLists.txt
./test/testIO4.cc
./test/inputIO4.root
./test/inputReaderFactory2.hepmc
./test/testThreads1.cc
./test/ValidationControl.cc
./test/OutputValidationTool.cc
./test/ValidationTool.h
./test/PythiaValidationTool.h
./test/TauolaValidationTool.cc
./test/testSkip1.cc
./test/testIO3.cc
./test/inputMass.hepmc
./test/SimpleEventTool.h
./test/PythiaValidationTool.cc
./test/inputIO2.hepmc
./test/OutputValidationTool.h
./test/inputSkip1.hepmc
./test/inputIO3.hepevt
./test/testUnits.cc
./test/testIO6.cc
./test/testIO5.cc
./test/testLoops.cc
./test/IsGoodEvent.h
./test/testPolarization.cc
./test/ValidationControl.h
./test/testIO2.cc
./test/Timer.h
./test/testReaderFactory1.cc
./test/testMass.cc
./test/inputMultipleCopies2.hepmc
./test/inputDelete2.hepmc
./test/testDelete.cc
./test/testReaderFactory2.cc
./test/testIO1.cc
./test/TauolaValidationTool.h
./test/testPythia2.cc
./test/PhotosValidationTool.cc
./test/inputIO6.hepmc
./test/testPrintBug.cc
./test/testPhotos1.cc
./test/testPythia3.cc
./test/testPythia1.cc
./test/SimpleEventTool.cc
./test/testMultipleCopies.cc
./test/testTauola1.cc
```
