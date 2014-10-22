#ifndef  HEPMC_GENPDFINFO_H
#define  HEPMC_GENPDFINFO_H
/**
 *  @file GenPdfInfo.h
 *  @brief Definition of \b struct GenPdfInfo
 *
 *  @struct HepMC::GenPdfInfo
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

struct GenPdfInfo {

//
// Fields
//
public:
    int    parton_id[2];
    int    pdf_id[2];
    double scale;
    double x[2];
    double xf[2];

//
// Functions
//
public:
    void set( int parton_id1, int parton_id2, double x1, double x2,
              double scale_in, double xf1, double xf2,
              int pdf_id1 = 0, int pdf_id2 = 0 ); ///< Set all fields

    bool operator==( const GenPdfInfo& ) const; ///< Operator ==
    bool operator!=( const GenPdfInfo& ) const; ///< Operator !=
    bool is_valid()                      const; ///< Verify that the instance contains non-zero information

    void print(std::ostream &ostr = std::cout) const; ///< Print the contents to output stream
};

} // namespace HepMC

#endif
