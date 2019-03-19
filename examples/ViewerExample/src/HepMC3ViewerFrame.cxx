#include "HepMC3/ReaderRootTree.h"
#include "HepMC3/ReaderFactory.h"
#include "HepMC3ViewerFrame.h"
char* read_from_buf(char* m_buffer);

char*  write_event(char* m_cursor,const HepMC3::GenEvent &evt);

void HepMC3ViewerFrame::DrawEvent()
{
    char* m_buffer = new char[100000]();
    char* m_cursor=m_buffer;
    m_cursor=write_event(m_cursor,*(evt));
    char *buf=read_from_buf(m_buffer);
    fCanvas->MapSubwindows();

    if(!C)  C=new TCanvas("C","C",1024,768);

    C->cd();
    C->Clear();
    float d=0.60;

    im2 = TImage::Create();
    im2->SetName("Event");
    im2->SetImageBuffer(&buf, TImage::kPng);

    im2->SetConstRatio(kFALSE);

    TPad *p1 = new TPad("i1", "i1", 0.05, 0.05, 0.05+d*im2->GetWidth()/im2->GetHeight(), 0.95);
    p1->Draw();
    p1->cd();

    im2->Draw("xxx");
    delete [] m_buffer;
    gPad->Update();
    DoAnalysis();
}

void HepMC3ViewerFrame::DoAnalysis()
{
    fCanvas1->MapSubwindows();

    C1->cd();
    C1->Clear();

    TH1S* particles= new TH1S();
    particles->SetFillColor(kBlue);
    for(auto p: evt->particles() )
        particles->Fill((std::to_string(p->pid())).c_str(),1.0);

    particles->LabelsOption(">","X");

    TH1S* particles1= new TH1S();
    particles1->SetFillColor(kBlue);
    for(auto p: evt->particles() )
        if(p->status()==1) particles1->Fill((std::to_string(p->pid())).c_str(),1.0);

    particles1->LabelsOption(">","X");


    C1->cd();
    TPad *p1 = new TPad("i2", "i2", 0.00, 0.750, 1.0, 1.0);
    p1->Draw();
    p1->cd();
    particles->Draw();
    C1->cd();
    TPad *p2 = new TPad("i3", "i3", 0.00, 0.5, 1.0, 0.75);
    p2->Draw();
    p2->cd();
    particles1->Draw();

    gPad->Update();

}

void HepMC3ViewerFrame::ClearEventCache()
{
    events.clear();
    evt=nullptr;
}

void HepMC3ViewerFrame::PreviousEvent()
{
    auto pos=find(events.begin(),events.end(),evt);
    if (pos==events.begin()) return;
    pos--;
    evt=*(pos);
    if (pos==events.end()) printf("Bad\n");
    printf("Bad   events.  %i       %i\n",events.size(),distance(events.begin(),pos));
    DrawEvent();
}

void HepMC3ViewerFrame::read_file(const char* a) {
    R=HepMC3::deduce_reader(a);
};
void HepMC3ViewerFrame::NextEvent()
{

    if (evt==nullptr||events.back()==evt)
    {
        HepMC3::GenEvent* evt1=new HepMC3::GenEvent(HepMC3::Units::GEV,HepMC3::Units::MM);
        bool ok=R->read_event(*(evt1));
        ok=(ok&&!R->failed());
        if (ok)
        {
            events.push_back(evt1);
            evt=evt1;
        }
        else return;
    }
    else
    {
        auto pos=find(events.begin(),events.end(),evt);
        pos++;
        evt=*(pos);
    }
    DrawEvent();
}
void HepMC3ViewerFrame::ChooseInput()
{
    static const char *FileType[] = {"All", "*.*","HepMC", "*.hepmc*","LHEF", "*.lhe*","ROOT", "*.root", 0, 0 };
    static TString dir("./");
    TGFileInfo fi;
    fi.fFileTypes = FileType;
    fi.fIniDir = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);
    if (R) R->close();
    R=HepMC3::deduce_reader(fi.fFilename);
}

HepMC3ViewerFrame::HepMC3ViewerFrame(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
{
    fCframe = new TGCompositeFrame(this, 1350, 500, kHorizontalFrame|kFixedWidth);
    fCframe2 = new TGCompositeFrame(fCframe, 150, 200, kFixedWidth);

    fCanvas =new TRootEmbeddedCanvas("legend0", fCframe, 850, 500);

    fCanvas1 =new TRootEmbeddedCanvas("legend1", fCframe, 350, 500);


    fCframe->AddFrame(fCanvas,new TGLayoutHints(kLHintsTop | kLHintsExpandX| kLHintsExpandY, 1, 1, 2, 2));
    fCframe->AddFrame(fCanvas1,new TGLayoutHints(kLHintsTop | kFixedWidth| kLHintsExpandY, 1, 1, 2, 2));
    fCframe->AddFrame(fCframe2,new TGLayoutHints(kLHintsTop , 1, 1, 2, 2));

    fStart = new TGTextButton(fCframe2, "&Next event");
    fStart->Connect("Clicked()", "HepMC3ViewerFrame", this, "NextEvent()");
    fCframe2->AddFrame(fStart, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 1, 1, 2, 2));
    fStart->SetToolTipText("Click to display next event");


    fPause = new TGTextButton(fCframe2, "&Previous event");
    fPause->Connect("Clicked()", "HepMC3ViewerFrame", this, "PreviousEvent()");
    fCframe2->AddFrame(fPause, new TGLayoutHints( kLHintsExpandX|kLHintsLeft, 1, 1, 2, 2));
    fPause->SetToolTipText("Click to display previous event");

    fChoose = new TGTextButton(fCframe2, "&Choose input");
    fChoose->Connect("Clicked()", "HepMC3ViewerFrame", this, "ChooseInput()");
    fChoose->SetToolTipText("Click to choose file");
    fCframe2->AddFrame(fChoose , new TGLayoutHints( kLHintsExpandX|kLHintsLeft, 1, 1, 2, 2));

    fClearCache = new TGTextButton(fCframe2, "&Clear event cache");
    fClearCache->Connect("Clicked()", "HepMC3ViewerFrame", this, "ClearEventCache()");
    fClearCache->SetToolTipText("Click to clear event cache ");
    fCframe2->AddFrame(fClearCache , new TGLayoutHints( kLHintsExpandX|kLHintsLeft, 1, 1, 2, 2));

    fExit = new TGTextButton(fCframe2, "&Exit ","gApplication->Terminate(0)");
    fExit->SetToolTipText("Click to exit");
    fCframe2->AddFrame(fExit, new TGLayoutHints( kLHintsExpandX|kLHintsLeft,1,1,2,2));

    AddFrame(fCframe, new TGLayoutHints(kLHintsTop |kLHintsExpandX| kLHintsExpandY, 1, 1, 2, 2));

    SetWindowName("Event viewer");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();

    R=nullptr;
    C=nullptr;
    C=fCanvas->GetCanvas();
    C1=fCanvas1->GetCanvas();
    evt=nullptr;
    im2 = TImage::Create();
}
HepMC3ViewerFrame::~HepMC3ViewerFrame()
{

    fCframe->Cleanup();
    R->close();
    Cleanup();
}
