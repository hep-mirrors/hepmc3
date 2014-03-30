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
using std::vector;

namespace HepMC3 {

class GenEvent;

class GenEventVersion {
//
// Constructors
//
public:
    /** Default constructor. */
    GenEventVersion(int number, const char *name);
    /** Default destructor */
    ~GenEventVersion();

//
// Functions
//
public:
    /** Record change to a particle
     *  If it hasn't been changed in this version yet - create new entry for it
     *  If particle does not exist in the event - add it to the event
     */
    void record_change(GenParticle *p);

    /** Record change to a vertex
     *  If it hasn't been changed in this version yet - create new entry for it
     *  If vertex does not exist in the event - add it to the event
     */
    void record_change(GenVertex *v);

private:
    /** Helper for record_deleted functions */
    void record_deleted(int barcode, vector<int> &container);
//
// Accessors
//
public:
    std::string name()                      const { return m_name; }         //!< Get name
    void        set_name(std::string name)        { m_name = name; }         //!< Set name

    const vector<GenParticle*>& particles() const { return m_particles; }    //!< Access particle list
    const vector<GenVertex*>&   vertices()  const { return m_vertices; }     //!< Access vertex list

//
// Fields
//
private:
    int                   m_version;                   //!< Version number
    std::string           m_name;                      //!< Version name. Indicates what tool/generator created this version
    vector<GenParticle*>  m_particles;                 //!< List of particles modified/added in this version (sorted by barcode)
    vector<GenVertex*>    m_vertices;                  //!< List of vertices  modified/added in this version (sorted by barcode)
};

} // namespace HepMC3

#endif
