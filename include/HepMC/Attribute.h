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

/** @brief Attribute container used for serialization and parsing
 *
 *  Any other container can be used if needed. Rest of the code should not
 *  be influenced by this as long as AttributeContainer can be casted
 *  to std::string and created from std::string using operator=
 */
typedef std::string AttributeContainer;

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
    /** @brief Protected constructor that allows to set AttributeContainer
     *
     *  Used when parsing attributes from file. An UnparsedAttribute class
     *  object is made, which uses this constructor to signify that
     *  it just holds AttributeContainer without parsing it.
     *
     *  @note There should be no need for user class to ever use this constructor
     */
    Attribute(const AttributeContainer &ac):m_is_parsed(false),m_attribute_container(ac) {}

//
// Functions
//
public:
    /** @brief Fill class content from AttributeContainer */
    virtual bool parse_attribute_container(const AttributeContainer &att) = 0;

    /** @brief Fill AttributeContainer from class content */
    virtual bool fill_attribute_container(AttributeContainer &att) const = 0;

    /** @brief Check if this attribute is parsed */
    bool is_parsed() { return m_is_parsed; }

    /** @brief Get attribute container */
    const AttributeContainer& attribute_container() const { return m_attribute_container; }

//
// Fields
//
private:
    const bool         m_is_parsed;
    AttributeContainer m_attribute_container;
};

/**
 *  @class HepMC::UnparsedAttribute
 *  @brief Class used when reading attributes from input file
 *
 *  Signifies that this class contains an unparsed AttributeContainer.
 *  Used when reading attributes from input file without the knowledge
 *  of what class it represents.
 *
 */
class UnparsedAttribute : public Attribute {
public:
    /** @brief Default constructor */
    UnparsedAttribute(const AttributeContainer &ac):Attribute(ac) {}

    /** @brief Implementation of Attribute::parse_attribute_container
     *
     *  No parsing can be done thus returning false if a parsing is attempted
     */
    bool parse_attribute_container(const AttributeContainer &att) {
        return false;
    }

    /** @brief Implementation of Attribute::fill_attribute_container
     *
     *  In case of this class, we do not have to fill the container at all;
     */
    bool fill_attribute_container(AttributeContainer &att) const {
        att = attribute_container();
        return true;
    }
};

} // namespace HepMC

#endif
