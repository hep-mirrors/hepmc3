// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_HEPEVT_WRAPPER_H
#define HEPMC3_HEPEVT_WRAPPER_H
/**
 *  @file HEPEVT_Wrapper.h
 *  @brief Definition of \b class HEPEVT_Wrapper
 *
 *  @class HepMC3::HEPEVT_Wrapper
 *  @brief An interface to HEPEVT common block implemented in a traditional way.
 *         When possible this implementation should be avoided and the templated
 *         version should be used instead.
 *
 *  @note This header file does not include HEPEVT definition, only declaration.
 *        Including this wrapper requires that HEPEVT is defined somewhere
 *        in the project (most likely as FORTRAN common block).
 *
 *  @note Make sure that HEPEVT definition in project matches this definition
 *        (NMXHEP, double precision, etc.) Change this definition if necessary.
 *
 */

#ifndef HEPMC3_HEPEVT_NMXHEP
/** Default number of particles in the HEPEVT structure */
#define HEPMC3_HEPEVT_NMXHEP 10000
#endif

#ifndef HEPMC3_HEPEVT_PRECISION
/** Default precision of the 4-momentum, time-space position and mass */
#define HEPMC3_HEPEVT_PRECISION double
#endif

/* This definition of HEPEVT corresponds to FORTRAN definition:

      PARAMETER (NMXHEP=10000)
      COMMON /HEPEVT/  NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &                 JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      INTEGER          NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      DOUBLE PRECISION PHEP,VHEP
*/

static const int NMXHEP = HEPMC3_HEPEVT_NMXHEP; //!< Number of particles in the HEPEVT structure
typedef HEPMC3_HEPEVT_PRECISION momentum_t;     //!< Precision of the 4-momentum, time-space position and mass

/** @struct HEPEVT
 *  @brief  C structure representing Fortran common block HEPEVT
 * T. Sjöstrand et al., "A proposed standard event record",
 *  in `Z physics at LEP 1', eds. G. Altarelli, R. Kleiss and C. Verzegnassi,
 * Geneva, Switzerland, September 4-5, 1989, CERN 89-08 (Geneva, 1989), Vol. 3, p. 327
 * Disk representation is given by Fortran WRITE/READ format.
 */
struct HEPEVT
{
    int        nevhep;             //!< Event number
    int        nhep;               //!< Number of entries in the event
    int        isthep[NMXHEP];     //!< Status code
    int        idhep [NMXHEP];     //!< PDG ID
    int        jmohep[NMXHEP][2];  //!< Pointer to position of 1st and 2nd (or last!) mother
    int        jdahep[NMXHEP][2];  //!< Pointer to position of 1nd and 2nd (or last!) daughter
    momentum_t phep  [NMXHEP][5];  //!< Momentum: px, py, pz, e, m
    momentum_t vhep  [NMXHEP][4];  //!< Time-space position: x, y, z, t
};                               //!< Fortran common block HEPEVT



#include <iostream>
#include <cstdio>
#include <set>
#include <map>
#include <cstring> // memset
#include <algorithm> //min max for VS2017
#ifndef HEPEVT_WRAPPER_HEADER_ONLY
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/HEPEVT_Helpers.h"
#endif

namespace HepMC3
{
/** @brief Pointer to external (e.g. in Pythia6) struct with HEPEVT */
#ifndef NO_DECLSPEC_hepevtptr
#ifdef WIN32
#ifdef HepMC3_EXPORTS
#define DECLSPEC_hepevtptr __declspec(dllexport)
#else
#define DECLSPEC_hepevtptr __declspec(dllimport)
#endif
#else
#define NO_DECLSPEC_hepevtptr
#endif
#endif

#ifdef NO_DECLSPEC_hepevtptr
/** @brief This is a pointer to HEPEVT common block */
extern struct HEPEVT*  hepevtptr;
#else
/** @brief This is a pointer to HEPEVT common block */
DECLSPEC_hepevtptr  extern struct HEPEVT*  hepevtptr;
#endif

class HEPEVT_Wrapper
{

//
// Functions
//
public:
    /** @brief Print information from HEPEVT common block */
    static void print_hepevt( std::ostream& ostr = std::cout );

    /** @brief Print particle information */
    static void print_hepevt_particle( int index, std::ostream& ostr = std::cout );

    /** @brief Check for problems with HEPEVT common block */
//!< @todo HEPEVT_Wrapper::check_hepevt_consistency is not implemented!
    /*    static bool check_hepevt_consistency( std::ostream& ostr = std::cout ); */

    /** @brief Set all entries in HEPEVT to zero */
    static void zero_everything();
#ifndef HEPEVT_WRAPPER_HEADER_ONLY
    /** @brief Convert GenEvent to HEPEVT*/
    static bool GenEvent_to_HEPEVT( const GenEvent* evt );
    /** @brief Convert HEPEVT to GenEvent*/
    static bool HEPEVT_to_GenEvent( GenEvent* evt );
#endif
    /** @brief Tries to fix list of daughters */
    static bool fix_daughters();
//
// Accessors
//
public:
    static void set_hepevt_address(char *c) { hepevtptr=(struct HEPEVT*)c;          } //!< Set Fortran block address
    static int    max_number_entries()      { return NMXHEP;                              } //!< Block size
    static int    event_number()            { return hepevtptr->nevhep;             } //!< Get event number
    static int    number_entries()          { return hepevtptr->nhep;               } //!< Get number of entries
    static int    status(const int& index )       { return hepevtptr->isthep[index-1];    } //!< Get status code
    static int    id(const int& index )           { return hepevtptr->idhep[index-1];     } //!< Get PDG particle id
    static int    first_parent(const int& index ) { return hepevtptr->jmohep[index-1][0]; } //!< Get index of 1st mother
    static int    last_parent(const int& index )  { return hepevtptr->jmohep[index-1][1]; } //!< Get index of last mother
    static int    first_child(const int& index )  { return hepevtptr->jdahep[index-1][0]; } //!< Get index of 1st daughter
    static int    last_child(const int& index )   { return hepevtptr->jdahep[index-1][1]; } //!< Get index of last daughter
    static double px(const int& index )           { return hepevtptr->phep[index-1][0];   } //!< Get X momentum
    static double py(const int& index )           { return hepevtptr->phep[index-1][1];   } //!< Get Y momentum
    static double pz(const int& index )           { return hepevtptr->phep[index-1][2];   } //!< Get Z momentum
    static double e(const int& index )            { return hepevtptr->phep[index-1][3];   } //!< Get Energy
    static double m(const int& index )            { return hepevtptr->phep[index-1][4];   } //!< Get generated mass
    static double x(const int& index )            { return hepevtptr->vhep[index-1][0];   } //!< Get X Production vertex
    static double y(const int& index )            { return hepevtptr->vhep[index-1][1];   } //!< Get Y Production vertex
    static double z(const int& index )            { return hepevtptr->vhep[index-1][2];   } //!< Get Z Production vertex
    static double t(const int& index )            { return hepevtptr->vhep[index-1][3];   } //!< Get production time
    static int    number_parents(const int& index );                                   //!< Get number of parents
    static int    number_children(const int& index );                                  //!< Get number of children from the range of daughters
    static int    number_children_exact(const int& index );                            //!< Get number of children by counting

    static void set_event_number( const int& evtno )       { hepevtptr->nevhep = evtno;         } //!< Set event number
    static void set_number_entries( const int& noentries ) { hepevtptr->nhep = noentries;       } //!< Set number of entries
    static void set_status( const int& index, const int& status ) { hepevtptr->isthep[index-1] = status; } //!< Set status code
    static void set_id(const int& index, const int& id )         { hepevtptr->idhep[index-1] = id;      } //!< Set PDG particle id
    static void set_parents( const int& index, const int& firstparent, const int& lastparent );              //!< Set parents
    static void set_children( const int& index, const int& firstchild, const int& lastchild );               //!< Set children
    static void set_momentum( const int& index, const double& px, const double& py, const double& pz, const double& e );   //!< Set 4-momentum
    static void set_mass( const int& index, double mass );                                     //!< Set mass
    static void set_position( const int& index, const double& x, const double& y, const double& z, const double& t );      //!< Set position in time-space
};

//
// inline definitions
//
inline void HEPEVT_Wrapper::print_hepevt( std::ostream& ostr )
{
    ostr << " Event No.: " << hepevtptr->nevhep << std::endl;
    ostr<< "  Nr   Type   Parent(s)  Daughter(s)      Px       Py       Pz       E    Inv. M." << std::endl;
    for ( int i=1; i<=hepevtptr->nhep; ++i )
    {
        HEPEVT_Wrapper::print_hepevt_particle( i, ostr );
    }
}

inline void HEPEVT_Wrapper::print_hepevt_particle( int index, std::ostream& ostr )
{
    char buf[255];//Note: the format is fixed, so no reason for complicatied tratment

    sprintf(buf,"%5i %6i",index,hepevtptr->idhep[index-1]);
    ostr << buf;
    sprintf(buf,"%4i - %4i  ",hepevtptr->jmohep[index-1][0],hepevtptr->jmohep[index-1][1]);
    ostr << buf;
    sprintf(buf,"%4i - %4i ",hepevtptr->jdahep[index-1][0],hepevtptr->jdahep[index-1][1]);
    ostr << buf;
    // print the rest of particle info
    sprintf(buf,"%8.2f %8.2f %8.2f %8.2f %8.2f",hepevtptr->phep[index-1][0],hepevtptr->phep[index-1][1],hepevtptr->phep[index-1][2],hepevtptr->phep[index-1][3],hepevtptr->phep[index-1][4]);
    ostr << buf << std::endl;
}

//!< @todo HEPEVT_Wrapper::check_hepevt_consistency is not implemented!
/*
inline bool HEPEVT_Wrapper::check_hepevt_consistency()
{

    printf("HEPEVT_Wrapper::check_hepevt_consistency is not implemented!\n");
    return true;
}
*/

inline void HEPEVT_Wrapper::zero_everything()
{
    memset(hepevtptr,0,sizeof(struct HEPEVT));
}

inline int HEPEVT_Wrapper::number_parents( const int& index )
{
    return (hepevtptr->jmohep[index-1][0]) ? (hepevtptr->jmohep[index-1][1]) ? hepevtptr->jmohep[index-1][1]-hepevtptr->jmohep[index-1][0] : 1 : 0;
}

inline int HEPEVT_Wrapper::number_children( const int& index )
{
    return (hepevtptr->jdahep[index-1][0]) ? (hepevtptr->jdahep[index-1][1]) ? hepevtptr->jdahep[index-1][1]-hepevtptr->jdahep[index-1][0] : 1 : 0;
}

inline int HEPEVT_Wrapper::number_children_exact( const int& index )
{
    int nc=0;
    for ( int i=1; i<=hepevtptr->nhep; ++i )
        if (((hepevtptr->jmohep[i-1][0]<=index&&hepevtptr->jmohep[i-1][1]>=index))||(hepevtptr->jmohep[i-1][0]==index)||(hepevtptr->jmohep[i-1][1]==index)) nc++;
    return nc;
}



inline void HEPEVT_Wrapper::set_parents( const int& index,  const int& firstparent, const int&lastparent )
{
    hepevtptr->jmohep[index-1][0] = firstparent;
    hepevtptr->jmohep[index-1][1] = lastparent;
}

inline void HEPEVT_Wrapper::set_children(  const int& index,  const int&  firstchild,  const int& lastchild )
{
    hepevtptr->jdahep[index-1][0] = firstchild;
    hepevtptr->jdahep[index-1][1] = lastchild;
}

inline void HEPEVT_Wrapper::set_momentum( const int& index, const double& px,const double& py, const double& pz, const double& e )
{
    hepevtptr->phep[index-1][0] = px;
    hepevtptr->phep[index-1][1] = py;
    hepevtptr->phep[index-1][2] = pz;
    hepevtptr->phep[index-1][3] = e;
}

inline void HEPEVT_Wrapper::set_mass( const int& index, double mass )
{
    hepevtptr->phep[index-1][4] = mass;
}

inline void HEPEVT_Wrapper::set_position(  const int& index, const double& x, const double& y, const double& z, const double& t )
{
    hepevtptr->vhep[index-1][0] = x;
    hepevtptr->vhep[index-1][1] = y;
    hepevtptr->vhep[index-1][2] = z;
    hepevtptr->vhep[index-1][3] = t;
}



inline bool HEPEVT_Wrapper::fix_daughters()
{
    /*AV The function should be called  for a record that has correct particle ordering and mother ids.
    As a result it produces a record with ranges where the daughters can be found.
    Not every particle in the range will be a daughter. It is true only for proper events.
    The return tells if the record was fixed succesfully.
    */

    for ( int i=1; i<=HEPEVT_Wrapper::number_entries(); i++ )
        for ( int k=1; k<=HEPEVT_Wrapper::number_entries(); k++ ) if (i!=k)
                if ((HEPEVT_Wrapper::first_parent(k)<=i)&&(i<=HEPEVT_Wrapper::last_parent(k)))
                    HEPEVT_Wrapper::set_children(i,(HEPEVT_Wrapper::first_child(i)==0?k:std::min(HEPEVT_Wrapper::first_child(i),k)),(HEPEVT_Wrapper::last_child(i)==0?k:std::max(HEPEVT_Wrapper::last_child(i),k)));
    bool is_fixed=true;
    for ( int i=1; i<=HEPEVT_Wrapper::number_entries(); i++ )
        is_fixed=(is_fixed&&(HEPEVT_Wrapper::number_children_exact(i)==HEPEVT_Wrapper::number_children(i)));
    return is_fixed;
}


#ifndef HEPEVT_WRAPPER_HEADER_ONLY

inline bool HEPEVT_Wrapper::HEPEVT_to_GenEvent(GenEvent* evt)
{
    if ( !evt ) { std::cerr << "IO_HEPEVT::fill_next_event error  - passed null event." << std::endl; return false;}
    evt->set_event_number(HEPEVT_Wrapper::event_number());
    std::map<GenParticlePtr, int > hepevt_particles;
    std::map<int, GenParticlePtr > particles_index;
    std::map<GenVertexPtr, std::pair<std::set<int>, std::set<int> > > hepevt_vertices;
    std::map<int, GenVertexPtr > vertex_index;
    for ( int i = 1; i <= HEPEVT_Wrapper::number_entries(); i++ )
    {
        GenParticlePtr p = std::make_shared<GenParticle>();
        p->set_momentum(FourVector(HEPEVT_Wrapper::px(i), HEPEVT_Wrapper::py(i), HEPEVT_Wrapper::pz(i), HEPEVT_Wrapper::e(i)));
        p->set_status(HEPEVT_Wrapper::status(i));
        p->set_pid(HEPEVT_Wrapper::id(i)); //Confusing!
        p->set_generated_mass(HEPEVT_Wrapper::m(i));
        hepevt_particles[p] = i;
        particles_index[i] = p;
        GenVertexPtr v = std::make_shared<GenVertex>();
        v->set_position(FourVector(HEPEVT_Wrapper::x(i), HEPEVT_Wrapper::y(i), HEPEVT_Wrapper::z(i), HEPEVT_Wrapper::t(i)));
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
            if   (HEPEVT_Wrapper::first_parent(it2->second) <= it1->second && it1->second <= HEPEVT_Wrapper::last_parent(it2->second)) /*I'm you father, Luck!*/
                hepevt_vertices[it2->first->production_vertex()].first.insert(it1->second);
    /* Now all incoming sets are correct for all vertices. But we have duplicates.*/

    /* Disconnect all particles from the vertices*/
    for ( int i = 1; i <= HEPEVT_Wrapper::number_entries(); i++ ) vertex_index[i]->remove_particle_out(particles_index[i]);

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


inline bool HEPEVT_Wrapper::GenEvent_to_HEPEVT(const GenEvent* evt)
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
    copy(longest_paths.begin(), longest_paths.end(), std::back_inserter(sorted_paths));
    sort(sorted_paths.begin(), sorted_paths.end(), pair_GenVertexPtr_int_greater());

    std::vector<ConstGenParticlePtr> sorted_particles;
    std::vector<ConstGenParticlePtr> stable_particles;
    /*For a valid "Trust mothers" HEPEVT record we must  keep mothers together*/
    for (std::pair<ConstGenVertexPtr, int> it: sorted_paths)
    {
        std::vector<ConstGenParticlePtr> Q = it.first->particles_in();
        sort(Q.begin(), Q.end(), GenParticlePtr_greater());
        copy(Q.begin(), Q.end(), std::back_inserter(sorted_particles));
        /*For each vertex put all outgoing particles w/o end vertex. Ordering of particles to produces reproduceable record*/
        for (ConstGenParticlePtr pp: it.first->particles_out())
            if (!(pp->end_vertex())) stable_particles.push_back(pp);
    }
    sort(stable_particles.begin(), stable_particles.end(), GenParticlePtr_greater());
    copy(stable_particles.begin(), stable_particles.end(), std::back_inserter(sorted_particles));

    int particle_counter = std::min(int(sorted_particles.size()), HEPEVT_Wrapper::max_number_entries());
    /* Fill the HEPEVT event record (MD code)*/
    HEPEVT_Wrapper::set_event_number(evt->event_number());
    HEPEVT_Wrapper::set_number_entries(particle_counter);
    for ( int i = 1; i <= particle_counter; ++i )
    {
        HEPEVT_Wrapper::set_status(i, sorted_particles[i-1]->status());
        HEPEVT_Wrapper::set_id(i, sorted_particles[i-1]->pid());
        FourVector m = sorted_particles[i-1]->momentum();
        HEPEVT_Wrapper::set_momentum(i, m.px(), m.py(), m.pz(), m.e());
        HEPEVT_Wrapper::set_mass(i, sorted_particles[i-1]->generated_mass());
        if ( sorted_particles[i-1]->production_vertex() &&
                sorted_particles[i-1]->production_vertex()->particles_in().size())
        {
            FourVector p = sorted_particles[i-1]->production_vertex()->position();
            HEPEVT_Wrapper::set_position(i, p.x(), p.y(), p.z(), p.t() );
            std::vector<int> mothers;
            mothers.clear();

            for (ConstGenParticlePtr it: sorted_particles[i-1]->production_vertex()->particles_in())
                for ( int j = 1; j <= particle_counter; ++j )
                    if (sorted_particles[j-1] == (it))
                        mothers.push_back(j);
            sort(mothers.begin(), mothers.end());
            if (mothers.size() == 0)
                mothers.push_back(0);
            if (mothers.size() == 1) mothers.push_back(mothers[0]);

            HEPEVT_Wrapper::set_parents(i, mothers.front(), mothers.back());
        }
        else
        {
            HEPEVT_Wrapper::set_position(i, 0, 0, 0, 0);
            HEPEVT_Wrapper::set_parents(i, 0, 0);
        }
        HEPEVT_Wrapper::set_children(i, 0, 0);
    }
    return true;
}
#endif

} // namespace HepMC3
#endif
