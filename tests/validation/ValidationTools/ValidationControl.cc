#include "ValidationControl.h"

#include "FileValidationTool.h"
#include "SimpleEventTool.h"
#include "PythiaValidationTool.h"
#include "PhotosValidationTool.h"
#include "TauolaValidationTool.h"
#include "McTesterValidationTool.h"

#include <fstream>
#include <cstdio>
#include <boost/foreach.hpp>

ValidationControl::ValidationControl():
m_events(0),
m_momentum_check_events(0),
m_print_events(0),
m_event_counter(0),
m_status(-1),
m_has_input_source(0) {
}

ValidationControl::~ValidationControl() {
    BOOST_FOREACH( ValidationTool *t, m_toolchain ) {
        delete t;
    }
}

void ValidationControl::read_file(const std::string &filename) {

    // Open config file
    ifstream in(filename.c_str());

    if(!in.is_open()) {
        printf("ValidationControl: error reading file %s.\n",filename.c_str());
        m_status = -1;
        return;
    }

    // Parse config file
    char buf[256];
    int line = 0;

    while(!in.eof()) {
        int status = 0;
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
            if( m_has_input_source ) status = -2;
            else {
                in >> buf;

                // Use tool as input source - currently only one supported tool
                if( strncmp(buf,"tool",4)==0 ) {
                    m_toolchain.push_back( new SimpleEventTool() );
                }
                else if( strncmp(buf,"hepmc2",4)==0) {
                    in >> buf;

                    FileValidationTool *tool = new FileValidationTool( buf, std::ios::in );
                    if( tool->rdstate() ) status = -3;
                    else m_toolchain.push_back(tool);
                }
                else if( strncmp(buf,"pythia8",7)==0) {
                    in >> buf;
                    m_toolchain.push_back( new PythiaValidationTool(buf) );
                }
                else status = 2;
                
                if(!status) m_has_input_source = true;
            }
        }
        // Parse tools used
        else if( strncmp(buf,"TOOL",3)==0 ) {
            if( !m_has_input_source ) status = -1;
            else {
                in >> buf;

                if     ( strncmp(buf,"tauola",6)==0 )   m_toolchain.push_back( new TauolaValidationTool()   );
                else if( strncmp(buf,"photos",6)==0 )   m_toolchain.push_back( new PhotosValidationTool()   );
                else if( strncmp(buf,"mctester",8)==0 ) m_toolchain.push_back( new McTesterValidationTool() );
                else status = 3;
            }
        }
        // Parse output file
        else if( strncmp(buf,"OUTPUT",6)==0 ) {
            if( !m_has_input_source ) status = -1;
            else {
                in >> buf;

                FileValidationTool *tool = new FileValidationTool( buf, std::ios::out );
                if( tool->rdstate() ) status = -3;
                else m_toolchain.push_back(tool);
            }
        }
        else status = 1;

        // Error checking
        if(status) printf("ValidationControl config file line %i: ",line);

        switch(status) {
            case  1: printf("skipping unrecognised command:      '%s'\n",buf); break;
            case  2: printf("skipping unrecognised input source: '%s'\n",buf); break;
            case  3: printf("skipping unrecognised tool:         '%s'\n",buf); break;
            case -1:
                printf("add input source first!\n");
                m_status = -1;
                return;
            case -2:
                printf("cannot add more than one input source!\n");
                m_status = -1;
                return;
            case -3:
                printf("could not open file: '%s'\n",buf);
                m_status = -1;
                return;
            default: break;
        }

        // Ignore rest of the line
        in.getline(buf,255);
    }

    // Having input source is enough to start validation
    if(m_has_input_source) m_status = 0;
}

bool ValidationControl::new_event() {
    if( m_status ) return false;
    if( m_events && ( m_event_counter >= m_events ) ) return false;

    ++m_event_counter;

    if( m_event_counter%1000 == 0) {
        if( !m_events ) printf("Event: %7i\n",m_event_counter);
        else            printf("Event: %7i (%6.2f%%)\n",m_event_counter,m_event_counter*100./m_events);
    }

    if( m_momentum_check_events ) {
        printf("\n");
        --m_momentum_check_events;
    }

    if( m_print_events ) --m_print_events;

    return true;
}

void ValidationControl::initialize() {
    BOOST_FOREACH( ValidationTool *t, m_toolchain ) {
        t->initialize();
    }
}

void ValidationControl::process(GenEvent &hepmc) {
    m_status = 0;
    BOOST_FOREACH( ValidationTool *t, m_toolchain ) {
        m_status = t->process(hepmc);

        // status != 0 means an error - stop processing current event
        if(m_status) return;

        if(t->tool_modifies_event() && m_print_events) {
            printf("--------------------------------------------------------------\n");
            printf("   Print event: %s\n",t->name().c_str());
            printf("--------------------------------------------------------------\n");

            HEPMC3CODE( hepmc.set_print_precision(8); )
            hepmc.print();
        }

        if(t->tool_modifies_event() && m_momentum_check_events ) {
            FourVector sum;

            FindParticles search( hepmc, FIND_ALL, STATUS == 1 && VERSION_DELETED > hepmc.last_version());

            BOOST_FOREACH( GenParticle *p, search.results() ) {
                sum += p->momentum();
            }

            printf("Vector sum: %+15.8e %+15.8e %+15.8e %+15.8e (evt: %7i, %s)\n",sum.px(),sum.py(),sum.pz(),sum.e(),m_event_counter,t->name().c_str());
        }
    }
}

void ValidationControl::finalize() {
    BOOST_FOREACH( ValidationTool *t, m_toolchain ) {
        t->finalize();
    }
    BOOST_FOREACH( ValidationTool *t, m_toolchain ) {
        printf(" Finished processing: %s\n",t->name().c_str());
    }
}
