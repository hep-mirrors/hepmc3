/**
    @page differences Differences between HepMC2 and HepMC3

    The following is a list of main differences that should be taken into
    account when transitioning from HepMC2 to HepMC3.

    ###########################################################################
    @section interface Changes to the interface
    ###########################################################################
    ###########################################################################
    @subsection deleting Function GenEvent::remove_particle() has been replaced
                         by GenEvent::delete_particle() with different meaning
    ###########################################################################

    Particles (and vertices) cannot be permanently detached from the event.
    Deleting a particle or vertex tags them as being deleted but keeps them
    in the event.

    @code{.cpp}
        GenParticle *p = new GenParticle();

        event->add_particle(p);
        event->delete_particle(p); // tags particle as deleted

        cout<<p->generated_mass()<<endl; // still valid as the pointer is valid

        p->set_generated_mass(777.0); // will throw a warning because particle
                                      // is marked as deleted
    @endcode

    ###########################################################################
    @section technical Technical changes
    ###########################################################################
    ###########################################################################
    @subsection barcodes Barcodes can no longer be set for particles/vertices
                         added to an event
    ###########################################################################

    Barcodes are now used solely by GenEvent. They can be set only for particle
    or vertex that does not belong to an event. Adding particle or vertex
    to the event will change the barcode.

    @code{.cpp}
        GenParticle *p = new GenParticle();

        p->set_barcode(10001);    // can be done but should not be used
        cout<<p->barcode()<<endl; // will print out '133'

        event->add_particle(p);
        cout<<p->barcode()<<endl; // will print out '1'

        p->set_barcode(10001);    // will be ignored because particle already
                                  // belongs to an event
    @endcode

    ###########################################################################
    @subsection ownership Ownership can no longer be transferred between events
    ###########################################################################

    Due to the addition of the versioning algorithm, once particle or vertex
    is added to the event it cannot be completely detached from this event
    or moved to any other event.

    @code{.cpp}
        event1->add_particle(p);
        event2->add_particle(p); // will be ignored as particle p already
                                 // belongs to an event
    @endcode

    will return an error and particle p will not be added to event2.\n
    To add it to event2, create a copy of this particle:

    @code{.cpp}
        GenParticle *p2 = new GenParticle(*p);
        event2->add_particle(p2);
    @endcode

    In case of vertices, since copy constructor by default creates a shallow
    copy, one has to create a deep copy using:

    @code{.cpp}
        GenVertex *v2 = v1->create_deep_copy();
        event2->add_vertex(v2);
    @endcode

    ###########################################################################
    @subsection topological_order Topological order
    ###########################################################################

    Particles and vertices in HepMC3 are stored in topological order. This means
    that if a particle or vertex have an ancestor, this ancestor must already
    exist in the event. Example:

    @code{.cpp}
        GenParticle *p  = new GenParticle();
        GenParticle *p2 = new GenParticle();

        p2->set_ancestor(p);

        event->add_particle(p2); // will cause an error

        event->add_particle(p);  // OK because p has no ancestors
        event->add_particle(p2); // OK because p already belongs to the event
    @endcode

    <hr>
    Last update 28 March 2014
*/
