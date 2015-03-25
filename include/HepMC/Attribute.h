// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_ATTRIBUTE_H
#define  HEPMC_ATTRIBUTE_H
/**
 *  @file Attribute.h
 *  @brief Definition of \b class Attribute and \b class StringAttribute
 *
 *  @class HepMC::Attribute
 *  @brief Base class for all attributes
 *
 *  Contains virtual functions to_string and from_string that
 *  each attribute must implement
 *
 *  @ingroup attributes
 *
 */
#include <string>
using std::string;

namespace HepMC {

class GenEvent;

class Attribute {
//
// Constructors
//
public:
    /** @brief Default constructor */
    Attribute():m_is_parsed(true) {}

    /** @brief Virtual destructor */
    virtual ~Attribute() {}

protected:
    /** @brief Protected constructor that allows to set string
     *
     *  Used when parsing attributes from file. An StringAttribute class
     *  object is made, which uses this constructor to signify that
     *  it just holds string without parsing it.
     *
     *  @note There should be no need for user class to ever use this constructor
     */
    Attribute(const string &st):m_is_parsed(false),m_string(st) {}

//
// Virtual Functions
//
public:
    /** @brief Fill class content from string.
     */
    virtual bool from_string(const string & att) = 0;

    /** @brief Optionally initialize the attribute after from_string
     *
     */
    virtual bool init(const GenEvent & geneve) {}

    /** @brief Fill string from class content */
    virtual bool to_string(string &att) const = 0;

    /** @brief Indicate if attribute is global to a run.
     *
     * @return true if this attribute is global to the run. Note that
     * a global attribute is not supposed to change during a run. Only
     * the first attribute with a given name will be written out to a
     * file. When reading in a file a global attribute with a given
     * name will be added to all GenEvents as an event attribute. */
    virtual bool is_global() const { return false; }

//
// Accessors
//
public:
    /** @brief Check if this attribute is parsed */
    bool is_parsed() { return m_is_parsed; }

    /** @brief Get unparsed string */
    const string& unparsed_string() const { return m_string; }

//
// Fields
//
private:
    const bool m_is_parsed; //!< Is this attribute parsed?
    string     m_string;    //!< Raw (unparsed) string
};

/**
 *  @class HepMC::StringAttribute
 *  @brief Attribute that holds a string
 *
 *  Default attribute constructed when reading input files.
 *  It can be then parsed by other attributes or left as a string.
 *
 *  @ingroup attributes
 *
 */
class StringAttribute : public Attribute {
public:
    /** @brief String-based constructor
     *
     *  The Attribute constructor used here marks that this is an unparsed
     *  string that can be (but does not have to be) parsed
     *
     */
    StringAttribute(const string &st):Attribute(st) {}

    /** @brief Implementation of Attribute::from_string
     *
     *  No parsing is needed
     */
    bool from_string(const string &att) {
        return true;
    }

    /** @brief Implementation of Attribute::to_string */
    bool to_string(string &att) const {
        att = unparsed_string();
        return true;
    }
};

} // namespace HepMC

#endif
