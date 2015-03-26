// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenRunInfo.h
/// @brief Definition of \b class GenRunInfo
///
#ifndef  HEPMC_GENRUNINFO_H
#define  HEPMC_GENRUNINFO_H

#include "HepMC/Common.h"
#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Units.h"
#include "HepMC/Attribute.h"

namespace HepMC {


/// @brief Stores run-related information
///
/// Manages run-related information.
/// Contains run-wide attributes
class GenRunInfo {

public:

    /// @brief Default constructor
    GenRunInfo() {}

    /// @brief Check if a weight name is present.
    bool has_weight(string name) const {
  	return m_weight_indices.find(name) !=  m_weight_indices.end();
    }

    /// @brief Return the index corresponding to a weight name.
    /// @return -1 if name was not found
    /// @todo Throw exception instead? Or return ssize_t for better signed/unsigned safety?
    int weight_index(string name) const {
	std::map<std::string, int>::const_iterator it =
	    m_weight_indices.find(name);
	return it == m_weight_indices.end()? -1: it->second;
    }

    /// @brief Get the vector of weight names.
    const std::vector<std::string> & weight_names() const {
	return m_weight_names;
    }

    /// @brief Set the names of the weights in this run.
    ///
    /// For consistency, the length of the vector should be the same as
    /// the number of weights in the events in the run.
    void set_weight_names(const std::vector<std::string> & names);

    /// @brief add an attribute
    /// This will overwrite existing attribute if an attribute
    /// with the same name is present
    void add_attribute(const string &name,
		       const shared_ptr<Attribute> &att) {
      if ( att ) m_attributes[name] = att;
    }

    /// @brief Remove attribute
    void remove_attribute(const string &name) {
      m_attributes.erase(name);
    }

    /// @brief Get attribute of type T
    template<class T>
    shared_ptr<T> attribute(const string &name) const;

    /// @brief Get list of attributes
    const std::map< std::string, shared_ptr<Attribute> > & attributes() const {
      return m_attributes;
    }


private:

    /// @name Fields
    //@{

    /// @brief A map of weight names mapping to indices.
    std::map<std::string, int> m_weight_indices;

    /// @brief A vector of weight names.
    std::vector<std::string> m_weight_names;

    /// @brief Map of attributes
    mutable std::map< std::string, shared_ptr<Attribute> > m_attributes;
    //@}

};


//
// Template methods
//

template<class T>
shared_ptr<T> GenRunInfo::attribute(const string &name) const {

    std::map< std::string, shared_ptr<Attribute> >::iterator i =
      m_attributes.find(name);
    if( i == m_attributes.end() ) return shared_ptr<T>();

    if( !i->second->is_parsed() ) {

	shared_ptr<T> att = make_shared<T>();
        if ( att->from_string(i->second->unparsed_string()) &&
	     att->init(*this) ) {
	    // update map with new pointer
	    i->second = att;

	    return att;
	}
	else
	    return shared_ptr<T>();
    }
    else return dynamic_pointer_cast<T>(i->second);
}


} // namespace HepMC

#endif
