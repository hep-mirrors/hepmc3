/**

    @mainpage Main page

    @section links Links to project-related webpages

    <a href="Https://git.cern.ch/web/hepmc3.git">HepMC3 Git repository</a> (requires login)

    <a href="https://sft.its.cern.ch/jira/browse/HEPMC"> HepMC project JIRA instance</a>

    ###########################################################################
    @section features New features
    ###########################################################################

    The following is a list of the new features available in HepMC3

    ###########################################################################
    @subsection generated_mass Check if generated mass is set
    ###########################################################################

    To determine if field generated_mass() is set use:

    @code{.cpp}
        bool is_Set = particle->is_generated_mass_set();
    @endcode

    Note that if generated_mass() is not set, call to this function
    will return momentum().m()

    ###########################################################################
    @subsection ancestors Particle as an ancestor
    ###########################################################################

    In cases where decay products originate from the decay of a single particle,
    no GenVertex has to be constructed to store this information. For example:

    @code{.cpp}
        vertex->add_particle_in(p);
        vertex->add_particle_out(p2);
        vertex->add_particle_out(p3);

        event->add_vertex(vertex);
    @endcode

    is equivalent to:

    @code{.cpp}
        p2->set_ancestor(p);
        p3->set_ancestor(p);

        event->add_particle(p);
        event->add_particle(p2);
        event->add_particle(p3);
    @endcode

    While this requires each particle to be added separately to the event,
    no GenVertex object is created using less memory then previous setup.

    @note First approach is still useful in cases where vertex position has
          to be provided

    ###########################################################################
    @subsection versioning Versioning
    ###########################################################################

    HepMC3 can record changes made to particles and vertices to keep track
    of modifications added by different tools. For example:

    @code{.cpp}
        event->add_particle(p);
        event->add_particle(p2);

        event->create_new_version("Second tool")

        p->set_generated_mass(777.0);
        events->remove_particle(p2);
    @endcode

    will indicate that in new version, generated mass of particle p is set
    to 777.0 and this version does not have particle p2. To access different
    versions of the event use:

    @code{.cpp}
        event->set_current_version(0);
        event->print();

        event->set_current_version(event->last_version());
        event->print();
    @endcode

    This will print the whole event structure before and after the changes.\n
    This works the same for all accessors related to particles and vertices,
    so calling:

    @code{.cpp}
        event->particles();
        event->vertices();
    @endcode

    will return particles and vertices as of current version. Setting
    current version number will change the returned particles/vertices.

    Following example will print all version names and their content:

    @code{.cpp}
        for(unsigned int i=0; i<event->versions().size(); ++i) {
            cout<<"Version name: "<<event->versions().name();
            event->set_current_version(i);
            event->print();
        }
    @endcode


    ###########################################################################
    @section differences Main differences between HepMC2 and HepMC3
    ###########################################################################

    The following is a list of main differences that should be taken into
    account when transitioning from HepMC2 to HepMC3.

    ###########################################################################
    @subsection ownership Ownership can no longer be transferred between events
    ###########################################################################

    Due to the addition of the versioning algorithm, once particle or vertex
    is added to the event it cannot be completely detached from this event
    or moved to any other event. Following code:

    @code{.cpp}
        event1->add_particle(p);
        event2->add_particle(p);
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
    Last update 27 March 2014
*/
