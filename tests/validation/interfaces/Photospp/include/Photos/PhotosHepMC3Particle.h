#ifndef _PhotosHepMC3Particle_h_included_
#define _PhotosHepMC3Particle_h_included_
/**
 *  @file PhotosHepMC3Particle.h
 *  @brief Definition of \b class Photospp::PhotosHepMC3Particle
 *
 *  @class PhotosHepMC3Particle
 *  @brief Interface to HepMC3::GenEvent objects
 *
 *  @implements Photospp::PhotosParticle
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b> 16 April 2014 </b>
 */
#include "Photos/PhotosParticle.h"
#include "HepMC3/GenParticle.h"
#include <vector>

namespace Photospp
{

class PhotosHepMC3Event;

class PhotosHepMC3Particle: public PhotosParticle {
//
// Constructors
//
public:
    /** Constructor with HepMC3::GenParticle pointer */
    PhotosHepMC3Particle(const HepMC3::GenParticle &particle);

    /** Default destructor */
    ~PhotosHepMC3Particle();
//
// Functions
//
public:
    /** check that the 4 momentum in conserved at the vertices producing
        and ending this particle */
    bool checkMomentumConservation();

    /** Create a new particle of the same type, with the given
        properties. The new particle bares no relations to this
        particle, but it provides a way of creating a intance of
        the derived class. eg. createNewParticle() is used inside
        filhep_() so that an eg. PhotosHepMC3Particle is created without
        the method having explicit knowledge of the PhotosHepMC3Particle
        class */
    PhotosParticle * createNewParticle(int pdg_id, int status,
                                               double mass, double px,
                                               double py, double pz,
                                               double e);

    /** Create history entry of this particle before modifications
    of PHOTOS. Implementation of this method depends strongly
    on the event record. */
    void createHistoryEntry();

    /** Create a self-decay vertex for this particle
        with 'out' being the outgoing particle in new vertex */
    void createSelfDecayVertex(PhotosParticle *out);

    /** Clear vector of Photospp::PhotosParticle */
    void clear(std::vector<PhotosParticle*> &vector);
//
// Accessors
//
public:
    /** Get parent event */
    PhotosHepMC3Event* parent_event()             { return m_parent_event; }

    /** Set parent event */
    void set_parent_event(PhotosHepMC3Event *evt) { m_parent_event = evt; }

    /** Set the mothers of this particle via a vector of PhotosParticle */
    void setMothers(std::vector<PhotosParticle*> mothers);

    /** Set the daughters of this particle via a vector of PhotosParticle */
    void setDaughters(std::vector<PhotosParticle*> daughters);

    /** Add daughters of this particle to the list of its daughters */
    void addDaughter(PhotosParticle* daughter);

    /** Returns the mothers of this particle via a vector of PhotosParticle */
    std::vector<PhotosParticle*> getMothers();

    /** Returns the daughters of this particle via a vector of PhotosParticle */
    std::vector<PhotosParticle*> getDaughters();

    /** Returns all particles in the decay tree of this particle
        via a vector of PhotosParticle */
    std::vector<PhotosParticle*> getAllDecayProducts();

    double getPx()                { if(!m_particle) return 0; return m_particle.momentum().px(); }   //!< Get px
    double getPy()                { if(!m_particle) return 0; return m_particle.momentum().py(); }   //!< Get py
    double getPz()                { if(!m_particle) return 0; return m_particle.momentum().pz(); }   //!< Get pz
    double getE()                 { if(!m_particle) return 0; return m_particle.momentum().e(); }    //!< Get energy

    void   setPx( double px ); //!< Set px
    void   setPy( double py ); //!< Set py
    void   setPz( double pz ); //!< Set pz
    void   setE ( double e );  //!< Set energy

    int    getPdgID()             { if(!m_particle) return 0; return m_particle.pdg_id(); }          //!< Get PDG ID
    void   setPdgID(int pdg_id)   { if(!m_particle) return;   m_particle.set_pdg_id(pdg_id); }       //!< Set PDG ID

    int    getStatus()            { if(!m_particle) return 0; return m_particle.status(); }          //!< Get status
    void   setStatus(int status)  { if(!m_particle) return;   m_particle.set_status(status); }       //!< Set status

    double getMass()              { if(!m_particle) return 0; return m_particle.generated_mass(); }  //!< Get mass
    void   setMass(double mass)   { if(!m_particle) return;   m_particle.set_generated_mass(mass); } //!< Set mass

    int    getBarcode()           { if(!m_particle) return 0; return m_particle.barcode(); }         //!< Get barcode

    const HepMC3::GenParticle& getHepMC() { return m_particle; }                                     //!< Get HepMC particle

    /** Print some information about this particle to standard output */
    void   print()                { if(!m_particle) return; m_particle.print(); }
//
// Fields
//
private:
    PhotosHepMC3Event            *m_parent_event;   //!< Parent event
    HepMC3::GenParticle           m_particle;       //!< Pointer to GenParticle
    std::vector<PhotosParticle*>  m_mothers;        //!< List of mothers
    std::vector<PhotosParticle*>  m_daughters;      //!< List of daughters
    std::vector<PhotosParticle*>  m_decay_products; //!< List of decay products

    /** Created particles
     *  Used to store temporary particles and to delete them later
     */
    std::vector<PhotosParticle*>  m_created_particles;
};

} // namespace Photospp
#endif

