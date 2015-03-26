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

    /// @brief Returns true if this is a plain default-constructed object.
    bool empty() const {
      return m_attributes.empty();
    }

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
