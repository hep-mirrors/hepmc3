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
  and Pythia8  that are used for tests.
  
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
