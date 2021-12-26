// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_HEPEVT_TEMPLATED_WRAPPER_H
#define HEPMC3_HEPEVT_TEMPLATED_WRAPPER_H
#include <iostream>
#include <cstdio>
#include <set>
#include <map>
#include <cstring> // memset
#include <algorithm> //min max for VS2017
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/HEPEVT_Helpers.h"

/**
 *  @file HEPEVT_Templated_Wrapper.h
 *  @brief Definition of \b class HEPEVT_Templated_Wrapper
 *
 *  @class HepMC3::HEPEVT_Templated_Wrapper
 *  @brief An interface to HEPEVT common block implemented as template class
 */
namespace HepMC3
{
/** @struct HEPEVT
 *  @brief  C structure representing Fortran common block HEPEVT
 * T. Sjöstrand et al., "A proposed standard event record", 
 *  in `Z physics at LEP 1', eds. G. Altarelli, R. Kleiss and C. Verzegnassi, 
 * Geneva, Switzerland, September 4-5, 1989, CERN 89-08 (Geneva, 1989), Vol. 3, p. 327
 * Disk representation is given by Fortran WRITE/READ format.
 */
template <int NMXHEP, typename momentum_t=double>
struct HEPEVT_Templated
{
    int        nevhep;             //!< Event number
    int        nhep;               //!< Number of entries in the event
    int        isthep[NMXHEP];     //!< Status code
    int        idhep [NMXHEP];     //!< PDG ID
    int        jmohep[NMXHEP][2];  //!< Pointer to position of 1st and 2nd (or last!) mother
    int        jdahep[NMXHEP][2];  //!< Pointer to position of 1nd and 2nd (or last!) daughter
    momentum_t phep  [NMXHEP][5];  //!< Momentum: px, py, pz, e, m
    momentum_t vhep  [NMXHEP][4];  //!< Time-space position: x, y, z, t
};  

template <int NMXHEP, typename momentum_t=double>
class HEPEVT_Templated_Wrapper
{

//
// Functions
//
public:
    /** @brief Print information from HEPEVT common block */
     void print_hepevt( std::ostream& ostr = std::cout );

    /** @brief Print particle information */
     void print_hepevt_particle( int index, std::ostream& ostr = std::cout );

    /** @brief Set all entries in HEPEVT to zero */
     void zero_everything();
    /** @brief Convert GenEvent to HEPEVT*/
     bool GenEvent_to_HEPEVT( const GenEvent* evt );
    /** @brief Convert HEPEVT to GenEvent*/
     bool HEPEVT_to_GenEvent( GenEvent* evt );
    /** @brief Tries to fix list of daughters */
     bool fix_daughters();

    //!< Fortran common block HEPEVT
     struct HEPEVT_Templated<NMXHEP,momentum_t>* m_hepevtptr;
private:
     //!< Some m_internal_storage storage. Optional. 
     std::shared_ptr<struct HEPEVT_Templated<NMXHEP,momentum_t> > m_internal_storage;
      
//
// Accessors
//
public:
     void allocate_internal_storage(); ///!< Allocates m_internal_storage storage in smart pointer to hold HEPEVT of fixed size
     void set_hepevt_address(char *c) { m_hepevtptr=(struct HEPEVT_Templated<NMXHEP,momentum_t>*)c;          } //!< Set Fortran block address
     int    max_number_entries()      { return NMXHEP;                              } //!< Block size
     int    event_number()            { return m_hepevtptr->nevhep;             } //!< Get event number
     int    number_entries()          { return m_hepevtptr->nhep;               } //!< Get number of entries
     int    status(const int& index )       { return m_hepevtptr->isthep[index-1];    } //!< Get status code
     int    id(const int& index )           { return m_hepevtptr->idhep[index-1];     } //!< Get PDG particle id
     int    first_parent(const int& index ) { return m_hepevtptr->jmohep[index-1][0]; } //!< Get index of 1st mother
     int    last_parent(const int& index )  { return m_hepevtptr->jmohep[index-1][1]; } //!< Get index of last mother
     int    first_child(const int& index )  { return m_hepevtptr->jdahep[index-1][0]; } //!< Get index of 1st daughter
     int    last_child(const int& index )   { return m_hepevtptr->jdahep[index-1][1]; } //!< Get index of last daughter
     double px(const int& index )           { return m_hepevtptr->phep[index-1][0];   } //!< Get X momentum
     double py(const int& index )           { return m_hepevtptr->phep[index-1][1];   } //!< Get Y momentum
     double pz(const int& index )           { return m_hepevtptr->phep[index-1][2];   } //!< Get Z momentum
     double e(const int& index )            { return m_hepevtptr->phep[index-1][3];   } //!< Get Energy
     double m(const int& index )            { return m_hepevtptr->phep[index-1][4];   } //!< Get generated mass
     double x(const int& index )            { return m_hepevtptr->vhep[index-1][0];   } //!< Get X Production vertex
     double y(const int& index )            { return m_hepevtptr->vhep[index-1][1];   } //!< Get Y Production vertex
     double z(const int& index )            { return m_hepevtptr->vhep[index-1][2];   } //!< Get Z Production vertex
     double t(const int& index )            { return m_hepevtptr->vhep[index-1][3];   } //!< Get production time
     int    number_parents(const int& index );                                   //!< Get number of parents
     int    number_children(const int& index );                                  //!< Get number of children from the range of daughters
     int    number_children_exact(const int& index );                            //!< Get number of children by counting

     void set_event_number( const int& evtno )       { m_hepevtptr->nevhep = evtno;         } //!< Set event number
     void set_number_entries( const int& noentries ) { m_hepevtptr->nhep = noentries;       } //!< Set number of entries
     void set_status( const int& index, const int& status ) { m_hepevtptr->isthep[index-1] = status; } //!< Set status code
     void set_id(const int& index, const int& id )         { m_hepevtptr->idhep[index-1] = id;      } //!< Set PDG particle id
     void set_parents( const int& index, const int& firstparent, const int& lastparent );              //!< Set parents
     void set_children( const int& index, const int& firstchild, const int& lastchild );               //!< Set children
     void set_momentum( const int& index, const double& px, const double& py, const double& pz, const double& e );   //!< Set 4-momentum
     void set_mass( const int& index, double mass );                                     //!< Set mass
     void set_position( const int& index, const double& x, const double& y, const double& z, const double& t );      //!< Set position in time-space
};

//
// inline definitions
//
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::print_hepevt( std::ostream& ostr )
{
    ostr << " Event No.: " << m_hepevtptr->nevhep << std::endl;
    ostr << "  Nr   Type   Parent(s)  Daughter(s)      Px       Py       Pz       E    Inv. M." << std::endl;
    for ( int i = 1; i <= m_hepevtptr->nhep; ++i )
    {
        print_hepevt_particle( i, ostr );
    }
}
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::print_hepevt_particle( int index, std::ostream& ostr )
{
    char buf[255];//Note: the format is fixed, so no reason for complicatied tratment

    sprintf(buf, "%5i %6i", index, m_hepevtptr->idhep[index-1]);
    ostr << buf;
    sprintf(buf, "%4i - %4i  ", m_hepevtptr->jmohep[index-1][0], m_hepevtptr->jmohep[index-1][1]);
    ostr << buf;
    sprintf(buf, "%4i - %4i ", m_hepevtptr->jdahep[index-1][0], m_hepevtptr->jdahep[index-1][1]);
    ostr << buf;
    // print the rest of particle info
    sprintf(buf, "%8.2f %8.2f %8.2f %8.2f %8.2f", m_hepevtptr->phep[index-1][0], m_hepevtptr->phep[index-1][1], m_hepevtptr->phep[index-1][2], m_hepevtptr->phep[index-1][3], m_hepevtptr->phep[index-1][4]);
    ostr << buf << std::endl;
}

template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::allocate_internal_storage()
{
    m_internal_storage = std::make_shared<struct HEPEVT_Templated<NMXHEP,momentum_t>>();
    m_hepevtptr = m_internal_storage.get();
}

template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::zero_everything()
{
    memset(m_hepevtptr, 0, sizeof(struct HEPEVT_Templated<NMXHEP,momentum_t>));
}

template <int NMXHEP, typename momentum_t>
inline int HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::number_parents( const int& index )
{
    return (m_hepevtptr->jmohep[index-1][0]) ? (m_hepevtptr->jmohep[index-1][1]) ? m_hepevtptr->jmohep[index-1][1]-m_hepevtptr->jmohep[index-1][0] : 1 : 0;
}

template <int NMXHEP, typename momentum_t>
inline int HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::number_children( const int& index )
{
    return (m_hepevtptr->jdahep[index-1][0]) ? (m_hepevtptr->jdahep[index-1][1]) ? m_hepevtptr->jdahep[index-1][1]-m_hepevtptr->jdahep[index-1][0] : 1 : 0;
}

template <int NMXHEP, typename momentum_t>
inline int HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::number_children_exact( const int& index )
{
    int nc = 0;
    for ( int i = 1; i <= m_hepevtptr->nhep; ++i )
        if (((m_hepevtptr->jmohep[i-1][0] <= index && m_hepevtptr->jmohep[i-1][1] >= index)) || (m_hepevtptr->jmohep[i-1][0] == index) || (m_hepevtptr->jmohep[i-1][1]==index)) nc++;
    return nc;
}

template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::set_parents( const int& index,  const int& firstparent, const int&lastparent )
{
    m_hepevtptr->jmohep[index-1][0] = firstparent;
    m_hepevtptr->jmohep[index-1][1] = lastparent;
}
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::set_children(  const int& index,  const int&  firstchild,  const int& lastchild )
{
    m_hepevtptr->jdahep[index-1][0] = firstchild;
    m_hepevtptr->jdahep[index-1][1] = lastchild;
}
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::set_momentum( const int& index, const double& px, const double& py, const double& pz, const double& e )
{
    m_hepevtptr->phep[index-1][0] = px;
    m_hepevtptr->phep[index-1][1] = py;
    m_hepevtptr->phep[index-1][2] = pz;
    m_hepevtptr->phep[index-1][3] = e;
}
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::set_mass( const int& index, double mass )
{
    m_hepevtptr->phep[index-1][4] = mass;
}
template <int NMXHEP, typename momentum_t>
inline void HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::set_position( const int& index, const double& x, const double& y, const double& z, const double& t )
{
    m_hepevtptr->vhep[index-1][0] = x;
    m_hepevtptr->vhep[index-1][1] = y;
    m_hepevtptr->vhep[index-1][2] = z;
    m_hepevtptr->vhep[index-1][3] = t;
}


template <int NMXHEP, typename momentum_t>
inline bool HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::fix_daughters()
{
    /*AV The function should be called  for a record that has correct particle ordering and mother ids.
    As a result it produces a record with ranges where the daughters can be found.
    Not every particle in the range will be a daughter. It is true only for proper events.
    The return tells if the record was fixed succesfully.
    */

    for ( int i = 1; i <= number_entries(); i++ )
        for ( int k=1; k <= number_entries(); k++ ) if (i != k)
                if ((first_parent(k) <= i) && (i <= last_parent(k)))
                    set_children(i, (first_child(i) == 0 ? k : std::min(first_child(i), k)), (last_child(i) == 0 ? k : std::max(last_child(i), k)));
    bool is_fixed = true;
    for ( int i=1; i <= number_entries(); i++ )
        is_fixed=(is_fixed && (number_children_exact(i) == number_children(i)));
    return is_fixed;
}


template <int NMXHEP, typename momentum_t>
bool HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::HEPEVT_to_GenEvent(GenEvent* evt)
{
    if ( !evt ) { std::cerr << "IO_HEPEVT::fill_next_event error  - passed null event." << std::endl; return false;}
    evt->set_event_number(event_number());
    std::map<GenParticlePtr, int > hepevt_particles;
    std::map<int, GenParticlePtr > particles_index;
    std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > > hepevt_vertices;
    std::map<int, GenVertexPtr > vertex_index;
    for ( int i = 1; i <= number_entries(); i++ )
    {
        GenParticlePtr p = std::make_shared<GenParticle>();
        p->set_momentum(FourVector(px(i), py(i), pz(i), e(i)));
        p->set_status(status(i));
        p->set_pid(id(i)); //Confusing!
        p->set_generated_mass(m(i));
        hepevt_particles[p] = i;
        particles_index[i] = p;
        GenVertexPtr v = std::make_shared<GenVertex>();
        v->set_position(FourVector(x(i), y(i), z(i), t(i)));
        v->add_particle_out(p);
        std::set<int> in;
        std::set<int> out;
        out.insert(i);
        vertex_index[i] = v;
        hepevt_vertices[v] = std::pair<std::set<int>, std::set<int> >(in, out);
    }
    /* The part above is always correct as it is a raw information without any topology.*/

    /* In this way we trust mother information. The "Trust daughters" is not implemented.*/
    for (std::map<GenParticlePtr, int >::iterator it1 = hepevt_particles.begin(); it1 != hepevt_particles.end(); ++it1)
        for (std::map<GenParticlePtr, int >::iterator it2 = hepevt_particles.begin(); it2 != hepevt_particles.end(); ++it2)
            if   (first_parent(it2->second) <= it1->second && it1->second <= last_parent(it2->second)) /*I'm you father, Luck!*/
                hepevt_vertices[it2->first->production_vertex()].first.insert(it1->second);
    /* Now all incoming sets are correct for all vertices. But we have duplicates.*/

    /* Disconnect all particles from the vertices*/
    for ( int i = 1; i <= number_entries(); i++ ) vertex_index[i]->remove_particle_out(particles_index[i]);

    /*Fill container with vertices with unique sets of incoming particles. Merge the outcoming particle sets.*/
    std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > > final_vertices_map;
    for (std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > >::iterator vs = hepevt_vertices.begin(); vs != hepevt_vertices.end(); ++vs)
    {
        if ((final_vertices_map.size() == 0) || (vs->second.first.size() == 0 && vs->second.second.size() != 0)) { final_vertices_map.insert(*vs);  continue; } /*Always insert particles out of nowhere*/
        std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > >::iterator  v2;
        for (v2 = final_vertices_map.begin(); v2 != final_vertices_map.end(); ++v2) if (vs->second.first == v2->second.first) {v2->second.second.insert(vs->second.second.begin(), vs->second.second.end()); break;}
        if (v2 == final_vertices_map.end()) final_vertices_map.insert(*vs);
    }

    std::vector<GenParticlePtr> final_particles;
    std::set<int> used;
    for (std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > >:: iterator it = final_vertices_map.begin(); it != final_vertices_map.end(); ++it)
    {
        GenVertexPtr v = it->first;
        std::set<int> in = it->second.first;
        std::set<int> out = it->second.second;
        used.insert(in.begin(), in.end());
        used.insert(out.begin(), out.end());
        for (std::set<int>::iterator el = in.begin(); el != in.end(); ++el) v->add_particle_in(particles_index[*el]);
        if (in.size() !=0 ) for (std::set<int>::iterator el = out.begin(); el != out.end(); ++el) v->add_particle_out(particles_index[*el]);
    }
    for (std::set<int>::iterator el = used.begin(); el != used.end(); ++el) final_particles.push_back(particles_index[*el]);
    /* One can put here a check on the number of particles/vertices*/

    evt->add_tree(final_particles);

    return true;
}

template <int NMXHEP, typename momentum_t>
bool HEPEVT_Templated_Wrapper<NMXHEP,momentum_t>::GenEvent_to_HEPEVT(const GenEvent* evt)
{
    /// This writes an event out to the HEPEVT common block. The daughters
    /// field is NOT filled, because it is possible to contruct graphs
    /// for which the mothers and daughters cannot both be make sequential.
    /// This is consistent with how pythia fills HEPEVT (daughters are not
    /// necessarily filled properly) and how IO_HEPEVT reads HEPEVT.
    //
    if ( !evt ) return false;

    /*AV Sorting the vertices by the lengths of their longest incoming paths assures the mothers will not go before the daughters*/
    /* Calculate all paths*/
    std::map<ConstGenVertexPtr, int> longest_paths;
    for (ConstGenVertexPtr v: evt->vertices()) calculate_longest_path_to_top(v, longest_paths);
    /* Sort paths*/
    std::vector<std::pair<ConstGenVertexPtr, int> > sorted_paths;
    std::copy(longest_paths.begin(), longest_paths.end(), std::back_inserter(sorted_paths));
    std::sort(sorted_paths.begin(), sorted_paths.end(), pair_GenVertexPtr_int_greater());

    std::vector<ConstGenParticlePtr> sorted_particles;
    std::vector<ConstGenParticlePtr> stable_particles;
    /*For a valid "Trust mothers" HEPEVT record we must  keep mothers together*/
    for (std::pair<ConstGenVertexPtr, int> it: sorted_paths)
    {
        std::vector<ConstGenParticlePtr> Q = it.first->particles_in();
        std::sort(Q.begin(), Q.end(), GenParticlePtr_greater());
        std::copy(Q.begin(), Q.end(), std::back_inserter(sorted_particles));
        /*For each vertex put all outgoing particles w/o end vertex. Ordering of particles to produces reproduceable record*/
        for (ConstGenParticlePtr pp: it.first->particles_out())
            if (!(pp->end_vertex())) stable_particles.push_back(pp);
    }
    std::sort(stable_particles.begin(), stable_particles.end(), GenParticlePtr_greater());
    std::copy(stable_particles.begin(), stable_particles.end(), std::back_inserter(sorted_particles));

    int particle_counter = std::min(int(sorted_particles.size()), max_number_entries());
    /* fill the HEPEVT event record (MD code)*/
    set_event_number(evt->event_number());
    set_number_entries(particle_counter);
    for ( int i = 1; i <= particle_counter; ++i )
    {
        set_status(i, sorted_particles[i-1]->status());
        set_id(i, sorted_particles[i-1]->pid());
        FourVector m = sorted_particles[i-1]->momentum();
        set_momentum(i, m.px(), m.py(), m.pz(), m.e());
        set_mass(i, sorted_particles[i-1]->generated_mass());
        if ( sorted_particles[i-1]->production_vertex() &&
                sorted_particles[i-1]->production_vertex()->particles_in().size())
        {
            FourVector p = sorted_particles[i-1]->production_vertex()->position();
            set_position(i, p.x(), p.y(), p.z(), p.t() );
            std::vector<int> mothers;
            mothers.clear();

            for (ConstGenParticlePtr it: sorted_particles[i-1]->production_vertex()->particles_in())
                for ( int j = 1; j <= particle_counter; ++j )
                    if (sorted_particles[j-1] == (it))
                        mothers.push_back(j);
            std::sort(mothers.begin(), mothers.end());
            if (mothers.size() == 0)
                mothers.push_back(0);
            if (mothers.size() == 1) mothers.push_back(mothers[0]);

            set_parents(i, mothers.front(), mothers.back());
        }
        else
        {
            set_position(i, 0, 0, 0, 0);
            set_parents(i, 0, 0);
        }
        set_children(i, 0, 0);
    }
    return true;
}

} // namespace HepMC3
#endif
