#include "HepMC/GenEvent.h"

/** @class MyClass
 *  @brief Sample class for root I/O test
 */
class MyClass {
public:

    /// @brief Default constructor
    MyClass();

    /// @brief Set HepMC event
    void SetEvent(HepMC::GenEvent*);

    /// @brief Get HepMC event
    HepMC::GenEvent* GetEvent();

private:
    int someint;            ///< Test int
    HepMC::GenEvent* event; ///< Test event
};
