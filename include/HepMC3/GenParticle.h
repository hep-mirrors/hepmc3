#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class GenParticle
 *
 *  @class HepMC3::GenParticle
 *  @brief Stores particle-related information
 *
 */
#include "HepMC3/Data/SmartPointer.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/FourVector.h"
#include <iostream>
#include <vector>

namespace HepMC3 {

class GenEvent;

class GenParticle {

friend class GenEvent;
friend class GenVertex;
friend class SmartPointer<GenParticle>;

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenParticle( const FourVector &momentum = FourVector::ZERO_VECTOR(), int pdg_id = 0, int status = 0 );

    /** @brief Constructor based on particle data */
    GenParticle( const GenParticleData &data );

//
// Functions
//
public:
    /** @brief Print information about the vertex */
    void print( std::ostream& ostr = std::cout ) const;

    /** @brief Print vertex content for selected version */
    void print_version( unsigned char version, std::ostream& ostr = std::cout ) const;

    /** @brief Check if this particle belongs to an event */
    bool in_event() const { return (bool)(m_event); }

    /** @brief Create new version of this particle
     *
     *  @return Pointer to the new version of this particle or null pointer if:
     *  - this particle does not belong to an event
     *  - this particle already belongs to the last version
     *  - newer version of this particle already exists
     *  - particle has been marked as deleted
     */
    GenParticlePtr new_version();

    /** @brief Mark particle as deleted */
    void mark_deleted();
//
// Accessors
//
public:
    GenEvent*              parent_event() const { return m_event; } //!< Get parent event
    int                    id()           const { return m_id;    } //!< Get particle id
    const GenParticleData& data()         const { return m_data;  } //!< Get particle data

    int   pdg_id()                const { return m_data.pdg_id;         } //!< Get PDG ID
    int   status()                const { return m_data.status;         } //!< Get status code
    int   status_subcode()        const { return m_data.status_subcode; } //!< Get status subcode
    const FourVector& momentum()  const { return m_data.momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data.is_mass_set;    } //!< Check if generated mass is set

    /** @brief Get generated mass
     *
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    GenVertexPtr production_vertex() const;        //!< Get production vertex
    GenVertexPtr end_vertex() const;               //!< Get end vertex

    void set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void set_status(int status);                   //!< Set status code
    void set_status_subcode(int subcode);          //!< Set status subcode
    void set_momentum(const FourVector& momentum); //!< Set momentum
    void set_generated_mass(double m);             //!< Set generated mass
    void unset_generated_mass();                   //!< Declare that generated mass is not set

    /** @brief Get barcode
     *
     *  Currently barcode = id
     *  @todo Write proper barcode once we decide how it should look like
     */
    int barcode() const { return m_id; }

    unsigned char version_created() const { return m_version_created; } //!< Get version created
    unsigned char version_deleted() const { return m_version_deleted; } //!< Get version deleted

    /** @brief Check if this particle exists in selected version */
    bool is_in_version(unsigned char version) const { return   m_version_created <= version &&
                                                             (!m_version_deleted || m_version_deleted>version); }

protected:
    void set_production_vertex( const shared_ptr<GenVertex> &v ) { m_production_vertex = v; } //!< Set production vertex
    void set_end_vertex       ( const shared_ptr<GenVertex> &v ) { m_end_vertex        = v; } //!< Set end vertex
//
// Fields
//
private:
    GenEvent        *m_event; //!< Parent event
    int              m_id;    //!< Index
    GenParticleData  m_data;  //!< Particle data

    unsigned char  m_version_created; //!< Version created
    unsigned char  m_version_deleted; //!< Version deleted
    GenParticlePtr m_next_version;    //!< Next version of this particle

    weak_ptr<GenVertex>    m_production_vertex; //!< Production vertex
    weak_ptr<GenVertex>    m_end_vertex;        //!< End vertex
    weak_ptr<GenParticle>  m_this;              //!< Pointer to shared pointer managing this particle
};

} // namespace HepMC3

#endif
