// -*- C++ -*-
#ifndef RIVET_Analysis_HH
#define RIVET_Analysis_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/AnalysisInfo.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/ProjectionApplier.hh"
#include "Rivet/ProjectionHandler.hh"
#include "Rivet/AnalysisLoader.hh"
#include "Rivet/Tools/RivetYODA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Tools/ParticleUtils.hh"


/// @def vetoEvent
/// Preprocessor define for vetoing events, including the log message and return.
#define vetoEvent                                                       \
  do { MSG_DEBUG("Vetoing event on line " << __LINE__ << " of " << __FILE__); return; } while(0)


namespace Rivet {


  // Forward declaration
  class AnalysisHandler;

  /// @brief This is the base class of all analysis classes in Rivet.
  ///
  /// There are
  /// three virtual functions which should be implemented in base classes:
  ///
  /// void init() is called by Rivet before a run is started. Here the
  /// analysis class should book necessary histograms. The needed
  /// projections should probably rather be constructed in the
  /// constructor.
  ///
  /// void analyze(const Event&) is called once for each event. Here the
  /// analysis class should apply the necessary Projections and fill the
  /// histograms.
  ///
  /// void finalize() is called after a run is finished. Here the analysis
  /// class should do whatever manipulations are necessary on the
  /// histograms. Writing the histograms to a file is, however, done by
  /// the Rivet class.
  class Analysis : public ProjectionApplier {

    /// The AnalysisHandler is a friend.
    friend class AnalysisHandler;


  public:

    /// @name Standard constructors and destructors.
    //@{

    // /// The default constructor.
    // Analysis();

    /// Constructor
    Analysis(const std::string& name);

    /// The destructor.
    virtual ~Analysis() {}

    //@}


  public:

    /// @name Main analysis methods
    //@{

    /// Initialize this analysis object. A concrete class should here
    /// book all necessary histograms. An overridden function must make
    /// sure it first calls the base class function.
    virtual void init() { }

    /// Analyze one event. A concrete class should here apply the
    /// necessary projections on the \a event and fill the relevant
    /// histograms. An overridden function must make sure it first calls
    /// the base class function.
    virtual void analyze(const Event& event) = 0;

    /// Finalize this analysis object. A concrete class should here make
    /// all necessary operations on the histograms. Writing the
    /// histograms to a file is, however, done by the Rivet class. An
    /// overridden function must make sure it first calls the base class
    /// function.
    virtual void finalize() { }

    //@}


  public:

    /// @name Metadata
    /// Metadata is used for querying from the command line and also for
    /// building web pages and the analysis pages in the Rivet manual.
    //@{

    /// Get the actual AnalysisInfo object in which all this metadata is stored.
    const AnalysisInfo& info() const {
      assert(_info.get() != 0 && "No AnalysisInfo object :O");
      return *_info;
    }

    /// @brief Get the name of the analysis.
    ///
    /// By default this is computed by combining the results of the experiment,
    /// year and Spires ID metadata methods and you should only override it if
    /// there's a good reason why those won't work.
    virtual std::string name() const {
      return (info().name().empty()) ? _defaultname : info().name();
    }

    /// Get the Inspire ID code for this analysis.
    virtual std::string inspireId() const {
      return info().inspireId();
    }

    /// Get the SPIRES ID code for this analysis (~deprecated).
    virtual std::string spiresId() const {
      return info().spiresId();
    }

    /// @brief Names & emails of paper/analysis authors.
    ///
    /// Names and email of authors in 'NAME \<EMAIL\>' format. The first
    /// name in the list should be the primary contact person.
    virtual std::vector<std::string> authors() const {
      return info().authors();
    }

    /// @brief Get a short description of the analysis.
    ///
    /// Short (one sentence) description used as an index entry.
    /// Use @a description() to provide full descriptive paragraphs
    /// of analysis details.
    virtual std::string summary() const {
      return info().summary();
    }

    /// @brief Get a full description of the analysis.
    ///
    /// Full textual description of this analysis, what it is useful for,
    /// what experimental techniques are applied, etc. Should be treated
    /// as a chunk of restructuredText (http://docutils.sourceforge.net/rst.html),
    /// with equations to be rendered as LaTeX with amsmath operators.
    virtual std::string description() const {
      return info().description();
    }

    /// @brief Information about the events needed as input for this analysis.
    ///
    /// Event types, energies, kinematic cuts, particles to be considered
    /// stable, etc. etc. Should be treated as a restructuredText bullet list
    /// (http://docutils.sourceforge.net/rst.html)
    virtual std::string runInfo() const {
      return info().runInfo();
    }

    /// Experiment which performed and published this analysis.
    virtual std::string experiment() const {
      return info().experiment();
    }

    /// Collider on which the experiment ran.
    virtual std::string collider() const {
      return info().collider();
    }

    /// When the original experimental analysis was published.
    virtual std::string year() const {
      return info().year();
    }

    /// Journal, and preprint references.
    virtual std::vector<std::string> references() const {
      return info().references();
    }

    /// BibTeX citation key for this article.
    virtual std::string bibKey() const {
      return info().bibKey();
    }

    /// BibTeX citation entry for this article.
    virtual std::string bibTeX() const {
      return info().bibTeX();
    }

    /// Whether this analysis is trusted (in any way!)
    virtual std::string status() const {
      return (info().status().empty()) ? "UNVALIDATED" : info().status();
    }

    /// Any work to be done on this analysis.
    virtual std::vector<std::string> todos() const {
      return info().todos();
    }


    /// Return the allowed pairs of incoming beams required by this analysis.
    virtual const std::vector<PdgIdPair>& requiredBeams() const {
      return info().beams();
    }
    /// Declare the allowed pairs of incoming beams required by this analysis.
    virtual Analysis& setRequiredBeams(const std::vector<PdgIdPair>& requiredBeams) {
      info().setBeams(requiredBeams);
      return *this;
    }


    /// Sets of valid beam energy pairs, in GeV
    virtual const std::vector<std::pair<double, double> >& requiredEnergies() const {
      return info().energies();
    }
    /// Declare the list of valid beam energy pairs, in GeV
    virtual Analysis& setRequiredEnergies(const std::vector<std::pair<double, double> >& requiredEnergies) {
      info().setEnergies(requiredEnergies);
      return *this;
    }


    /// Return true if this analysis needs to know the process cross-section.
    /// @todo Remove this and require HepMC >= 2.06
    bool needsCrossSection() const {
      return info().needsCrossSection();
    }
    /// Declare whether this analysis needs to know the process cross-section from the generator.
    /// @todo Remove this and require HepMC >= 2.06
    Analysis& setNeedsCrossSection(bool needed=true) {
      info().setNeedsCrossSection(needed);
      return *this;
    }

    //@}


    /// @name Internal metadata modifying methods
    //@{

    /// Get the actual AnalysisInfo object in which all this metadata is stored (non-const).
    AnalysisInfo& info() {
      assert(_info.get() != 0 && "No AnalysisInfo object :O");
      return *_info;
    }

    //@}


    /// @name Run conditions
    //@{

    /// Incoming beams for this run
    const ParticlePair& beams() const;

    /// Incoming beam IDs for this run
    const PdgIdPair beamIds() const;

    /// Centre of mass energy for this run
    double sqrtS() const;

    //@}


    /// @name Analysis / beam compatibility testing
    //@{

    /// Check if analysis is compatible with the provided beam particle IDs and energies
    bool isCompatible(const ParticlePair& beams) const;

    /// Check if analysis is compatible with the provided beam particle IDs and energies
    bool isCompatible(PdgId beam1, PdgId beam2, double e1, double e2) const;

    /// Check if analysis is compatible with the provided beam particle IDs and energies
    bool isCompatible(const PdgIdPair& beams, const std::pair<double,double>& energies) const;

    //@}


    /// Set the cross section from the generator
    Analysis& setCrossSection(double xs);

    /// Access the controlling AnalysisHandler object.
    AnalysisHandler& handler() const { return *_analysishandler; }


  protected:

    /// Get a Log object based on the name() property of the calling analysis object.
    Log& getLog() const;

    /// Get the process cross-section in pb. Throws if this hasn't been set.
    double crossSection() const;

    /// Get the process cross-section per generated event in pb. Throws if this
    /// hasn't been set.
    double crossSectionPerEvent() const;

    /// Get the number of events seen (via the analysis handler). Use in the
    /// finalize phase only.
    size_t numEvents() const;

    /// Get the sum of event weights seen (via the analysis handler). Use in the
    /// finalize phase only.
    double sumOfWeights() const;


  protected:

    /// @name Histogram paths
    //@{

    /// Get the canonical histogram "directory" path for this analysis.
    const std::string histoDir() const;

    /// Get the canonical histogram path for the named histogram in this analysis.
    const std::string histoPath(const std::string& hname) const;

    /// Get the canonical histogram path for the numbered histogram in this analysis.
    const std::string histoPath(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const;

    /// Get the internal histogram name for given d, x and y (cf. HepData)
    const std::string makeAxisCode(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const;

    //@}


    /// @name Histogram reference data
    //@{

    /// Get reference data for a named histo
    const YODA::Scatter2D & refData(const string& hname) const;

    /// Get reference data for a numbered histo
    const YODA::Scatter2D & refData(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const;

    //@}


    /// @name 1D histogram booking
    //@{

    /// Book a 1D histogram with @a nbins uniformly distributed across the range @a lower - @a upper .
    Histo1DPtr bookHisto1D(const std::string& name,
                           size_t nbins, double lower, double upper,
                           const std::string& title="",
                           const std::string& xtitle="",
                           const std::string& ytitle="");

    /// Book a 1D histogram with non-uniform bins defined by the vector of bin edges @a binedges .
    Histo1DPtr bookHisto1D(const std::string& name,
                           const std::vector<double>& binedges,
                           const std::string& title="",
                           const std::string& xtitle="",
                           const std::string& ytitle="");

    /// Book a 1D histogram with binning from a reference scatter.
    Histo1DPtr bookHisto1D(const std::string& name,
                           const Scatter2D& refscatter,
                           const std::string& title="",
                           const std::string& xtitle="",
                           const std::string& ytitle="");

    /// Book a 1D histogram, using the binnings in the reference data histogram.
    Histo1DPtr bookHisto1D(const std::string& name,
                           const std::string& title="",
                           const std::string& xtitle="",
                           const std::string& ytitle="");

    /// Book a 1D histogram, using the binnings in the reference data histogram.
    ///
    /// The paper, dataset and x/y-axis IDs will be used to build the histo name in the HepData standard way.
    Histo1DPtr bookHisto1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                           const std::string& title="",
                           const std::string& xtitle="",
                           const std::string& ytitle="");

    //@}


    // /// @name 2D histogram booking
    // //@{

    // /// Book a 2D histogram with @a nxbins and @a nybins uniformly
    // /// distributed across the ranges @a xlower - @a xupper and @a
    // /// ylower - @a yupper respectively along the x- and y-axis.
    // Histogram2DPtr bookHisto2D(const std::string& name,
    //                            size_t nxbins, double xlower, double xupper,
    //                            size_t nybins, double ylower, double yupper,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D histogram with non-uniform bins defined by the
    // /// vectorx of bin edges @a xbinedges and @a ybinedges.
    // Histogram2DPtr bookHisto2D(const std::string& name,
    //                            const std::vector<double>& xbinedges,
    //                            const std::vector<double>& ybinedges,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    /// Book a 2D histogram with binning from a reference scatter.
    // Histogram2DPtr bookHisto2D(const std::string& name,
    //                            const Scatter3D& refscatter,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D histogram, using the binnings in the reference data histogram.
    // Histogram2DPtr bookHisto2D(const std::string& name,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D histogram, using the binnings in the reference data histogram.
    // ///
    // /// The paper, dataset and x/y-axis IDs will be used to build the histo name in the HepData standard way.
    // Histogram2DPtr bookHisto2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // //@}


    /// @name 1D profile histogram booking
    //@{

    /// Book a 1D profile histogram with @a nbins uniformly distributed across the range @a lower - @a upper .
    Profile1DPtr bookProfile1D(const std::string& name,
                               size_t nbins, double lower, double upper,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// Book a 1D profile histogram with non-uniform bins defined by the vector of bin edges @a binedges .
    Profile1DPtr bookProfile1D(const std::string& name,
                               const std::vector<double>& binedges,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// Book a 1D profile histogram with binning from a reference scatter.
    Profile1DPtr bookProfile1D(const std::string& name,
                               const Scatter2D& refscatter,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// Book a 1D profile histogram, using the binnings in the reference data histogram.
    Profile1DPtr bookProfile1D(const std::string& name,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// Book a 1D profile histogram, using the binnings in the reference data histogram.
    ///
    /// The paper, dataset and x/y-axis IDs will be used to build the histo name in the HepData standard way.
    Profile1DPtr bookProfile1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    //@}


    // /// @name 2D profile histogram booking
    // //@{

    // /// Book a 2D profile histogram with @a nxbins and @a nybins uniformly
    // /// distributed across the ranges @a xlower - @a xupper and @a ylower - @a
    // /// yupper respectively along the x- and y-axis.
    // Profile2DPtr bookProfile2D(const std::string& name,
    //                            size_t nxbins, double xlower, double xupper,
    //                            size_t nybins, double ylower, double yupper,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D profile histogram with non-uniform bins defined by the vectorx
    // /// of bin edges @a xbinedges and @a ybinedges.
    // Profile2DPtr bookProfile2D(const std::string& name,
    //                            const std::vector<double>& xbinedges,
    //                            const std::vector<double>& ybinedges,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    /// Book a 2D profile histogram with binning from a reference scatter.
    // Profile2DPtr bookProfile2D(const std::string& name,
    //                            const Scatter3D& refscatter,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D profile histogram, using the binnings in the reference data histogram.
    // Profile2DPtr bookProfile2D(const std::string& name,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // /// Book a 2D profile histogram, using the binnings in the reference data histogram.
    // ///
    // /// The paper, dataset and x/y-axis IDs will be used to build the histo name in the HepData standard way.
    // Profile2DPtr bookProfile2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
    //                            const std::string& title="",
    //                            const std::string& xtitle="",
    //                            const std::string& ytitle="",
    //                            const std::string& ztitle="");

    // //@}


    /// @name 2D scatter booking
    //@{

    /// @brief Book a 2-dimensional data point set with the given name.
    ///
    /// @note Unlike histogram booking, scatter booking by default makes no
    /// attempt to use reference data to pre-fill the data object. If you want
    /// this, which is sometimes useful e.g. when the x-position is not really
    /// meaningful and can't be extracted from the data, then set the @a
    /// copy_pts parameter to true. This creates points to match the reference
    /// data's x values and errors, but with the y values and errors zeroed...
    /// assuming that there is a reference histo with the same name: if there
    /// isn't, an exception will be thrown.
    Scatter2DPtr bookScatter2D(const std::string& name,
                               bool copy_pts=false,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// @brief Book a 2-dimensional data point set, using the binnings in the reference data histogram.
    ///
    /// The paper, dataset and x/y-axis IDs will be used to build the histo name in the HepData standard way.
    ///
    /// @note Unlike histogram booking, scatter booking by default makes no
    /// attempt to use reference data to pre-fill the data object. If you want
    /// this, which is sometimes useful e.g. when the x-position is not really
    /// meaningful and can't be extracted from the data, then set the @a
    /// copy_pts parameter to true. This creates points to match the reference
    /// data's x values and errors, but with the y values and errors zeroed.
    Scatter2DPtr bookScatter2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId,
                               bool copy_pts=false,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// @brief Book a 2-dimensional data point set with equally spaced x-points in a range.
    ///
    /// The y values and errors will be set to 0.
    Scatter2DPtr bookScatter2D(const std::string& name,
                               size_t npts, double lower, double upper,
                               const std::string& title="",
                               const std::string& xtitle="",
                               const std::string& ytitle="");

    /// @brief Book a 2-dimensional data point set based on provided contiguous "bin edges".
    ///
    /// The y values and errors will be set to 0.
    Scatter2DPtr bookScatter2D(const std::string& hname,
                               const std::vector<double>& binedges,
                               const std::string& title,
                               const std::string& xtitle,
                               const std::string& ytitle);

    //@}


    /// @todo Should really be protected: only public to keep BinnedHistogram happy for now...
  public:

    /// @name Histogram manipulation
    //@{

    /// Normalize the given histogram, @a histo, to area = @a norm.
    ///
    /// @note The histogram is no longer invalidated by this procedure.
    void normalize(Histo1DPtr histo, double norm=1.0, bool includeoverflows=true);

    /// Multiplicatively scale the given histogram, @a histo, by factor @s scale.
    ///
    /// @note The histogram is no longer invalidated by this procedure.
    void scale(Histo1DPtr histo, double scale);

    /// Normalize the given histogram, @a histo, to area = @a norm.
    ///
    /// @note The histogram is no longer invalidated by this procedure.
    // void normalize(Histo2DPtr histo, double norm=1.0);

    /// Multiplicatively scale the given histogram, @a histo, by factor @s scale.
    ///
    /// @note The histogram is no longer invalidated by this procedure.
    // void scale(Histo2DPtr histo, double scale);


    /// Helper for histogram division.
    ///
    /// @note Preserves the path information of the target.
    void divide(Histo1DPtr h1, Histo1DPtr h2, Scatter2DPtr s) const;

    /// Helper for profile histogram division.
    ///
    /// @note Preserves the path information of the target.
    void divide(Profile1DPtr p1, Profile1DPtr p2, Scatter2DPtr s) const;

    /// Helper for histogram division.
    ///
    /// @note Preserves the path information of the target.
    void divide(const YODA::Histo1D & h1,
                const YODA::Histo1D & h2, Scatter2DPtr s) const;

    /// Helper for profile histogram division
    ///
    /// @note Preserves the path information of the target.
    void divide(const YODA::Profile1D & p1,
                const YODA::Profile1D & p2, Scatter2DPtr s) const;


    /// Helper for converting a differential histo to an integral one.
    ///
    /// @note Preserves the path information of the target.
    void integrate(Histo1DPtr h, Scatter2DPtr s) const;

    /// Helper for converting a differential histo to an integral one.
    ///
    /// @note Preserves the path information of the target.
    void integrate(const Histo1D& h, Scatter2DPtr s) const;


    /// @todo Add integrate helpers for 2D histos (defined somehow...)

    //@}


  public:

    /// List of registered analysis data objects
    const vector<AnalysisObjectPtr>& analysisObjects() const {
      return _analysisobjects;
    }


  protected:

    /// @name Data object registration, retrieval, and removal
    //@{

    /// Register a data object in the histogram system
    void addAnalysisObject(AnalysisObjectPtr ao);

    /// Get a data object from the histogram system
    /// @todo Use this default function template arg in C++11
    // template <typename AO=AnalysisObjectPtr>
    template <typename AO>
    const shared_ptr<AO> getAnalysisObject(const std::string& name) const {
      foreach (const AnalysisObjectPtr& ao, analysisObjects()) {
        if (ao->path() == histoPath(name)) return dynamic_pointer_cast<AO>(ao);
      }
      throw Exception("Data object " + histoPath(name) + " not found");
    }

    /// Get a data object from the histogram system (non-const)
    /// @todo Use this default function template arg in C++11
    // template <typename AO=AnalysisObjectPtr>
    template <typename AO>
    shared_ptr<AO> getAnalysisObject(const std::string& name) {
      foreach (const AnalysisObjectPtr& ao, analysisObjects()) {
        if (ao->path() == histoPath(name)) return dynamic_pointer_cast<AO>(ao);
      }
      throw Exception("Data object " + histoPath(name) + " not found");
    }

    /// Unregister a data object from the histogram system (by name)
    void removeAnalysisObject(const std::string& path);

    /// Unregister a data object from the histogram system (by pointer)
    void removeAnalysisObject(AnalysisObjectPtr ao);


    /// Get a named Histo1D object from the histogram system
    const Histo1DPtr getHisto1D(const std::string& name) const {
      return getAnalysisObject<Histo1D>(name);
    }

    /// Get a named Histo1D object from the histogram system (non-const)
    Histo1DPtr getHisto1D(const std::string& name) {
      return getAnalysisObject<Histo1D>(name);
    }

    /// Get a Histo1D object from the histogram system by axis ID codes (non-const)
    const Histo1DPtr getHisto1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
      return getAnalysisObject<Histo1D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }

    /// Get a Histo1D object from the histogram system by axis ID codes (non-const)
    Histo1DPtr getHisto1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) {
      return getAnalysisObject<Histo1D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }


    // /// Get a named Histo2D object from the histogram system
    // const Histo2DPtr getHisto2D(const std::string& name) const {
    //   return getAnalysisObject<Histo2D>(name);
    // }

    // /// Get a named Histo2D object from the histogram system (non-const)
    // Histo2DPtr getHisto2D(const std::string& name) {
    //   return getAnalysisObject<Histo2D>(name);
    // }

    // /// Get a Histo2D object from the histogram system by axis ID codes (non-const)
    // const Histo2DPtr getHisto2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
    //   return getAnalysisObject<Histo2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    // }

    // /// Get a Histo2D object from the histogram system by axis ID codes (non-const)
    // Histo2DPtr getHisto2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) {
    //   return getAnalysisObject<Histo2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    // }


    /// Get a named Profile1D object from the histogram system
    const Profile1DPtr getProfile1D(const std::string& name) const {
      return getAnalysisObject<Profile1D>(name);
    }

    /// Get a named Profile1D object from the histogram system (non-const)
    Profile1DPtr getProfile1D(const std::string& name) {
      return getAnalysisObject<Profile1D>(name);
    }

    /// Get a Profile1D object from the histogram system by axis ID codes (non-const)
    const Profile1DPtr getProfile1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
      return getAnalysisObject<Profile1D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }

    /// Get a Profile1D object from the histogram system by axis ID codes (non-const)
    Profile1DPtr getProfile1D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) {
      return getAnalysisObject<Profile1D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }


    // /// Get a named Profile2D object from the histogram system
    // const Profile2DPtr getProfile2D(const std::string& name) const {
    //   return getAnalysisObject<Profile2D>(name);
    // }

    // /// Get a named Profile2D object from the histogram system (non-const)
    // Profile2DPtr getProfile2D(const std::string& name) {
    //   return getAnalysisObject<Profile2D>(name);
    // }

    // /// Get a Profile2D object from the histogram system by axis ID codes (non-const)
    // const Profile2DPtr getProfile2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
    //   return getAnalysisObject<Profile2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    // }

    // /// Get a Profile2D object from the histogram system by axis ID codes (non-const)
    // Profile2DPtr getProfile2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) {
    //   return getAnalysisObject<Profile2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    // }


    /// Get a named Scatter2D object from the histogram system
    const Scatter2DPtr getScatter2D(const std::string& name) const {
      return getAnalysisObject<Scatter2D>(name);
    }

    /// Get a named Scatter2D object from the histogram system (non-const)
    Scatter2DPtr getScatter2D(const std::string& name) {
      return getAnalysisObject<Scatter2D>(name);
    }

    /// Get a Scatter2D object from the histogram system by axis ID codes (non-const)
    const Scatter2DPtr getScatter2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) const {
      return getAnalysisObject<Scatter2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }

    /// Get a Scatter2D object from the histogram system by axis ID codes (non-const)
    Scatter2DPtr getScatter2D(unsigned int datasetId, unsigned int xAxisId, unsigned int yAxisId) {
      return getAnalysisObject<Scatter2D>(makeAxisCode(datasetId, xAxisId, yAxisId));
    }

    //@}


  private:

    /// Name passed to constructor (used to find .info analysis data file, and as a fallback)
    string _defaultname;

    /// Pointer to analysis metadata object
    shared_ptr<AnalysisInfo> _info;

    /// Storage of all plot objects
    /// @todo Make this a map for fast lookup by path?
    vector<AnalysisObjectPtr> _analysisobjects;

    /// @name Cross-section variables
    //@{
    double _crossSection;
    bool _gotCrossSection;
    //@}

    /// The controlling AnalysisHandler object.
    AnalysisHandler* _analysishandler;

    /// Collection of cached refdata to speed up many autobookings: the
    /// reference data file should only be read once.
    mutable std::map<std::string, Scatter2DPtr> _refdata;


  private:

    /// @name Utility functions
    //@{

    /// Get the reference data for this paper and cache it.
    void _cacheRefData() const;

    //@}


    /// The assignment operator is private and must never be called.
    /// In fact, it should not even be implemented.
    Analysis& operator=(const Analysis&);

  };


}


// Include definition of analysis plugin system so that analyses automatically see it when including Analysis.hh
#include "Rivet/AnalysisBuilder.hh"

/// @def DECLARE_RIVET_PLUGIN
/// Preprocessor define to prettify the global-object plugin hook mechanism.
#define DECLARE_RIVET_PLUGIN(clsname) Rivet::AnalysisBuilder<clsname> plugin_ ## clsname

/// @def DEFAULT_RIVET_ANA_CONSTRUCTOR
/// Preprocessor define to prettify the manky constructor with name string argument
#define DEFAULT_RIVET_ANA_CONSTRUCTOR(clsname) clsname() : Analysis(# clsname) {}


#endif
