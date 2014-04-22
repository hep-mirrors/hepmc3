#include "Pythia8/Pythia8ToHepMC3.h"
#include "HepMC3/GenEvent.h"

#include <deque>
#include <cassert>

namespace HepMC3 {

/** What is not in current HepMC3 implementation:
 *  - units
 *  - color flow (will probably be removed altogether)
 *  - beam particles
 *  - PDF
 *  - process code, scale, alpha_em, alpha_s
 *  - weight, cross section
 */
bool Pythia8ToHepMC3::fill_next_event( Pythia8::Event& pyev, GenEvent* evt, int ievnum, Pythia8::Info* pyinfo, Pythia8::Settings* pyset) {

    // 1. Error if no event passed.
    if (!evt) {
        std::cerr << "Pythia8ToHepMC::fill_next_event error - passed null event."
                  << std::endl;
        return 0;
    }

    // Event number counter.
    if ( ievnum >= 0 ) {
        evt->set_event_number(ievnum);
        m_internal_event_number = ievnum;
    }
    else {
        evt->set_event_number(m_internal_event_number);
        ++m_internal_event_number;
    }
/*
    // Conversion factors from Pythia units GeV and mm to HepMC ones.
    double momFac = HepMC::Units::conversion_factor(HepMC::Units::GEV,evt->momentum_unit());
    double lenFac = HepMC::Units::conversion_factor(HepMC::Units::MM,evt->length_unit());
*/
    double momFac = 1.;
    double lenFac = 1.;

    // 2. Fill particle information
    // 3. Fill vertex information
    // Create a particle instance for each that belongs to a decay tree and
    // add these particles to HepMC3 event in topological order. Vertices will
    // be created along the way and filled with mother/daughter information
    // NOTE: 'hepevt_particles' stores pointers to GenParticles corresponding
    //       to pyev Particles. This pointer is NULL for Particles not added
    //       to the event

    std::vector<GenParticle*> hepevt_particles( pyev.size() );
    std::deque<int>           particle_order;

    // Here we assume that the first two particles are the incoming beam particles
    particle_order.push_back(1);
    particle_order.push_back(2);

    // Add particles in topological order
    while( !particle_order.empty() ) {

        int i = particle_order.front();

        bool added_to_front = false;

        // Add mothers to the front of the list
        std::vector<int> mothers = pyev.motherList(i);
        for(unsigned int j=0; j<mothers.size(); ++j) {
            if(!mothers[j] || hepevt_particles[mothers[j]]) continue;
            added_to_front = true;
            particle_order.push_front(mothers[j]);
        }

        // If we have added at least one mother, our particle is not the first particle on the list
        if( added_to_front ) continue;

        particle_order.pop_front();

        // If this particle has not yet been added to the event
        if( hepevt_particles[i] == NULL ) {

            // Fill the particle
            hepevt_particles[i] = &evt->new_particle(
                                   FourVector( momFac * pyev[i].px(), momFac * pyev[i].py(),
                                   momFac * pyev[i].pz(), momFac * pyev[i].e()  ),
                                   pyev[i].id(), pyev.statusHepMC(i) );

/*
            hepevt_particles[i]->suggest_barcode(i);
*/
            hepevt_particles[i]->set_generated_mass( momFac * pyev[i].m() );

/*
            // Colour flow uses index 1 and 2.
            int colType = pyev[i].colType();
            if (colType ==  1 || colType == 2)
                hepevt_particles[i]->set_flow(1, pyev[i].col());
            if (colType == -1 || colType == 2)
                hepevt_particles[i]->set_flow(2, pyev[i].acol());
*/
            FourVector prod_pos( lenFac * pyev[i].xProd(), lenFac * pyev[i].yProd(), lenFac * pyev[i].zProd(), lenFac * pyev[i].tProd() );

            bool has_position = !prod_pos.is_zero();

            // Check if we need a production vertex
            if( mothers.size() || has_position ) {

                GenVertex *prod_vtx = NULL;
                if( mothers.size() ) prod_vtx = hepevt_particles[mothers[0]]->end_vertex();

                // No existing production vertex - create one
                if(!prod_vtx) {
                    prod_vtx = &evt->new_vertex();

                    for(unsigned int j=0; j<mothers.size(); ++j) {
                        // NOTE: if sorting algorithm is correct all mothers
                        //       of any particle already exist in the event
                        assert( hepevt_particles[mothers[j]] );

                        prod_vtx->add_particle_in( *hepevt_particles[mothers[j]] );
                    }

                    if(has_position) prod_vtx->set_position(prod_pos);
                }
                // Production vertex exist - update its position if necessary
                else if( has_position && !prod_vtx->position().is_zero()) {
                    prod_vtx->set_position(prod_pos);
                }

                prod_vtx->add_particle_out( *hepevt_particles[i] );
            }

            // Add daughters to the end of the list
            std::vector<int> daughters = pyev[i].daughterList();
            for(unsigned int j=0; j<daughters.size(); ++j) {
                if(!daughters[j] || hepevt_particles[daughters[j]]) continue;
                particle_order.push_back(daughters[j]);
            }
        }
    }

/*
    // Here we assume that the first two particles in the list
    // are the incoming beam particles.
    evt->set_beam_particles( hepevt_particles[1], hepevt_particles[2] );
*/

    // If hadronization switched on then no final coloured particles.
    bool doHadr = (pyset == 0) ? m_free_parton_warnings : pyset->flag("HadronLevel:all") && pyset->flag("HadronLevel:Hadronize");

    // 4. Check for particles which come from nowhere, i.e. are without
    // mothers or daughters. These need to be attached to a vertex, or else
    // they will never become part of the event.
    for (int i = 1; i < pyev.size(); ++i) {

        // Check for particles not added to the event
        // NOTE: We have to check if this step makes any sense in HepMC3 event standard
        if ( !hepevt_particles[i] ) {
            std::cerr << "hanging particle " << i << std::endl;
            hepevt_particles[i] = &evt->new_particle(
                                   FourVector( momFac * pyev[i].px(), momFac * pyev[i].py(),
                                   momFac * pyev[i].pz(), momFac * pyev[i].e()  ),
                                   pyev[i].id(), pyev.statusHepMC(i) );

            GenVertex *prod_vtx = &evt->new_vertex();

            prod_vtx->add_particle_out( *hepevt_particles[i] );
        }

        // Also check for free partons (= gluons and quarks; not diquarks?).
        if ( doHadr && m_free_parton_warnings ) {
            if ( hepevt_particles[i]->pdg_id() == 21 && !hepevt_particles[i]->end_vertex() ) {
                std::cerr << "gluon without end vertex " << i << std::endl;
                if ( m_crash_on_problem ) exit(1);
            }
            if ( abs(hepevt_particles[i]->pdg_id()) <= 6 && !hepevt_particles[i]->end_vertex() ) {
                std::cerr << "quark without end vertex " << i << std::endl;
                if ( m_crash_on_problem ) exit(1);
            }
        }
    }

/*
    // 5. Store PDF, weight, cross section and other event information.
    // Flavours of incoming partons.
    if (m_store_pdf && pyinfo != 0) {
        int id1pdf = pyinfo->id1pdf();
        int id2pdf = pyinfo->id2pdf();
        if ( m_convert_gluon_to_0 ) {
            if (id1pdf == 21) id1pdf = 0;
            if (id2pdf == 21) id2pdf = 0;
        }

        // Store PDF information.
        evt->set_pdf_info( PdfInfo( id1pdf, id2pdf, pyinfo->x1pdf(),
        pyinfo->x2pdf(), pyinfo->QFac(), pyinfo->pdf1(), pyinfo->pdf2() ) );
    }

    // Store process code, scale, alpha_em, alpha_s.
    if (m_store_proc && pyinfo != 0) {
        evt->set_signal_process_id( pyinfo->code() );
        evt->set_event_scale( pyinfo->QRen() );
        if (evt->alphaQED() <= 0) evt->set_alphaQED( pyinfo->alphaEM() );
        if (evt->alphaQCD() <= 0) evt->set_alphaQCD( pyinfo->alphaS() );
    }

    // Store cross-section information in pb and event weight. The latter is
    // usually dimensionless, but in units of pb for Les Houches strategies +-4.
    if (m_store_xsec && pyinfo != 0) {
        HepMC::GenCrossSection xsec;
        xsec.set_cross_section( pyinfo->sigmaGen() * 1e9,
        pyinfo->sigmaErr() * 1e9);
        evt->set_cross_section(xsec);
        evt->weights().push_back( pyinfo->weight() );
    }
*/
    // Done.
    return true;
}

} // end namespace HepMC
