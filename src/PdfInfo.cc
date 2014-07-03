/**
 *  @file PdfInfo.cc
 *  @brief Implementation of \b class PdfInfo
 *
 */
#include "HepMC3/PdfInfo.h"

namespace HepMC3 {

PdfInfo::PdfInfo():
m_id1(0),
m_id2(0),
m_pdf_id1(0),
m_pdf_id2(0),
m_x1(0),
m_x2(0),
m_scalePDF(0),
m_pdf1(0),
m_pdf2(0) {
}

PdfInfo::PdfInfo( int i1, int i2, double xin1, double xin2,
                  double q, double p1, double p2,
                  int pid1, int pid2 ):
m_id1(i1),
m_id2(i2),
m_pdf_id1(pid1),
m_pdf_id2(pid2),
m_x1(xin1),
m_x2(xin2),
m_scalePDF(q),
m_pdf1(p1),
m_pdf2(p2) {
}

PdfInfo::PdfInfo( const PdfInfo &orig ):
m_id1(orig.m_id1),
m_id2(orig.m_id2),
m_pdf_id1(orig.m_pdf_id1),
m_pdf_id2(orig.m_pdf_id2),
m_x1(orig.m_x1),
m_x2(orig.m_x2),
m_scalePDF(orig.m_scalePDF),
m_pdf1(orig.m_pdf1),
m_pdf2(orig.m_pdf2) {
}

PdfInfo& PdfInfo::operator=( PdfInfo const & rhs ) {
    m_id1      = rhs.m_id1;
    m_id2      = rhs.m_id2;
    m_pdf_id1  = rhs.m_pdf_id1;
    m_pdf_id2  = rhs.m_pdf_id2;
    m_x1       = rhs.m_x1;
    m_x2       = rhs.m_x2;
    m_scalePDF = rhs.m_scalePDF;
    m_pdf1     = rhs.m_pdf1;
    m_pdf2     = rhs.m_pdf2;
    return *this;
}

bool PdfInfo::operator==( const PdfInfo& a ) const {
    return (    a.id1()      == this->id1()
             && a.id2()      == this->id2()
             && a.pdf_id1()  == this->pdf_id1()
             && a.pdf_id2()  == this->pdf_id2()
             && a.x1()       == this->x1()
             && a.x2()       == this->x2()
             && a.scalePDF() == this->scalePDF()
             && a.pdf1()     == this->pdf1()
             && a.pdf2()     == this->pdf2() );
}

bool PdfInfo::operator!=( const PdfInfo& a ) const {
    return !( a == *this );
}

bool PdfInfo::is_valid() const
{
    if( m_id1      != 0 ) return true;
    if( m_id2      != 0 ) return true;
    if( m_pdf_id1  != 0 ) return true;
    if( m_pdf_id2  != 0 ) return true;
    if( m_x1       != 0 ) return true;
    if( m_x2       != 0 ) return true;
    if( m_scalePDF != 0 ) return true;
    if( m_pdf1     != 0 ) return true;
    if( m_pdf2     != 0 ) return true;

    return false;
}

void PdfInfo::print( std::ostream & ostr) const {
    ostr << "PdfInfo: " << id1()
         << " " << id2()
         << " " << x1()
         << " " << x2()
         << " " << scalePDF()
         << " " << pdf1()
         << " " << pdf2()
         << " " << pdf_id1()
         << " " << pdf_id2()
         << " " << std::endl;
}

} // namespace HepMC3
