#ifndef  HEPMC3_PDFINFO_H
#define  HEPMC3_PDFINFO_H
/**
 *  @file PdfInfo.h
 *  @brief Definition of \b class PdfInfo
 *
 *  @class HepMC3::PdfInfo
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

namespace HepMC3 {

class PdfInfo {
//
// Constructors
//
public:

    /** @brief Default constructor */
    PdfInfo();

    /** @brief Constructor providing  all values except pdf_id1 and pdf_id2 */
    PdfInfo( int i1, int i2, double x1, double x2,
           double q, double p1, double p2,
           int pdf_id1 = 0, int pdf_id2 = 0 );

    /** @brief Copy constructor */
    PdfInfo( PdfInfo const & orig );

//
// Functions
//
public:
    PdfInfo&  operator=( PdfInfo const & rhs );   //!< Assignment operator
    bool      operator==( const PdfInfo& ) const; //!< Operator ==
    bool      operator!=( const PdfInfo& ) const; //!< Operator !=
    bool      is_valid()                   const; //!< Verify that the instance contains non-zero information

    void      print(std::ostream &ostr)    const; //!< Print the contents to output stream
//
// Accessors
//
public:
    int    id1()                   const { return m_id1; }       //!< Get flavour code of first parton
    void   set_id1(const int &i)         { m_id1 = i;    }       //!< Set flavour code of first parton

    int    id2()                   const { return m_id2; }       //!< Get flavour code of second parton
    void   set_id2(const int &i)         { m_id2 = i;    }       //!< Set flavour code of second parton

    int    pdf_id1()               const { return m_pdf_id1; }   //!< Get LHAPDF set id of first parton
    void   set_pdf_id1(const int &i)     { m_pdf_id1 = i;    }   //!< Set LHAPDF set id of first parton

    int    pdf_id2()               const { return m_pdf_id2; }   //!< Get LHAPDF set id of second parton
    void   set_pdf_id2(const int &i)     { m_pdf_id2 = i;    }   //!< Set LHAPDF set id of second parton

    double x1()                    const { return m_x1; }        //!< Get fraction of beam momentum carried by first parton ("beam side")
    void   set_x1(const double &f)       { m_x1 = f;    }        //!< Set fraction of beam momentum carried by first parton ("beam side")

    double x2()                    const { return m_x2; }        //!< Get fraction of beam momentum carried by second parton ("target side")
    void   set_x2(const double &f)       { m_x2 = f;    }        //!< Set fraction of beam momentum carried by second parton ("target side")

    double scalePDF()              const { return m_scalePDF;  } //!< Get Q-scale used in evaluation of PDF's (in GeV)
    void   set_scalePDF(const double &f) { m_scalePDF = f;     } //!< Set Q-scale used in evaluation of PDF's (in GeV)

    double pdf1()                  const { return m_pdf1; }      //!< Get PDF (id1, x1, Q) - x*f(x)
    void   set_pdf1(const double &f)     { m_pdf1 = f;    }      //!< Set x*f(x) of first parton

    double pdf2()                  const { return m_pdf2; }      //!< Get PDF (id2, x2, Q) - x*f(x)
    void   set_pdf2(const double &f)     { m_pdf2 = f;    }      //!< Set x*f(x) of second parton

//
// Fields
//
private:
    int    m_id1;
    int    m_id2;
    int    m_pdf_id1;
    int    m_pdf_id2;
    double m_x1;
    double m_x2;
    double m_scalePDF;
    double m_pdf1;
    double m_pdf2;

};

} // namespace HepMC3

#endif
