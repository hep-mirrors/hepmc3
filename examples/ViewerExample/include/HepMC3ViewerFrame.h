#include <TGClient.h>
#include <TBuffer.h>
#include <TGButton.h>
#include <TGFrame.h>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Reader.h"
#include "TImage.h"
#include "TCanvas.h"
#include "TGCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include <vector>
#include <TGClient.h>
#include <TCanvas.h>
#include <TBuffer.h>
#include <TGButton.h>
#include <TGFrame.h>
#include "TROOT.h"
#include "TImage.h"
#include "TH1S.h"
#include "TGFileDialog.h"
class HepMC3ViewerFrame : public TGMainFrame
{
private:
    TGCompositeFrame *fCframe;
    TGCompositeFrame *fCframe2,*fCframe1;
    TGTextButton     *fStart, *fPause, *fExit, *fChoose,*fClearCache;
    TRootEmbeddedCanvas*  fCanvas,*  fCanvas1;
public:
    std::shared_ptr<HepMC3::Reader> R;
    HepMC3::GenEvent *evt;
    std::vector<HepMC3::GenEvent*> events;
    TCanvas* C, *C1;
    TImage *im2;
    void read_file(const char* a);
    HepMC3ViewerFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~HepMC3ViewerFrame();
    // slots
    void DrawEvent();
    void DoAnalysis();
    void NextEvent();
    void PreviousEvent();
    void ClearEventCache();
    void ChooseInput();
//   ClassDef(HepMC3ViewerFrame, 0)
};
