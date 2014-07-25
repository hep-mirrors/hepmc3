// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Analysis.hh"
#include "Rivet/AnalysisHandler.hh"
#include "Rivet/AnalysisInfo.hh"
#include "Rivet/BeamConstraint.hh"
#include "Rivet/Tools/RivetYODA.hh"
#include "Rivet/Tools/Logging.hh"

namespace Rivet {


  Analysis::Analysis(const string& name)
    : _crossSection(-1.0),
      _gotCrossSection(false),
      _analysishandler(NULL)
  {
    ProjectionApplier::_allowProjReg = false;
    _defaultname = name;

    AnalysisInfo* ai = AnalysisInfo::make(name);
    assert(ai != 0);
    _info.reset(ai);
    assert(_info.get() != 0);
  }

  double Analysis::sqrtS() const {
    return handler().sqrtS();
  }

  const ParticlePair& Analysis::beams() const {
    return handler().beams();
  }

  const PdgIdPair Analysis::beamIds() const {
    return handler().beamIds();
  }


  const string Analysis::histoDir() const {
    /// @todo Cache in a member variable
    string _histoDir;
    if (_histoDir.empty()) {
      _histoDir = "/" + name();
      if (handler().runName().length() > 0) {
        _histoDir = "/" + handler().runName() + _histoDir;
      }
      while (find_first(_histoDir, "//")) {
        replace_all(_histoDir, "//", "/");
      }
    }
    return _histoDir;
  }


  const string Analysis::histoPath(const string& hname) const {
    const string path = histoDir() + "/" + hname;
    return path;
  }


  const string Analysis::histoPath(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
    return histoDir() + "/" + makeAxisCode(datasetId, xAxisId, yAxisId);
  }


  const string Analysis::makeAxisCode(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
    stringstream axisCode;
    axisCode << "d";
    if (datasetId < 10) axisCode << 0;
    axisCode << datasetId;
    axisCode << "-x";
    if (xAxisId < 10) axisCode << 0;
    axisCode << xAxisId;
    axisCode << "-y";
    if (yAxisId < 10) axisCode << 0;
    axisCode << yAxisId;
    return axisCode.str();
  }


  Log& Analysis::getLog() const {
    string logname = "Rivet.Analysis." + name();
    return Log::getLog(logname);
  }


  size_t Analysis::numEvents() const {
    return handler().numEvents();
  }


  double Analysis::sumOfWeights() const {
    return handler().sumOfWeights();
  }


  ///////////////////////////////////////////


  bool Analysis::isCompatible(const ParticlePair& beams) const {
    return isCompatible(beams.first.pdgId(),  beams.second.pdgId(),
                        beams.first.energy(), beams.second.energy());
  }


  bool Analysis::isCompatible(PdgId beam1, PdgId beam2, double e1, double e2) const {
    PdgIdPair beams(beam1, beam2);
    pair<double,double> energies(e1, e2);
    return isCompatible(beams, energies);
  }


  bool Analysis::isCompatible(const PdgIdPair& beams, const pair<double,double>& energies) const {
    // First check the beam IDs
    bool beamIdsOk = false;
    foreach (const PdgIdPair& bp, requiredBeams()) {
      if (compatible(beams, bp)) {
        beamIdsOk =  true;
        break;
      }
    }
    if (!beamIdsOk) return false;

    // Next check that the energies are compatible (within 1% or 1 GeV, whichever is larger, for a bit of UI forgiveness)

    /// @todo Use some sort of standard ordering to improve comparisons, esp. when the two beams are different particles
    bool beamEnergiesOk = requiredEnergies().size() > 0 ? false : true;
    typedef pair<double,double> DoublePair;
    foreach (const DoublePair& ep, requiredEnergies()) {
      if ((fuzzyEquals(ep.first, energies.first, 0.01) && fuzzyEquals(ep.second, energies.second, 0.01)) ||
          (fuzzyEquals(ep.first, energies.second, 0.01) && fuzzyEquals(ep.second, energies.first, 0.01)) ||
          (abs(ep.first - energies.first) < 1*GeV && abs(ep.second - energies.second) < 1*GeV) ||
          (abs(ep.first - energies.second) < 1*GeV && abs(ep.second - energies.first) < 1*GeV)) {
        beamEnergiesOk =  true;
        break;
      }
    }
    return beamEnergiesOk;

    /// @todo Need to also check internal consistency of the analysis'
    /// beam requirements with those of the projections it uses.
  }


  ///////////////////////////////////////////


  Analysis& Analysis::setCrossSection(double xs) {
    _crossSection = xs;
    _gotCrossSection = true;
    return *this;
  }

  double Analysis::crossSection() const {
    if (!_gotCrossSection || std::isnan(_crossSection)) {
      string errMsg = "You did not set the cross section for the analysis " + name();
      throw Error(errMsg);
    }
    return _crossSection;
  }

  double Analysis::crossSectionPerEvent() const {
    const double sumW = sumOfWeights();
    assert(sumW != 0.0);
    return _crossSection / sumW;
  }



  ////////////////////////////////////////////////////////////
  // Histogramming


  void Analysis::_cacheRefData() const {
    if (_refdata.empty()) {
      MSG_TRACE("Getting refdata cache for paper " << name());
      _refdata = getRefData(name());
    }
  }


  const Scatter2D& Analysis::refData(const string& hname) const {
    _cacheRefData();
    MSG_TRACE("Using histo bin edges for " << name() << ":" << hname);
    if (!_refdata[hname]) {
      MSG_ERROR("Can't find reference histogram " << hname);
    }
    return *_refdata[hname];
  }


  const Scatter2D& Analysis::refData(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
    const string hname = makeAxisCode(datasetId, xAxisId, yAxisId);
    return refData(hname);
  }


  Histo1DPtr Analysis::bookHisto1D(const string& hname,
                                   size_t nbins, double lower, double upper,
                                   const string& title,
                                   const string& xtitle,
                                   const string& ytitle) {
    const string path = histoPath(hname);
    Histo1DPtr hist( new Histo1D(nbins, lower, upper, path, title) );
    addAnalysisObject(hist);
    MSG_TRACE("Made histogram " << hname <<  " for " << name());
    hist->setAnnotation("XLabel", xtitle);
    hist->setAnnotation("YLabel", ytitle);
    return hist;
  }


  Histo1DPtr Analysis::bookHisto1D(const string& hname,
                                   const vector<double>& binedges,
                                   const string& title,
                                   const string& xtitle,
                                   const string& ytitle) {
    const string path = histoPath(hname);
    Histo1DPtr hist( new Histo1D(binedges, path, title) );
    addAnalysisObject(hist);
    MSG_TRACE("Made histogram " << hname <<  " for " << name());
    hist->setAnnotation("XLabel", xtitle);
    hist->setAnnotation("YLabel", ytitle);
    return hist;
  }


  Histo1DPtr Analysis::bookHisto1D(const string& hname,
                                   const Scatter2D& refscatter,
                                   const string& title,
                                   const string& xtitle,
                                   const string& ytitle) {
    const string path = histoPath(hname);
    Histo1DPtr hist( new Histo1D(refscatter, path) );
    addAnalysisObject(hist);
    MSG_TRACE("Made histogram " << hname <<  " for " << name());
    hist->setTitle(title);
    hist->setAnnotation("XLabel", xtitle);
    hist->setAnnotation("YLabel", ytitle);
    return hist;
  }


  Histo1DPtr Analysis::bookHisto1D(const string& hname,
                                   const string& title,
                                   const string& xtitle,
                                   const string& ytitle) {
    const Scatter2D& refdata = refData(hname);
    return bookHisto1D(hname, refdata, title, xtitle, ytitle);
  }


  Histo1DPtr Analysis::bookHisto1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                                   const string& title,
                                   const string& xtitle,
                                   const string& ytitle) {
    const string axisCode = makeAxisCode(datasetId, xAxisId, yAxisId);
    return bookHisto1D(axisCode, title, xtitle, ytitle);
  }


  /// @todo Add booking methods which take a path, titles and *a reference Scatter from which to book*


  /////////////////


  // Histo2DPtr Analysis::bookHisto2D(const string& hname,
  //                                  size_t nxbins, double xlower, double xupper,
  //                                  size_t nybins, double ylower, double yupper,
  //                                  const string& title,
  //                                  const string& xtitle,
  //                                  const string& ytitle,
  //                                  const string& ztitle)
  // {
  //   const string path = histoPath(hname);
  //   Histo2DPtr hist( new Histo2D(path, nxbins, xlower, xupper, nybins, ylower, yupper) );
  //   addAnalysisObject(hist);
  //   MSG_TRACE("Made histogram " << hname <<  " for " << name());
  //   hist->setTitle(title);
  //   hist->setAnnotation("XLabel", xtitle);
  //   hist->setAnnotation("YLabel", ytitle);
  //   hist->setAnnotation("ZLabel", ztitle);
  //   return hist;
  // }


  // Histo2DPtr Analysis::bookHisto2D(const string& hname,
  //                                  const vector<double>& xbinedges,
  //                                  const vector<double>& ybinedges,
  //                                  const string& title,
  //                                  const string& xtitle,
  //                                  const string& ytitle,
  //                                  const string& ztitle)
  // {
  //   const string path = histoPath(hname);
  //   Histo2DPtr hist( new Histo2D(path, xbinedges, ybinedges) );
  //   addAnalysisObject(hist);
  //   MSG_TRACE("Made histogram " << hname <<  " for " << name());
  //   hist->setTitle(title);
  //   hist->setAnnotation("XLabel", xtitle);
  //   hist->setAnnotation("YLabel", ytitle);
  //   hist->setAnnotation("ZLabel", ztitle);
  //   return hist;
  // }


  // Histo2DPtr Analysis::bookHisto2D(const string& hname,
  //                                  const Scatter3D& refscatter,
  //                                  const string& title="",
  //                                  const string& xtitle="",
  //                                  const string& ytitle="",
  //                                  const string& ztitle="") {
  //   const string path = histoPath(hname);
  //   Histo2DPtr hist( new Histo2D(refscatter, path) );
  //   addAnalysisObject(hist);
  //   MSG_TRACE("Made histogram " << hname <<  " for " << name());
  //   hist->setTitle(title);
  //   hist->setAnnotation("XLabel", xtitle);
  //   hist->setAnnotation("YLabel", ytitle);
  //   hist->setAnnotation("ZLabel", ztitle);
  //   return hist;
  // }


  // Histo2DPtr Analysis::bookHisto2D(const string& hname,
  //                                  const string& title,
  //                                  const string& xtitle,
  //                                  const string& ytitle,
  //                                  const string& ztitle) {
  //   const Scatter3D& refdata = refData(hname);
  //   return bookHisto2D(hname, refdata, title, xtitle, ytitle, ztitle);
  // }


  // Histo2DPtr Analysis::bookHisto2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
  //                                  const string& title,
  //                                  const string& xtitle,
  //                                  const string& ytitle,
  //                                  const string& ztitle) {
  //   const string axisCode = makeAxisCode(datasetId, xAxisId, yAxisId);
  //   return bookHisto2D(axisCode, title, xtitle, ytitle, ztitle);
  // }


  /////////////////


  Profile1DPtr Analysis::bookProfile1D(const string& hname,
                                       size_t nbins, double lower, double upper,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string path = histoPath(hname);
    Profile1DPtr prof( new Profile1D(nbins, lower, upper, path, title) );
    addAnalysisObject(prof);
    MSG_TRACE("Made profile histogram " << hname <<  " for " << name());
    prof->setAnnotation("XLabel", xtitle);
    prof->setAnnotation("YLabel", ytitle);
    return prof;
  }


  Profile1DPtr Analysis::bookProfile1D(const string& hname,
                                       const vector<double>& binedges,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string path = histoPath(hname);
    Profile1DPtr prof( new Profile1D(binedges, path, title) );
    addAnalysisObject(prof);
    MSG_TRACE("Made profile histogram " << hname <<  " for " << name());
    prof->setAnnotation("XLabel", xtitle);
    prof->setAnnotation("YLabel", ytitle);
    return prof;
  }


  Profile1DPtr Analysis::bookProfile1D(const string& hname,
                                       const Scatter2D& refscatter,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string path = histoPath(hname);
    Profile1DPtr prof( new Profile1D(refscatter, path) );
    addAnalysisObject(prof);
    MSG_TRACE("Made profile histogram " << hname <<  " for " << name());
    prof->setTitle(title);
    prof->setAnnotation("XLabel", xtitle);
    prof->setAnnotation("YLabel", ytitle);
    return prof;
  }


  Profile1DPtr Analysis::bookProfile1D(const string& hname,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const Scatter2D& refdata = refData(hname);
    return bookProfile1D(hname, refdata, title, xtitle, ytitle);
  }


  Profile1DPtr Analysis::bookProfile1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string axisCode = makeAxisCode(datasetId, xAxisId, yAxisId);
    return bookProfile1D(axisCode, title, xtitle, ytitle);
  }


  ///////////////////


  Scatter2DPtr Analysis::bookScatter2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                                       bool copy_pts,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string axisCode = makeAxisCode(datasetId, xAxisId, yAxisId);
    return bookScatter2D(axisCode, copy_pts, title, xtitle, ytitle);
  }


  Scatter2DPtr Analysis::bookScatter2D(const string& hname,
                                       bool copy_pts,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    Scatter2DPtr s;
    const string path = histoPath(hname);
    if (copy_pts) {
      const Scatter2D& refdata = refData(hname);
      s.reset( new Scatter2D(refdata, path) );
      foreach (Point2D& p, s->points()) p.setY(0, 0);
    } else {
      s.reset( new Scatter2D(path) );
    }
    addAnalysisObject(s);
    MSG_TRACE("Made scatter " << hname <<  " for " << name());
    s->setTitle(title);
    s->setAnnotation("XLabel", xtitle);
    s->setAnnotation("YLabel", ytitle);
    return s;
  }


  Scatter2DPtr Analysis::bookScatter2D(const string& hname,
                                       size_t npts, double lower, double upper,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string path = histoPath(hname);
    Scatter2DPtr s( new Scatter2D(path) );
    const double binwidth = (upper-lower)/npts;
    for (size_t pt = 0; pt < npts; ++pt) {
      const double bincentre = lower + (pt + 0.5) * binwidth;
      s->addPoint(bincentre, 0, binwidth/2.0, 0);
    }
    addAnalysisObject(s);
    MSG_TRACE("Made scatter " << hname <<  " for " << name());
    s->setTitle(title);
    s->setAnnotation("XLabel", xtitle);
    s->setAnnotation("YLabel", ytitle);
    return s;
  }


  Scatter2DPtr Analysis::bookScatter2D(const string& hname,
                                       const vector<double>& binedges,
                                       const string& title,
                                       const string& xtitle,
                                       const string& ytitle) {
    const string path = histoPath(hname);
    Scatter2DPtr s( new Scatter2D(path) );
    for (size_t pt = 0; pt < binedges.size()-1; ++pt) {
      const double bincentre = (binedges[pt] + binedges[pt+1]) / 2.0;
      const double binwidth = binedges[pt+1] - binedges[pt];
      s->addPoint(bincentre, 0, binwidth/2.0, 0);
    }
    addAnalysisObject(s);
    MSG_TRACE("Made scatter " << hname <<  " for " << name());
    s->setTitle(title);
    s->setAnnotation("XLabel", xtitle);
    s->setAnnotation("YLabel", ytitle);
    return s;
  }


  /////////////////////


  void Analysis::divide(Histo1DPtr h1, Histo1DPtr h2, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = *h1 / *h2;
    s->setPath(path);
  }

  void Analysis::divide(Profile1DPtr p1, Profile1DPtr p2, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = *p1 / *p2;
    s->setPath(path);
  }

  void Analysis::divide(const Histo1D& h1, const Histo1D& h2, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = h1 / h2;
    s->setPath(path);
  }

  void Analysis::divide(const Profile1D& p1, const Profile1D& p2, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = p1 / p2;
    s->setPath(path);
  }

  void Analysis::normalize(Histo1DPtr histo, double norm, bool includeoverflows) {
    if (!histo) {
      MSG_ERROR("Failed to normalize histo=NULL in analysis " << name() << " (norm=" << norm << ")");
      return;
    }
    MSG_TRACE("Normalizing histo " << histo->path() << " to " << norm);
    try {
      histo->normalize(norm, includeoverflows);
    } catch (YODA::Exception& we) {
      MSG_WARNING("Could not normalize histo " << histo->path());
      return;
    }
  }


  void Analysis::scale(Histo1DPtr histo, double scale) {
    if (!histo) {
      MSG_ERROR("Failed to scale histo=NULL in analysis " << name() << " (scale=" << scale << ")");
      return;
    }
    if (std::isnan(scale) || std::isinf(scale)) {
      MSG_ERROR("Failed to scale histo=" << histo->path() << " in analysis: " << name() << " (invalid scale factor = " << scale << ")");
      scale = 0;
    }
    MSG_TRACE("Scaling histo " << histo->path() << " by factor " << scale);
    try {
      histo->scaleW(scale);
    } catch (YODA::Exception& we) {
      MSG_WARNING("Could not scale histo " << histo->path());
      return;
    }
    // // Transforming the histo into a scatter after scaling
    // vector<double> x, y, ex, ey;
    // for (size_t i = 0, N = histo->numBins(); i < N; ++i) {
    //   x.push_back( histo->bin(i).midpoint() );
    //   ex.push_back(histo->bin(i).width()*0.5);
    //   y.push_back(histo->bin(i).height()*scale);
    //   ey.push_back(histo->bin(i).heightErr()*scale);
    // }
    // string title = histo->title();
    // Scatter2DPtr dps( new Scatter2D(x, y, ex, ey, hpath, title) );
    // addAnalysisObject(dps);
  }


  /// @todo 2D versions of scale and normalize...


  void Analysis::integrate(Histo1DPtr h, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = toIntegralHisto(*h);
    s->setPath(path);
  }

  void Analysis::integrate(const Histo1D& h, Scatter2DPtr s) const {
    // preserve the path info
    const string path = s->path();
    *s = toIntegralHisto(h);
    s->setPath(path);
  }


  /// @todo 2D versions of integrate... defined how, exactly?!?


  //////////////////////////////////


  void Analysis::addAnalysisObject(AnalysisObjectPtr ao) {
    _analysisobjects.push_back(ao);
  }

  void Analysis::removeAnalysisObject(const string& path) {
    for (vector<AnalysisObjectPtr>::iterator it = _analysisobjects.begin();  it != _analysisobjects.end(); ++it) {
      if ((*it)->path() == path) {
        _analysisobjects.erase(it);
        break;
      }
    }
  }

  void Analysis::removeAnalysisObject(AnalysisObjectPtr ao) {
    for (vector<AnalysisObjectPtr>::iterator it = _analysisobjects.begin();  it != _analysisobjects.end(); ++it) {
      if (*it == ao) {
        _analysisobjects.erase(it);
        break;
      }
    }
 }


}
