// -*- C++ -*-
#ifndef RIVET_BINNEDHISTOGRAM_HH
#define RIVET_BINNEDHISTOGRAM_HH
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/RivetYODA.hh"

namespace Rivet {

  class Analysis;


  /**
   * BinnedHistogram contains a series of histograms of the same quantity
   * each in a different region of a second quantity.  For example, a
   * BinnedHistogram may contain histograms of the cross section differential
   * in \f$ p_T \f$ in different \f$ \eta \f$  regions.
   **/
  template<typename T>
  class BinnedHistogram {
  public:

    /// Create a new empty BinnedHistogram
    BinnedHistogram() {
      return;
    }

    ///  Add a histogram in the region between @a binMin and @a binMax to this
    ///  set of BinnedHistograms.
    const BinnedHistogram<T>& addHistogram(const T& binMin,
                                           const T& binMax,
                                           Histo1DPtr histo);

    /// Fill the histogram that lies in the same region as @a bin with the value
    /// @a val of weight @a weight.
    Histo1DPtr fill(const T& bin,
                             const T& val,
                             double weight);

    /// Scale histograms taking into account its "external" binwidth, i.e. by
    /// scale/binWidth
    void scale(const T& scale, Analysis* ana);

    const vector<Histo1DPtr>& getHistograms() const { return _histos; }
    vector<Histo1DPtr>& getHistograms() { return _histos; }


  private:

    map<T, Histo1DPtr> _histosByUpperBound;
    map<T, Histo1DPtr> _histosByLowerBound;
    vector<Histo1DPtr> _histos;
    map<Histo1DPtr, T> _binWidths;

  };


}

#endif
