#include <TGClient.h>
#include <TBuffer.h>
#include <TGButton.h>
#include <TGFrame.h>
#include "TImage.h"
#include "TCanvas.h"
#include "TGCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include <TGClient.h>
#include <TCanvas.h>
#include <TBuffer.h>
#include <TGButton.h>
#include <TGFrame.h>
#include "TROOT.h"
#include "TImage.h"
#include "TH1S.h"
#include "TGFileDialog.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/Reader.h"
class HepMC3ViewerFrame : public TGMainFrame
{
private:
    TGCompositeFrame *fMainFrame;
    TGCompositeFrame *fButtonFrame;
    TGTextButton     *fNextEvent, *fPreviousEvent, *fExit, *fChooseInput,*fClearEventCache;
    TRootEmbeddedCanvas *fEmbEventImageCanvas, *fEmbAnalysisCanvas;
    //Reader
    std::shared_ptr<HepMC3::Reader> fReader;
    //Pointer to current event in cache
    HepMC3::GenEvent *fCurrentEvent;
    //Event cache
    std::vector<HepMC3::GenEvent*> fEventsCache;
    TCanvas* fEventImageCanvas, *fAnalysisCanvas;
    //Image passed from graphviz
    TImage *fGraphImage;
    std::map<std::string, TH1*> fAnalysisH;
    static const size_t m_char_buffer_size=100000;
public:
    void ReadFile(const char* a);
    HepMC3ViewerFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~HepMC3ViewerFrame();
//Helper functions
//To get image from graphviz
    void DrawEvent();
//To do extra analysiz of the event
    void DoAnalysis();
    // slots
    void NextEvent();
    void PreviousEvent();
    void ClearEventCache();
    void ChooseInput();
//   ClassDef(HepMC3ViewerFrame, 0)
};
