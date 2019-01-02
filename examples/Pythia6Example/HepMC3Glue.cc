#include "HepMC/HEPEVT_Wrapper.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Writer.h"
#include "HepMC/WriterHEPEVT.h"
#include "HepMC/WriterAscii.h"
#include "HepMC/WriterAsciiHepMC2.h"
#include "HepMC/Print.h"
#include "HepMC/Attribute.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenRunInfo.h"
#ifdef HEPMC_ROOTIO
#include "HepMC/WriterRoot.h"
#include "HepMC/WriterRootTree.h"
#endif
std::map<int,std::pair<HepMC::Writer*,HepMC::GenEvent*> > gWriters;
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
        if (!HepMC::hepevtptr)
            {
                printf("Error in %s: HEPEVT block does not exist\n",__FUNCTION__);
                return 1;
            }
        gWriters[position].second=new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM);
        for( int i=1; i<=HepMC::HEPEVT_Wrapper::number_entries(); i++ )
            if (HepMC::hepevtptr->jmohep[i-1][1]<HepMC::hepevtptr->jmohep[i-1][0])  HepMC::hepevtptr->jmohep[i-1][1]=HepMC::hepevtptr->jmohep[i-1][0];

        HepMC::HEPEVT_Wrapper::HEPEVT_to_GenEvent(gWriters[position].second);
        std::shared_ptr<HepMC::GenRunInfo> run=std::make_shared<HepMC::GenRunInfo>();
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
        HepMC::GenCrossSectionPtr cs=std::make_shared< HepMC::GenCrossSection>();
       cs->set_cross_section(x,xe,n1,n2);
       gWriters[position].second->set_cross_section(cs);
       return 0;
    }
    int set_hepevt_address_(int* a)
    {
        if (!HepMC::hepevtptr)
            {
                printf("Info in %s: setting /hepevt/ block adress\n",__FUNCTION__);
                HepMC::HEPEVT_Wrapper::set_hepevt_address((char*)a);
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
    gWriters[position].second->add_attribute(attname,std::make_shared<HepMC::IntAttribute>(attval));
    return 0;
    }
    int set_attribute_double_(const int & position,const double & attval,const char* attname, size_t len)
    {
    if (gWriters.find(position)==gWriters.end()) { printf("Warning in %s: Writer at position %i does not exist\n",__FUNCTION__,position); return 1;}
    gWriters[position].second->add_attribute(attname,std::make_shared<HepMC::DoubleAttribute>(attval));
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
                gWriters[r_position]=std::pair<HepMC::Writer*,HepMC::GenEvent*>(new HepMC::WriterAscii(filename.c_str()),new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM));
                break;
            case 2:
                gWriters[r_position]=std::pair<HepMC::Writer*,HepMC::GenEvent*>(new HepMC::WriterAsciiHepMC2(filename.c_str()),new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM));
                break;
            case 3:
                gWriters[r_position]=std::pair<HepMC::Writer*,HepMC::GenEvent*>(new HepMC::WriterHEPEVT(filename.c_str()),new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM));
                break;
#ifdef HEPMC_ROOTIO
            case 4:
                gWriters[r_position]=std::pair<HepMC::Writer*,HepMC::GenEvent*>(new HepMC::WriterRoot(filename.c_str()),new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM));
                break;
            case 5:
                gWriters[r_position]=std::pair<HepMC::Writer*,HepMC::GenEvent*>(new HepMC::WriterRootTree(filename.c_str()),new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM));
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
