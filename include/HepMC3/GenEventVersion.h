#ifndef  HEPMC3_GENEVENTVERSION_H
#define  HEPMC3_GENEVENTVERSION_H
/**
 *  @file GenEventVersion.h
 *  @brief Definition of \b class HepMC3::GenEventVersion
 *
 *  @class HepMC3::GenEventVersion
 *  @brief Stores version-related information
 *
 *  Used to record changes made to the event record by tools or generators
 *  used in the analysis. Versions use name and description to provide details
 *  regarding the tool used to create them
 *
 *  @date Created       <b> 27th March 2014 </b>
 *  @date Last modified <b> 27th March 2014 </b>
 */
#include <vector>
#include <string>
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/DataList.h"
using std::vector;

namespace HepMC3 {

class GenEvent;

class GenEventVersion {

friend class GenEvent;

//
// Constructors
//
public:
    /** Default constructor. */
    GenEventVersion(int number, int particle_barcode, int vertex_barcode, const char *name);

//
// Functions
//
public:
    /** Get particle by barcode */
    const GenParticle& get_particle(int barcode) const;

    /** Get vertex by barcode */
    const GenVertex&   get_vertex(int barcode) const;
protected:
    /** Create new particle */
    GenParticle& create_particle();

    /** Create new vertex */
    GenVertex&   create_vertex();

    /** Mark particle as deleted in this version */
    void delete_particle(GenParticle &p);

    /** Mark vertex as deleted in this version */
    void delete_vertex(GenVertex &p);
//
// Accessors
//
public:
    std::string name()                      const { return m_name; }         //!< Get name
    void        set_name(std::string name)        { m_name = name; }         //!< Set name

    const DataList<GenParticle,8>& particles()  const { return m_particles; }    //!< Get particle list
    const DataList<GenVertex,8>&   vertices()   const { return m_vertices; }     //!< Get vertex list

//
// Fields
//
private:
    int          m_version;                //!< Version number
    int          m_first_particle_barcode; //!< Barcode of the first particle
    int          m_first_vertex_barcode;   //!< Barcode of the first vertex
    std::string  m_name;                   //!< Version name. Indicates what tool/generator created this version
    DataList<GenParticle,8> m_particles;   //!< List of particles modified/added in this version (sorted by barcode)
    DataList<GenVertex,8> m_vertices;      //!< List of vertices  modified/added in this version (sorted by barcode)
};

} // namespace HepMC3

#endif
