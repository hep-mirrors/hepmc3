# HepMC3 change log

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [3.3.2] - Unreleased

### Added
- Add `ReaderAscii` tests for event positions, position reset, invalid headers, and read/write round trips. (Dmitri Konstantinov)
- Add Python compressed I/O helpers `ReaderGZ` and `WriterGZ` for binding compressed input/output streams through Python compression modules. (Andrii Verbytskyi)
- Add comments/documentation to C++ tests. (Andrii Verbytskyi)
- Add rudimentary testing that the HepMC3-config script does not crash. (Andy Buckley)
- Added Fedora45 jobs to CI. (Andrii Verbytskyi)
- Add `python/test/test_IO8.py` and `python/test/inputIO8.hepmc` to validate gzip round-trip support and install the new test. (Andrii Verbytskyi)

### Fixed
- Make the translation of LHE weight structure to HepMC weight names more compliant with the arXiv:2203.08230 standard and downstream tools. (Andy Buckley)
- Fix reading event positions in ReaderAscii by skipping the `@` marker. (Dmitri Konstantinov)
- Reset the event position before each read to avoid keeping the previous event's position. (Dmitri Konstantinov)
- Fix LHEF header init weight confusion. (Leif Lonnblad)
- Avoid potentially empty tag in `Reader` in `LHEF.h`. (Andrii Verbytskyi)
- Fixes to Doxygen setup. (Andrii Verbytskyi)
- Fix the way `ENVIRONMENT` is set in tests, use canonical name for components in CMake config, checks for C++29. (Andrii Verbytskyi)
- Fixes for `ReaderLHEF` to properly report beams from LHE init block instead of first particles in event record.  Now raise an error if there are fewer particles than beams in the event.

### Changed
- Update the logic of defining the C++ standard for each component. (Andrii Verbytskyi)
- Report event-header parsing errors instead of returning an empty event as a success. (Dmitri Konstantinov)
- Add CI for Gentoo linux which tests the CMake configuration randomly. (Alexander Puck Neuwirth)
- Update installation of Python metadata. (Andrii Verbytskyi)
- Add pybind11 v3.1.0 headers as a separate directory to support the newest Python. Move the old headers of v2.12.0 into a separate directory. (Andrii Verbytskyi)
- Increase default `pystreambuf` buffer size to improve performance for Python-based compressed stream I/O. (Andrii Verbytskyi)
- Implement suggestions from `clang-tidy` for tests, examples, and the core library. (Andrii Verbytskyi)
- Add Python bindings for `ReaderMT` and its multithreaded event-reading support. (Andrii Verbytskyi)
- Add a GitLab CI rule to require a `ChangeLog` entry before a merge request is considered ready. (Andrii Verbytskyi)
- Allow for CMake policies to be in the range `3.10.0...4.0.3`. Add CMake version testing in CI. Thanks to Matthew Feickert and Henry Schreiner.
- Tweak `ReaderAscii` logic to be more flexible about the number of space characters (Luke Pickering)


## [3.3.1] - 2025-03-24

### Added
- Add uproot5 to the tests. (Andrii Verbytskyi)

### Fixed
- Fix weights usage in the `AnalysisExample.cc` file. (Andrii Verbytskyi)
- Fix for a `Conditional jump or move depends on uninitialised value(s)` problem detected by valgrind. (Andrii Verbytskyi)
- Multiple bugfixes and improvements. Thanks to Mattias Ellert, Matthew Feickert.
- Add static libraries and other fixes to `HepMC3-config` output. (Andrii Verbytskyi)

### Changed
- Require CMake > 3.10 for compatibility with CMake4. Thanks to Mattias Ellert.
- Update `convert_example.cc` to allow for one argument for the "dump" format. (Andrii Verbytskyi)


## [3.3.0] - 2024-06-17

### Added
- Add support for reading with uproot. (Andrii Verbytskyi)

### Removed
- Removed functions that operate on raw pointers. (Andrii Verbytskyi)
- Drop Python2 support. (Andrii Verbytskyi)

### Fixed
- Fix compilation with Python 3.13.0a1. See: https://github.com/pybind/pybind11/pull/4902 . (Mattias Ellert)

### Changed
- Update Python bindings with binder 1.4.2. (Andrii Verbytskyi)
- Update bxzstr to version 1.2.2 + patches. (Andrii Verbytskyi)
- Major update of the build system, removed most of the custom modules. (Andrii Verbytskyi)
- Explicit return for `inf`-generating eta / massless rapidity along $p_z$, to avoid FPE triggering. (Andy Buckley)
- Suppress cross-section/weight count-mismatch warning if only one cross-section. (Andy Buckley)


## [3.2.7] - 2023-03-10

### Fixed
* Patch release to fix a problem with vertex-attributes removal. (Andrii Verbytskyi)


## [3.2.6] - 2023-04-12

### Added
- Add a protobuf-based `HepMC3::Reader/Writer`. (Luke Pickering)
### Removed
- Deprecated the `set_cross_section` function with `double` in favour of those with `vector<double>`. (Andrii Verbytskyi)
### Fixed
- Fix output value of `ReaderLHEF::read_event`. (Andrii Verbytskyi)
### Changed
- Better Doxygen documentation. (Andrii Verbytskyi)
- Improvements in `ReaderFactory` that should help detect the file type more successfully. (Andrii Verbytskyi)
- Multiple optimizations inspired by `clang-tidy`. (Andrii Verbytskyi)
- The attributes are now parsed after calls to `from_string` or `set_value`. (Andrii Verbytskyi)
- API-breaking changes in the search module: pass by const reference. (Andrii Verbytskyi)
- Update Python bindings. (Andrii Verbytskyi)
- Modernisation of CMake. (Luke Pickering)
- Better documentation for standalone examples. (Andrii Verbytskyi)
- Update pybind11 headers to fix Python 3.11. Reported by Mattias Ellert.
- License update. (Andrii Verbytskyi)
- Make the R/W plugins transparent - the set/get functions call the handled object. (Andrii Verbytskyi)


## [3.2.5] - 2022-02-21

### Added
- Added implementation for Relatives class that works in Windows. (Andrii Verbytskyi)
- New interfaces for HEPEVT and update of Python interface. (Andrii Verbytskyi)
- More functions in GenEvent to deal with attributes. (Andrii Verbytskyi)
- Implemented compressed I/O with zip, lzma and bz2 via bxzst library. (Andrii Verbytskyi)
- Added `Reader`/`Writer` interfaces with smart pointers. (Andrii Verbytskyi)
- Added multithreaded reader. (Andrii Verbytskyi)

### Removed
- Removal of unused codes and comments. (Andrii Verbytskyi)
- Remove outdated interfaces to MCEGs. (Andrii Verbytskyi)

### Fixed
- Fixes to documentation of some examples; thanks to Rakesh Naik for report. (Andrii Verbytskyi)
- Fixes to Pythia8ToHepMC3.py. (Ahmed Hussein)
- Fixed `#ifdefs` for non-Linux system to allow the usage of `Reader`/`Writer` plugins. (Andrii Verbytskyi)

### Changed
- Improved MSYS2 support. (Andrii Verbytskyi)


## [3.2.4] - 2021-07-07

### Added
- Enable reading of multiple run-info objects from ASCII files. (Andrii Verbytskyi)
- Added uproot-based reader to examples. (Andrii Verbytskyi)

### Removed
- Deprecate Pythia8, MC-TESTER, `Taoulapp` and `Photospp` interfaces, as these were picked-up by the upstream. (Andrii Verbytskyi)
- Drop CMake2 support. (Andrii Verbytskyi)

### Fixed
- Minor fixes to the CMake configuration of examples. (Andrii Verbytskyi)
- Minor fixes in Pythia6 interface. (Andrii Verbytskyi)
- Bugfix for HI output in `WriterAsciiHepMC2`. (Andrii Verbytskyi)
- Multiple fixes to style and `printf` by Mattias Ellert. (Andrii Verbytskyi)
- Bugfix: avoid creation of empty attributes in `ReaderAsciiHepMC2`. Thanks to Pavel Demin for a hint. (Andrii Verbytskyi)
- Fix `WriterAscii*` to avoid problems with memory tests on `aarch64`. Thanks to Mattias Ellert for reporting. (Andrii Verbytskyi)
- Fix buffer overflow in `WriterAscii`/`WriterAsciiHepMC2`. Thanks for Fabian Klimpel for help with debug. (Andrii Verbytskyi)
- Fixes to the tests. (Andrii Verbytskyi)
- Fix a bug in `GenCrossSection`: access to non-existing vector element in `to_string()` function for incomplete objects. (Andrii Verbytskyi)
- Fixed a bug in Tauola test, which was not reading proper config. (Andrii Verbytskyi)
- Fixed some tests to make them return non-`0` in case of problems. (Andrii Verbytskyi)
- Fixed CMake configuration to allow multiple Python versions. (Andrii Verbytskyi)
- Fixes of compilation warnings by Mattias Ellert. (Andrii Verbytskyi)

### Changed
- Speed optimizations for `ReaderAsciiHepMC2`. (Andrii Verbytskyi)
- Update cmake so the C++ standard is deduced from the ROOT configuration if ROOT is used. Otherwise C++11 is used. (Andrii Verbytskyi)
- Added compiler flags to reduce the amount of warnings. (Andrii Verbytskyi)
- Streamlined cmake for PGI. (Andrii Verbytskyi)
- Reduce debug-printout verbosity in `ReaderFactory` format autodetection. (Andy Buckley)


## [3.2.3] - 2020-12-14

### Added
- Add Python test with attributes. (Andrii Verbytskyi)

### Fixed
- ReaderLHEF was fixed for to treat correctly more complicated input. Inspired by discussion with Bryan Foo and Christopher Palmer.	(Andrii Verbytskyi)

### Changed
- Python bindings regenerated with binder 1.1.0 and the pybind11 copy updated to 2.6.0. (Andrii Verbytskyi)
- Removed `#ifdef`s around the functions that had to be excluded from bindings, but binder 1.0.0 was not doing. (Andrii Verbytskyi)
- Updates to many CMake and Python scripts. (Marian Heil)


## [3.2.2] - 2020-06-10

### Added
- Added an example for search module. (Andrii Verbytskyi)
- Added an interface for HepMC2 (in-memory-conversion). (Andrii Verbytskyi)
- Added functions to search module: search for relatives (particle or vertex)->(particle or vertex) in different combinations. (Andrii Verbytskyi)

### Fixed
- Fixed treatment of the weights in case of different number of weights and weight names. Reported by Frank Siegert. (Andrii Verbytskyi)
- Fixed `WriterAsciiHepMC2` option for separate flows. Reported by Julian Lukwata. (Andrii Verbytskyi)
- Fixed compilation of the search module with Clang. (Andrii Verbytskyi)
- Fixed the HepMC2 output for events without weights. (Andrii Verbytskyi)
- Fixed the signal vertex ID for HepMC2 output. (Andrii Verbytskyi)
- Fixed compatibility of ROOT trees written with HepMC 3.0 and removed the typedefs in the dictionaries. (Andrii Verbytskyi)

### Changed
- Improved treatment of Python installation, Thanks to Mattias Ellert. (Andrii Verbytskyi)
- Split the sources of Python bindings into smaller files to optimize compilation. (Andrii Verbytskyi)
- Improved CMake configuration for special cases, e.g. old CMake, no ROOT, some compilers, etc. Added more error handling. (Andrii Verbytskyi)
- Bump `.so` version of the `libHepMC3search` to 4. (Andrii Verbytskyi)
- Improved the configuration of Pythia8 for tests (minimal version requirement). (Andrii Verbytskyi)


## [3.2.1] - 2020-03-17

### Added
- Added an option for explicit selection of packages for testing. (Andrii Verbytskyi)
- Experimental support for Python-module compilation with pypy. (Andrii Verbytskyi)
- Added an option to handle Python 3.9. Thanks to Mattias Ellert.
- Implemented special treatment of some broken HepMC2 events. Thanks to Christian Holm Christensen.

### Fixed
- Bugfix in `VectorString` attribute. (Luke Pickering)
- Improvement in the attribute parsing. Thanks to Peter Onyisi.

### Changed
- Explicit usage of namespaces. (Andrii Verbytskyi)
- Disallow empty lines for the attribute names. (Andrii Verbytskyi)
- Adjustments in the interfaces of Tauola/Photos/MC-TESTER as part of preparation to the new releases of these libraries. (Andrii Verbytskyi)


## [3.2.0] - 2019-11-27

### Added
- First release with Python bindings. (Andrii Verbytskyi)
- Added `ReaderPlugin` and `WriterPlugin` and macros to declare plugins. (Andrii Verbytskyi)
- Added `set_options`, `get_options` to I/O classes for fine control of I/O. (Andrii Verbytskyi)
- Added `HEPMC3_` prefix to `DEBUG`, `ERROR`, `WARNING` macros. (Andrii Verbytskyi)
- Added `Reader::skip(int &)` for fast-forwarding of input. (Andrii Verbytskyi)
- Added to wrapper functions `LHEF::Writer` that can be used in Python. (Andrii Verbytskyi)

### Removed
- Removed `Error` class. (Andrii Verbytskyi)
- Removed `ReaderHEPEVT::read_hepevt_particle(int i, bool iflong=true)`, `ReaderHEPEVT::read_event(GenEvent &evt, bool iflong)`, `ReaderHEPEVT::get_vertices_positions_present` and `ReaderHEPEVT::set_vertices_positions_present`. This functionality is now implemented as `Reader::set_options`, `Reader::get_options`. (Andrii Verbytskyi)

### Fixed
- Fixed `GenCrossSection` for the case of multiple cross-sections. (Andrii Verbytskyi)
- Fixed `GenEvent::weight_names()`. Now it should be called without arguments. (Andrii Verbytskyi)

### Changed
- The standard `Selector` members from `Selector` (e.g. `Selector::MASS`) were moved into a new class `StandardSelector`. (Andrii Verbytskyi)
- Use `delete`/override for class members. (Andrii Verbytskyi)


## [3.1.2] - 2019-08-29

### Added
- Add `GenParticle::abs_pid()` to match `FourVector::abs_eta()`, `abs_rap()`. Experience from Rivet and ATLAS is that these convenience wrappers are  welcomed by users. (Andy Buckley)
- Add `FourVector` component-setting functions in HepMC method naming convention; deprecate old `camelCase` ones. (Andy Buckley)
- Added more standard attributes. (Andrii Verbytskyi)
- Added streamer output to Reader/Writer classes. (Andrii Verbytskyi)
- Added procedures for boost, rotation and reflection to GenEvent. (Andrii Verbytskyi)
- Added operators for printing events, particles, etc. (Andrii Verbytskyi)
- Added example with reading of compressed file and more tests.  (Andrii Verbytskyi)

### Fixed
- Fixed name of ROOT dictionary. (Andrii Verbytskyi)

### Changed
- Extended documentation for doxygen. (Andrii Verbytskyi)
- Thanks to Hans Dembinski, John Chapman, Mattias Ellert, Marian Heil, Attila Krasznahorkay, Dominik Muller, Juergen Reuter


## [3.1.1] - 2019-04-01

### Fixed
- Improved treatment of events with cycles. (Andrii Verbytskyi)

### Changed
- Improved documentation and examples. (Andrii Verbytskyi)


## [3.1.0] - 2019-02-08

### Added
- New search-engine with highly abstract search capabilities. (Andrii Verbytskyi)
- New readers and writers: `WriterAsciiHepMC2`, `ReaderLHEF`. (Andrii Verbytskyi)
- Deduction of input file format in `ReaderFactory`. (Andrii Verbytskyi)
- Implemented test suite. (Andrii Verbytskyi)

### Removed
- Removal of most deprecated functions and multiple bugfixes. (Andrii Verbytskyi)

### Changed
- Renaming the namespace, header directory and library name to `HepMC3`. (Andrii Verbytskyi)
- SmartPointer is replaced with `shared_ptr` from C++11, so C++11 is always required now. (Andrii Verbytskyi)
- Split library into `HepMC3` (core) and `HepMC3search` (search engine). (Andrii Verbytskyi)
- Ascii files now have own string in header: `Asciiv3`. (Andrii Verbytskyi)
- Improved consistency of interface. (Andrii Verbytskyi)
- Improved examples and documentation. (Andrii Verbytskyi)


## [3.0.0] - 2017-03-20

### Added
- Add a `GenVertex::particles(range)` method (as in HepMC 2.07). (Andy Buckley)
- Add `ancestors()` and `descendants()` methods to `GenParticle`, and make `parents()` and `children()` accessors const. (Andy Buckley)
- Add stream constructors to `IO_GenEvent` wrapper, and an `ostream` implementation to `WriterAscii`. (Andy Buckley)
- Add unbound accessor functions in `FindParticles`. (Andy Buckley)
- Add unisex `children` and `parents` accessors in `FindParticles`. (Andy Buckley)
- Builds of Debian packages. (Andrii Verbytskyi)
- Add section to build `rpm`s and `deb`s with cpack. (Andrii Verbytskyi)
- Missing cross-section parsing added to HepMC2 reader. (Andrii Verbytskyi)
- Adding tree-based version of ROOT reader/writer. (Witek Pokorski)
- Few missing I/O features added. (Tomasz Przedzinski)
- Adding backward compatibility typedefs to `CrossSection`, `HeavyIon`, `PdfInfo` (i.e. without the `Gen` prefixes). (Andy Buckley)
- Implemented ROOT I/O custom streamer for `GenEvent`. (Witek Pokorski)
- Implemented the LHEF attributes classes. (Leif Lonnblad)
- Adding new `Reader` and `Writer` I/O base classes and first-draft ASCII I/O interfaces. (Andy Buckley)
- Adding new `GenWeights` container (from HepMC2 update, to be cleaned). (Andy Buckley)
- Added beam particles. (Tomasz Przedzinski)
- Added backward-compatibility iterators. (Tomasz Przedzinski)
- Added `Attributes` mechanism. Currently used only for event and keys (strings) are stored in `GenEvent` (which will change). (Tomasz Przedzinski)
- Add `HEPMC_DEPRECATED` macro in `Setup.h`. (Andy Buckley)
- Adding first version of CMake for validation. (Witek Pokorski)
- Added ROOT I/O and examples. (Tomasz Przedzinski & Witold Pokorski)
- Added check for C++11 to CMake. Now `#ifndef BUILD_WITH_11` can be used in code. (Tomasz Przedzinski & Witold Pokorski)
- Added `examples` directory. Currently only with Pythia8 example and with a rushed `Makefile` that should be rewritten. (Tomasz Przedzinski)
- Added `README` with coding standards for HepMC developers. (Tomasz Przedzinski)
- Added `HEPEVT` wrapper and sample test for it. (Tomasz Przedzinski)
- Added cross-section struct. (Tomasz Przedzinski)
- Added prototype for Rivet interface on branch `rivet`. (Tomasz Przedzinski)
- Adding `HepMC.h` and `Version.h` headers. (Andy Buckley)
- Adding `ChangeLog` and `TODO` to track developments. (Andy Buckley)
- Added `PDFinfo` and `HeavyIon` structs. (Tomasz Przedzinski)
- Added `Units` class. (Tomasz Przedzinski)
- Added serialization module for future ROOT interface. (Tomasz Przedzinski)
- Added vertex position. Position accessor checks all vertices down the decay tree for first vertex that has position set. (Tomasz Przedzinski)
- Added first version of search engine. (Tomasz Przedzinski)
- Added validation framework prototype. (Tomasz Przedzinski)
- Added versioning prototype. (Tomasz Przedzinski)
- Added first prototype for HepMC3 in-memory representation. (Tomasz Przedzinski)
- Added first version of HepMC2 plain text input file reader. (Tomasz Przedzinski)
- Added first version of HepMC3 plain text output file writer. (Tomasz Przedzinski)

### Removed
- Other I/O cleanup. `include/HepMC/IO` and `src/IO` removed. `IO_FileBase` and `IO_Base` removed. Examples updated. (Tomasz Przedzinski)

### Fixed
- Fixing Fortran flags. (Andrii Verbytskyi)
- Compilation working with GCC 4.4. (Andrii Verbytskyi)
- Fixes for ROOT6 dictionary. (Witek Pokorski)
- Fix in CMake for `rootIO` library to work with ROOT6. (Witek Pokorski)
- Fixes in CMake to export also `rootIO` library; removed obsolete forcing of `.so` on Mac. (Witek Pokorski)

### Changed
- Make `FindParticles` available from `HepMC.h` convenience header. (Andy Buckley)
- Make installation of generator interfaces optional. (Dmitri Konstantinov)
- Changing `ROOTIO_LIB` `#define` to `HEPMC_ROOTIO_LIB`. (Witek Pokorski)
- Removing warnings, adding `README`, updating documentation. (Witek Pokorski)
- Moving `GenEvent` and `GenRunInfo` ROOT streamers to a separate file. (Witek Pokorski)
- Update of CMake files to allow proper RedHat `x86_64` directory structure. (Andrii Verbytskyi)
- ROOT IO is enabled if `ROOTConfig.cmake` or `FindROOT.cmake` set `ROOT_FOUND`. (Andrii Verbytskyi)
- Documentation updated. Added previously missing 'examples' section. (Tomasz Przedzinski)
- Make sure that all macros in `Config.h` have a numerical value (thanks to Marek Schoenherr for the heads-up). (Andy Buckley)
- ROOT reader and writer inherit now from `Reader` and `Writer` base classes. (Witek Pokorski)
- Optimisations in `read_data` method. (Witek Pokorski)
- Reduce/add `HepMCDefs.h` and `SimpleVector.h` to stubs for backward compatibility, which include the modern equivalent headers and produce preprocessor warnings. These should be conditionally installed only when building in compatibility mode. (Andy Buckley)
- Adding, removing, and tweaking feature detection macros. (Andy Buckley)
- Overhaul of `FourVector` class and removal of `FourVector.icc`. (Andy Buckley)
- Differentiate between `SmartPointer` const and non-const dereferencing and arrow operators, to pass on the constness semantics to the contained type. This blocks accidents like calling non-const modifying functions on a `const GenVertexPtr` or `const GenParticlePtr`, which is not protected against by e.g. `shared_ptr<GenParticle>`. Plus adding an operator for bool comparisons to `SmartPointer` and (many) updates through the main object classes to respect the newly invigorated constness rules. (Andy Buckley)
- Changing `GenEvent::event_pos()` to return a `FourVector` ref rather than the root vertex itself, and changing `GenEvent::offset_event(v)` to two explicitly named `shift_event_by(v)` and `shift_event_to(v)` methods. (Andy Buckley)
- `ReaderAscii` and `WriterAscii` finished. `IO_GenEvent` is now only backward-compatibility header file. (Tomasz Przedzinski)
- `IO_Root` divided into `WriterRoot` and `ReaderRoot`. (Tomasz Przedzinski)
- Updating example event file. (Witek Pokorski)
- Updated ASCII file I/O to work with `Attributes`. ROOT I/O will follow. (Tomasz Przedzinski)
- Modified `GenPdfInfo`, `GenHeavyIon` and `GenCrossSection` to work as `Attributes`. (Tomasz Przedzinski)
- Made `-DHEPMC_ENABLE_CPP11` option `ON` by default. (Witek Pokorski)
- Redesign of ROOT I/O, moved to a separate library. (Witek Pokorski)
- Added `.exe` in examples executables names. (Witek Pokorski)
- Improvement in Pythia8 example; possibility of setting number of events and passing conf and output file names as arguments. (Witek Pokorski)
- CMake updated for use with ROOT. (Tomasz Przedzinski & Witold Pokorski)
- `GenPdfInfo`, `GenHeavyIon`, `GenCrossSection` are now stored by `shared_ptr` not raw pointer. (Tomasz Przedzinski & Witold Pokorski)
- `GenPdfInfo`, `GenHeavyIon`, `GenCrossSection` added to ROOT I/O and IO_GenEvent. Now properly written and read from/to text files and ROOT files. (Tomasz Przedzinski & Witold Pokorski)
- Changed naming convention from `HepMC3` to `HepMC` (namespaces, comments, defines, etc.). (Tomasz Przedzinski)
- Changed `PdfInfo` to `GenPdfInfo` and changed names of the fields. (Tomasz Przedzinski)
- Changed `HeavyIon` to `GenHeavyIon`. (Tomasz Przedzinski)
- Use `CXX` and `CXXFLAGS` in place of `CC`, `CFLAGS`. (Andy Buckley)
- Improved and optimized in-memory representation prototype. (Tomasz Przedzinski)
- Tested new in-memory representation prototype based on `smart_ptr`. (Tomasz Przedzinski)
- Changed validation framework. Now it can be configured through config files and can be used with selected with any (or none) of the tools for which interface to HepMC3 is prepared. (Tomasz Przedzinski)
- Tested new in-memory representation prototype based on classes acting like smart pointers. (Tomasz Przedzinski)
- I/O classes updated to read and store vertex positions. (Tomasz Przedzinski)
- Tested new in-memory representation prototype prioritizing serialization. (Tomasz Przedzinski)
- Validation framework expanded. (Tomasz Przedzinski)
- Project started. (Tomasz Przedzinski)
