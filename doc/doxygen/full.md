# One page HepMC3 documentation
  
## Build instructions
For the full list of the installation options including the description of the flags to build the
HepMC3 from the sources see  at [HepMC3 page at CERN GitLab](https://gitlab.cern.ch/hepmc/HepMC3).

A quick minimalist build that requires only C++11 compiler and a
recent version of cmake (3.15+) is described below.

### Minimalist build with CMake
To build HepMC3 is  using CMake on the supported platforms, C++11 compiler
and a recent cmake  is needed (3.15).
The commands executed in the unpacked source tarball of HepMC3

```
  cmake -DCMAKE_INSTALL_PREFIX=desired_installation_path -DHEPMC3_ENABLE_ROOTIO=OFF   -DHEPMC3_ENABLE_PYTHON=OFF CMakeLists.txt
  cmake --build .
  cmake --install .
```
will configure the HepMC3 sources, compile them and install the library into "desired_installation_path".


## Differences between HepMC2 and HepMC3
The following is a list of main differences that should be taken into
account when transitioning from HepMC2 to HepMC3.

### Structure change and header file organisation
Following changes in header files have been applied:

```
  HepMC/HeavyIons.h    ->  HepMC3/GenHeavyIons.h   (now a POD struct)
  HepMC/PdfInfo.h      ->  HepMC3/GenPdfInfo.h     (now a POD struct)
  HepMC/SimpleVector.h ->  HepMC3/FourVector.h     (ThreeVector class removed)
```

The structure of GenCrossSection class has been changed to handle multiple
values of cross-sections. The cross-section values and errors (uncertainties)
  can be accessed only by public functions, the corresponding data members are
  private. By default, the number of cross-section values in every event is equal
  to the number of event weights.  Accordingly, each cross-section value can be
  accessed using the corresponding event weight name (std::string) or event
  weight index (int).

Following header files are no longer available:

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

# Fortran generators
An example of interface to Pythia6 Fortran blocks  is given in the examples.
Please note that the provided interface Pythia6ToHepMC3.cc and Pythia6ToHepMC3.inc
is an interface for HepMC3 from Pythia6 and not an interface to Pythia6 from HepMC,
as it was in the case of the HepMC2.

# Changes to the I/O handling
Multiple file formats are supported. The implementation of reading  and writing
is separated in HepMC3. All the reading operations are performed in
"reader" objects inherited from HepMC::Reader and the writing operations in the "writer"
objects inherited from HePMC::Writer. Therefore it is to use the desired headers explicitly, as needed.

The IO_GenEvent.h header is not available anymore.
to write and/or read HepMC2 files the following includes

```
  WriterAsciiHepMC2.h
  ReaderAsciiHepMC2.h
```

should be used instead of

```
  IO_GenEvent.h
```
Please note that HepMC2 format  is outdated and is not able to contain a lot
  of information stored into event record by the modern Monte Carlo event generators.
  It is recommended to use HepMC3 native event record in plain text or in ROOT TTree format.
  The corresponding readers and writers are

```
  WriterAscii.h
  ReaderAscii.h
```
and

```
  WriterRootTree.h
  ReaderRootTree.h
```

Implementation of custom Reader and Writer objects is possible as well.

Please, note the difference in the behaviour of default Readers with respect to HepMC2.
  when reading files with multiple headers. The ASCII files with multiple headers ( e.g. obtained with
  cat 1.hepmc 2.hepmc > 12.hepmc) will be processed by the readers only till the
  first occurrence of END_EVENT_LISTING.

  In addition to the standard readers, starting for the version 3.2.5 HepMC3 provides as set of
  templates readers/writers to handle the zip-,lzma-,bz2-compressed files (ReaderGZ and WriterGZ) and to perform multithread
  reading (ReaderMT).

# Memory managed by shared pointers
Particles and vertices are managed using shared pointers, so they should 
not be created through the call to 'new'.

```
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

### Iterators
The iterator-bases classes and access functions from HepMC2, e.g.

```
  class particle_iterator;
  class vertex_iterator;
  class edge_iterator;
  ...
  inline int GenEvent::particles_size() const;
  inline int GenEvent::vertices_size() const;
  ...

```
were removed. The C++11 iterations should be used instead, e.g. instead of

```
  for (GenEvent::particle_const_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ) {
  ...
  }
```
  one should use
```
  for (const auto& p: evt->particles()) {
  ...
  }
```
or alternatively

```
  for (size_t i=0;i<evt->particles().size();++i) {
  ...
  evt->particles().at(i)
  ...
  }
```



### Topological order
Particles and vertices in HepMC3 are stored in topological order. This means
  that when creating vertices, incoming particles must have id lower than
  any of the outgoing particles.

This forces the tree structure to be constructed top-to-bottom
  and disallows creating loops.

```
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


## Changes to user interface and to HepMC functionality


### Deleting particles and vertices
Deleting a particle using GenEvent::remove_particle() will also remove
  its end_vertex if this is the only particle that is on this vertex
  particles_in() list.

Deleting a vertex will delete all of its outgoing
  particles. (and subsequently, all of their decays).


### Barcodes can no longer be se (Use constant ID instead)
The "barcode" integer in HepMC2 was an uncomfortable object, simultaneously
  declared in the code documentation to be a meaningless unique identifier for
  vertex and particle objects, and set to specific ranges by experiments'
  production systems to encode information about a particle's origins. It proved
  impossible to satisfactorily reconcile these twin uses, and experiments' demands
  for particle provenance information have exceeded the capacity of an int (or
  even a long int).

Hence, barcodes are no longer available. Use attributes to provide additional
  information that was previously encoded using barcodes
  (see module @ref attributes).

The unique identifier of particles and vertices is now called id() to
  separate its role from barcodes. Id is set automatically and cannot
  be changed. Id is not permanently attached to particle/vertex. When
  a particle or vertex is removed from the event, id's of other particles
  or vertices may change.

### Flow is not a class on its own (it is an attribute).
The Flow class has been removed, since it was unused by any widespread event
  generator, and to our knowledge the only active use-case is an abuse of it to
  provide more ints in which to encode provenance information. As this is now done
  via attributes, there is no case for Flow's continued existence. No backward
  compatibility Flow class is provided since this usage is extremely localised in
  one piece of user code and migration to the newer scheme should be simple.


### Units are no longer defined at compilation time
The default units are set to GEV and MM. They can be provided as
  constructor parameters or changed later using HepMC::GenEvent::set_units

```
  GenEvent event(Units::GEV,Units::CM);

  GenParticlePtr p = make_shared<GenParticle>();

  event.add_particle(p);
  ...

  event.print(); // event printed in GEV/CM

  event.set_units(Units::MEV,Units::MM); // will trigger unit conversion for all particles and vertices

  event.print(); // event printed in MEV/MM
```


### Deprecated code
A lot of HepMC2 functions has been declared obsolete and are marked as
  deprecated. Warnings displayed at compilation time hint to what functions
  or classes should be used instead.


```
  // HepMC2 code:
  HepMC::FourVector position(pos[1],pos[2],pos[3],pos[0]);
  vertex = new HepMC::GenVertex(position, id);

  // Replace with:
  HepMC3::FourVector position(pos[1],pos[2],pos[3],pos[0]);
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>(position);
  vertex->set_status(1);
```

```
  // HepMC2 code:
  std::vector<HepMC::GenParticle*> beamparticles
  // ...
  event.set_beam_particles(beamparticles[0],beamparticles[1]);

  // Replace with:
  std::vector<HepMC3::GenParticlePtr> beamparticles;
  // ...
  event.add_particle(beamparticles[0]);
  event.add_particle(beamparticles[1]);
```

```
  // HepMC2 code:
  HepMC::GenVertex * vertex;
  vertex->set_id(1);
  vertex->id();

  // Replace with:
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
  vertex->set_status(1);
  vertex->status();
```

```
  // HepMC2 code:
  HepMC::GenVertex * vertex;
  for (HepMC::GenVertex::particles_out_const_iterator pout
         =v->particles_out_const_begin();
       pout!=(*vit)->particles_out_const_end(); ++pout) { }

  // Replace with (similarly for particles_in):
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
  for (HepMC3::GenParticlePtr pout : vertex->particles_out() ) { }
```

```
  // HepMC2 code:
  vertex->weights().push_back(1.);
  // Replace with:
  TODO
```

```
  GenEvent evt(Units::GEV,Units::MM);
  // HepMC2 code:
  evt.set_alphaQCD(m_alphas);
  evt.set_alphaQED(m_alpha);
  evt.set_event_scale(m_event_scale);
  evt.set_mpi(m_mpi);
  evt.set_signal_process_id(m_signal_process_id);
  evt.set_random_states(m_random_states);
  // Replace with:
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
  for ( size_t i=0;i<m_random_states.size();i++)
    evt.add_attribute("random_states"+to_string(i),make_shared<IntAttribute>(m_random_states[i]));
```

```
  // HepMC2 code:
  HepMC::GenVertex * vertex;
  ...
  vertex->weights().push_back(1.);
  // Replace with:
  vertex->add_attribute("weight1", make_shared<DoubleAttribute>(1.0));

```

```
  // HepMC2 code:
  HepMC::GenVertex * vertex;
  vertex->check_momentum_conservation();
  // Replace with:
  TODO
```

```
  // HepMC2 code:
  HepMC::GenParticle::set_flow(int code_index, int code = 0)
  HepMC::GenParticle::set_polarization( Polarization(theta,phi))
  // Replace with:
  HepMC3::GenParticle::add_attribute("flow"+to_string(code_index),make_shared<IntAttribute>(code));
  HepMC3::GenParticle::add_attribute("theta",make_shared<DoubleAttribute>(theta));
  HepMC3::GenParticle::add_attribute("phi",make_shared<DoubleAttribute>(phi));
```

```
  // HepMC2 code:
  GenCrossSection* XS;
  double xs, xs_err;
  XS->set_cross_section(xs,xs_err)
  // Replace with:
  GenCrossSectionPtr XS;
  ....
  std::vector<double> xs, xs_err;
  ....
  XS->set_cross_section(xs,xs_err)
```


### Standard attributes
For the user convenience and backward compatibility the following standard attributes are
  supported for the

GenEvent
```
  double alphaQCD
  double alphaEM
  double event_scale
  int mpi
  int signal_process_id
  int signal_vertex_id
  int random_states1... random_statesN
```

GenVertex
```
  double weight1... weightN
```

GenParticle
```
  int flow
  double theta
  double phi
```

The presence of cycles in the event structure is indicated with an attribute
```
  int cycles
```

 Note that attributes belong to the event, therefore these can be set only for particles and vertices
 that belong to a GenEvent object.


## Interface to HEPEVT block
The most recent versions of HepMC3 has multiple implementations of the interfaces to HEPEVT Fortran common
  block. These are

- include/HepMC3/HEPEVT_Wrapper.h -- the default implementation. The size of common block is defined in
  compile time via appropriate @c define directive. The block can hold float/double precision momenta.
  This implementation is not compiled into any library. All functions and variables are static,
  so only one instance of the interface can exists.

- include/HepMC3/HEPEVT_Wrapper_Runtime.h -- The size of common block is defined in runtime.
  The block can be held in the object. Multiple instances can exists.
  The interface is compiled into the library. This interface is also available in Python.

- include/HepMC3/HEPEVT_Wrapper_Runtime_Static.h -- The size of common block is defined in runtime.
  All functions and variables are static,
  so only one instance of the interface can exists. The interface is compiled into the library.

- include/HepMC3/HEPEVT_Wrapper_Template.h --  The size of common block is defined in compile
  time as a parameter of template. The block can hold float/double precision momenta.
  The block can be held in the object. Multiple instances can exists.


## GenRunInfo class
A new class has been provided to store run-level information, such as
  weight names, names and description of tools used to generate the event,
  global attributes such as LHE run information or any other run information
  provided by user. See HepMC::GenRunInfo class description for details.



## Attributes
Attributes can be attached to GenEvent, GenParticle or GenVertex
  and they can have any format defined by the user
  (see @ref writing_attributes). An attribute is accessed through
  a shared pointer and identified by its name.

Example of reading an attribute from the event:

```
  shared_ptr<GenPdfInfo> pdf_info = event.attribute<GenPdfInfo>("GenPdfInfo");

  if( pdf_info ) pdf_info->print();
```

Example of adding an attribute to the event:

```
  shared_ptr<GenPdfInfo> pdf_info = make_shared<GenPdfInfo>();
  evt.add_attribute("GenPdfInfo",pdf_info);

  // Setting values can be done before or after adding it to the event
  pdf_info->set(1,2,3.4,5.6,7.8,9.0,1.2,3,4);
```

Adding and getting attributes of a vertex or particle uses the same
  principles.

Note: An event (or particle or vertex) can have more than one attribute
    of the same type distinguished by different names. This might be
    useful in some applications, however, we strongly encourage
    to use just one instance named by its class name, as in these
    examples.

### Writing custom attributes

Any class that derives from HepMC::Attribute class can be used as
  an attribute that can be attached to the event, vertex or particle.

User has to provide two abstract methods from HepMC::Attribute used
  to parse the class content from/to string.

Example:

```
  #include "HepMC3/Attribute.h"

  struct MyAttribute : public HepMC::Attribute {

      double val1; /// First value
      int    val2; /// Second value

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



## IO-related classes and interfaces
The main HepMC3 library contains the classes for the I/O of multiple event formats.

Optionally the I/O capabilities can be implemented as plugin Reader/Writer classes compiled
  separately into dynamically loadable libraries and used via  RearedPlugin and WriterPlugin classes.
  Please note that all required libraries/dlls should be loadable.
  See examples for details.

In some cases the fine tuning of the Reader/Writer classes behavior can be done using a
  map of string "options"

```
     void Reader::set_options(const std::map<std::string, std::string>& options)

     std::map<std::string, std::string> Reader::get_options() const
```

The options for ReaderAsciiHepMC2

 - "disable_pad_cross_sections", "pad_cross_section_value"/"pad_cross_section_error"
 If "disable_pad_cross_sections" is present the reader will keep a single cross-section per event, just
 in the HepMC2 style. This is pre-3.2.6 default behaviour. 
 Otherwise, the cross-section vector will be expanded to the size  of event weights. This is 3.2.6+ default behaviour.
 If present, "pad_cross_section_value"/"pad_cross_section_error" values will be inserted into the cross-section vector.
 Otherwise, the cross-sections and errors will be filled with zeros.
 

 - "particle_flows_are_separated" ,"event_random_states_are_separated"
 "vertex_weights_are_separated"
 "particle_flows_are_separated"
 Regulate if the corresponding information from IO_GenEvent would be stored into multiple attributes as
 individual numbers, i.e. "separated" or as a single std::vector. The former behavior is used if
 the corresponding option name is present in the list of options, regardless of the option value.
 The later behavior is the default one.

The option for WriterAscii and WriterAsciiHepMC2

 - "float_printf_specifier"
 specifies the float printf specifier used for the output format of the floats.
 Two first characters from the  option value are used.
 The default behavior is equivalent to setting this option to "e" and results in the output formatted as
 " %.*e". To save the disk space one can use the "g" option, e.g.
```
  WriterAscii       outputA("someoutput.hepmc");
  auto optionsA =  outputA.get_options();
  optionsA["float_printf_specifier"] = "g";
  outputA.set_options(optionsA);
```
This option will be the default on in the future.




### Links
The relations between vertices and particles in GenEventData are encoded via
  members links1 and links2, wich are std::vector<int> containing object ids.
  Direct manipulations with links1 and links2 can be useful. For instance,
  when the events are saved in ROOT format, one can extract the information
  from links1 and links2 without reading whole event.
  In case links1[i] is particle, links2[i] is end vertex. In case links1[i] is
  vertex, links2[i] is outgoing particle. An example of usage is given below.
```
  // Andrii Verbytskyi, 2017, MPI fuer Physik
  // Here is a code to look for a scattered DIS electron  in HepMC3 event record using links.
  // The implementation is extended to provide example of links1, links2 usage.
  // Dummy code.
  GenEventData*  A=...
  ...
  int i;
  int j;
  int current_l=0;                             // If the incoming electron is the first particle in the list
  int vertex_l=0;                              // We set final vertex to some nonsense value.
  bool found_next_vertex=true;
  while(found_next_vertex)                     // Looking for the electron end vertex
  {
      found_next_vertex=false;
      for (i=0; i<A->links1.size(); i++)       // Points from to ...
          if (A->links1[i]>0 &&                // The link1 should be particle, i.e. >0
              A->links1[i]==current_l+1)       // The link1 should be our electron
          {
              vertex_l=A->links2[i];           // The end vertex of this electron is found
              found_next_vertex=true;
          }
      std::vector<int> out;                    // Here we will save the outgoing particles
      if (found_next_vertex)
      {
          for (j=0; j<A->links1.size(); j++)   // Points from to ...
              if (A->links1[j]<0 &&            // The link1 should be a vertex, i.e. <0
                  A->links1[j]==vertex_l)      // The link1 should be our end vertex
                  if (std::abs(A->particles_pid[A->links2[j]-1])==11)  // If the outgoing particle is e+/e-.
                      out.push_back(A->links2[j]);
          if (out.size()==0) {
              printf("Warning: no electron in the new vertex.\n");
              break;
          }
          else
          {
              if (out.size()>1) printf("Warning: more than one electron in the new vertex.\n");
              current_l=out.at(0)-1;           // Pick up the  first electron in the list and use it as current electron.
          }
      }
      if (A->particles_status[current_l]==1) break; // The end particle is stable. It is our scattered electron.
  }
  ...
  // Here we have cuts on electron
```

## Search-related classes and interfaces
HepMC3 comes with an optional Search library for finding particles
  related to other particles or vertices.
  It provides a set of functions to perform simple search operations e.g.
  
```
  std::vector<HepMC3::GenParticlePtr>      children_particles(const HepMC3::GenVertexPtr& O);   ///< Return children particles
  std::vector<HepMC3::ConstGenVertexPtr>   grandchildren_vertices(const HepMC3::ConstGenVertexPtr& O); ///< Return grandchildren vertices
  std::vector<HepMC3::GenParticlePtr>      parent_particles(const HepMC3::GenVertexPtr& O);  ///< Return parent particles
  std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(const HepMC3::GenVertexPtr& obj);      ///< Return ancestor vertices
  
```
and interfaces for a more advanced usage. For the latter two main interfaces are defined:
  Relatives, for finding a particular type of relative, and Feature, for
  generating filters based on Features extracted from particles.
  In addition, operator on Filters are also defined.


## Relatives Interface
The Relatives interface is defined within search/include/HepMC3/Relatives.h.
  Classes that obey this interface must provide a set of operator functions
  that take either a GenParticlePtr, ConstGenParticlePtr, GenVertexPtr or
  ConstGenVertexPtr and return a vector of either GenParticlePtr or
  ConstGenParticlePtr.   Note that the versions of the operator functions that
  receive a consted input parameter also return a vector<ConstGenParticlePtr>,
  while the versions that take non-consted input return non-const output.
  This ensures consistency with the rule that consted objects may only return
  pointers to const objects.

The Relatives base class is abstract, and has a concrete implementation in
  the templated RelativesInterface class.  The RelativesInterface uses type
  erasure so that any class that obeys the defined Relatives interface
  (i.e that has the necessary four operator functions) can be wrapped in the
  RelativesInterface without needing to inherit from Relatives directly.

For example, if class foo implements the four necessary functions then the
  following will work

```
  using FooRelatives = RelativesInterface<Foo>;
  Relatives * relos = new FooRelatives();
  GenParticlePtr someInput;
  vector<GenParticlePtr> foos = (*relos)(someInput);
```

The purpose of Relatives is to be able to wrap any viable class in a common
  interface for finding relatives from a particle or vertex.  Examples are
  provided in the form of the _parents and _children classes.  These do not
  inherit from Raltives, but they do implement the necessary functions.
  The _parents and _children class are not intended to be used directly, but
  they are aliased by wrapping in the RelativesInterface:

```
  using Parents  = RelativesInterface<_parents>;
  using Children = RelativesInterface<_children>;
```

Note as well that the _parents and _children classes use some utility aliases
  to help declare the appropriately consted return type.  For example

```
  template<typename GenObject_type>
  GenParticles_type<GenObject_type> operator()(GenObject_type);
```

has a return type GenParticles_type that is a vector of GenParticlePtr that
  is consted if GenObject_type is const, but is not consted if GenObject_type
  is not consted.  Note as well the use of enable_if so that a single
  implementation can be used for both the const and non-const version of the
  functions.  For the simple case of _parents the four required funcs could
  have been implemented directly without such templating, but for more
  complicated relatives it avoids duplicated code.


## Recursive Relatives
In addition to the RelativesInterface wrapper, Relatives.h also contains a
  Recursive class that can wrap the underlying relation in recursion.  For
  example, recursion applied to the parents relationship provides all of the
  ancestors, i.e. parents repeatedly applied to the output of parents.  The
  only additional requirement to use the Recursive wrapper is that the
  underlying class must implement a vertex(GenParticlePtr) method that returns
  the appropriate vertex to follow from a given GenParticle.  As long as a
  class has such a method, it is possible to make a recursive version of it

```
  using Ancestors = RelativesInterface<Recursive<_parents> >;
```


## Existing Relatives
The Relatives class contains static implementations of the Parents,
  Children, Ancestors and Descendants relatives, which can be accessed and
  used as follows

```

  vector<const Relatives*> relos{&Relatives::PARENTS, &Relatives::ANCESTORS, &Relatives::CHILDREN, &Relatives::DESCENDANTS};
  ConstGenVertexPtr startPosition;
  // loop over different relationships.
  for(const Relatives* r: relos){
  for(auto p: r(startPosition)){
  // Do something with search result p
  }
  }

```


## Filters
A Filter is any object that has an operator that takes as input a
  ConstGenParticlePtr and returns a bool that reflects whether the input
  particle passes the filter requirements or not.  Filter is defined in
  Filter.h as

```
  using Filter = std::function<bool(ConstGenParticlePtr)>;
```

  Filter.h also contains some logical operators that allow filters to be
  combined to create new filters, for example

```
  Filter filter1, filter2, filter3;
  Filter filter4 = filter1 && filter2;
  Filter filter5 = filter3 || filter4;
  Filter filter6 = !filter1;
```

Filter.h additionally contains a dummy filter that always accepts every
  possible particle.  This may be needed in functions that require a default
  filter.  The dummy filter is accessed as

```
  Filter dummy = ACCEPT_ALL;
```

It is possible to define a Filter by hand.  However, there are some utility
  classes to define Filters based on features that can be obtained from GenParticles


## Feature Interface
The Feature interface is defined in Feature.h.  The interface is templated
  on a Feature_type that is any type that can be extracted from a GenParticle.
  This is very flexible, and the only criteria is that the Feature must have
  the set of comparison operators.  While the templated Feature is general
  enough to be used with any type of Feature, there are specialisations
  for both integral and floating point features.  The specialisations will
  cover the vast majority of Features that are likely to be useful, although
  note that Attributes may be a source of more complicated Features.

To create a Feature, one need only wrap a lambda expression in the Feature
  interface.  For example, to create a Feature based on particle status or pT:

```
  Feature<int> status([](ConstGenParticlePtr p)->int{return p->status();});
  Feature<double> pT([](ConstGenParticlePtr p)->double{return p->momentum().pt()});
```

The more general form for any type of Feature would be

```
  Feature<type> foo([](ConstGenParticlePtr p)->type{return p->foo();});
```

Having created a Feature, it can be used to create Filters for particle
  selection.  Applying operators to Features creates the Filter, which is
  a functor that evaluates on a particle.  For example

```
  ConstGenParticlePtr p;
  Filter is_stable = (status == 1);
  bool stable = is_stable(p);

  // this evaluates true if p has pT above 100.
  bool passPTCut = (pT > 100.)(p);

  // The Features can be combined
  bool combined = ((pT > 100.) && (status == 1))(p);

```

  It is also possible to make a new Feature from the absolute value of
  a previous Feature, e.g.

```
  Feature<double> rapidity([](ConstGenParticlePtr p)->double{return p->momentum().rapidity()});
  bool passes_rapCut = (abs(rapidity) < 2.5)(p);
```

Some standard features are contained within the non-templated Selector class


## Selectors and SelectorWrapper
Selector is a simplified interface that contains some predefined Features
  that can be used to search.  Selector defines comparisons operators for
  both integral and floating point types, as well as the following selection
  Features:

```
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

So, for example, a filter can be defined as follows

```
  Filter f = (Selector::STATUS == 1 && Selector::PT > 60.) || (Selector::MASS > 70. && Selector::MASS < 110.);
  GenParticlePtr p;
  bool passesCuts = f(p);
```

As with Feature, it is possible to take tbe absolute value of a Selector.
  However, note that while Featue is templated, Selector is abstract and so
  it is not possible for abs() to return a Selector object directly, only a
  pointer

```
  Filter f = *abs(Selector::RAPIDITY) < 2.5;
  bool passRapidity = f(p);
```

Note that the ATTRIBUTE selection is different from the others and does not
  have the full set of comparison operators.  This is a current limitation of
  the Attributes, which are not guaranteed to offer all comparisons.
  ATTRIBUTE takes a string, which is the name of the attribute, and permits
  the equality operator and the method exists, which checks if the attribute is
  even present

```
  string name = "MyAttribute";
  Filter f = Selector::ATTRIBUTE(name).exists() && Selector::ATTRIBUTE(name) == "My Value";
  bool passesAttribute = f(p);
```


## Applying Filters
The function applyFilter is used to apply the Filter to a set of particles.
  See for example examples/BasicExamples/basic_tree.cc

```
  for(ConstGenParticlePtr p: applyFilter( *abs(Selector::PDG_ID) <= 6, someParticles)){
  Print::line(p);
  }
```  

  
## Python Bindings
HepMC3 includes Python bindings codes suitable for compilation of python
  modules for Python3.

  The binding codes were  generated automatically using the binder utility
  version 1.4.0 created by Sergey Lyskov (Johns Hopkins University) et al..
  See
  - https://cppbinder.readthedocs.io/en/latest/ for details.

  The binding codes use the pybind11 library version 2.6.0 by Wenzel Jakob,
  EPFL's School of Computer and Communication Sciences.
  See
  - https://pybind11.readthedocs.io/en/master/
  - Wenzel Jakob and Jason Rhinelander and Dean Moldovan,
  "pybind11 -- Seamless operability between C++11 and Python", 2017,  https://github.com/pybind/pybind11

### Installation from repositories
The Python bindings together with the HepMC3 itself can be installed from PyPy and multiple other repositories.
  Please see [HepMC3 page](https://gitlab.cern.ch/hepmc/HepMC3) at CERN GitLab for details.

### Installation from sources
To turn on the compilation of bindings use -DHEPMC3_ENABLE_PYTHON = ON.
  By default the python modules will be generated for python3 if these are found in the system.
  In case the test suite is enabled, tests of python bindings with all the enabled versions will run as well.

  Despite not recommended, it should be possible to compile the python bindings using the installed version of HepMC3.
  To do this, copy the python directory outside of source tree,  uncomment #project(pyHepMC3 CXX) in  python/CMakeLists.txt and
  run cmake inside python directory with -DUSE_INSTALLED_HEPMC3=ON  option.

### Selected aspects of Python  bindings
In general, the syntax used in the python bindings is exactly the same as in the C++ code.
 However, some C++ classes and routines are don't have their Python equivalents, namsly:
 
  - The constructors of Readers/Writers with ifstreams/ostreams were not binded.
  - The multithread reader ReaderMT was not binded. 
  - The explicit readers and writers with compression support  ReaderGZ/WriterGZ were not binded. 
  It is recommended to use build-in python compression modules in combination with the  desired readers/writers.
  - The `deduce_reader` was binded and uses the internal python compression libraries, i.e. it has no dependence on zlib, zstd etc.
  The only requirement is that the corresponding module is available.
  - A limited support for the ROOTTree format I/O with the uproot ( see [uproot](https://github.com/scikit-hep/uproot5)) is offered, namely, with uproot module installed
 HepMC3 ROOTTree files can be read  with ReaderuprootTree.


#LHEF Handling Les Houches Event Files
This module contains helper classes and Reader and Writer classes
  for handling Les Houches event files - LHEF.
  
## Introduction
The Les Houches accord on an event file format (LHEF) to be used
  for passing events from a matrix element generator program (MEG)
  to an event generator program (EG) implementing parton showers,
  underlying event models, and hadronisation models etc., was not
  originally included in the HepMC event record format. But as the
  demand for more information to be included in HepMC, it was
  decided to allow HepMC to include also the original information
  from a MEG in the LHEF format (see the run attribute
  HepMC3::HEPRUPAttribute and event attribute
  HepMC3::HEPEUPAttribute). A separate /standard/ implementation in
  C++ of the LHEF format had already been maintained by Leif
  Lönnblad, and it was decided to include this (header only -
  LHEF.h) as a part of HepMC3. This will both be used in above
  mentioned HepMC3::Attribute classes and as a kind of definition of
  the LHEF format, which so far has not been extremely well
  documented. From now on these pages will serve as the defining
  information about the LHEF format.
  
## Background
The original Les Houches accord for communicating between MEGs and EGs
  was agreed upon in
  2001 [arXiv:hep-ph/0109068](http://archive.org/abs/hep-ph/0109068)
  and consisted of two simple FORTRAN common blocks. In fact this
  structure survived in the LHEF format, which was introduced in
  2006 
  [arXiv:hep-ph/0609017](http://archive.org/abs/hep-ph/0609017), and is still there after the updated
  versions 2 in 2009 
  [arXiv:1003.1643](http://archive.org/abs/1003.1643), and 3 in
  2013 
  [arXiv:1405.1067](http://archive.org/abs/1405.1067), and in the current proposal developed at the
  Les Houches workshop on TeV Colliders 2015.
  
As the methods for combining MEGs and EGs has advanced since the first
  accord, from the tree-level merging methods and NLO matching at the
  turn of the millennium, to the multi-jet (N)NLO matching and merging
  methods being perfected to day, the LHEF format has developed and a lot
  of optional information can be passed beyond the original common block
  structures. In the following all features included will be described,
  also those that were added a bit prematurely and later became
  deprecated.
  
## The basic structure
The LHEF format is based on XML, but has some oddities that goes
  beyond pure XML. As the name indicates, XML is extensible, and anyone
  writing a LHEF file can add whatever information she or he wants,
  however the following basic structure must be observed.
  
```
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
  addition to the structure is that the <tt>init</tt> and
  <tt>event</tt> (and their respective end tags) are required to be
  alone on a line, and the content of these blocks are required to
  start with a number of lines on a specific format that follows
  exactly the structure of the fortran common block in original Les
  Houches Accord. This means that the first line in the
  <tt>init</tt> block must start with a line containing the numbers
```
  IDBMUP(1) IDBMUP(2) EBMUP(1) EBMUP(2) PDFGUP(1) PDFGUP(2) PDFSUP(1) PDFSUP(2) IDWTUP NPRUP
```
  and the following <tt>NPRUP</tt> lines should be numbers on the form
```
  XSECUP(IPR) XERRUP(IPR) XMAXUP(IPR) LPRUP(IPR)
```
  where the different  variable names are defined as follows:
  <ul>
  <li>
  <tt>IDBMUP</tt>: the PDG numbers for the incoming beams;
  </li>
  <li>
  <tt>EBMUP</tt>: the energy (in GeV) of the incoming beams;
  </li>
  <li>
  <tt>PDFGUP</tt> and <tt>PDFSUP</tt>: the LHAPDF group and set
  numbers for the corresponding parton densities used;
  </li>
  <li>
  <tt>IDWTUP</tt>: the weight strategy used;
  </li>
  <li>
  <tt>NPRUP</tt> The number of different processes included in the file;
  </li>
  </ul>
  and for each process <tt>IPR</tt>:
  <ul>
  <li>
  <tt>XSECUP</tt>, <tt>XERRUP</tt>: the Monte Carlo integrated
  cross section and error estimate;
  </li>
  <li>
  <tt>XMAXUP</tt>: the overestimated cross section such that the
  sum over the individual weights in each <tt>&lt;event&gt;</tt> times
  this file times <tt>XMAXUP</tt> equals <tt>XSECUP</tt> times
  the number of events;
  </li>
  <li>
  <tt>LPRUP</tt>: a unique integer identifying the corresponding
  process.
  </li>
  </ul>
  In the LHEF::Reader and LHEF::Writer classes this information is
  available as the public <tt>heprup</tt> member of class
  LHEF::HEPRUP with public members mimicking the Fortran common
  block variables.
  
  
  
  Similarly, every <tt>event</tt> block must start with a line
  containing then numbers
```
  NUP IDPRUP XWGTUP SCALUP AQEDUP AQCDUP
```
  and the following <tt>NUP</tt> lines should be numbers describing
  each particle on the form
```
  IDUP(I) ISTUP(I) MOTHUP(1,I) MOTHUP(2,I) ICOLUP(1,I) ICOLUP(2,I) PUP(1,I) PUP(2,I) PUP(3,I) PUP(4,I) PUP(5,I) VTIMUP(I) SPINUP(I)
```
  where the different  variable names are defined as follows:
  <ul>
  <li>
  <tt>NUP</tt>: the number of particles in the event;
  </li>
  <li>
  <tt>IDPRUP</tt>: the integer identifying the process;
  </li>
  <li>
  <tt>XWGTUP</tt>: the (default) weight of the event
  </li>
  <li>
  <tt>SCALUP</tt>: the scale of the hard process in GeV;
  </li>
  <li>
  <tt>AQEDUP</tt>: the value of &alpha;<sub>EM</sub> used;
  </li>
  <li>
  <tt>AQCDUP</tt>: the value of &alpha;<sub>S</sub> used;
  </li>
  </ul>
  and for each particle <tt>I</tt>:
  <ul>
  <li>
  <tt>IDUP</tt>: the PDG code for the particle;
  </li>
  <li>
  <tt>ISTUP</tt>: the status code for the particle;
  </li>
  <li>
  <tt>MOTHUP</tt>: the line numbers of two particles considered
  to be the mothers of this particle;
  </li>
  <li>
  <tt>ICOLUP</tt>: indices of the colour and anti-colour lines connected to this particle;
  </li>
  <li>
  <tt>PUP</tt>: the <i>x</i>, <i>y</i>, <i>z</i> and energy
  component of the 4-momentum and invariant masss (in units of
  GeV);
  </li>
  <li>
  <tt>VTIMUP</tt>; the proper lifetime of this particle;
  </li>
  <li>
  <tt>SPINUP</tt>: the spin.
  </li>
  </ul>
  In the LHEF::Reader and LHEF::Writer classes this information is
  available as the public <tt>hepeup</tt> member of class
  LHEF::HEPEUP with public members mimicking the Fortran common block
  variables.
  
## LHEF-taglist Additional information
  
  Over the years several additional XML-tags has been formalised to
  specify information on top of what is given in the original Les
  Houches accord common block. These are listed below. In most cases
  the tag name corresponds to a class with a corresponding name
  available as suitably named public members in the LHEF::HEPRUP and
  LHEF::HEPEUP class respectively.
  
  Note that a tag may contain attributes in the following ways:
```
  <tag attribute1="value" attribute2="value">things marked by the tag</tag>
  <tag attribute1="value" attribute2="value" attribute3="value" />
```
  where the second case is a tag with only attributes an no contents.
  In the following an attribute may be described as required (R) or
  optional with a default value (D).
  
###  LHEF-init Standardised tags in the init block
  
  The <tt>&lt;init&gt;</tt> block contains information about the full run
  (similar to the information contained in HepMC3::GenRunInfo). The
  following tags are defined.
  
  <ul>
  <li><b><tt>&lt;generator&gt;</tt></b> (optional, multiple, see LHEF::HEPRUP::generators)<br> For easy
  access to the generator(s) used to generate this file. An
  optional attribute <tt>version</tt> can be given with a string
  containg a version string. The content of the tag can include
  any generator specific information.
  </li>
  <li><b><tt>&lt;xsecinfo&gt;</tt></b> (required, multiple, see LHEF::HEPRUP::xsecinfos)<br>
  The information in the HEPRUP common block is in principle
  sufficient to figure out the cross sections of the processes
  involved. However, the way things are specified is a bit
  confusing and complicated since it was assumed to be used to
  pass information between the MEG and PSG in both
  directions. For the event file, the communication is per
  definition one-way, and the information can be made more
  easily provided. The attributes for the xsecinfo tag are as
  follows.
  <ul>
  <li>
    <tt>weightname</tt>: in case of multiple weights for each
    event several <tt>xsecinfo</tt> tags can be given, in
    which case This should give the corresponding weight
    name. If missing This is assumed to describe the default
    weight.
  </li>
  <li>
    <tt>neve</tt> (R): the number of events in the file
  </li>
  <li>
    <tt>totxsec</tt> (R): the total cross section (in units of pb) of all processes in the file
  </li>
  <li>
    <tt>maxweight</tt> (D=1): the maximum weight of any event
    in the file (in an arbitrary unit)
  </li>
  <li>
    <tt>minweight</tt>: if the file contains negative weights,
    the minweight may contain the most negative of the
    negative weights in the file. If not given, minweight is
    assumed to be <tt>-maxweight</tt> .
  </li>
  <li>
    <tt>meanweight</tt> (D=1): The average weight of the
    events in the file (same unit as <tt>maxweight</tt> ).
  </li>
  <li>
    <tt>negweights</tt> (D=no): If yes, the file may contain
    events with negative weights.
  </li>
  <li>
    <tt>varweights</tt> (D=no): If yes, the file may contain
    varying weights (if no, all events are weighted with
  </li>
  <li>
    <tt>maxweight</tt> (or <tt>minweight</tt> )).
  </li>
  </ul>
  </li>
  <li><b><tt>&lt;cutsinfo&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::cuts)<br>
  Used by a generator to supply information about the cuts used
  in the generation. Inside this tag any number of <tt>cut</tt>
  and <tt>ptype</tt> tags can be supplied.
  </li>
  <li><b><tt>&lt;ptype&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::ptypes)<br>
  To be used inside the <tt>cutinfo</tt> tag to define a group
  of particles to which a cut can be applied. Its contents
  should be a white-space separated list of PDG particle
  codes. The only attribute is <tt>name</tt> (R): the string
  used to represent this <tt>ptype</tt> in a <tt>cut</tt> .
  </li>
  <li><b><tt>&lt;cut&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::cuts)<br>
  
  This tag has information of a particular cut used. The information
  needed is which particle(s) are affected, what variable is used
  and the maximum and/or minimum value of that parameter. The
  contents should be the minimum and maximum allowed values of this
  variable (in that order). If only one number is given, there is no
  maximum. If the minumum is larger or equal to the maximum, there
  is no minimum. The attributes are:
  <ul>
  <li>
    <tt>p1</tt> (D=0): The particles for which this cut
    applies. This can either be a number corresponding to a given
    particle PDG code or 0 for any particle or the name of a
    previously defined <tt>ptype</tt> tag.
  </li>
  <li>
    <tt>p2</tt> (D=0): If cut is between pairs of particles, this
    attribute should be non-zero. Allowed values are the same as
    for <tt>p1</tt>.
  </li>
  <li>
    <tt>type</tt> (R) This defines the variable which is cut. The
    following values are standardised (the lab frame is assumed in all
    cases):
    <ul>
    <li>"m" the invariant mass of two particles (or, if only one
    particle type is given, the invariant mass of that particle).
    </li>
    <li>"kt": the transverse momenta of a particle matching p1 (in GeV)
    </li>
    <li>"eta": the pseudorapidity of a particle matching p1
    </li>
    <li>"y": the true rapidity of a particle matching p1
    </li>
    <li>"deltaR": the pseudorapidity--azimuthal-angle difference between two particles matching p1 and p2 respectively.
    </li>
    <li>"E": the energy of a particle matching p1
    </li>
    <li>"ETmiss": the norm of the vectorial sum of the pt of particles matching p1 and not matching p2.
    </li>
    <li>"HT": the scalar sum of the transverse momentum of the particles matching p1 and not matching p2.
    </li>
    <li> other values are allowed but are not included in the standard.
    </li>
    </ul>
  </li>
  </ul>
  </li>
  <li><b><tt>&lt;procinfo&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::procinfo)<br>
  For each process number used in the LPRUP variable in the HEPEUP
  common block we can have additional information given in the
  following attributes:
  <ul>
  <li>
    <tt>iproc</tt> (D=0): The process number for which the information is given. 0 means all processes.
  </li>
  <li>
    <tt>loops</tt>: The number of loops used in calculating this process.
  </li>
  <li>
    <tt>qcdorder</tt>: The number of QCD vertices used in calculating this process.
  </li>
  <li>
    <tt>eworder</tt>: The number of electro-weak vertices used in calculating this process.
  </li>
  <li>
    <tt>rscheme</tt>: The renormalization scheme used, if applicable.
  </li>
  <li>
    <tt>fscheme</tt>: The factorization scheme used, if applicable.
  </li>
  <li>
    <tt>scheme</tt>: Information about the scheme used to
    calculate the matrix elements. If absent, a pure tree-level
    calculation is assumed. Other possible values could be
    CSdipole (NLO calculation with Catani-Seymour subtraction),
    MCatNLO, POWHEG and NLOexclusive (NLO calculation according to
    the exclusive cross section withing the given cuts).
  </li>
  </ul>
  The content of this tag is a string with arbitrary information about the process.
  </li>
  <li><b><tt>&lt;mergeinfo&gt;</tt></b>(DEPRECATED, multiple, see LHEF::HEPRUP::mergeinfo)<br>
  For some merging schemes (eg. for CKKW) it is possible to reweight the
  the events with Sudakov form factors already in the MEG. If this has
  been done the content of the mergetype tag for the corresponding
  process should give a name corresponding to the scheme used. The
  attributes are:
  <ul>
  <li>
    <tt>iproc</tt> (D=0): The process number for which the
    information is given. "0" means all processes.
  </li>
  <li>
    <tt>mergingscale</tt> (R): The value of the merging scale in GeV.
  </li>
  <li>
    <tt>maxmult</tt> (D=no): If yes the corresponding process is
    reweighted as if it is the maximum multiplicity process
    (ie. the Sudakov for the last step down to the merging scale
    is not included.
  </li>
  </ul>
  </li>
  <li><b><tt>&lt;eventfile&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::eventfiles)<br>
  Sometimes it is desirable to have the events in a separate file(s)
  from the <tt>init</tt> block, eg. when the event files become
  extremely long or when one wants to write the <tt>init</tt> block
  after having generated all events in order to collect the correct
  information from the run. The names of these files are then
  specified in <tt>eventfile</tt> tags with attributes:
  <ul>
  <li>
    <tt>name</tt> (R): the file name, interpreted as an absolute
    path if starting with "/", otherwise as a relative path to
    where the file with the <tt>init</tt> block is located;
  </li>
  <li>
    <tt>neve</tt>: the number of events in the file
  </li>
  <li>
    <tt>ntries</tt>: the number of attempts the generator needed
    to produce the <tt>neve</tt> events (for statistics purposes.
  </li>
  </ul>
  </li>
  <li><b><tt>&lt;weightinfo&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::weightinfo)<br>
  When using multiple weights for events, each weight is given an
  index and a name using the <tt>weightinfo</tt> tag. The default
  weight (as given by the LHEF:HEPEUP::XWGTUP variable) is always
  treated as index 0 and given the name "Default", while the
  additional weights are indexed by the order of
  the <tt>weightinfo</tt> tags. The attributes are:
  <ul>
  <li>
    <tt>name</tt> (R): the name of the weight (in the best of all
    worlds this would be standardised);
  </li>
  <li>
    <tt>muf</tt>: he factor multiplying the nominal factorisation
    scale of the event for the given weight;
  </li>
  <li>
    <tt>mur</tt>: he factor multiplying the nominal
    renormalisation scale of the event for the given weight;
  </li>
  <li>
    <tt>pdf</tt> (D=0): the LHAPDF code used for the given weight
    (where 0 corresponds to the default PDF of the run);
  </li>
  <li>
    <tt>pdf2</tt> (D=pdf): the LHAPDF code used for the second
    beam for the given weight;
  </li>
  </ul>
  </li>
  <li><b><tt>&lt;weightgroup&gt;</tt></b>(optional, multiple, see
  LHEF::HEPRUP::weightgroup)<br> This tag can be used to
  group <tt>weightinfo</tt> tags together.  The only well defined
  attribute is
  <tt>name</tt> giving a string which will be combined with
  the <tt>name</tt> attribute of the included <tt>weightinfo</tt>
  tags to give the HepMC weight names. Also other attributes can be
  included to include information about the weight variation used
  and is available in LHEF::WeightGroup::attributes.
  </li>
  <li><b><tt>&lt;initrwgt&gt;</tt></b>(optional, multiple, see LHEF::HEPRUP::weightinfo)<br>
  Due to an initial confusion some generators uses this tag instead
  of the <tt>weightinfo</tt> tag. It accepts the same attributes
  as <tt>weightinfo</tt>, except that the name attribute is
  named <tt>id</tt>.  Note that some generators puts these tags in
  the <tt>header</tt> block. The current implementation of
  LHEF::Reader cannot handle this. Note that this is handled by the
  same classes (LHEF::WeightInfo and LHEF::WeightGroup) in
  LHEF::Reader and LHEF::Writer.
  </li>
  </ul>
  
###  LHEF-events Standardised tags in the events block.
  
  After the <tt>init</tt> block any number of events can be given. In
  addition events can be given in separate files declared
  with <tt>eventfile</tt> tags in the <tt>init</tt> block.
  
  The main tag here is simply called <tt>event</tt> and can (for
  statistics purposes) have an attribute <tt>ntries</tt> specifying how
  many attempts the generator needed to produce the event. Also other
  attributes can be given (and will be stored in the
  LHEF::HEPEUP::attributes member variable).
  
  The <tt>event</tt> tags may be grouped together in
  a <tt>eventgroup</tt> tag. This is useful mainly for NLO generators
  that produces a (number) "real" event(s) accompanied with a number of
  "counter" events, where these events should be treated together for
  statistics purposes. For this reason the <tt>eventgroup</tt> tag can
  be provided with the optional tags <tt>nreal</tt>
  and <tt>ncounter</tt> to indicate the number of <tt>event</tt> tags
  included of each type.
  
  As indicated above the block must start with required information
  corresponding to the original Les Houches Accord Fortran common
  block. Here is a list of additional tags that may be provided.
  
  <ul>
  <li><b><tt>&lt;weights&gt;</tt></b> (optional, see LHEF::HEPEUP::weights)<br>
  This tag should contain a list of weights or the events given in
  the order they are defined by the <tt>weightinfo</tt> tags in the <tt>init</tt>
  block. If for some obscure reason a weight is absent, it should be
  anyway be included as a zero weight. Note that the weight for the
  nominal event is still given by <tt>XWGTUP</tt>. This tag has no attributes.
  </li>
  <li><b><tt>&lt;rwgt&gt;</tt></b> (optional, see LHEF::HEPEUP::weights)<br>
  For weights declared in <tt>initrwgt</tt> this should contain the
  weights for this event. The difference wrt. the <tt>weights</tt>
  tag is that each weight needs to be given in an <tt>wgt</tt> tag.
  </li>
  <li><b><tt>&lt;wgt&gt;</tt></b> (optional, see LHEF::HEPEUP::weights)<br>
  This tag contains a single weight correspoding to a weight
  declared in the <tt>initrwgt</tt> tag. The only attribute is
  the <tt>id</tt> of the weight defined in the
  corresponing <tt>weightinfo</tt> tag.
  </li>
  <li><b><tt>&lt;scales&gt;</tt></b> (optional, see LHEF::HEPEUP::scales)<br>
  Since it is often necessary to define more than one scale in an
  event, this tag is provided to specify these. The scales are given
  as the followin attributes:
  <ul>
  <li>
    <tt>muf</tt> (D=SCLAUP): the factorisation scale (in GeV);
  </li>
  <li>
    <tt>mur</tt> (D=SCLAUP): the renormalisation scale (in GeV);
  </li>
  <li>
    <tt>mups</tt> (D=SCLAUP): the suggested parton shower starting
    scale (in GeV).
  </li>
  </ul>
  Also other attributes are allowed and will be stored in
  LHEF::Scales::attributes.<br>
  </li>
  <li><b><tt>&lt;scale&gt;</tt></b> (optional inside a <tt>&lt;scales&gt;</tt>
  tag)<br> It is also possible to specify an individual scale for
  any particle in the event in a <tt>&lt;scale&gt;</tt> tag. This is
  typically used to define a starting scale for a parton shower in
  resonance decays. This tag can also be used to specify the scale
  of a set of particle types. The following attributes can be given:
  <ul>
  <li>
    <tt>pos</tt> (optional): the index of the particle for which this scale
    applies, optionally followed by a space-separated list of
    indices of recoilers involved when the particle was created.
  </li>
  <li>
    <tt>etype</tt> (optional): a space-separated list of PDG codes
    for which this scale applies. The short-hand notation "EW" can
    be used to specify all leptons and electro-weak bosons, and
    "QCD" can be used to specify the guon and all quarks.
  </li>
  </ul>
  The contents of the tag gives the scale in GeV.
  </li>
  </ul>
  <hr>

