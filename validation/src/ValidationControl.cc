// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#include "ValidationControl.h"

#include "SimpleEventTool.h"
#include "FileValidationTool.h"


#ifdef HEPMC2
/* NOTE: we have to copy this code frome HepMC/Version.h
         as this header is not available when compiling
         validation program with HepMC2 */


/// Neater/extensible C++11 availability test
#if __cplusplus >= 201103L
#define HEPMC_HAS_CXX11
#endif
#if !defined(HEPMC_HAS_CXX11) && (__GNUC__) && (__cplusplus) && (__GXX_EXPERIMENTAL_CXX0X__)
#define HEPMC_HAS_CXX0X_GCC_ONLY
#endif


/// Define a FOREACH directive
#ifdef  HEPMC_HAS_CXX11
#define FOREACH( iterator, container ) for ( iterator: container )
#else
#if !defined(__CINT__)
/* This code was adapted from
 * https://github.com/assimp/assimp/blob/master/code/BoostWorkaround/boost/foreach.hpp
 * to rid of boost dependency.
 */
///////////////////////////////////////////////////////////////////////////////
// A stripped down version of FOREACH for
// illustration purposes. NOT FOR GENERAL USE.
// For a complete implementation, see BOOST_FOREACH at
// http://boost-sandbox.sourceforge.net/vault/index.php?directory=eric_niebler
//
// Copyright 2004 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Adapted to Assimp November 29th, 2008 (Alexander Gessler).
// Added code to handle both const and non-const iterators, simplified some
// parts.
//////////////////////////////////////////////////////////////////////////////
namespace hepmc_embedded_boost {
namespace foreach_detail {

///////////////////////////////////////////////////////////////////////////////
// auto_any

struct auto_any_base
{
    operator bool() const { return false; }
};

template<typename T>
struct auto_any : auto_any_base
{
    auto_any(T const& t) : item(t) {}
    mutable T item;
};

template<typename T>
T& auto_any_cast(auto_any_base const& any)
{
    return static_cast<auto_any<T> const&>(any).item;
}

///////////////////////////////////////////////////////////////////////////////
// FOREACH helper function

template<typename T>
auto_any<typename T::const_iterator> begin(T const& t)
{
    return t.begin();
}

template<typename T>
auto_any<typename T::const_iterator> end(T const& t)
{
    return t.end();
}

// iterator
template<typename T>
bool done(auto_any_base const& cur, auto_any_base const& end, T&)
{
    typedef typename T::iterator iter_type;
    return auto_any_cast<iter_type>(cur) == auto_any_cast<iter_type>(end);
}

template<typename T>
void next(auto_any_base const& cur, T&)
{
    ++auto_any_cast<typename T::iterator>(cur);
}

template<typename T>
typename T::reference deref(auto_any_base const& cur, T&)
{
    return *auto_any_cast<typename T::iterator>(cur);
}

} // end foreach_detail

///////////////////////////////////////////////////////////////////////////////
// FOREACH

#define FOREACH(item, container)                      \
	if(hepmc_embedded_boost::foreach_detail::auto_any_base const& b = hepmc_embedded_boost::foreach_detail::begin(container)) {} else       \
    if(hepmc_embedded_boost::foreach_detail::auto_any_base const& e = hepmc_embedded_boost::foreach_detail::end(container))   {} else       \
    for(;!hepmc_embedded_boost::foreach_detail::done(b,e,container);  hepmc_embedded_boost::foreach_detail::next(b,container))   \
        if (bool ugly_and_unique_break = false) {} else							\
        for(item = hepmc_embedded_boost::foreach_detail::deref(b,container); !ugly_and_unique_break; ugly_and_unique_break = true)

} // end hepmc_embedded_boost
#endif
#endif

// root not supported for HepMC2
#undef ROOTCONFIG

#endif // HEPMC2

#ifdef ROOTCONFIG
#include "RootTool.h"
#include "RootTool2.h"
#endif

#ifdef PHOTOSPP
#include "PhotosValidationTool.h"
#endif

#ifdef TAUOLAPP
#include "TauolaValidationTool.h"
#endif

#ifdef MCTESTER
#include "McTesterValidationTool.h"
#endif

#ifdef PYTHIA8
#include "PythiaValidationTool.h"
#endif

#include <fstream>
#include <cstdio>

ValidationControl::ValidationControl():
m_events(0),
m_momentum_check_events(0),
m_momentum_check_threshold(10e-6),
m_print_events(0),
m_event_counter(0),
m_status(-1),
m_timer("processing time"),
m_has_input_source(0) {
}

ValidationControl::~ValidationControl() {
    FOREACH( ValidationTool *t, m_toolchain ) {
        delete t;
    }
}

void ValidationControl::read_file(const std::string &filename) {

    // Open config file
    std::ifstream in(filename.c_str());

    if(!in.is_open()) {
        printf("ValidationControl: error reading config file: %s\n",filename.c_str());
        m_status = -1;
        return;
    }
    else printf("ValidationControl: parsing config file: %s\n",filename.c_str());

    // Parse config file
    char buf[256];
    int line = 0;

    while(!in.eof()) {
        PARSING_STATUS status = PARSING_OK;
        ++line;

        in >> buf;

        if( strlen(buf) < 3 || buf[0] == ' ' || buf[0] == '#' ) {
            in.getline(buf,255);
            continue;
        }

        // Parse event number
        if( strncmp(buf,"EVENTS",6)==0 ) {
            in>>m_events;
        }
        // Parse input source
        else if( strncmp(buf,"INPUT",5)==0 ) {
            in >> buf;

            if( m_has_input_source ) status = ADDITIONAL_INPUT;
            else {
                ValidationTool *input = NULL;
                // Use tool as input source - currently only one supported tool
                if( strncmp(buf,"tool",4)==0 ) {
                    input = new SimpleEventTool();
                }
                else if( strncmp(buf,"hepmc2",6)==0) {
                    in >> buf;

                    FileValidationTool *tool = new FileValidationTool( buf, std::ios::in );
                    if( tool->failed() ) status = CANNOT_OPEN_FILE;
                    else input = tool;
                }
                else if( strncmp(buf,"pythia8",7)==0) {
#ifdef PYTHIA8
                    in >> buf;
                    input = new PythiaValidationTool(buf);
#else
                    status = UNAVAILABLE_TOOL;
#endif
                }
                else if( strncmp(buf,"root_reader",11)==0) {
#ifdef ROOTCONFIG
                    in >> buf;
                    RootTool * tool = new RootTool(buf,ios::in);
                    if( tool->failed() ) status = CANNOT_OPEN_FILE;
                    else input = tool;
#else
                    status = UNAVAILABLE_TOOL;
#endif
                }
                else if( strncmp(buf,"root_streamer",13)==0) {
#ifdef ROOTCONFIG
                    in >> buf;
                    RootTool2 * tool = new RootTool2(buf,ios::in);
                    if( tool->failed() ) status = CANNOT_OPEN_FILE;
                    else input = tool;
#else
                    status = UNAVAILABLE_TOOL;
#endif
                }
                else status = UNRECOGNIZED_INPUT;

                if(!status) {
                    m_has_input_source = true;
                    m_toolchain.insert(m_toolchain.begin(),input);
                }
            }
        }
        // Parse tools used
        else if( strncmp(buf,"TOOL",3)==0 ) {
            in >> buf;

            if     ( strncmp(buf,"tauola",6)==0 ) {
#ifdef TAUOLAPP
                m_toolchain.push_back( new TauolaValidationTool()   );
#else
                status = UNAVAILABLE_TOOL;
#endif
            }
            else if( strncmp(buf,"photos",6)==0 ) {
#ifdef PHOTOSPP
                m_toolchain.push_back( new PhotosValidationTool()   );
#else
                status = UNAVAILABLE_TOOL;
#endif
            }
            else if( strncmp(buf,"mctester",8)==0 ) {
#ifdef MCTESTER
                m_toolchain.push_back( new McTesterValidationTool() );
#else
                status = UNAVAILABLE_TOOL;
#endif
            }
            else status = UNRECOGNIZED_TOOL;
        }
        // Parse output file
        else if( strncmp(buf,"OUTPUT",6)==0 ) {
            in >> buf;

            if( strncmp(buf,"ascii",5)==0) {
                in >> buf;
                FileValidationTool *tool = new FileValidationTool( buf, std::ios::out );
                if( tool->failed() ) status = CANNOT_OPEN_FILE;
                else m_toolchain.push_back(tool);
            }
            else if( strncmp(buf,"root_writer",11)==0) {
                in >> buf;
#ifdef ROOTCONFIG
                RootTool *tool = new RootTool( buf, std::ios::out );
                if( tool->failed() ) status = CANNOT_OPEN_FILE;
                else m_toolchain.push_back(tool);
#else
                status = UNAVAILABLE_TOOL;
#endif
            }
            else if( strncmp(buf,"root_streamer",13)==0) {
                in >> buf;
#ifdef ROOTCONFIG
                RootTool2 *tool = new RootTool2( buf, std::ios::out );
                if( tool->failed() ) status = CANNOT_OPEN_FILE;
                else m_toolchain.push_back(tool);
#else
                status = UNAVAILABLE_TOOL;
#endif
            }
            else status = UNRECOGNIZED_TOOL;
        }
        // Parse option
        else if( strncmp(buf,"SET",3)==0 ) {
            in >> buf;

            if     ( strncmp(buf,"print_events",12)==0 ) {
                in >> buf;

                int events = 0;
                if( strncmp(buf,"ALL",3)==0 ) events = -1;
                else                          events = atoi(buf);

                print_events(events);
            }
            else if( strncmp(buf,"check_momentum",14)==0 ) {
                in >> buf;

                int events = 0;
                if( strncmp(buf,"ALL",3)==0 ) events = -1;
                else                          events = atoi(buf);

                check_momentum_for_events(events);
            }
            else status = UNRECOGNIZED_OPTION;
        }
        else status = UNRECOGNIZED_COMMAND;

        // Error checking
        if(status != PARSING_OK) printf("ValidationControl: config file line %i: ",line);

        switch(status) {
            case  UNRECOGNIZED_COMMAND: printf("skipping unrecognised command:      '%s'\n",buf); break;
            case  UNRECOGNIZED_OPTION:  printf("skipping unrecognised option:       '%s'\n",buf); break;
            case  UNRECOGNIZED_INPUT:   printf("skipping unrecognised input source: '%s'\n",buf); break;
            case  UNRECOGNIZED_TOOL:    printf("skipping unrecognised tool:         '%s'\n",buf); break;
            case  UNAVAILABLE_TOOL:     printf("skipping unavailable tool:          '%s'\n",buf); break;
            case  ADDITIONAL_INPUT:     printf("skipping additional input source:   '%s'\n",buf); break;
            case  CANNOT_OPEN_FILE:     printf("skipping input file:                '%s'\n",buf); break;
            default: break;
        }

        // Ignore rest of the line
        in.getline(buf,255);
    }

    // Having input source is enough to start validation
    if(m_has_input_source) m_status = 0;
    else printf("ValidationControl: no valid input source\n");
}

bool ValidationControl::new_event() {
    if( m_status ) return false;
    if( m_events && ( m_event_counter >= m_events ) ) return false;

    if(m_event_counter) {
        if( m_momentum_check_events>0 ) --m_momentum_check_events;
        if( m_print_events>0 )          --m_print_events;
    }
    else m_timer.start();

    ++m_event_counter;

    if( m_events ) {
        if( m_event_counter == 1 ) {
            printf("ValidationControl: event       1 of %-7i\n",m_events);
            m_events_print_step = m_events/10;
        }
        else if( m_event_counter%m_events_print_step == 0 ) {
            int elapsed = m_timer.elapsed_time();
            m_timer.stop();
            int total   = m_timer.total_time();
            printf("ValidationControl: event %7i (%6.2f%%, %7ims current, %7ims total)\n",m_event_counter,m_event_counter*100./m_events,elapsed,total);
            m_timer.start();
        }
    }
    else {
        if( m_event_counter == 1 ) {
            printf("ValidationControl: event       1\n");
            m_events_print_step = 1000;
        }
        else if( m_event_counter%m_events_print_step == 0 ) {
            int elapsed = m_timer.elapsed_time();
            m_timer.stop();
            int total   = m_timer.total_time();
            printf("ValidationControl: event %7i (%6ims current, %7ims total)\n",m_event_counter,elapsed,total);
            m_timer.start();
        }
    }

    return true;
}

void ValidationControl::initialize() {
    printf("ValidationControl: initializing\n");

    FOREACH( ValidationTool *tool, m_toolchain ) {
        tool->initialize();
    }
}

void ValidationControl::process(GenEvent &hepmc) {

    m_status = 0;

    FourVector input_momentum;

    FOREACH( ValidationTool *tool, m_toolchain ) {

        Timer *timer = tool->timer();

        if(timer) timer->start();
        m_status = tool->process(hepmc);
        if(timer) timer->stop();

        // status != 0 means an error - stop processing current event
        if(m_status) return;

        if(tool->tool_modifies_event() && m_print_events) {
            printf("--------------------------------------------------------------\n");
            printf("   Print event: %s\n",tool->name().c_str());
            printf("--------------------------------------------------------------\n");

            HEPMC2CODE( hepmc.print();           )
            HEPMC3CODE( Print::listing(hepmc,8); )
        }

        if(tool->tool_modifies_event() && m_momentum_check_events ) {
            FourVector sum;
            double     delta = 0.0;

            HEPMC2CODE(
                for ( GenEvent::particle_const_iterator p = hepmc.particles_begin();
                                                        p != hepmc.particles_end();  ++p ) {
                    if( (*p)->status() != 1 ) continue;
                    //(*p)->print();
                    FourVector m = (*p)->momentum();
                    sum.setPx( sum.px() + m.px() );
                    sum.setPy( sum.py() + m.py() );
                    sum.setPz( sum.pz() + m.pz() );
                    sum.setE ( sum.e()  + m.e()  );
                }

                double momentum = input_momentum.px() + input_momentum.py() + input_momentum.pz() + input_momentum.e();
                if( fabs(momentum) > 10e-12 ) {
                    double px = input_momentum.px() - sum.px();
                    double py = input_momentum.py() - sum.py();
                    double pz = input_momentum.pz() - sum.pz();
                    double e  = input_momentum.e()  - sum.e();
                    delta = sqrt(px*px + py*py + pz*pz + e*e);
                }
            )
            HEPMC3CODE(
                FindParticles search( hepmc, FIND_ALL, STATUS == 1 );
                //hepmc.set_print_precision(8);

                FOREACH( const GenParticlePtr &p, search.results() ) {
                    //p->print();
                    sum += p->momentum();
                }

                if(!input_momentum.is_zero()) delta = (input_momentum - sum).length();
            )

            printf("Momentum sum: %+15.8e %+15.8e %+15.8e %+15.8e (evt: %7i, %s)",sum.px(),sum.py(),sum.pz(),sum.e(),m_event_counter,tool->name().c_str());

            if( delta < m_momentum_check_threshold ) printf("\n");
            else                                     printf(" - WARNING! Difference = %+15.8e\n",delta);

            input_momentum = sum;
        }
    }
}

void ValidationControl::finalize() {
    printf("ValidationControl: finalizing\n");

    // Finalize
    FOREACH( ValidationTool *tool, m_toolchain ) {
        tool->finalize();
    }

    printf("ValidationControl: printing timers\n");

    // Print timers
    FOREACH( ValidationTool *tool, m_toolchain ) {
        if(tool->timer()) tool->timer()->print();
    }

    printf("ValidationControl: finished processing:\n");

    // List tools
    FOREACH( ValidationTool *tool, m_toolchain ) {
        printf("  tool: %s\n",tool->long_name().c_str());
    }
}
