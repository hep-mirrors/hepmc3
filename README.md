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



