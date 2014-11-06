#ifndef HEPMC_HEPEVT_WRAPPER_H
#define HEPMC_HEPEVT_WRAPPER_H
/**
 *  @file HEPEVT_Wrapper.h
 *  @brief Definition of \b class HEPEVT_Wrapper
 *
 *  @class HepMC::HEPEVT_Wrapper
 *  @brief An interface to HEPEVT common block
 *
 *  @note This header file does not include HEPEVT definition, only declaration.
 *        Including this wrapper requires that HEPEVT is defined somewhere
 *        in the project (most likely as FORTRAN common block).
 *
 *  @note Make sure that HEPEVT definition in project matches this definition
 *        (NMXHEP, double precision, etc.) Change this definition if necessary.
 *
 *  @todo Do we just make write_event and fill_next_event instead?
 */

#ifndef HEPMC_HEPEVT_NMXHEP
/** Default number of particles in the HEPEVT structure */
#define HEPMC_HEPEVT_NMXHEP 10000
#endif

#ifndef HEPMC_HEPEVT_PRECISION
/** Default precision of the 4-momentum, time-space position and mass */
#define HEPMC_HEPEVT_PRECISION double
#endif

/* This definition of HEPEVT corresponds to FORTRAN definition:

      PARAMETER (NMXHEP=10000)
      COMMON /HEPEVT/  NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &                 JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      INTEGER          NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      DOUBLE PRECISION PHEP,VHEP
*/

static const int NMXHEP = HEPMC_HEPEVT_NMXHEP; //!< Number of particles in the HEPEVT structure
typedef HEPMC_HEPEVT_PRECISION momentum_t;     //!< Precision of the 4-momentum, time-space position and mass

/** @struct HEPEVT
 *  @brief  C structure representing Fortran common block HEPEVT
 */
extern "C" struct HEPEVT
{
  int        nevhep;             //!< Event number
  int        nhep;               //!< Number of entries in the event
  int        isthep[NMXHEP];     //!< Status code
  int        idhep [NMXHEP];     //!< PDG ID
  int        jmohep[NMXHEP][2];  //!< Pointer to position of 1st and 2nd (or last!) mother
  int        jdahep[NMXHEP][2];  //!< Pointer to position of 1nd and 2nd (or last!) daughter
  momentum_t phep  [NMXHEP][5];  //!< Momentum: px, py, pz, e, m
  momentum_t vhep  [NMXHEP][4];  //!< Time-space position: x, y, z, t
} hepevt_;                       //!< Fortran common block HEPEVT

#include <iostream>
#include <cstdio>
#include <cstring> // memset
using std::endl;

namespace HepMC {

class HEPEVT_Wrapper {

//
// Functions
//
public:
    /** @brief Print information from HEPEVT common block */
    static void print_hepevt( std::ostream& ostr = std::cout );

    /** @brief Print particle information */
    static void print_hepevt_particle( int index, std::ostream& ostr = std::cout );

    /** @brief Check for problems with HEPEVT common block */
    static bool check_hepevt_consistency( std::ostream& ostr = std::cout );

    /** @brief Set all entries in HEPEVT to zero */
    static void zero_everything();
//
// Accessors
//
public:
    static int    event_number()            { return hepevt_.nevhep;             } //!< Get event number
    static int    number_entries()          { return hepevt_.nhep;               } //!< Get number of entries
    static int    status( int index )       { return hepevt_.isthep[index-1];    } //!< Get status code
    static int    id( int index )           { return hepevt_.idhep[index-1];     } //!< Get PDG particle id
    static int    first_parent( int index ) { return hepevt_.jmohep[index-1][0]; } //!< Get index of 1st mother
    static int    last_parent( int index )  { return hepevt_.jmohep[index-1][1]; } //!< Get index of last mother
    static int    first_child( int index )  { return hepevt_.jdahep[index-1][0]; } //!< Get index of 1st daughter
    static int    last_child( int index )   { return hepevt_.jdahep[index-1][1]; } //!< Get index of last daughter
    static double px( int index )           { return hepevt_.phep[index-1][0];   } //!< Get X momentum
    static double py( int index )           { return hepevt_.phep[index-1][1];   } //!< Get Y momentum
    static double pz( int index )           { return hepevt_.phep[index-1][2];   } //!< Get Z momentum
    static double e( int index )            { return hepevt_.phep[index-1][3];   } //!< Get Energy
    static double m( int index )            { return hepevt_.phep[index-1][4];   } //!< Get generated mass
    static double x( int index )            { return hepevt_.phep[index-1][0];   } //!< Get X Production vertex
    static double y( int index )            { return hepevt_.phep[index-1][1];   } //!< Get Y Production vertex
    static double z( int index )            { return hepevt_.phep[index-1][2];   } //!< Get Z Production vertex
    static double t( int index )            { return hepevt_.phep[index-1][3];   } //!< Get production time
    static int    number_parents( int index );                                   //!< Get number of parents
    static int    number_children( int index );                                  //!< Get number of children

    static void set_event_number( int evtno )       { hepevt_.nevhep = evtno;         } //!< Set event number
    static void set_number_entries( int noentries ) { hepevt_.nhep = noentries;       } //!< Set number of entries
    static void set_status( int index, int status ) { hepevt_.isthep[index-1] = status; } //!< Set status code
    static void set_id( int index, int id )         { hepevt_.idhep[index-1] = id;      } //!< Set PDG particle id
    static void set_parents( int index, int firstparent, int lastparent );              //!< Set parents
    static void set_children( int index, int firstchild, int lastchild );               //!< Set children
    static void set_momentum( int index, double px, double py, double pz, double e );   //!< Set 4-momentum
    static void set_mass( int index, double mass );                                     //!< Set mass
    static void set_position( int index, double x, double y, double z, double t );      //!< Set position in time-space
};

//
// inline definitions
//
inline void HEPEVT_Wrapper::print_hepevt( std::ostream& ostr ) {
    ostr << " Event No.: " << hepevt_.nevhep << endl;
    ostr<< "  Nr   Type   Parent(s)  Daughter(s)      Px       Py       Pz       E    Inv. M." << endl;
    for( int i=1; i<=hepevt_.nhep; ++i ) {
        HEPEVT_Wrapper::print_hepevt_particle( i, ostr );
    }
}

inline void HEPEVT_Wrapper::print_hepevt_particle( int index, std::ostream& ostr ) {
    char buf[255];

    sprintf(buf,"%5i %6i",index,hepevt_.idhep[index-1]);
    ostr << buf;

    // check number of mothers
    if( hepevt_.jmohep[index-1][1] == 0 ) {
        sprintf(buf,"   %4i      ",hepevt_.jmohep[index-1][0]);
    }
    else {
        sprintf(buf,"%4i - %4i  ",hepevt_.jmohep[index-1][0],hepevt_.jmohep[index-1][1]);
    }
    ostr << buf;

    // print daughters
    if( hepevt_.jdahep[index-1][0] != 0 ) {
        sprintf(buf,"%4i - %4i ",hepevt_.jdahep[index-1][0],hepevt_.jdahep[index-1][1]);
    }
    else {
        sprintf(buf,"   Stable   ");
    }
    ostr << buf;

    // print the rest of particle info
    sprintf(buf,"%8.2f %8.2f %8.2f %8.2f %8.2f",hepevt_.phep[index-1][0],hepevt_.phep[index-1][1],hepevt_.phep[index-1][2],hepevt_.phep[index-1][3],hepevt_.phep[index-1][4]);
    ostr << buf << endl;
}

inline bool HEPEVT_Wrapper::check_hepevt_consistency( std::ostream& ostr ) {
    //!< @todo HEPEVT_Wrapper::check_hepevt_consistency unimplemented!
    printf("HEPEVT_Wrapper::check_hepevt_consistency unimplemented!\n");
    return true;
}

inline void HEPEVT_Wrapper::zero_everything() {
    memset(&hepevt_,0,sizeof(hepevt_));
}

inline int HEPEVT_Wrapper::number_parents( int index ) {
    return (hepevt_.jmohep[index-1][0]) ? (hepevt_.jmohep[index-1][1]) ? 2 : 1 : 0;
}

inline int HEPEVT_Wrapper::number_children( int index ) {
    return (hepevt_.jdahep[index-1][0]) ? (hepevt_.jdahep[index-1][1]) ? hepevt_.jdahep[index-1][1]-hepevt_.jdahep[index-1][0] : 1 : 0;
}

inline void HEPEVT_Wrapper::set_parents( int index, int firstparent, int lastparent ) {
    hepevt_.jmohep[index-1][0] = firstparent;
    hepevt_.jmohep[index-1][1] = lastparent;
}

inline void HEPEVT_Wrapper::set_children( int index, int firstchild, int lastchild ) {
    hepevt_.jdahep[index-1][0] = firstchild;
    hepevt_.jdahep[index-1][1] = lastchild;
}

inline void HEPEVT_Wrapper::set_momentum( int index, double px, double py, double pz, double e ) {
    hepevt_.phep[index-1][0] = px;
    hepevt_.phep[index-1][1] = py;
    hepevt_.phep[index-1][2] = pz;
    hepevt_.phep[index-1][3] = e;
}

inline void HEPEVT_Wrapper::set_mass( int index, double mass ) {
    hepevt_.phep[index-1][4] = mass;
}

inline void HEPEVT_Wrapper::set_position( int index, double x, double y, double z, double t ) {
    hepevt_.vhep[index-1][0] = x;
    hepevt_.vhep[index-1][1] = y;
    hepevt_.vhep[index-1][2] = z;
    hepevt_.vhep[index-1][3] = t;
}

} // namespace HepMC

#endif
