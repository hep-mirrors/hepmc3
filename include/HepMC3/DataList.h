#ifndef  HEPMC3_DATALIST_H
#define  HEPMC3_DATALIST_H
/**
 *  @file DataList.h
 *  @brief Definition of \b class HepMC3::DataList
 *
 *  @class HepMC3::DataList
 *  @brief List of blocks of data
 *
 *  Container for handling memory management of fixed-size
 *  blocks of data (pages). The data structure can be resized
 *  without reallocating previously created data blocks.
 *
 *  Objects cannot be moved or deleted. This allows references
 *  to the objects to be valid as long as this container is valid.
 *
 *  This is fast implementation that has no error-checking so it needs
 *  to be properly handled. The template parameters are:
 *
 *  @param T class that will be stored in the container
 *  @param N page size in terms of power of 2
 *
 */
#include <cmath>  // pow
#include <vector>

namespace HepMC3 {

template<class T, unsigned int N>
class DataList {
public:

    /** Default constructor */
    DataList():m_block_size(pow(2,N)),m_size(0) {}

    /** Default destructor */
    ~DataList() {
        for(unsigned int i=0; i<m_data.size(); ++i) {
            for(unsigned int j=0; j<m_block_size; ++j) {
                if( m_size <= 0 ) break;
                m_data[i][j].~T();
                --m_size;
            }

            operator delete[](m_data[i]);
        }
    }

    /** operator[]
     *  Translates index to pair <page,subindex>
     */
    T& operator[](unsigned int idx) const {
        unsigned int page = idx >> N;
        idx %= m_block_size;
        return m_data[page][idx];
    }

    /** Default initialization of the new object */
    T* new_object() {
        T *p = new_uninitialized_object();

        new (p) T();

        return p;
    }

    /** Initialize new object using copy constructor */
    T* new_object(const T &o) {
        T *p = new_uninitialized_object();

        new (p) T(o);

        return p;
    }

    /** Registers that new object has been added.
     *  Reserves more memory if needed
     *  @return Pointer to the new (uninitialized) object
     */
    T* new_uninitialized_object() {
        ++m_size;
        if( m_size % m_block_size == 1 ) {
            T *p = static_cast<T*>( operator new[](sizeof(T)*m_block_size) );
            m_data.push_back( p );
        }

        return static_cast<T*>(m_data.back() + (m_size-1)%m_block_size);
    }

    /** Get size of the container */
    unsigned int size() const { return m_size; }

    /** @todo Needs iterator, const_iterator, begin(), end() */

private:
    unsigned int    m_block_size; //!< Block size. Calculated as pow(2,N)
    unsigned int    m_size;       //!< Number of objects in container
    std::vector<T*> m_data;       //!< Vector of pointers to data blocks
};

} // namespace HepMC3

#endif
