#  HepMC3 usage examples

These programs can be compiled during installation as described above
or after the installation (for HepMC3 > 3.1.0). To compile the
examples after the installation copy the installed directory with
examples to desired directory and run CMake, e.g.

```sh
mkdir -p myexamples
cd myexamples
cp -r /usr/share/doc/HepMC3-3.3.2/examples ./
cd examples
cmake -D USE_INSTALLED_HEPMC3=ON CMakeLists.txt
make
```

If CMake cannot locate your HepMC3 installation automatically, add `-D
HepMC3_DIR=/path/to/hepmc3/cmake` to your CMake configuration
command. Replace the placeholder with the path to the directory
containing the installed `HepMC3Config.cmake` file.

The examples use different HepMC3 components and external
dependencies. To build and run all examples, install the required
optional HepMC3 components, including ROOT I/O and the search library,
together with the external dependencies required by individual
examples, such as ROOT and the relevant Monte Carlo event generators.

### Building and Running the `ConvertExample` with the output EDM4HEP

Set up your favorite Key4hep nightly:

```sh
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```

or release:

```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
```

Then build with Key4hep by adding the following flag to the `cmake`
command:

```
-D HEPMC3_ENABLE_EDM4HEP=ON -D HEPMC3_BUILD_EXAMPLES=ON
```

This flag will initiate some sanity checks. The converter resides in
the example programs, therefore these also have to be turned on during
the build.
