/**
 *  @file PdfInfo.cc
 *  @brief Implementation of \b struct PdfInfo
 *
 */
#include "HepMC3/PdfInfo.h"

namespace HepMC3 {

void PdfInfo::set( int i1, int i2, double xin1, double xin2,
                   double q, double p1, double p2,
                   int pid1, int pid2 ) {
    id1      = i1;
    id2      = i2;
    x1       = xin1;
    x2       = xin2;
    scalePDF = q;
    pdf1     = p1;
    pdf2     = p2;
    pdf_id1  = pid1;
    pdf_id2  = pid2;
}

bool PdfInfo::operator==( const PdfInfo& a ) const {
    return (    a.id1      == this->id1
             && a.id2      == this->id2
             && a.x1       == this->x1
             && a.x2       == this->x2
             && a.scalePDF == this->scalePDF
             && a.pdf1     == this->pdf1
             && a.pdf2     == this->pdf2
             && a.pdf_id1  == this->pdf_id1
             && a.pdf_id2  == this->pdf_id2 );
}

bool PdfInfo::operator!=( const PdfInfo& a ) const {
    return !( a == *this );
}

bool PdfInfo::is_valid() const
{
    if( id1      != 0 ) return true;
    if( id2      != 0 ) return true;
    if( x1       != 0 ) return true;
    if( x2       != 0 ) return true;
    if( scalePDF != 0 ) return true;
    if( pdf1     != 0 ) return true;
    if( pdf2     != 0 ) return true;
    if( pdf_id1  != 0 ) return true;
    if( pdf_id2  != 0 ) return true;

    return false;
}

void PdfInfo::print( std::ostream & ostr) const {
    ostr << "PdfInfo: " << id1
         << " " << id2
         << " " << x1
         << " " << x2
         << " " << scalePDF
         << " " << pdf1
         << " " << pdf2
         << " " << pdf_id1
         << " " << pdf_id2
         << " " << std::endl;
}

} // namespace HepMC3
