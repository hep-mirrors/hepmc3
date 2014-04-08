#ifndef  HEPMC3_DATALIST_H
#define  HEPMC3_DATALIST_H
/**
 *  @file DataList.h
 *  @brief Definition of \b class HepMC3::DataList
 *
 *  @class HepMC3::DataList
 *  @brief List of blocks of data
 *
 *  Handles memory management of fixed-size blocks of data.
 *  The data structure can be resized without reallocating previously
 *  created data blocks
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <vector>
#include "HepMC3/FourVector.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

template<class T, unsigned int N>
class DataList {
public:

    DataList():m_block_size(pow(2,N)),m_size(0) {}
    
    ~DataList() { for(unsigned int i=0; i<m_data.size(); ++i) delete[] m_data[i]; }

    T& operator[](unsigned int idx) const {
        unsigned int page = idx >> N;
        idx %= m_block_size;
        return m_data[page][idx];
    }

    void increase_size() {
        ++m_size;
        if( m_size % m_block_size == 1 ) {
            m_data.push_back( new T[m_block_size] );
        }
    }

    unsigned int size() const { return m_size; }

    //iterator, const_iterator, begin(), end()
private:
    unsigned int    m_block_size;
    unsigned int    m_size;
    std::vector<T*> m_data;
};

} // namespace HepMC3

#endif
