#ifndef  HEPMC_PDFINFO_H
#define  HEPMC_PDFINFO_H
/**
 *  @file PdfInfo.h
 *  @brief Definition of \b struct PdfInfo
 *
 *  @struct HepMC::PdfInfo
 *  @brief Stores additional information about PDFs
 *
 *  Creation and use of this information is optional
 *
 *  @note Input parton flavour codes id1 & id2 are expected to obey the
 *        PDG code conventions, especially g = 21.
 *
 *  @note The contents of pdf1 and pdf2 are expected to be x*f(x).
 *        The LHAPDF set ids are the entries in the first column of
 *        http:///projects.hepforge.org/lhapdf/PDFsets.index
 */
#include <iostream>

namespace HepMC {

struct PdfInfo {

//
// Fields
//
public:
    int    id1;
    int    id2;
    int    pdf_id1;
    int    pdf_id2;
    double x1;
    double x2;
    double scalePDF;
    double pdf1;
    double pdf2;

//
// Functions
//
public:
    void set( int i1, int i2, double x1, double x2,
              double q, double p1, double p2,
              int pdf_id1 = 0, int pdf_id2 = 0 ); //!< Set all fields

    bool      operator==( const PdfInfo& ) const; //!< Operator ==
    bool      operator!=( const PdfInfo& ) const; //!< Operator !=
    bool      is_valid()                   const; //!< Verify that the instance contains non-zero information

    void      print(std::ostream &ostr)    const; //!< Print the contents to output stream
};

} // namespace HepMC

#endif
