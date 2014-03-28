/**
    @page features New features

    The following is a list of the new features available in HepMC3

    ###########################################################################
    @section small Small enchantments
    ###########################################################################
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
    @section versioning Versioning
    ###########################################################################
    ###########################################################################
    @subsection versioning_adding Adding version tracking
    ###########################################################################

    HepMC3 can record changes made to particles and vertices to keep track
    of modifications added by different tools. For example:

    @code{.cpp}
        event->add_particle(p);
        event->add_particle(p2);

        event->create_new_version("Second tool")

        p->set_generated_mass(777.0);
        events->delete_particle(p2);
    @endcode

    will indicate that in new version, generated mass of particle p is set
    to 777.0 and this version does not have particle p2.

    ###########################################################################
    @subsection versioning_accessing Accessing different versions of the event
    ###########################################################################

    To access selected version of the event use:

    @code{.cpp}
        event->set_current_version(0);
        event->print();  // print original event

        event->set_current_version(event->last_version());
        event->print();  // print latest version of the event
    @endcode

    This works the same for all accessors related to particles and vertices,
    so calling event->particles() or event->vertices() will return particles
    and vertices as of current version.

    Following example will print all version names and their content:

    @code{.cpp}
        for(unsigned int i=0; i<event->versions().size(); ++i) {
            cout<<"Version name: "<<event->versions()[i].name();
            event->set_current_version(i);
            event->print();
        }
    @endcode

    ###########################################################################
    @subsection versioning_flattening Merging selected versions into one
    ###########################################################################

    When some of the information about the versions of the event is no longer
    needed, it can be removed from the event. This is especially useful when
    creating a final version of the event before writing to file to save disk
    space

    @code{.cpp}
        // Merge versions 3-5. The result will be stored as version 3
        // Note that this will change subsequent versions numbers
        // i.e. version 6 will now be 4, etc.
        event->merge_versions(3,5);

        // Flatten the whole event removing all version information
        event->merge_versions(0,event->last_version());
    @endcode

    ###########################################################################
    @subsection versioning_dropping Dropping versions
    ###########################################################################

    Use event->drop_last_version() to undo changes made by the last tool.
    Note that this can be used to permanently bring event back to selected
    version
    
    @code{.cpp}
        // Find version that we need
        int i=0;
        for(; i<event->versions().size(); ++i) {
            if( event->versions()[i].name() == "pythia8" ) break;
        }
        
        // Drop all versions after the version that we want to revert to
        while( event->last_version()>i ) event->drop_last_version();
    @endcode

    ###########################################################################
    @subsection versioning_writing Writing single version to file
    ###########################################################################
    
    By default, IO_GenEvent class writes all available versions of the event.
    This can be changed to write just one selected version.
    
    @code{.cpp}
        
        IO_GenEvent event("output.hepmc",ios::out);
        
        // Write all versions of the event
        event.write_event( event );
        
        // Write only the selected version of the event
        event.write_event( event,event->last_version() );
    @endcode

    <hr>
    Last update 28 March 2014
*/
