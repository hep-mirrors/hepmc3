# One page HepMC3 documentation
  
## 1 Build instructions

For the full list of the installation options, including the
description of the flags to build the HepMC3 from the sources see at
[HepMC3 page at CERN GitLab](https://gitlab.cern.ch/hepmc/HepMC3).

A quick minimalist build that requires only C++11 compiler and a
recent version of CMake (3.15+) is described below.  To build HepMC3
using CMake on the supported platforms, a C++11 compiler and a recent
CMake is needed (3.15).  The commands executed in the unpacked source
tarball of HepMC3 are:

```sh
  cmake -DCMAKE_INSTALL_PREFIX=desired_installation_path -DHEPMC3_ENABLE_ROOTIO=OFF   -DHEPMC3_ENABLE_PYTHON=OFF CMakeLists.txt
  cmake --build .
  cmake --install .
```

These will configure the HepMC3 sources, compile them and install the
library into `desired_installation_path`.


## 2 Differences between HepMC2 and HepMC3

The following is a list of main differences that should be taken into
account when transitioning from HepMC2 to HepMC3.

### 2.1 Structure change and header file organisation

The following changes in header files have been applied:

```
  HepMC/HeavyIons.h    ->  HepMC3/GenHeavyIons.h   (now a POD struct)
  HepMC/PdfInfo.h      ->  HepMC3/GenPdfInfo.h     (now a POD struct)
  HepMC/SimpleVector.h ->  HepMC3/FourVector.h     (ThreeVector class removed)
```

The structure of GenCrossSection class has been changed to handle
multiple values of cross-sections. The cross-section values and errors
(uncertainties) can be accessed only by public functions, the
corresponding data members are private. By default, the number of
cross-section values in every event is equal to the number of event
weights.  Accordingly, each cross-section value can be accessed using
the corresponding event-weight name (`std::string`) or event-weight
index (`int`).

The following header files are no longer available:

```
  CompareGenEvent.h
  Flow.h
  GenRanges.h
  HepMCDefs.h
  HerwigWrapper.h
  IteratorRange.h
  Polarization.h
  SearchVector.h
  StreamHelpers.h
  StreamInfo.h
  TempParticleMap.h
  WeightContainer.h
  IO_GenEvent.h

  enable_if.h
  is_arithmetic.h

  IO_AsciiParticles.h
  IO_Exception.h
  IO_HEPEVT.h
  IO_HERWIG.h

  PythiaWrapper.h
  PythiaWrapper6_4.h
  PythiaWrapper6_4_WIN32.h
```


### 2.2 Fortran generators

An example of interface to Pythia6 Fortran blocks is given in the
examples. Note that the provided interface `Pythia6ToHepMC3.cc` and
`Pythia6ToHepMC3.inc` is an interface for HepMC3 from Pythia6 and not
an interface to Pythia6 from HepMC, as it was in the case of the
HepMC2.


### 2.3 Changes to the I/O handling

Multiple file formats are supported. The implementation of reading and
writing is separated in HepMC3. All the reading operations are
performed in "reader" objects inherited from HepMC::Reader and the
writing operations in the "writer" objects inherited from
HepMC::Writer. Therefore it is to use the desired headers explicitly,
as needed.

The `IO_GenEvent.h` header is not available anymore.  to write and/or
read HepMC2 files use the following includes instead:

```
  WriterAsciiHepMC2.h
  ReaderAsciiHepMC2.h
```

Note that the HepMC2 format is outdated and is not able to contain
a lot of information stored into event record by the modern Monte
Carlo event generators.  It is recommended to use the HepMC3 native
event record in plain text or in ROOT TTree format.  The corresponding
readers and writers are defined in

```
  WriterAscii.h
  ReaderAscii.h
```

and

```
  WriterRootTree.h
  ReaderRootTree.h
```

It is also possible to implement custom reader and writer objects
implementing the general HepMC3::Reader and HepMC3::Writer interfaces.

Note the difference in the behaviour of default readers with respect
to HepMC2, when reading files with multiple headers. The ASCII files
with multiple headers (e.g. obtained with `cat 1.hepmc 2.hepmc >
12.hepmc`) will be processed by the readers only until the first
occurrence of `END_EVENT_LISTING`.

In addition to the standard readers, starting from the version 3.2.5
HepMC3 provides as set of templates readers/writers to handle the
zip-,lzma-,bz2-compressed files (`ReaderGZ` and `WriterGZ`) and to
perform multithread reading (`ReaderMT`).


### 2.4 Memory managed by shared pointers

Particles and vertices are managed using shared pointers, and should be
created through `make_shared` rather than a raw call to `new`.

```c++
  GenEvent event;

  // Create particles
  GenParticlePtr p1 = make_shared<GenParticle>();
  GenParticlePtr p2 = make_shared<GenParticle>();
  GenParticlePtr p3 = make_shared<GenParticle>();

  p1->set_pdg_id(23);
  p2->set_pdg_id(15);
  p3->set_pdg_id(-15);

  // Create vertex
  GenVertexPtr v1 = make_shared<GenVertex>();
  v1->add_particle_in(p1);
  v1->add_particle_out(p2);
  v1->add_particle_out(p3);

  event.add_vertex(v1);
```


### 2.5 Iterators

The iterator base-classes and access functions from HepMC2, e.g.

```c++
  class particle_iterator;
  class vertex_iterator;
  class edge_iterator;
  ...
  inline int GenEvent::particles_size() const;
  inline int GenEvent::vertices_size() const;
  ...
```

were removedin HepMC3. C++11 iterators should be used instead, e.g. instead of

```c++
  for (GenEvent::particle_const_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ) {
    ...
  }
```

one should use

```c++
  for (ConstGenParticlePtr& p : evt->particles()) {
    ...
  }
```

or alternatively

```c++
  for (size_t i=0; i < evt->particles().size(); ++i) {
    ...
    evt->particles().at(i)
    ...
  }
```

if the index is needed in the loop.


### 2.6 Topological order

Particles and vertices in HepMC3 are stored in topological order. This
means that when creating vertices, incoming particles must have id
lower than any of the outgoing particles.

This forces the tree structure to be constructed top-to-bottom and
disallows creating loops.

```c++
  GenParticlePtr p1 = make_shared<GenParticle>();
  GenParticlePtr p2 = make_shared<GenParticle>();
  GenParticlePtr p3 = make_shared<GenParticle>();
  GenParticlePtr p4 = make_shared<GenParticle>();

  GenVertexPtr v1 = make_shared<GenVertex>();
  GenVertexPtr v2 = make_shared<GenVertex>();
  GenVertexPtr v3 = make_shared<GenVertex>();

  event.add_particle(p1);
  event.add_particle(p2);
  event.add_particle(p3);
  event.add_particle(p4);
  event.add_vertex(v1);
  event.add_vertex(v2);
  event.add_vertex(v3);

  v1->add_particle_in (p2);
  v1->add_particle_out(p3);
  v1->add_particle_in (p4); // will cause error, because p3
                            // has higher index than p4

  v2->add_particle_in (p4);
  v2->add_particle_out(p3); // will also cause error

  // Order of vertices does not matter. Index of end vertex
  // can be lower than index of production vertex
  v3->add_particle_in (p1);
  v3->add_particle_out(p2);
```


### 2.7 Deleting particles and vertices

Deleting a particle using `GenEvent::remove_particle()` will also
remove its `end_vertex` if this is the only particle on this vertex's
`particles_in()` list.

Deleting a vertex will delete all of its outgoing particles (and
subsequently, all of their decays).


### 2.8 Barcodes can no longer be se (Use constant ID instead)

The `barcode` integer in HepMC2 was an uncomfortable object,
simultaneously declared in the code documentation to be a meaningless
unique identifier for vertex and particle objects, and set to specific
ranges by experiments' production systems to encode information about
a particle's origins. It proved impossible to satisfactorily reconcile
these twin uses, and experiments' demands for particle provenance
information have exceeded the capacity of an `int` (or even a `long
int`).

Hence, barcodes are no longer available. Use attributes to provide
additional information that was previously encoded using barcodes (see
module @ref attributes).

The unique identifier of particles and vertices is now called `id()`
to separate its role from barcodes. The ID is set automatically and
cannot be changed. ID is not permanently attached to
particle/vertex. When a particle or vertex is removed from the event,
ID's of other particles or vertices may change.


### 2.9 `Flow` is not a class (it is an attribute).

The `Flow` class has been removed, since it was unused by any
widespread event generator, and to our knowledge the only active
use-case is an abuse of it to provide more ints in which to encode
provenance information. As this is now done via attributes, there is
no case for `Flow`'s continued existence. No backward compatibility
`Flow` class is provided since this usage is extremely localised in
one piece of user code and migration to the newer scheme should be
simple.


### 2.10 Units are no longer defined at compilation time

The default units are set to `GEV` and `MM`. They can be provided as
constructor parameters or changed later using
`HepMC::GenEvent::set_units`:

```c++
  GenEvent event(Units::GEV,Units::CM);
  GenParticlePtr p = make_shared<GenParticle>();
  event.add_particle(p);
  ...
  event.print(); // event printed in GEV/CM
  event.set_units(Units::MEV,Units::MM); // will trigger unit conversion for all particles and vertices
  event.print(); // event printed in MEV/MM
```


### 2.11 Deprecated code

Many HepMC2 functions have been declared obsolete and are marked as
deprecated. Warnings displayed at compilation time hint to what
functions or classes should be used instead.

```c++
  // HepMC2 code:
  HepMC::FourVector position(pos[1], pos[2], pos[3], pos[0]);
  vertex = new HepMC::GenVertex(position, id);

  // Replace with:
  HepMC3::FourVector position(pos[1], pos[2], pos[3], pos[0]);
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>(position);
  vertex->set_status(1);
```

```c++
  // HepMC2 code:
  std::vector<HepMC::GenParticle*> beamparticles
  // ...
  event.set_beam_particles(beamparticles[0], beamparticles[1]);

  // Replace with:
  std::vector<HepMC3::GenParticlePtr> beamparticles;
  // ...
  event.add_particle(beamparticles[0]);
  event.add_particle(beamparticles[1]);
```

```c++
  // HepMC2 code:
  HepMC::GenVertex* vertex;
  vertex->set_id(1);
  vertex->id();

  // Replace with:
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
  vertex->set_status(1);
  vertex->status();
```

```c++
  // HepMC2 code:
  HepMC::GenVertex * vertex;
  for (HepMC::GenVertex::particles_out_const_iterator pout
        = v->particles_out_const_begin();
        pout != (*vit)->particles_out_const_end(); ++pout) {
    ...
  }

  // Replace with (and similarly for particles_in):
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
  for (HepMC3::GenParticlePtr pout : vertex->particles_out() ) {
    ...
  }
```

```c++
  GenEvent evt(Units::GEV,Units::MM);
  // HepMC2 code:
  evt.set_alphaQCD(m_alphas);
  evt.set_alphaQED(m_alpha);
  evt.set_event_scale(m_event_scale);
  evt.set_mpi(m_mpi);
  evt.set_signal_process_id(m_signal_process_id);
  evt.set_random_states(m_random_states);
  
  // Replace with (if really needed):
  evt.add_attribute("alphaQCD",
                     make_shared<DoubleAttribute>(m_alphas));
  evt.add_attribute("alphaEM",
                     make_shared<DoubleAttribute>(m_alpha));
  evt.add_attribute("event_scale",
                     make_shared<DoubleAttribute>(m_event_scale));
  evt.add_attribute("mpi",
                     make_shared<IntAttribute>(m_mpi));
  evt.add_attribute("signal_process_id",
                     make_shared<IntAttribute>(m_signal_process_id));
  for (size_t i = 0; i < m_random_states.size(); i++)
    evt.add_attribute("random_states" + to_string(i),
                      make_shared<IntAttribute>(m_random_states[i]));
```

```c++
  // HepMC2 code:
  HepMC::GenVertex* vertex;
  ...
  vertex->weights().push_back(1.);
  
  // Replace with:
  vertex->add_attribute("weight1", make_shared<DoubleAttribute>(1.0));
```

```c++
  // HepMC2 code:
  HepMC::GenVertex* vertex;
  vertex->check_momentum_conservation();
  
  // Replace with:
  TODO
```

```c++
  // HepMC2 code:
  HepMC::GenParticle::set_flow(int code_index, int code = 0)
  HepMC::GenParticle::set_polarization( Polarization(theta,phi) )

  // Replace with:
  HepMC3::GenParticle::add_attribute("flow"+to_string(code_index),make_shared<IntAttribute>(code));
  HepMC3::GenParticle::add_attribute("theta",make_shared<DoubleAttribute>(theta));
  HepMC3::GenParticle::add_attribute("phi",make_shared<DoubleAttribute>(phi));
```

```c++
  // HepMC2 code:
  GenCrossSection* xsec;
  double xs, xs_err;
  // ...
  xsec->set_cross_section(xs, xs_err)

  // Replace with:
  GenCrossSectionPtr xsec;
  xsec->set_cross_section({xs, xs_err})
```


## 3 Standard attributes

For the user convenience and backward compatibility the following
standard attributes are supported for `GenEvent`:
```c++
  double alphaQCD
  double alphaEM
  double event_scale
  int mpi
  int signal_process_id
  int signal_vertex_id
  int random_states1... random_statesN
```

And for `GenVertex`:
```c++
  double weight1... weightN
```

And `GenParticle`:
```c++
  int flow
  double theta
  double phi
```

The presence of cycles in the event structure is indicated with an attribute:
```c++
  int cycles
```

Note that attributes belong to the event, therefore they can be set
only for particles and vertices that already belong to a `GenEvent` object.


## 4 Interface to HEPEVT block

The most recent versions of HepMC3 have multiple implementations of
the interfaces to HEPEVT Fortran common blocks. These are

- `include/HepMC3/HEPEVT_Wrapper.h` -- the default implementation. The
  size of common block is defined in compile time via an appropriate
  `define` directive. The block can hold `float`/`double`-precision
  momenta.  This implementation is not compiled into any library. All
  functions and variables are static, so only one instance of the
  interface can exists.

- `include/HepMC3/HEPEVT_Wrapper_Runtime.h` -- The size of common
  block is defined in runtime.  The block can be held in the
  object. Multiple instances can exists.  The interface is compiled
  into the library. This interface is also available in Python.

- `include/HepMC3/HEPEVT_Wrapper_Runtime_Static.h` -- The size of
  common block is defined in runtime.  All functions and variables are
  static, so only one instance of the interface can exists. The
  interface is compiled into the library.

- `include/HepMC3/HEPEVT_Wrapper_Template.h` -- The size of common
  block is defined in compile time as a parameter of template. The
  block can hold `float`/`double`-precision momenta.  The block can be
  held in the object. Multiple instances can exist.


## 5 GenRunInfo class

A new class has been provided to store run-level information, such as
weight names, names and description of tools used to generate the
event, global attributes such as LHE run information or any other run
information provided by user. See `HepMC::GenRunInfo` class
description for details.


## 6 Attributes

Attributes can be attached to GenEvent, GenParticle or GenVertex and
they can have any format defined by the user (see @ref
writing_attributes). An attribute is accessed through a shared pointer
and identified by its name.

Example of reading an attribute from the event:

```c++
  shared_ptr<GenPdfInfo> pdfinfo = event.attribute<GenPdfInfo>("GenPdfInfo");
  if (pdfinfo) pdf_info->print();
```

Example of adding an attribute to the event:

```c++
  shared_ptr<GenPdfInfo> pdf_info = make_shared<GenPdfInfo>();
  evt.add_attribute("GenPdfInfo",pdf_info);

  // Setting values can be done before or after adding it to the event
  pdf_info->set(1,2,3.4,5.6,7.8,9.0,1.2,3,4);
```

Adding and getting attributes of a vertex or particle uses the same
principles.

Note: An event (or particle or vertex) can have more than one
attribute of the same type distinguished by different names. This
might be useful in some applications, however, we encourage to use
just one instance named to match its class name, as in these examples.


### 6.1 Writing custom attributes

Any class that derives from HepMC::Attribute class can be used as
an attribute that can be attached to the event, vertex or particle.

The user has to provide implementation of two abstract methods from
HepMC3::Attribute, used to parse the class content from/to string.

Example:

```c++
  #include "HepMC3/Attribute.h"

  struct MyAttribute : public HepMC3::Attribute {

      double val1; /// First value
      int val2; /// Second value

  public:
  
      /// Implementation of Attribute::from_string
      bool from_string(const string &att) {
          val1 = stof( att );
          val2 = stol( att.substr( att.find(' ')+1 ) );
          return true;
      }

      /// Implementation of Attribute::to_string
      bool to_string(string &att) const {
          char buf[64];
          sprintf(buf,"%.8e %i",val1,val2);
          att = buf;
          return true;
      }
  };
```

For other examples see attributes provided in the HepMC3 package.


## 7 IO-related classes and interfaces

The main HepMC3 library contains the classes for the I/O of multiple event formats.

Optionally the I/O capabilities can be implemented as plugin
Reader/Writer classes compiled separately into dynamically loadable
libraries and used via RearedPlugin and WriterPlugin classes.  Please
note that all required libraries should be loadable. See examples for
details.

In some cases the fine tuning of the Reader/Writer classes behavior
can be done using a map of string "options":

```c++
  void Reader::set_options(const std::map<std::string, std::string>& options)
  std::map<std::string, std::string> Reader::get_options() const
```

The options for HepMC3::ReaderAsciiHepMC2:

 - `disable_pad_cross_sections`, `pad_cross_section_value`/`pad_cross_section_error`:
   If `disable_pad_cross_sections` is present the reader will keep a single cross-section per event, just
   in the HepMC2 style. This is pre-3.2.6 default behaviour. 
   Otherwise, the cross-section vector will be expanded to the size  of event weights. This is 3.2.6+ default behaviour.
   If present, `pad_cross_section_value`/`pad_cross_section_error` values will be inserted into the cross-section vector.
   Otherwise, the cross-sections and errors will be filled with zeros.
 

 - `particle_flows_are_separated` ,`event_random_states_are_separated`, `vertex_weights_are_separated`
   Regulate if the corresponding information from IO_GenEvent would be stored into multiple attributes as
   individual numbers, i.e. "separated" or as a single `std::vector`. The former behavior is used if
   the corresponding option name is present in the list of options, regardless of the option value.
   The latter behavior is the default one.

The options for HepMC3::WriterAscii and HepMC3::WriterAsciiHepMC2:

 - `float_printf_specifier`
   Specifies the float printf specifier used for the output format of the floats.
   Two first characters from the  option value are used.
   The default behavior is equivalent to setting this option to `e` and results in the output formatted as
   ` %.*e`. To save the disk space one can use the `g` option, e.g.
   ```c++
     WriterAscii outputA("someoutput.hepmc");
     auto optionsA =  outputA.get_options();
     optionsA["float_printf_specifier"] = "g";
     outputA.set_options(optionsA);
   ```
   This option will be the default in future.


### 7.1 Links

The relations between vertices and particles in GenEventData are
encoded via members `links1` and `links2`, wich are `std::vector<int>`
containing object IDs.  Direct manipulations with `links1` and
`links2` can be useful. For instance, when the events are saved in
ROOT format, one can extract the information from `links1` and
`links2` without reading whole event.  In case `links1[i]` is
particle, `links2[i]` is end vertex. In case `links1[i]` is vertex,
`links2[i]` is outgoing particle. An example of usage is given below.
  
```c++
  // Code to look for a scattered DIS electron in a HepMC3 event record using links.
  // The implementation is extended to provide example of links1, links2 usage.
  GenEventData*  A = ...
  int current_l = 0;                             // If the incoming electron is the first particle in the list
  int vertex_l = 0;                              // We set final vertex to some nonsense value.
  bool found_next_vertex = true;
  while (found_next_vertex) {                    // Looking for the electron end vertex
      found_next_vertex = false;
      for (int i=0; i < A->links1.size(); i++) { // Points from to ...
          if (A->links1[i] > 0 &&                // The link1 should be particle, i.e. >0
              A->links1[i] == current_l+1) {     // The link1 should be our electron
              vertex_l = A->links2[i];           // The end vertex of this electron is found
              found_next_vertex = true;
          }
      }
      std::vector<int> out;                      // Here we will save the outgoing particles
      if (found_next_vertex) {
          for (int j = 0; j < A->links1.size(); j++) { // Points from to ...
              if (A->links1[j] < 0 &&            // The link1 should be a vertex, i.e. < 0
                  A->links1[j] == vertex_l) {    // The link1 should be our end vertex
                  if (std::abs(A->particles_pid[A->links2[j]-1]) == 11) {  // If the outgoing particle is e+/e-
                      out.push_back(A->links2[j]);
                  }
              }
          }
          if (out.size() == 0) {
              printf("Warning: no electron in the new vertex.\n");
              break;
          } else {
              if (out.size() > 1) printf("Warning: more than one electron in the new vertex.\n");
              current_l = out.at(0) - 1;         // Pick up the first electron and use it as current
          }
      }
      if (A->particles_status[current_l] == 1) break; // The end particle is stable. It is our scattered electron.
  }
  ...
```


## 8 Search-related classes and interfaces

HepMC3 comes with an optional Search library for finding particles
related to other particles or vertices.  It provides a set of
functions to perform simple search operations e.g.
  
```c++
  std::vector<HepMC3::GenParticlePtr>      children_particles(const HepMC3::GenVertexPtr& O);   ///< Return children particles
  std::vector<HepMC3::ConstGenVertexPtr>   grandchildren_vertices(const HepMC3::ConstGenVertexPtr& O); ///< Return grandchildren vertices
  std::vector<HepMC3::GenParticlePtr>      parent_particles(const HepMC3::GenVertexPtr& O);  ///< Return parent particles
  std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(const HepMC3::GenVertexPtr& obj);      ///< Return ancestor vertices  
```

and interfaces for a more advanced usage. For the latter two main
interfaces are defined: Relatives, for finding a particular type of
relative, and Feature, for generating filters based on Features
extracted from particles. In addition, operator on Filters are also
defined.


### 8.1 Relatives Interface

The Relatives interface is defined within
`search/include/HepMC3/Relatives.h`.  Classes that implement this
interface must provide a set of operator functions that take either a
`GenParticlePtr`, `ConstGenParticlePtr`, `GenVertexPtr` or
`ConstGenVertexPtr` and return a vector of either `GenParticlePtr` or
`ConstGenParticlePtr`.  Note that the versions of the operator
functions that receive a consted input parameter also return a
`vector<ConstGenParticlePtr>`, while the versions that take non-const
input return non-const output.  This ensures consistency with the rule
that const objects may only return pointers to const objects.

The HepMC3::Relatives base class is abstract, and has a concrete
implementation in the templated HepMC3::RelativesInterface class.  The
`RelativesInterface` uses type erasure so that any class that obeys the
defined `Relatives` interface (i.e. that has the necessary four operator
functions) can be wrapped in the `RelativesInterface` without needing to
inherit from `Relatives` directly.

For example, if class `Foo` implements the four necessary functions then
the following will work

```c++
  using FooRelatives = RelativesInterface<Foo>;
  Relatives * relos = new FooRelatives();
  GenParticlePtr someInput;
  vector<GenParticlePtr> foos = (*relos)(someInput);
```

The purpose of `Relatives` is to be able to wrap any viable class in a
common interface for finding relatives from a particle or vertex.
Examples are provided in the form of the _parents and _children
classes.  These do not inherit from `Relatives`, but they do implement
the necessary functions.  The `_parents` and `_children` classes are
not intended to be used directly, but they are aliased by wrapping in
the RelativesInterface:

```c++
  using Parents  = RelativesInterface<_parents>;
  using Children = RelativesInterface<_children>;
```

Note also that the `_parents` and `_children` classes use some utility
aliases to help declare the appropriately consted return type.  For
example

```c++
  template<typename GenObject_type>
  GenParticles_type<GenObject_type> operator()(GenObject_type);
```

has a return type `GenParticles_type` that is a vector of
`GenParticlePtr`; this is const if `GenObject_type` is const, but is
not const if `GenObject_type` is not const.  Note as well the use of
`enable_if` so that a single implementation can be used for both the
const and non-const version of the functions.  For the simple case of
_parents the four required funcs could have been implemented directly
without such templating, but for more complicated relatives it avoids
duplicated code.


### 8.2 Recursive Relatives

In addition to the `RelativesInterface` wrapper, `Relatives.h` also
contains a `Recursive` class that can wrap the underlying relation in
recursion.  For example, recursion applied to the parents relationship
provides all of the ancestors, i.e. parents repeatedly applied to the
output of parents.  The only additional requirement to use the
`Recursive` wrapper is that the underlying class must implement a
`vertex(GenParticlePtr)` method that returns the appropriate vertex to
follow from a given `GenParticle`.  As long as a class has such a
method, it is possible to make a recursive version of it

```c++
  using Ancestors = RelativesInterface<Recursive<_parents> >;
```


###  8.3 Existing relatives

The `Relatives` class contains static implementations of the
`Parents`, `Children`, `Ancestors` and `Descendants` relatives, which
can be accessed and used as follows

```c++
  vector<const Relatives*> relos{&Relatives::PARENTS, &Relatives::ANCESTORS,
                                 &Relatives::CHILDREN, &Relatives::DESCENDANTS};
  ConstGenVertexPtr startPosition;
  // Loop over different relationships.
  for (const Relatives* r: relos) {
    for (auto p: r(startPosition)) {
      // Do something with search result p
    }
  }
```


### 8.4 Filters

A HepMC3::Filter is any object that has an operator that takes as
input a `ConstGenParticlePtr` and returns a bool that reflects whether
the input particle passes the filter requirements or not.  `Filter` is
defined in `Filter.h` as
  
```c++
  using Filter = std::function<bool(ConstGenParticlePtr)>;
```

`Filter.h` also contains some logical operators that allow filters to
be combined to create new filters, for example

```c++
  Filter filter1, filter2, filter3;
  Filter filter4 = filter1 && filter2;
  Filter filter5 = filter3 || filter4;
  Filter filter6 = !filter1;
```

`Filter.h` additionally contains a dummy filter that always accepts
every possible particle.  This may be needed in functions that require
a default filter.  The dummy filter is accessed as

```c++
  Filter dummy = ACCEPT_ALL;
```

It is possible to define a filter by hand.  However, there are some
utility classes to define filters based on features that can be
obtained from `GenParticle`s.


### 8.5 Feature interface

The HepMC3::Feature interface is defined in `Feature.h`.  The
interface is templated on a `Feature_type` that is any type that can
be extracted from a `GenParticle`.  This is very flexible, and the
only criteria is that the `Feature` must have the set of comparison
operators.  While the templated `Feature` is general enough to be used
with any type of `Feature`, there are specialisations for both
integral and floating point features.  The specialisations will cover
the vast majority of `Feature`s that are likely to be useful, although
note that `Attribute`s may be a source of more complicated `Feature`s.

To create a `Feature`, one need only wrap a lambda expression in the
`Feature` interface.  For example, to create a `Feature` based on
particle status or pT:
```c++
  Feature<int> status([](ConstGenParticlePtr p)->int{return p->status();});
  Feature<double> pT([](ConstGenParticlePtr p)->double{return p->momentum().pt()});
```

The more general form for any type of `Feature` would be
```c++
  Feature<type> foo([](ConstGenParticlePtr p)->type{return p->foo();});
```

Having created a `Feature`, it can be used to create `Filter`s for
particle selection.  Applying operators to `Feature`s creates the
`Filter`, which is a functor that evaluates on a particle. For example
```c++
  ConstGenParticlePtr p;
  Filter is_stable = (status == 1);
  bool stable = is_stable(p);

  // this evaluates true if p has pT above 100.
  bool passPTCut = (pT > 100.)(p);

  // The Features can be combined
  bool combined = ((pT > 100.) && (status == 1))(p);
```

It is also possible to make a new `Feature` from the absolute value of
a previous `Feature`, e.g.
```c++
  Feature<double> rapidity([](ConstGenParticlePtr p)->double{return p->momentum().rapidity()});
  bool passes_rapCut = (abs(rapidity) < 2.5)(p);
```

Some standard features are contained within the non-templated `Selector` class.


## 9 Selectors and SelectorWrapper

HepMC3::Selector is a simplified interface that contains some
predefined `Feature`s that can be used to search.  `Selector` defines
comparisons operators for both integral and floating point types, as
well as the following selection features:
```c++
  Selector::STATUS
  Selector::PDG_ID
  Selector::PT
  Selector::ENERGY
  Selector::RAPIDITY
  Selector::ETA
  Selector::PHI
  Selector::ET
  Selector::MASS
  Selector::ATTRIBUTE(const std::string)
```

So, for example, a filter can be defined as follows:
```c++
  Filter f = (Selector::STATUS == 1 && Selector::PT > 60.) || (Selector::MASS > 70. && Selector::MASS < 110.);
  GenParticlePtr p;
  bool passesCuts = f(p);
```

As with `Feature`, it is possible to take tbe absolute value of a
Selector.  However, note that while `Feature` is templated, `Selector`
is abstract and so it is not possible for `abs()` to return a
`Selector` object directly, only a pointer:
```c++
  Filter f = *abs(Selector::RAPIDITY) < 2.5;
  bool passRapidity = f(p);
```

Note that the ATTRIBUTE selection is different from the others and
does not have the full set of comparison operators.  This is a current
limitation of the Attributes, which are not guaranteed to offer all
comparisons.  ATTRIBUTE takes a string, which is the name of the
attribute, and permits the equality operator and the method exists,
which checks if the attribute is even present:
```c++
  string name = "MyAttribute";
  Filter f = Selector::ATTRIBUTE(name).exists() && Selector::ATTRIBUTE(name) == "My Value";
  bool passesAttribute = f(p);
```


### 9.1 Applying Filters

The function `applyFilter()` is used to apply a `Filter` to a set of particles.
See for example `examples/BasicExamples/basic_tree.cc`:
```c++
  for (ConstGenParticlePtr p : applyFilter( *abs(Selector::PDG_ID) <= 6, someParticles)) {
      Print::line(p);
  }
```  

  
## 10 Python Bindings

HepMC3 includes Python bindings codes suitable for compilation of
Python modules for Python3.

The binding codes were generated automatically using the binder
utility version 1.4.0 created by Sergey Lyskov (Johns Hopkins
University) et al.  See - https://cppbinder.readthedocs.io/en/latest/
for details.

The binding codes use the pybind11 library version 2.6.0 by Wenzel
Jakob, EPFL's School of Computer and Communication Sciences.  See -
https://pybind11.readthedocs.io/en/master/ - Wenzel Jakob and Jason
Rhinelander and Dean Moldovan, "pybind11 -- Seamless operability
between C++11 and Python", 2017, https://github.com/pybind/pybind11


### 10.1 Installation from repositories

The Python bindings together with the HepMC3 itself can be installed
from PyPy and multiple other repositories.  Please see [HepMC3
page](https://gitlab.cern.ch/hepmc/HepMC3) at CERN GitLab for details.

### 10.2 Installation from sources

To turn on the compilation of bindings use -DHEPMC3_ENABLE_PYTHON =
ON.  By default the Python modules will be generated for Python3 if
these are found in the system.  In case the test suite is enabled,
tests of Python bindings with all the enabled versions will run as
well.

Despite not being recommended, it should be possible to compile the
Python bindings using the installed version of HepMC3.  To do this,
copy the Python directory outside the source tree, uncomment
`#project(pyHepMC3 CXX)` in `python/CMakeLists.txt` and run CMake inside
the `python/` directory with the `-DUSE_INSTALLED_HEPMC3=ON` option.

### 10.3 Selected aspects of Python bindings

In general, the syntax used in the Python bindings is exactly the same
as in the C++ code.  However, some C++ classes and routines differ from
their Python equivalents, namely:
 
  - The constructors of Readers/Writers with `ifstream`s/`ostream`s are not binded.
  - The multithread reader `ReaderMT` is not binded. 
  - The explicit readers and writers with compression support (`ReaderGZ`/`WriterGZ`) are not binded. 
    It is recommended to use built-in Python compression modules in combination with the desired readers/writers.
    The `deduce_reader` is binded and uses the internal Python compression libraries, i.e. it has no dependence on zlib, zstd etc.
    The only requirement is that the corresponding module is available.
  - A limited support for the ROOTTree format I/O with `uproot`
    (see [uproot](https://github.com/scikit-hep/uproot5)) is available if the `uproot` module is installed.
    HepMC3 ROOTTree files can be read with `ReaderUprootTree`.


## 11 Handling Les Houches Event Files

This module contains helper classes and Reader and Writer classes for
handling [Les Houches event files (LHEF)](https://arxiv.org/abs/hep-ph/0609017).
  
### 11.1 Introduction

The Les Houches accord on an event file format (LHEF) to be used for
passing events from a matrix element generator program (MEG) to an
event generator program (EG) implementing parton showers, underlying
event models, and hadronisation models etc., was not originally
included in the HepMC event record format. But as the demand for more
information to be included in HepMC, it was decided to allow HepMC to
include also the original information from a MEG in the LHEF format
(see the run attribute HepMC3::HEPRUPAttribute and event attribute
HepMC3::HEPEUPAttribute). A separate /standard/ implementation in C++
of the LHEF format had already been maintained by Leif Lönnblad, and
it was decided to include this (header only - LHEF.h) as a part of
HepMC3. This will both be used in above mentioned HepMC3::Attribute
classes and as a kind of definition of the LHEF format, which so far
has not been extremely well documented. From now on these pages will
serve as the defining information about the LHEF format.

### 11.2 Background

The original Les Houches accord for communicating between MEGs and EGs
was agreed upon in 2001
[arXiv:hep-ph/0109068](http://archive.org/abs/hep-ph/0109068) and
consisted of two simple FORTRAN common blocks. In fact this structure
survived in the LHEF format, which was introduced in 2006
[arXiv:hep-ph/0609017](http://archive.org/abs/hep-ph/0609017), and is
still there after the updated versions 2 in 2009
[arXiv:1003.1643](http://archive.org/abs/1003.1643), and 3 in 2013
[arXiv:1405.1067](http://archive.org/abs/1405.1067), and in the
current proposal developed at the Les Houches workshop on TeV
Colliders 2015.
  
As the methods for combining MEGs and EGs has advanced since the first
accord, from the tree-level merging methods and NLO matching at the
turn of the millennium, to the multi-jet (N)NLO matching and merging
methods being perfected to day, the LHEF format has developed and a
lot of optional information can be passed beyond the original common
block structures. In the following all features included will be
described, also those that were added a bit prematurely and later
became deprecated.
  
### 11.3 The basic structure

The LHEF format is based on XML, but has some oddities that go
beyond pure XML. As the name indicates, XML is extensible, and anyone
writing a LHEF file can add whatever information she or he wants,
however the following basic structure must be observed.  
```xml
  <LesHouchesEvents version="3.0">
  <!--
  # optional information in completely free format,
  # except for the reserved end tag (see next line)
  -->
  <header>
  <!-- individually designed XML tags, in fancy XML style -->
  </header>
  <init>
  compulsory initialization information
  # optional initialization information
  </init>
  <event>
  compulsory event information
  # optional event information
  </event>
  <event>
  compulsory event information
  <!-- more optional information -->
  </event>
  <!-- and as many events that you want, but ending with -->
  </LesHouchesEvents>
```
  
This looks like fairly normal XML tags, and indeed they are. The only
addition to the structure is that the `init` and `event` (and their
respective end tags) are required to be alone on a line, and the
content of these blocks are required to start with a number of lines
on a specific format that follows exactly the structure of the fortran
common block in original Les Houches Accord. This means that the first
line in the `init` block must start with a line containing the numbers
```
  IDBMUP(1) IDBMUP(2) EBMUP(1) EBMUP(2) PDFGUP(1) PDFGUP(2) PDFSUP(1) PDFSUP(2) IDWTUP NPRUP
```
and the following `NPRUP` lines should be numbers in the form
```
  XSECUP(IPR) XERRUP(IPR) XMAXUP(IPR) LPRUP(IPR)
```

where the different variable names are defined as follows:

 - `IDBMUP`: the PDG numbers for the incoming beams;
 - `EBMUP`: the energy (in GeV) of the incoming beams;
 - `PDFGUP` and `PDFSUP`: the LHAPDF group and set
    numbers for the corresponding parton densities used;
 - `IDWTUP`: the weight strategy used;
 - `NPRUP`: the number of different processes included in the file;

and for each process `IPR`:

 - `XSECUP`, `XERRUP`: the Monte Carlo integrated cross section and error estimate;
 - `XMAXUP`: the overestimated cross section such that the
    sum over the individual weights in each `<event>` times
    this file times `XMAXUP` equals `XSECUP` times the number of events;
 - `LPRUP`: a unique integer identifying the corresponding process.

In the LHEF::Reader and LHEF::Writer classes this information is
available as the public `heprup` member of class LHEF::HEPRUP with
public members mimicking the Fortran common-block variables.  
    
Similarly, every `<event>` block must start with a line containing the numbers
```
  NUP IDPRUP XWGTUP SCALUP AQEDUP AQCDUP
```

and the following `NUP` lines should be numbers describing each particle in the form

```
  IDUP(I) ISTUP(I) MOTHUP(1,I) MOTHUP(2,I) ICOLUP(1,I) ICOLUP(2,I) PUP(1,I) PUP(2,I) PUP(3,I) PUP(4,I) PUP(5,I) VTIMUP(I) SPINUP(I)
```

where the different variable names are defined as:

 - `NUP`: the number of particles in the event;
 - `IDPRUP`: the integer identifying the process;
 - `XWGTUP`: the (default) weight of the event
 - `SCALUP`: the scale of the hard process in GeV;
 - `AQEDUP`: the value of &alpha;<sub>EM</sub> used;
 - `AQCDUP`: the value of &alpha;<sub>S</sub> used;

and for each particle `I`:

  - `IDUP`: the PDG code for the particle;
  - `ISTUP`: the status code for the particle;
  - `MOTHUP`: the line numbers of two particles considered to be the mothers of this particle;
  - `ICOLUP`: indices of the colour and anti-colour lines connected to this particle;
  - `PUP`: the <i>x</i>, <i>y</i>, <i>z</i> and energy component of the 4-momentum and invariant masss (in units of GeV);
  - `VTIMUP`; the proper lifetime of this particle;
  - `SPINUP`: the spin.

In the LHEF::Reader and LHEF::Writer classes this information is
available as the public `hepeup` member of class LHEF::HEPEUP with
public members mimicking the Fortran common block variables.


### 11.4 Additional information
  
Over the years several additional XML-tags have been formalised to
specify information on top of what is given in the original Les
Houches accord common block. These are listed below. In most cases the
tag name corresponds to a class with a corresponding name available as
suitably named public members in the LHEF::HEPRUP and LHEF::HEPEUP
class respectively.
  
Note that a tag may contain attributes in the following ways:

```xml
  <tag attribute1="value" attribute2="value">things marked by the tag</tag>
  <tag attribute1="value" attribute2="value" attribute3="value" />
```

where the second case is a tag with only attributes and no contents.
In the following an attribute may be described as required (R) or
optional with a default value (D).


### 11.5 Standardised tags in the init block
  
The `<init>` block contains information about the full run (similar to
the information contained in HepMC3::GenRunInfo). The following tags
are defined.
  
  - `<generator` (optional, multiple, see LHEF::HEPRUP::generators):
  For easy access to the generator(s) used to generate this file. An
  optional attribute `version` can be given with a string containing a
  version string. The content of the tag can include any generator
  specific information.
  
  - `<xsecinfo>`</b> (required, multiple, see LHEF::HEPRUP::xsecinfos):
  The information in the HEPRUP common block is in principle
  sufficient to figure out the cross sections of the processes
  involved. However, the way things are specified is a bit
  confusing and complicated since it was assumed to be used to
  pass information between the MEG and PSG in both
  directions. For the event file, the communication is per
  definition one-way, and the information can be made more
  easily provided. The attributes for the `<xsecinfo>` tag are as
  follows:
   * `weightname`: in case of multiple weights for each
     event several `<xsecinfo>` tags can be given, in
     which case this should give the corresponding weight
     name. If missing, this is assumed to be the default weight.
   * `neve` (R): the number of events in the file
   * `totxsec` (R): the total cross-section (in units of pb) of all
     processes in the file
   * `maxweight` (D=1): the maximum weight of any event
     in the file (in an arbitrary unit)
   * `minweight`: if the file contains negative weights,
     to can be used to specify the most negative of the
     negative weights. If not given, `minweight` is
     assumed to be -`maxweight` .
   * `meanweight` (D=1): The average weight of the
     events in the file (same unit as `maxweight` ).
   * `negweights` (D=no): If yes, the file may contain
     events with negative weights.
   * `varweights` (D=no): If yes, the file may contain
     varying weights (if no, all events are weighted with
   * `maxweight` (or `minweight` )).

  - `<cutsinfo>` (optional, multiple, see LHEF::HEPRUP::cuts):
  Used by a generator to supply information about the cuts used
  in the generation. Inside this tag any number of `cut`
  and `ptype` tags can be supplied.

  - `<ptype>` (optional, multiple, see LHEF::HEPRUP::ptypes):
  To be used inside the `cutinfo` tag to define a group
  of particles to which a cut can be applied. Its contents
  should be a white-space separated list of PDG particle
  codes. The only attribute is `name` (R): the string
  used to represent this `ptype` in a `cut` .

  - `<cut>` (optional, multiple, see LHEF::HEPRUP::cuts):  
    This tag has information of a particular cut used. The information
    needed is which particle(s) are affected, what variable is used
    and the maximum and/or minimum value of that parameter. The
    contents should be the minimum and maximum allowed values of this
    variable (in that order). If only one number is given, there is no
    maximum. If the minumum is larger or equal to the maximum, there
    is no minimum. The attributes are:
     * `p1` (D=0): The particles for which this cut
       applies. This can either be a number corresponding to a given
       particle PDG code or 0 for any particle or the name of a
       previously defined `ptype` tag.
     * `p2` (D=0): If cut is between pairs of particles, this
       attribute should be non-zero. Allowed values are the same as
       for `p1`.
     * `type` (R) This defines the variable which is cut. The
       following values are standardised (the lab frame is assumed in all
       cases):
        - "m" the invariant mass of two particles (or, if only one
          particle type is given, the invariant mass of that particle).
        - "kt": the transverse momenta of a particle matching p1 (in GeV)
        - "eta": the pseudorapidity of a particle matching p1
        - "y": the true rapidity of a particle matching p1
        - "deltaR": the pseudorapidity--azimuthal-angle difference between two particles matching p1 and p2 respectively.
        - "E": the energy of a particle matching p1
        - "ETmiss": the norm of the vectorial sum of the pt of particles matching p1 and not matching p2.
        - "HT": the scalar sum of the transverse momentum of the particles matching p1 and not matching p2.
        - other values are allowed but are not included in the standard.
     
  - `<procinfo>` (optional, multiple, see LHEF::HEPRUP::procinfo):
    For each process number used in the LPRUP variable in the HEPEUP
    common block we can have additional information given in the
    following attributes:
     * `iproc` (D=0): The process number for which the information is given. 0 means all processes.
     * `loops`: The number of loops used in calculating this process.
     * `qcdorder`: The number of QCD vertices used in calculating this process.
     * `eworder`: The number of electro-weak vertices used in calculating this process.
     * `rscheme`: The renormalization scheme used, if applicable.
     * `fscheme`: The factorization scheme used, if applicable.
     * `scheme`: Information about the scheme used to
       calculate the matrix elements. If absent, a pure tree-level
       calculation is assumed. Other possible values could be
       CSdipole (NLO calculation with Catani-Seymour subtraction),
       MCatNLO, POWHEG and NLOexclusive (NLO calculation according to
       the exclusive cross section withing the given cuts).
    The content of this tag is a string with arbitrary information about the process.

  - `<mergeinfo>` (DEPRECATED, multiple, see LHEF::HEPRUP::mergeinfo):
    For some merging schemes (eg. for CKKW) it is possible to reweight the
    the events with Sudakov form factors already in the MEG. If this has
    been done the content of the mergetype tag for the corresponding
    process should give a name corresponding to the scheme used. The
    attributes are:
     * `iproc` (D=0): The process number for which the information is given. "0" means all processes.
     * `mergingscale` (R): The value of the merging scale in GeV.
     * `maxmult` (D=no): If yes the corresponding process is reweighted as if it is the
       maximum-multiplicity process (i.e. the Sudakov for the last step down to the merging
       scale is not included.)

  - `<eventfile>` (optional, multiple, see LHEF::HEPRUP::eventfiles):
    Sometimes it is desirable to have the events in a separate file(s)
    from the `init` block, eg. when the event files become
    extremely long or when one wants to write the `init` block
    after having generated all events in order to collect the correct
    information from the run. The names of these files are then
    specified in `eventfile` tags with attributes:
      * `name` (R): the file name, interpreted as an absolute
        path if starting with `/`, otherwise as a relative path to
        where the file with the `init` block is located;
      * `neve`: the number of events in the file
      * `ntries`: the number of attempts the generator needed
        to produce the `neve` events (for statistics purposes).

  - `<weightinfo>` (optional, multiple, see LHEF::HEPRUP::weightinfo):
    When using multiple weights for events, each weight is given an
    index and a name using the `weightinfo` tag. The default
    weight (as given by the LHEF:HEPEUP::XWGTUP variable) is always
    treated as index 0 and given the name "Default", while the
    additional weights are indexed by the order of
    the `weightinfo` tags. The attributes are:
      * `name` (R): the name of the weight (in the best of all
        worlds this would be standardised);
      * `muf`: he factor multiplying the nominal factorisation
        scale of the event for the given weight;
      * `mur`: he factor multiplying the nominal
        renormalisation scale of the event for the given weight;
      * `pdf` (D=0): the LHAPDF code used for the given weight
        (where 0 corresponds to the default PDF of the run);
      * `pdf2` (D=pdf): the LHAPDF code used for the second
        beam for the given weight;

  - `<weightgroup>` (optional, multiple, see LHEF::HEPRUP::weightgroup):
    This tag can be used to group `<weightinfo>` tags together.  The only well-defined
    attribute is `name`, giving a string which will be combined with the `name`
    attributes of the included `<weightinfo>` tags to give the HepMC weight names.
    Also, other attributes can be included to include information about the weight
    variation used and is available in LHEF::WeightGroup::attributes.

  - `<initrwgt>` (optional, multiple, see LHEF::HEPRUP::weightinfo):
    Due to an initial confusion some generators uses this tag instead
    of the `weightinfo` tag. It accepts the same attributes
    as `weightinfo`, except that the name attribute is
    named `id`.  Note that some generators puts these tags in
    the `header` block. The current implementation of
    LHEF::Reader cannot handle this. Note that this is handled by the
    same classes (LHEF::WeightInfo and LHEF::WeightGroup) in
    LHEF::Reader and LHEF::Writer.


### 11.6 Standardised tags in the events block.
  
After the `<init>` block any number of events can be given. In
addition events can be given in separate files declared
with `eventfile` tags in the `init` block.
  
The main tag here is simply called `event` and can (for
statistics purposes) have an attribute `ntries` specifying how
many attempts the generator needed to produce the event. Also other
attributes can be given (and will be stored in the
LHEF::HEPEUP::attributes member variable).
  
The `<event>` tags may be grouped together in a `<eventgroup>` tag. This
is useful mainly for NLO generators that produces a (number of) "real"
event(s) accompanied with a number of "counter" events, where these
events should be treated together for statistics purposes. For this
reason the `eventgroup` tag can be provided with the optional tags
`nreal` and `ncounter` to indicate the number of `event` tags included
of each type.
  
As indicated above the block must start with required information
corresponding to the original Les Houches Accord Fortran
common-block. Here is a list of additional tags that may be provided:

  - `<weights>` (optional, see LHEF::HEPEUP::weights):
    This tag should contain a list of weights or the events given in
    the order they are defined by the `weightinfo` tags in the `init`
    block. If for some obscure reason a weight is absent, it should be
    anyway be included as a zero weight. Note that the weight for the
    nominal event is still given by `XWGTUP`. This tag has no attributes.

  -  `<rwgt>` (optional, see LHEF::HEPEUP::weights):
    For weights declared in `initrwgt` this should contain the
    weights for this event. The difference wrt. the `weights`
    tag is that each weight needs to be given in an `wgt` tag.

  - `<wgt>` (optional, see LHEF::HEPEUP::weights):
    This tag contains a single weight correspoding to a weight
    declared in the `initrwgt` tag. The only attribute is
    the `id` of the weight defined in the corresponding `weightinfo` tag.

  - `<scales>` (optional, see LHEF::HEPEUP::scales):
    Since it is often necessary to define more than one scale in an
    event, this tag is provided to specify these. The scales are given
    as the following attributes:
     * `muf` (D=SCLAUP): the factorisation scale (in GeV);
     * `mur` (D=SCLAUP): the renormalisation scale (in GeV);
     * `mups` (D=SCLAUP): the suggested parton shower starting scale (in GeV).
    Other attributes are also allowed and will be stored in LHEF::Scales::attributes.

  - `<scale>` (optional inside a `<scales>` tag):
    It is also possible to specify an individual scale for any particle in
    the event via a `<scale>` tag. This is typically used to define a starting
    scale for a parton shower in resonance decays. This tag can also be used to
    specify the scale of a set of particle types. The following attributes can be given:
     * `pos` (optional): the index of the particle for which this scale
       applies, optionally followed by a space-separated list of
       indices of recoilers involved when the particle was created.
     * `etype` (optional): a space-separated list of PDG codes
       for which this scale applies. The short-hand notation "EW" can
       be used to specify all leptons and electro-weak bosons, and
       "QCD" can be used to specify the gluon and all quarks.
    The contents of the tag gives the scale in GeV.


## 12 Handling of errors and warnings

The errors and warnings since HepMC 3.3.0 have the following categories:

 - 100: Generic I/O problem, i.e. cannot open a file, cannot import a python module from C++, etc.
 - 200: Buffer overflows
 - 300: Problem serializing attributes, problems with the momentum/lengths units
 - 400: Math error 
 - 500: Unsupported expression in the input 
 - 600: Warious event inconsistencies
 - 700: Various operational warnings, e.g. adding a null particle to vertex
 - 800: GenCrossSection warnings 
 - 900: Outdate formats or objects 

The categories are numbered approximately according to their
importance and if the current warning/error level is set below the
warning level of the category, the warnings/errors from the category
will not be printed.

The warning level can be obtained/set with the `int Setup::warnings_level()`
and `void Setup::set_warnings_level(const int)` functions.  The functions
`bool Setup::print_warnings()` and `void Setup::set_print_errors(const bool flag)`
obtain and set the global flag for printing the warnings but do not change
the warning levels.  The default warning level is 750.

The error level can be obtained/set with the `int Setup::errors_level()`
and `void Setup::set_errors_level(const int)` functions respectively.
The functions `bool Setup::print_errors()` and
`void Setup::set_print_errors(const bool flag)` obtain/set the global flag
for printing the errors but don't change the warning levels.
The default error level is 1000.
