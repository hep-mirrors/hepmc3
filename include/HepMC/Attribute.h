#ifndef  HEPMC_ATTRIBUTE_H
#define  HEPMC_ATTRIBUTE_H
/**
 *  @file Attribute.h
 *  @brief Definition of \b type AttributeContainer and \b class Attribute
 *
 *  @class HepMC::Attribute
 *  @brief Base class for all attributes
 *
 *  Defines functions for parsing AttributeContainer
 *  and serializing the class as AttributeContainer
 *  that each attribute must have
 *
 */
#include <string>

namespace HepMC {

/** @brief Attribute container used for serialization and parsing */
typedef std::string AttributeContainer;

class Attribute {
//
// Constructors
//
public:
    /** @brief Virtual destructor */
    virtual ~Attribute() {}

//
// Functions
//
public:

    /** @brief Fill class content from AttributeContainer */
    virtual bool parse_attribute_container(const AttributeContainer &att) = 0;

    /** @brief Fill AttributeContainer from class content */
    virtual bool fill_attribute_container(AttributeContainer &att) const = 0;
};

} // namespace HepMC

#endif
