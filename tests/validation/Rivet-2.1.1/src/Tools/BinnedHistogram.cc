// -*- C++ -*-
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/Tools/RivetYODA.hh"
#include "Rivet/Analysis.hh"

namespace Rivet {


  template<typename T>
  const BinnedHistogram<T>& BinnedHistogram<T>::addHistogram(const T& binMin,
                                                             const T& binMax,
                                                             Histo1DPtr histo){
    if (binMin > binMax) {
      throw Error
        ("Cannot add a binned histogram where the lower bin edge is above the upper edge");
    }
    _histosByUpperBound[binMax] = histo;
    _histosByLowerBound[binMin] = histo;
    bool found = false;
    foreach (Histo1DPtr hist, _histos) {
      if (hist == histo) {
        found = true;
        break;
      }
    }

    if (!found){
      _histos.push_back(histo);
      _binWidths[histo]=binMax-binMin;
    }

    return *this;
  }



  template<typename T>
  Histo1DPtr BinnedHistogram<T>::fill(const T& bin,
                                                     const T& val,
                                                     double weight) {

    typename map<T, Histo1DPtr>::iterator histIt =
      _histosByUpperBound.upper_bound(bin);
    //check that the bin is not out of range
    if (histIt == _histosByUpperBound.end()) {
      return Histo1DPtr();
    }

    Histo1DPtr histo = histIt->second;
    histIt = _histosByLowerBound.lower_bound(bin);

    // No need to check going beyond the upper bound if we already passed above
    // (given that upper bound > lower bound is checked)
    // Check it is not before the start of the map
    if (histIt == _histosByLowerBound.begin()) {
      return Histo1DPtr();
    }
    // By-lower-bound actually gives us the iterator one above the nearest element,
    // so decrement it. This is safe because we already checked we're not at the start!
    --histIt;

    if (histo != histIt->second) {
      return Histo1DPtr();
    }

    histo->fill(val, weight);

    return histo;
  }


  template<typename T>
  void BinnedHistogram<T>::scale(const T& scale, Analysis* ana) {
    foreach (Histo1DPtr hist, getHistograms()) {
      ana->scale(hist, scale/_binWidths[hist]);
    }
  }



  // Template declarations for the compiler.
  template class BinnedHistogram<double>;
  template class BinnedHistogram<int>;
  template class BinnedHistogram<float>;


}
