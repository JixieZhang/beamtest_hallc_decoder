#ifndef HC_TRACKING_H
#define HC_TRACKING_H

#include <unordered_map>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include "GEMModule.h"
#include "EventWrapper.h"

class TClonesArray;
class TDatime;

// tracking class
class HCTracking
{
public:
    HCTracking();
    ~HCTracking();

    void InitLayerCombos();
    void InitGridBins();

    // Retrieve the global position of a hit by module and hit index:
    TVector3 GetHitPosGlobal(int modidx, int clustidx);
    int GetGridBin(int modidx, int clustidx);

    // Utility method to take a list of hits mapped by layer as input, and give track parameters and chi2 as output.
    // This relies on the "hit list" and "free hit list" information also being sensibly populated
    // FitTrack calculates chi2 and residuals as well as best fit parameters
    void FitTrack(const std::map<int, int> &hitcombo, double &xtrack, double &ytrack, double &xptrack, double &yptrack, double &chi2ndf, std::vector<double> &uresid, std::vector<double> &vresid);
    // CalcLineOfBestFit only calculates the track parameters, does not calculate chi2 or residuals:
    void CalcLineOfBestFit(const std::map<int, int> &hitcombo, double &xtrack, double &ytrack, double &xptrack, double &yptrack);

    // Calculates the position of the track's intersection with a module in "module" coordinates U/V (the ones measured by the strips)
    TVector2 GetUVTrack(int module, TVector3 track_origin, TVector3 track_direction);

    // Calculate the intersection of a track with the plane of a module's active area:
    TVector3 TrackIntersect(int module, TVector3 track_origin, TVector3 track_direction, double &sintersect);

    // Utility method to iterate over combinations of hits in layers, used by find_tracks()
    bool GetNextCombo(const std::set<int> &layers, std::map<int, int> &hitcounter, std::map<int, int> &hitcombo, bool &firstcombo);

    virtual bool PassedOpticsConstraint(TVector3 track_origin, TVector3 track_direction, bool coarse = false);
    bool CheckConstraint(double xtr, double ytr, double xptr, double yptr, bool coarse = false);

    // 1D and 2D clustering:
    void hit_reconstruction();
    // track-finding:
    void find_tracks();

    void Clear();
    void Decode(const EventWrapper &event);

    Double_t InitHitList();     // Initialize (unchanging) "hit list" arrays used by track-finding: this only happens at the beginning of tracking
    Double_t InitFreeHitList(); // Initialize "free hit list" arrays used on each track-finding iteration

    // Fill arrays of "good" hits (hits that end up on fitted tracks)
    void fill_good_hit_arrays();

    // Method to add a new Track to the track arrays: this takes the best hit combination and the parameters of the line of best fit to those hits
    // and the (already calculated) chi2 and fills the tracking results arrays: best fit parameters, inclusive and exclusive tracking residuals, and hit lists by track:
    void AddNewTrack(const std::map<int, int> &hitcombo, const std::vector<double> &BestTrack, double chi2ndf, const std::vector<double> &uresid, const std::vector<double> &vresid);
    void PurgeHits(int itrack);

    void InitEfficiencyHistos(const char *dname); // initialize efficiency histograms
    void CalcEfficiency();                        // essentially, divide "did hit/should hit" histograms

    void CompleteInitialization();
    void LoadPedestals(const char *fname);
    void LoadCM(const char *fname);
    void LoadConfig(const TDatime &date);

    void Begin(int run_number);
    void End(int run_number);

private:
    // Data members:
    std::vector<GEMModule *> fModules; // array of SBSGEMModules:
    bool fModulesInitialized;

    int fNmodules; // Total number of modules
    // total number of layers
    int fNlayers;

    int fTrackingAlgorithmFlag; // Choose track algorithm

    int fMinHitsOnTrack; // default = 3; cannot be less than 3, cannot be more than total number of layers

    long fMaxHitCombinations;             // default = 10000; this is for "outer" layers
    long fMaxHitCombinations_InnerLayers; // default = 10000?
    double fMaxHitCombinations_Total;     // default = 100000000
    bool fTryFastTrack;                   // default = true?

    std::set<int> fLayers;
    std::vector<int> fLayerByIndex;                  // idiot-proofing just in case the user defines something stupid:
    std::map<int, int> fIndexByLayer;                // idiot-proofing in case the user defines something stupid.
    std::map<int, int> fNumModulesByLayer;           // key = unique layer ID (logical tracking layer), mapped value = number of modules per layer
    std::map<int, std::set<int>> fModuleListByLayer; // key = unique layer ID, mapped value = list of unique modules associated with this layer

    std::map<int, double> fZavgLayer; // Average z position of the modules in a logical tracking layer. This IS used when projecting candidate tracks to each layer
    // to decide which grid bins to search for hits.
    // But NOTE: the z positions of individual modules are not, in general, identical to the average z position of the layer. If too fine a grid is used and the
    // variations of module z positions within a layer are too big, the "grid search" track-finding algorithm may not work too well!

    // key = minimum hit requirement to form a track.
    // Mapped value = 2D array of layer combinations at a given minimum hit requirement,
    // with outer index a dummy index for looping over combinations, and the inner index the list of layers in each combo
    std::map<int, std::vector<std::vector<int>>> fLayerCombinations;

    //"Grid bins" for fast track-finding algorithm(s): define limits of layer active area:
    std::map<int, double> fXmin_layer, fXmax_layer, fYmin_layer, fYmax_layer;

    // Grid bin size: smaller values should give faster track finding, but bins should be large compared to GEM spatial resolution.
    // Default bin size = 10 mm for both. we are using same grid bin width at all layers:
    double fGridBinWidthX, fGridBinWidthY;
    double fGridEdgeToleranceX, fGridEdgeToleranceY;

    // In the standalone code, these are typically derived from the grid bin size and the layer active area dimensions.
    std::map<int, int> fGridNbinsX_layer, fGridNbinsY_layer;

    // These variables are arguably redundant with the ones above, but as defined, these include a bit of
    // extra "slop" to account for resolution, misalignments, z staggering of
    // modules within a layer, etc.
    std::map<int, double> fGridXmin_layer, fGridYmin_layer, fGridXmax_layer, fGridYmax_layer;

    double fTrackChi2Cut;        // chi2/NDF cut for track validity
    bool fIsSpectrometerTracker; // default to true:
    bool fIsPolarimeterTracker;

    bool fUseConstraint;
    bool fUseOpticsConstraint; // default to FALSE:
    bool fUseForwardOpticsConstraint;

    // "Constraint points" to restrict the search region for track-finding:
    TVector3 fConstraintPoint_Front;
    TVector3 fConstraintPoint_Back;

    TVector2 fConstraintWidth_Front;
    TVector2 fConstraintWidth_Back;

    double fConstraintWidth_theta;
    double fConstraintWidth_phi;

    bool fConstraintPoint_Front_IsInitialized;
    bool fConstraintPoint_Back_IsInitialized;
    bool fConstraintWidth_Front_IsInitialized;
    bool fConstraintWidth_Back_IsInitialized;
    bool fConstraintInitialized;

    // Optics-based constraints:
    double fPmin_track; // GeV
    double fPmax_track; // GeV
    double fxptarmin_track, fxptarmax_track;
    double fyptarmin_track, fyptarmax_track;
    double fytarmin_track, fytarmax_track;

    // forward optics constraints: if applicable
    double fdxfp0, fdyfp0, fdxpfp0, fdypfp0;
    double fdxfpcut, fdyfpcut, fdxpfpcut, fdypfpcut;
    // Values for coarse forward optics check:
    // these are calculated from the grid bin width and the Z lever arm between the layers under consideration
    // at the time the coarse optics check is calculated (needs testing):
    double fdxfpcut_coarse, fdyfpcut_coarse, fdxpfpcut_coarse, fdypfpcut_coarse;

    bool fUseSlopeConstraint;
    double fxpfpmin, fxpfpmax;
    double fypfpmin, fypfpmax;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //            DATA members to hold the track information (at least temporarily, will eventually                 //
    //            be passed to the THaSpectrometer tracks TClonesArray for SBSGEMSpectrometerTracker                //
    //            We'll need to figure out how to handle things for SBSGEMPolarimeterTracker                        //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////// "Hit list" arrays used by tracking algorithm /////////////////////////
    Double_t fSigma_hitpos; // sigma parameter controlling resolution entering track chi^2 calculation

    /////////// unchanging arrays that are filled at the beginning of track-finding: ///////////
    // Define and initialize the "hit lists" and other arrays that we will need to do the tracking:
    // These "hit list" arrays will NOT be modified throughout the track-finding procedure, except at the beginning:
    std::set<int> layers_with_2Dhits; // list of tracking layers with at least one 2D hit reconstructed (within the track search region)
    // std::map<int,int> N2Dhits_layer; // key = layer, mapped value = total number of reconstructed 2D hits
    std::vector<int> N2Dhits_layer;
    std::vector<std::vector<int>> modindexhit2D;   // key = layer, mapped value = module index of hits in that layer:
    std::vector<std::vector<int>> clustindexhit2D; // key = layer, mapped value = index of hits within 2D cluster array of module in question
    std::vector<std::vector<bool>> hitused2D;      // flag to tell each track-finding iteration whether hit was already used in a previous track
    std::vector<std::vector<int>> gridbinhit2D;

    //////////////////// "Free hit list" arrays used on individual track-finding iterations: /////////////////////////////
    std::vector<int> Nfreehits_layer;                // key = layer, mapped value = number of unused hits available:
    std::set<int> layerswithfreehits;                // list of layers with at least one unused hit
    std::vector<std::vector<int>> freehitlist_layer; // list of unused hits mapped by layer: index in the unchanging array defined above (clustindex2D)

    std::map<int, int> freehitcounter; // When using a "brute force" track-finding algorithm, this counter is used for looping over hit combinations ("odometer" algorithm)

    // Hit lists mapped by grid bin:
    std::vector<std::vector<int>> Nfreehits_binxy_layer;                // number of free hits by grid bin in each layer
    std::vector<std::vector<std::vector<int>>> freehitlist_binxy_layer; // list of free hits by layer and 2D grid bin; again, the "hit list" contains the index in the unchanging array clustindex2D
    std::vector<std::set<int>> binswithfreehits_layer;                  // List of X/Y grid bins with free hits by layer;

    // Array to hold the "reduced free hit list":
    std::vector<std::vector<int>> freehitlist_goodxy;
    std::set<int> layerswithfreehits_goodxy;

    bool fclustering_done;
    bool ftracking_done;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// Tracking results: /////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    int fNtracks_found;
    std::vector<int> fNhitsOnTrack;                 // number of hits on track:
    std::vector<std::vector<int>> fModListTrack;    // list of modules containing hits on fitted tracks
    std::vector<std::vector<int>> fHitListTrack;    // list of hits on fitted tracks: NOTE--the "hit list" of the track refers to the index in the 2D cluster array. To locate the hit and its properties you need the module index and the hit index, i.e., fModules[fModListTrack[ihit]]->fHits[fHitListTrack[ihit]]
    std::vector<std::vector<double>> fresidu_hits;  // inclusive residuals: track - hit along direction measured by u strips
    std::vector<std::vector<double>> fresidv_hits;  // inclusive residuals: track - hit along direction measured by v strips
    std::vector<std::vector<double>> feresidu_hits; // exclusive residuals: track - hit along direction measured by u strips
    std::vector<std::vector<double>> feresidv_hits; // exclusive residuals: track - hit along direction measured by v strips

    // Fitted track parameters: coordinates at Z = 0 and slopes
    std::vector<double> fXtrack;
    std::vector<double> fYtrack;
    std::vector<double> fXptrack;
    std::vector<double> fYptrack;
    std::vector<double> fChi2Track; // chi2/ndf

    int fBestTrackIndex; // Index of "golden track" within the TClonesArray defined by THaSpectrometer or other "best track" selection method (if not a spectrometer tracker)

    // We will need to define some global variables that are either in the form of basic data or vectors of basic data,
    // that are more convenient for ROOT Tree and/or Histogram output:
    // Generally speaking, we will mainly be interested in histogramming the results for clusters/2D hits that end up on good tracks:
    // Note that for the time being, we ONLY consider 2D hits for tracking; this means that we require both U and V strips to fire!
    // Moreover, each 2D hit (and therefore, each 1D cluster) can ONLY be used in exactly one good track
    // The following arrays, used to store properties of hits on good tracks

    // What do we want to (potentially) store?
    // Global and local coordinates (both U/V and X/Y)
    // Module index
    // layer index
    // Track index of the hits
    // ADC cluster sums U and V, and ADC asymmetry
    // Timing information U, V, and U - V
    // U and V cluster moments
    // Inclusive and exclusive tracking residuals
    // U/V Correlation coefficients (cluster and strip level)
    // Cluster sizes in strips along U and V
    // Strip indices along U and V in which maximum occurs
    // lower and upper strip indices in the cluster.

    // We will define all of these in a way that is ROOT-tree friendly (basic data types or 1D STL vectors):
    int fNgoodhits;                  // Total number of good 2D hits ending up on good tracks:
    std::vector<int> fHitTrackIndex; // Index of track containing this hit
    std::vector<int> fHitModule;     // Module index of hit
    std::vector<int> fHitLayer;      // Layer index of hit
    std::vector<int> fHitNstripsU;   // number of U strips on hit
    std::vector<int> fHitUstripMax;  // U strip index of maximum
    std::vector<int> fHitUstripLo;   // lo U strip index of hit
    std::vector<int> fHitUstripHi;   // hi U strip index
    std::vector<int> fHitNstripsV;   // number of V strips on hit
    std::vector<int> fHitVstripMax;  // V strip index of maximum
    std::vector<int> fHitVstripLo;   // lo V strip index of hit
    std::vector<int> fHitVstripHi;   // hi V strip index
    std::vector<double> fHitUlocal;  // reconstructed "U" coordinate
    std::vector<double> fHitVlocal;  // reconstructed "V" coordinate
    std::vector<double> fHitXlocal;  // reconstructed "X" coordinate (rotation of U/V)
    std::vector<double> fHitYlocal;  // reconstructed "Y" coordinate (rotation of U/V)
    std::vector<double> fHitXglobal; // global x coordinate of hit
    std::vector<double> fHitYglobal; // global y coordinate of hit
    std::vector<double> fHitZglobal; // global z coordinate of hit
    std::vector<double> fHitUmoment; // cluster "U" moment
    std::vector<double> fHitVmoment; // cluster "V" moment
    std::vector<double> fHitUsigma;  //"sigma" of reconstructed U position (measure of cluster width)
    std::vector<double> fHitVsigma;  //"sigma of reconstructed V position (measure of cluster width)
    std::vector<double> fHitResidU;  // U tracking residual ("inclusive")
    std::vector<double> fHitResidV;  // V tracking residual ("inclusive")
    std::vector<double> fHitEResidU; // U tracking residual ("exclusive");
    std::vector<double> fHitEResidV; // V tracking residual ("exclusive");
    std::vector<double> fHitUADC;    // cluster ADC sum, U strips
    std::vector<double> fHitVADC;    // cluster ADC sum, V strips
    // additional cluster-level deconvoluted variables:
    std::vector<double> fHitUADCclust_deconv;          // deconvoluted cluster ADC sum, U strips
    std::vector<double> fHitVADCclust_deconv;          // deconvoluted cluster ADC sum, V strips
    std::vector<double> fHitUADCclust_maxsamp_deconv;  // max deconvoluted cluster-summed U ADC sample
    std::vector<double> fHitVADCclust_maxsamp_deconv;  // max deconvoluted cluster-summed V ADC sample
    std::vector<double> fHitUADCclust_maxcombo_deconv; // max deconvoluted cluster-summed two-sample combo, U strips
    std::vector<double> fHitVADCclust_maxcombo_deconv; // max deconvoluted cluster-summed two-sample combo, V strips
    //
    std::vector<double> fHitUADCmaxstrip;         // ADC sum on max U strip
    std::vector<double> fHitVADCmaxstrip;         // ADC sum on max V strip
    std::vector<double> fHitUADCmaxstrip_deconv;  // ADC sum on max U strip, deconvoluted
    std::vector<double> fHitVADCmaxstrip_deconv;  // ADC sum on max V strip, deconvoluted
    std::vector<double> fHitUADCmaxsample;        // max ADC sample on max U strip
    std::vector<double> fHitVADCmaxsample;        // max ADC sample on max V strip
    std::vector<double> fHitUADCmaxsample_deconv; // max deconvoluted ADC sample on max U strip
    std::vector<double> fHitVADCmaxsample_deconv; // max deconvoluted ADC sample on max V strip
    std::vector<double> fHitUADCmaxcombo_deconv;  // max two-sample combination max U strip
    std::vector<double> fHitVADCmaxcombo_deconv;  // max two-sample combination max V strip
    std::vector<double> fHitUADCmaxclustsample;
    std::vector<double> fHitVADCmaxclustsample;
    std::vector<double> fHitADCasym; // (ADCU-ADCV)/(ADCU + ADCV)
    std::vector<double> fHitADCavg;  //(ADCU+ADCV)/2
    // new variables for deconvoluted ADC values:
    std::vector<double> fHitADCasym_deconv; // U/V asymmetry of cluster-summed deconvoluted ADC "max combo"
    std::vector<double> fHitADCavg_deconv;  // U/V average of cluster-summed deconvoluted ADC "max combo"
    //
    std::vector<double> fHitUTime; // cluster-mean time, U strips
    std::vector<double> fHitVTime; // cluster-mean time, V strips
    // New deconvoluted hit time
    std::vector<double> fHitUTimeDeconv; // cluster-mean time, deconvoluted, U strips
    std::vector<double> fHitVTimeDeconv; // cluster-mean time, deconvoluted, V strips
    //
    std::vector<double> fHitUTimeMaxStrip;       // strip-mean time, U strips
    std::vector<double> fHitVTimeMaxStrip;       // strip-mean time, V strips
    std::vector<double> fHitUTimeMaxStripFit;    // fitted strip t0
    std::vector<double> fHitVTimeMaxStripFit;    // fitted strip t0
    std::vector<double> fHitUTimeMaxStripDeconv; // deconvoluted strip time
    std::vector<double> fHitVTimeMaxStripDeconv; // deconvoluted strip time
    std::vector<double> fHitDeltaT;              // TU - TV;
    std::vector<double> fHitTavg;                //(TU+TV)/2
    //
    std::vector<double> fHitDeltaTDeconv;
    std::vector<double> fHitTavgDeconv;
    //
    std::vector<double> fHitIsampMaxUclust;        // Time-sample peak in cluster-summed ADC samples, U strips
    std::vector<double> fHitIsampMaxVclust;        // Time-sample peak in cluster-summed ADC samples, V strips
    std::vector<double> fHitIsampMaxUstrip;        // Same but for max strip in cluster
    std::vector<double> fHitIsampMaxVstrip;        // same but for max strip in cluster
    std::vector<double> fHitIsampMaxUstripDeconv;  // Same but for max strip in cluster, deconvoluted
    std::vector<double> fHitIsampMaxVstripDeconv;  // same but for max strip in cluster, deconvoluted
    std::vector<double> fHitIcomboMaxUstripDeconv; //
    std::vector<double> fHitIcomboMaxVstripDeconv;
    // additional new variables related to deconvolution, max sample and max combo for cluster-summed quantities:
    std::vector<double> fHitIsampMaxUclustDeconv;
    std::vector<double> fHitIsampMaxVclustDeconv;
    std::vector<double> fHitIcomboMaxUclustDeconv;
    std::vector<double> fHitIcomboMaxVclustDeconv;
    //
    std::vector<double> fHitCorrCoeffClust;    // cluster U/V correlation coefficient
    std::vector<double> fHitCorrCoeffMaxStrip; // U/V correlation coefficient, strips with largest ADC.
    // New variables to hold correlation coefficients for deconvoluted samples max strip and cluster-level:
    std::vector<double> fHitCorrCoeffClustDeconv;    // correlation coefficient between cluster-summed deconvoluted U and V samples
    std::vector<double> fHitCorrCoeffMaxStripDeconv; // correlation coefficient between max strip U and V deconv. ADC samples
    //
    // For pulse shape studies:
    std::vector<double> fHitADCfrac0_MaxUstrip; // time sample 0 of max U strip
    std::vector<double> fHitADCfrac1_MaxUstrip; // time sample 1 of max U strip
    std::vector<double> fHitADCfrac2_MaxUstrip; // time sample 2 of max U strip
    std::vector<double> fHitADCfrac3_MaxUstrip; // time sample 3 of max U strip
    std::vector<double> fHitADCfrac4_MaxUstrip; // time sample 4 of max U strip
    std::vector<double> fHitADCfrac5_MaxUstrip; // time sample 5 of max U strip
    std::vector<double> fHitADCfrac0_MaxVstrip; // time sample 0 of max V strip
    std::vector<double> fHitADCfrac1_MaxVstrip; // time sample 1 of max V strip
    std::vector<double> fHitADCfrac2_MaxVstrip; // time sample 2 of max V strip
    std::vector<double> fHitADCfrac3_MaxVstrip; // time sample 3 of max V strip
    std::vector<double> fHitADCfrac4_MaxVstrip; // time sample 4 of max V strip
    std::vector<double> fHitADCfrac5_MaxVstrip; // time sample 5 of max V strip
    // Deconvoluted ADC samples:
    std::vector<double> fHitDeconvADC0_MaxUstrip; // time sample 0 of max U strip
    std::vector<double> fHitDeconvADC1_MaxUstrip; // time sample 1 of max U strip
    std::vector<double> fHitDeconvADC2_MaxUstrip; // time sample 2 of max U strip
    std::vector<double> fHitDeconvADC3_MaxUstrip; // time sample 3 of max U strip
    std::vector<double> fHitDeconvADC4_MaxUstrip; // time sample 4 of max U strip
    std::vector<double> fHitDeconvADC5_MaxUstrip; // time sample 5 of max U strip
    std::vector<double> fHitDeconvADC0_MaxVstrip; // time sample 0 of max V strip
    std::vector<double> fHitDeconvADC1_MaxVstrip; // time sample 1 of max V strip
    std::vector<double> fHitDeconvADC2_MaxVstrip; // time sample 2 of max V strip
    std::vector<double> fHitDeconvADC3_MaxVstrip; // time sample 3 of max V strip
    std::vector<double> fHitDeconvADC4_MaxVstrip; // time sample 4 of max V strip
    std::vector<double> fHitDeconvADC5_MaxVstrip; // time sample 5 of max V strip

    // And I THINK that's all we need to get started!
    std::vector<UInt_t> fHitU_ENABLE_CM; // this is set based on the value for the MAX strip. Except for clusters at the border straddling APV card edges, it should be the same for all strips in a cluster:
    std::vector<UInt_t> fHitU_CM_GOOD;
    std::vector<UInt_t> fHitU_BUILD_ALL_SAMPLES;
    std::vector<UInt_t> fHitV_ENABLE_CM; // this is set based on the value for the MAX strip. Except for clusters at the border straddling APV card edges, it should be the same for all strips in a cluster:
    std::vector<UInt_t> fHitV_CM_GOOD;
    std::vector<UInt_t> fHitV_BUILD_ALL_SAMPLES;

    // number of layers fired per event
    int fNlayers_hit;   // number of layers with ANY strip fired in this layer (U or V)
    int fNlayers_hitU;  // number of layers with any U strip fired
    int fNlayers_hitV;  // number of layers with any V strip fired
    int fNlayers_hitUV; // number of layers with at least one U and V hit

    std::vector<int> fNstripsU_layer;
    std::vector<int> fNstripsV_layer;
    std::vector<int> fNstripsU_layer_neg;
    std::vector<int> fNstripsV_layer_neg;
    std::vector<int> fNstripsU_layer_neg_hit;
    std::vector<int> fNstripsV_layer_neg_hit;
    std::vector<int> fNstripsU_layer_neg_miss;
    std::vector<int> fNstripsV_layer_neg_miss;
    std::vector<int> fNclustU_layer;
    std::vector<int> fNclustV_layer;
    std::vector<int> fNclustU_layer_neg;
    std::vector<int> fNclustV_layer_neg;
    std::vector<int> fNclustU_layer_miss;
    std::vector<int> fNclustV_layer_miss;
    std::vector<int> fN2Dhit_layer;

    std::vector<int> neg_event;
    std::vector<int> neg_MPD;
    std::vector<int> neg_APV;
    std::vector<int> neg_strip;
    std::vector<int> is_neg;

    //"did hit" and "should hit" by module (numerators and denominators for efficiency determination)
    std::vector<int> fDidHit_Module;
    std::vector<int> fShouldHit_Module;

    // We'll define hit map/efficiency histograms here.
    //  NOTE: in order for these to actually show up in output, derived classes must initialize these histograms
    //  in SBSGEMSpectrometerTracker::Begin() or SBSGEMPolarimeterTracker::Begin() and
    //  write them to the output ROOT file in SBSGEMSpectrometerTracker::End() or SBSGEMPolarimeterTracker::End()
    TClonesArray *hdidhit_x_layer;
    TClonesArray *hdidhit_y_layer;
    TClonesArray *hdidhit_xy_layer;

    TClonesArray *hshouldhit_x_layer;
    TClonesArray *hshouldhit_y_layer;
    TClonesArray *hshouldhit_xy_layer;

    TClonesArray *hefficiency_x_layer;
    TClonesArray *hefficiency_y_layer;
    TClonesArray *hefficiency_xy_layer;

    TClonesArray *hdidnothit_x_layer;
    TClonesArray *hdidnothit_y_layer;

    TClonesArray *hdidhit_fullreadout_x_layer;
    TClonesArray *hdidhit_fullreadout_y_layer;

    TClonesArray *hneghit_x_layer;
    TClonesArray *hneghit_y_layer;

    TClonesArray *hneghit1D_x_layer;
    TClonesArray *hneghit1D_y_layer;

    TClonesArray *hneghit_good_x_layer;
    TClonesArray *hneghit_good_y_layer;

    TClonesArray *hneghit_good1D_x_layer;
    TClonesArray *hneghit_good1D_y_layer;

    double fBinSize_efficiency2D; // Efficiency bin sizes for 1D and 2D plots
    double fBinSize_efficiency1D; // define bin size for efficiency plots (assume equal bin width along X and Y, default to 1 cm)

    bool fEfficiencyInitialized;
    bool fMakeEfficiencyPlots; // default to TRUE
    bool fDumpGeometryInfo;    // default to FALSE

    bool fNegSignalStudy;

    bool fPedMode_DBoverride;
    bool fPedestalMode;

    bool fSubtractPedBeforeCommonMode; // flag only applies to pedestal-mode analysis

    bool fCommonModePlotsFlagIsSet;
    int fCommonModePlotsFlag;
    // bool fMakeCommonModePlots; //this will get propagated down to the modules

    // bool fPedestalsInitialized;

    bool fIsMC;
    // output files for pedestal info when running in pedestal mode:
    std::ofstream fpedfile_dbase, fCMfile_dbase, fpedfile_daq, fCMfile_daq, fCMbiasfile_dbase;
    // input files for (optional) loading of pedestals from database:

    std::string fpedfilename;
    std::string fcmfilename;
};

#endif