// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenEvent.h
/// @brief Definition of \b class GenEvent
///
#ifndef HEPMC3_GENEVENT_H
#define HEPMC3_GENEVENT_H

#include "HepMC3/Units.h"
#include "HepMC3/GenParticle_fwd.h"
#include "HepMC3/GenVertex_fwd.h"
#include "HepMC3/GenPdfInfo_fwd.h"
#include "HepMC3/GenHeavyIon_fwd.h"
#include "HepMC3/GenCrossSection_fwd.h"

#if !defined(__CINT__)
#include "HepMC3/GenHeavyIon.h"
#include "HepMC3/GenPdfInfo.h"
#include "HepMC3/GenCrossSection.h"
#include "HepMC3/GenRunInfo.h"
#include <mutex>
#endif // __CINT__

#ifdef HEPMC3_ROOTIO
class TBuffer;
#endif

#ifdef HEPMC3_PROTOBUFIO
namespace HepMC3_pb {
class GenEventData;
}
#endif


namespace HepMC3 {

struct GenEventData;

/// @brief Stores event-related information
///
/// Manages event-related information.
/// Contains lists of GenParticle and GenVertex objects
class GenEvent {

public:

    /// @brief Event constructor without a run
    GenEvent(Units::MomentumUnit mu = Units::GEV,
             Units::LengthUnit lu = Units::MM);

#if !defined(__CINT__)

    /// @brief Constructor with associated run
    GenEvent(std::shared_ptr<GenRunInfo> run,
             Units::MomentumUnit mu = Units::GEV,
             Units::LengthUnit lu = Units::MM);

    /// @brief Copy constructor
    GenEvent(const GenEvent&);

    /// @brief Destructor
    ~GenEvent();

    /// @brief Copy Assignment operator
    GenEvent& operator=(const GenEvent&);

    /// @name Particle and vertex access
    /// @{

    /// @brief Get list of particles (const)
    const std::vector<ConstGenParticlePtr>& particles() const;
    /// @brief Get list of vertices (const)
    const std::vector<ConstGenVertexPtr>& vertices() const;

    /// @brief Get/set list of particles (non-const)
    const std::vector<GenParticlePtr>& particles() { return m_particles; }
    /// @brief Get/set list of vertices (non-const)
    const std::vector<GenVertexPtr>& vertices() { return m_vertices; }

    /// @}


    /// @name Particle and vertex access
    /// @{

    /// @brief Particles size, HepMC2 compatibility
    inline int particles_size() const { return m_particles.size(); }
    /// @brief Particles empty, HepMC2 compatibility
    inline bool particles_empty() const { return m_particles.empty(); }
    /// @brief Vertices size, HepMC2 compatibility
    inline int vertices_size() const { return m_vertices.size(); }
    /// @brief Vertices empty, HepMC2 compatibility
    inline bool vertices_empty() const { return m_vertices.empty(); }
    /// @}


    /// @name Event weights
    /// @{

    /// Get event weight values as a vector
    const std::vector<double>& weights() const { return m_weights; }
    /// Get event weights as a vector (non-const)
    std::vector<double>& weights() { return m_weights; }
    /// Get event weight accessed by index (or the canonical/first one if there is no argument)
    /// @note It's the user's responsibility to ensure that the given index exists!
    double weight(const unsigned long& index=0) const { if ( index < weights().size() ) return weights().at(index); else  throw std::runtime_error("GenEvent::weight(const unsigned long&): weight index outside of range"); return 0.0; }
    /// Get event weight accessed by weight name
    /// @note Requires there to be an attached GenRunInfo, otherwise will throw an exception
    /// @note It's the user's responsibility to ensure that the given name exists!
    double weight(const std::string& name) const {
        if (!run_info()) throw std::runtime_error("GenEvent::weight(const std::string&): named access to event weights requires the event to have a GenRunInfo");
        return weight(run_info()->weight_index(name));
    }
    /// Get event weight accessed by weight name
    /// @note Requires there to be an attached GenRunInfo, otherwise will throw an exception
    /// @note It's the user's responsibility to ensure that the given name exists!
    double& weight(const std::string& name) {
        if (!run_info()) throw std::runtime_error("GenEvent::weight(const std::string&): named access to event weights requires the event to have a GenRunInfo");
        int pos = run_info()->weight_index(name);
        if ( pos < 0 ) throw std::runtime_error("GenEvent::weight(const std::string&): no weight with given name in this run");
        if ( pos >= int(m_weights.size())) throw std::runtime_error("GenEvent::weight(const std::string&): weight index outside of range");
        return m_weights[pos];
    }
    /// Get event weight names, if there are some
    /// @note Requires there to be an attached GenRunInfo with registered weight names, otherwise will throw an exception
    const std::vector<std::string>& weight_names() const {
        if (!run_info()) throw std::runtime_error("GenEvent::weight_names(): access to event weight names requires the event to have a GenRunInfo");
        const std::vector<std::string>& weightnames = run_info()->weight_names();
        if (weightnames.empty()) throw std::runtime_error("GenEvent::weight_names(): no event weight names are registered for this run");
        return weightnames;
    }

    /// @}


    /// @name Auxiliary info and event metadata
    /// @{

    /// @brief Get a pointer to the the GenRunInfo object.
    std::shared_ptr<GenRunInfo> run_info() const {
        return m_run_info;
    }
    /// @brief Set the GenRunInfo object by smart pointer.
    void set_run_info(std::shared_ptr<GenRunInfo> run) {
        m_run_info = run;
        if ( run && !run->weight_names().empty() )
            m_weights.resize(run->weight_names().size(), 1.0);
    }

    /// @brief Get event number
    int  event_number() const { return m_event_number; }
    /// @brief Set event number
    void set_event_number(const int& num) { m_event_number = num; }

    /// @brief Get momentum unit
    const Units::MomentumUnit& momentum_unit() const { return m_momentum_unit; }
    /// @brief Get length unit
    const Units::LengthUnit& length_unit() const { return m_length_unit; }
    /// @brief Change event units
    /// Converts event from current units to new ones
    void set_units( Units::MomentumUnit new_momentum_unit, Units::LengthUnit new_length_unit);

    /// @brief Get heavy ion generator additional information
    GenHeavyIonPtr heavy_ion() { return attribute<GenHeavyIon>("GenHeavyIon"); }
    /// @brief Get heavy ion generator additional information (const version)
    ConstGenHeavyIonPtr heavy_ion() const { return attribute<GenHeavyIon>("GenHeavyIon"); }
    /// @brief Set heavy ion generator additional information
    void set_heavy_ion(GenHeavyIonPtr hi) { add_attribute("GenHeavyIon",hi); }

    /// @brief Get PDF information
    GenPdfInfoPtr pdf_info() { return attribute<GenPdfInfo>("GenPdfInfo"); }
    /// @brief Get PDF information (const version)
    ConstGenPdfInfoPtr pdf_info() const { return attribute<GenPdfInfo>("GenPdfInfo"); }
    /// @brief Set PDF information
    void set_pdf_info(GenPdfInfoPtr pi) { add_attribute("GenPdfInfo",pi); }

    /// @brief Get cross-section information
    GenCrossSectionPtr cross_section() { return attribute<GenCrossSection>("GenCrossSection"); }
    /// @brief Get cross-section information (const version)
    ConstGenCrossSectionPtr cross_section() const { return attribute<GenCrossSection>("GenCrossSection"); }
    /// @brief Set cross-section information
    void set_cross_section(GenCrossSectionPtr cs) { add_attribute("GenCrossSection",cs); }

    /// @}


    /// @name Event position
    /// @{

    /// Vertex representing the overall event position
    const FourVector& event_pos() const;

    /// @brief Vector of beam particles
    std::vector<ConstGenParticlePtr> beams() const;

    /// @brief Vector of beam particles
    std::vector<ConstGenParticlePtr> beams(const int status) const;


    /// @brief Vector of beam particles
    const std::vector<GenParticlePtr> & beams();

    /// @brief Shift position of all vertices in the event by @a delta
    void shift_position_by( const FourVector & delta );

    /// @brief Shift position of all vertices in the event to @a op
    void shift_position_to( const FourVector & newpos ) {
        const FourVector delta = newpos - event_pos();
        shift_position_by(delta);
    }

    /// @brief Boost event using x,y,z components of @a delta as velocity fractions
    bool boost( const FourVector&  delta );
    /// @brief Rotate event using x,y,z components of @a delta as rotation angles
    ///
    /// @todo Clarify orders of rotations?
    bool rotate( const FourVector&  delta );
    /// @brief Change sign of @a axis
    bool reflect(const int axis);

    /// @}


    /// @name Additional attributes
    /// @{
    /// @brief Add event attribute to event
    ///
    /// This will overwrite existing attribute if an attribute
    /// with the same name is present
    void add_attribute(const std::string &name, const std::shared_ptr<Attribute> &att,  const int& id = 0);

    /// @brief Add multiple attributes to event
    ///
    /// This will overwrite existing attributes if attributes
    /// with the same names are present
    void add_attributes(const std::vector<std::string> &names, const std::vector<std::shared_ptr<Attribute> > &atts,  const std::vector<int>& ids);

    /// @brief Add multiple attributes to event
    ///
    /// This will overwrite existing attributes if attributes
    /// with the same names are present
    void add_attributes(const std::string& name, const std::vector<std::shared_ptr<Attribute> > &atts,  const std::vector<int>& ids);

    /// @brief Add multiple attributes to event
    ///
    /// This will overwrite existing attributes if attributes
    /// with the same names are present
    void add_attributes(const std::string& name, const std::vector<std::pair<int, std::shared_ptr<Attribute> > > &atts);


    /// @brief Remove attribute
    void remove_attribute(const std::string &name,  const int& id = 0);

    /// @brief Get attribute of type T
    template<class T>
    std::shared_ptr<T> attribute(const std::string &name,  const int& id = 0) const;

    /// @brief Get attribute of any type as string
    std::string attribute_as_string(const std::string &name,  const int& id = 0) const;

    /// @brief Get list of attribute names
    std::vector<std::string> attribute_names( const int& id = 0) const;

    /// @brief Get a copy of the list of attributes
    /// @note To avoid thread issues, this is returns a copy. Better solution may be needed.
    std::map< std::string, std::map<int, std::shared_ptr<Attribute> > > attributes() const {
        std::lock_guard<std::recursive_mutex> lock(m_lock_attributes);
        return m_attributes;
    }

    /// @}


    /// @name Particle and vertex modification
    /// @{

    /// @brief Add particle
    void add_particle( GenParticlePtr p );

    /// @brief Add vertex
    void add_vertex( GenVertexPtr v );

    /// @brief Remove particle from the event
    ///
    /// This function  will remove whole sub-tree starting from this particle
    /// if it is the only incoming particle of this vertex.
    /// It will also production vertex of this particle if this vertex
    /// has no more outgoing particles
    void remove_particle( GenParticlePtr p );

    /// @brief Remove a set of particles
    ///
    /// This function follows rules of GenEvent::remove_particle to remove
    /// a list of particles from the event.
    void remove_particles( std::vector<GenParticlePtr> v );

    /// @brief Remove vertex from the event
    ///
    /// This will remove all sub-trees of all outgoing particles of this vertex
    void remove_vertex( GenVertexPtr v );

    /// @brief Add whole tree in topological order
    ///
    /// This function will find the beam particles (particles
    /// that have no production vertices or their production vertices
    /// have no particles) and will add the whole decay tree starting from
    /// these particles.
    ///
    /// @note Any particles on this list that do not belong to the tree
    ///       will be ignored.
    void add_tree( const std::vector<GenParticlePtr> &parts );

    /// @brief Reserve memory for particles and vertices
    ///
    /// Helps optimize event creation when size of the event is known beforehand
    void reserve(const size_t& parts, const size_t& verts = 0);

    /// @brief Remove contents of this event
    void clear();

    /// @}

    /// @name Deprecated functionality
    /// @{

    /// @brief Set incoming beam particles
    /// @deprecated Backward compatibility
    void set_beam_particles(GenParticlePtr p1, GenParticlePtr p2);


    /// @brief Add  particle to root vertex

    void add_beam_particle(GenParticlePtr p1);


    /// @}

#endif // __CINT__


    /// @name Methods to fill GenEventData and to read it back
    /// @{

    /// @brief Fill GenEventData object
    void write_data(GenEventData &data) const;

    /// @brief Fill GenEvent based on GenEventData
    void read_data(const GenEventData &data);

#ifdef HEPMC3_ROOTIO
    /// @brief ROOT I/O streamer
    void Streamer(TBuffer &b);
#endif

#ifdef HEPMC3_PROTOBUFIO
    /// @brief protobuf I/O reader
    void read_data(HepMC3_pb::GenEventData const &data);
#endif
    /// @}

private:

    /// @name Fields
    /// @{

#if !defined(__CINT__)

    /// List of particles
    std::vector<GenParticlePtr> m_particles;
    /// List of vertices
    std::vector<GenVertexPtr> m_vertices;

    /// Event number
    int m_event_number = 0;

    /// Event weights
    std::vector<double> m_weights;

    /// Momentum unit
    Units::MomentumUnit m_momentum_unit = Units::GEV;
    /// Length unit
    Units::LengthUnit m_length_unit = Units::MM;

    /// The root vertex is stored outside the normal vertices list to block user access to it
    GenVertexPtr m_rootvertex;

    /// Global run information.
    std::shared_ptr<GenRunInfo> m_run_info;

    /// @brief Map of event, particle and vertex attributes
    ///
    /// Keys are name and ID (0 = event, <0 = vertex, >0 = particle)
    mutable std::map< std::string, std::map<int, std::shared_ptr<Attribute> > > m_attributes;

    /// @brief Attribute map key type
    typedef std::map< std::string, std::map<int, std::shared_ptr<Attribute> > >::value_type att_key_t;

    /// @brief Attribute map value type
    typedef std::map<int, std::shared_ptr<Attribute> >::value_type att_val_t;

    /// @brief Mutex lock for the m_attibutes map.
    mutable std::recursive_mutex m_lock_attributes;
#endif // __CINT__

    /// @}

};



#if !defined(__CINT__)
//
// Template methods
//
template<class T>
std::shared_ptr<T> GenEvent::attribute(const std::string &name,  const int& id) const {
    std::lock_guard<std::recursive_mutex> lock(m_lock_attributes);
    std::map< std::string, std::map<int, std::shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if ( i1 == m_attributes.end() ) {
        if ( id == 0 && run_info() ) {
            return run_info()->attribute<T>(name);
        }
        return std::shared_ptr<T>();
    }

    std::map<int, std::shared_ptr<Attribute> >::iterator i2 = i1->second.find(id);
    if (i2 == i1->second.end() ) return std::shared_ptr<T>();

    if (!i2->second->is_parsed() ) {

        std::shared_ptr<T> att = std::make_shared<T>();
        att->m_event = this;

        if ( id > 0 && id <= int(particles().size()) ) {
            att->m_particle = m_particles[id - 1];
        }
        if ( id < 0 && -id <= int(vertices().size()) ) {
            att->m_vertex = m_vertices[-id - 1];
        }
        if ( att->from_string(i2->second->unparsed_string()) &&
                att->init() ) {
            // update map with new pointer
            i2->second = att;
            return att;
        } else {
            return std::shared_ptr<T>();
        }
    }
    else return std::dynamic_pointer_cast<T>(i2->second);
}
#endif // __CINT__


} // namespace HepMC3
#endif
