#include "HepMC3/HEPEVT_Wrapper.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Writer.h"
#include "HepMC3/WriterHEPEVT.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/Print.h"
#include "HepMC3/Attribute.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenRunInfo.h"
#ifdef HEPMC3_ROOTIO
#include "HepMC3/WriterRoot.h"
#include "HepMC3/WriterRootTree.h"
#endif
using namespace HepMC3;
std::map<int,std::pair<Writer*,GenEvent*> > gWriters;
extern "C" {

    int delete_writer_(const int & position)
    {
        if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
        gWriters[position].first->close();
        gWriters.erase(gWriters.find(position));
        return 0;

    }
    int convert_event_(const int & position)
    {
        if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
        if (!hepevtptr)
            {
                printf("Error in %s: HEPEVT block does not exist\n",__FUNCTION__);
                return 1;
            }
        gWriters[position].second=new GenEvent(Units::GEV,Units::MM);
        for( int i=1; i<=HEPEVT_Wrapper::number_entries(); i++ )
            if (hepevtptr->jmohep[i-1][1]<hepevtptr->jmohep[i-1][0])  hepevtptr->jmohep[i-1][1]=hepevtptr->jmohep[i-1][0];

        HEPEVT_Wrapper::HEPEVT_to_GenEvent(gWriters[position].second);
        std::shared_ptr<GenRunInfo> run=std::make_shared<GenRunInfo>();
        std::vector<std::string>  names;
        names.push_back("Default");
        run->set_weight_names(names);
        gWriters[position].second->set_run_info(run);
        return 0;
    }
    int write_event_(const int & position)
    {
        if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
        gWriters[position].first->write_event(*(gWriters[position].second));
        return 0;
    }
    int clear_event_(const int & position)
    {
        if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
        gWriters[position].second->clear();
        return 0;
    }
    int set_cross_section_(const int & position, const double& x,const double& xe, const int& n1,const int& n2)
    {
      if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
        GenCrossSectionPtr cs=std::make_shared< GenCrossSection>();
       cs->set_cross_section(x,xe,n1,n2);
       gWriters[position].second->set_cross_section(cs);
       return 0;
    }
    int set_hepevt_address_(int* a)
    {
        if (!hepevtptr)
            {
                printf("Info in %s: setting /hepevt/ block adress\n",__FUNCTION__);
                HEPEVT_Wrapper::set_hepevt_address((char*)a);
                return 0;
            }
        else
            {
                printf("Info in %s: /hepevt/ block adress is already set\n",__FUNCTION__);
                return 1;
            }
    }
    int set_attribute_int_(const int & position,const int & attval,const char* attname, size_t len)
    {
    if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
    gWriters[position].second->add_attribute(attname,std::make_shared<IntAttribute>(attval));
    return 0;
    }
    int set_attribute_double_(const int & position,const double & attval,const char* attname, size_t len)
    {
    if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
    gWriters[position].second->add_attribute(attname,std::make_shared<DoubleAttribute>(attval));
    return 0;
    }

    int new_writer_(const int & position,const int & mode,const char* ffilename, size_t len)
    {
        std::string filename=std::string(ffilename,len);
        int r_position=position;
        if (r_position==0)
            {
                if (gWriters.size()==0) r_position=1;
                if (gWriters.size()!=0) r_position=gWriters.rend()->first+1;
            }
        if (gWriters.find(r_position)!=gWriters.end()) { printf("Error in %s: Writer at position %i already exists\n",__FUNCTION__,r_position); exit(1);}
        switch (mode)
            {
            case 1:
                gWriters[r_position]=std::pair<Writer*,GenEvent*>(new WriterAscii(filename.c_str()),new GenEvent(Units::GEV,Units::MM));
                break;
            case 2:
                gWriters[r_position]=std::pair<Writer*,GenEvent*>(new WriterAsciiHepMC2(filename.c_str()),new GenEvent(Units::GEV,Units::MM));
                break;
            case 3:
                gWriters[r_position]=std::pair<Writer*,GenEvent*>(new WriterHEPEVT(filename.c_str()),new GenEvent(Units::GEV,Units::MM));
                break;
#ifdef HEPMC3_ROOTIO
            case 4:
                gWriters[r_position]=std::pair<Writer*,GenEvent*>(new WriterRoot(filename.c_str()),new GenEvent(Units::GEV,Units::MM));
                break;
            case 5:
                gWriters[r_position]=std::pair<Writer*,GenEvent*>(new WriterRootTree(filename.c_str()),new GenEvent(Units::GEV,Units::MM));
                break;
#endif
            default:
                printf("Error in %s:Output format %d is unknown or not supported.\n",__FUNCTION__,mode);
                exit(2);
                break;
            }
        return  r_position;
    }
}
