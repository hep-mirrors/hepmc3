#include <graphviz/cgraph.h>
#include <graphviz/types.h>
#include <stdlib.h>
#include <gvc.h>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"


static bool show_as_parton(HepMC3::ConstGenParticlePtr p )
{
    const int pd=std::abs(p->pid());
    bool parton=false;

    if (pd==81||pd==82||pd<25) parton=true;
    if (
        (pd/1000==1||pd/1000==2||pd/1000==3||pd/1000==4||pd/1000==5)
        &&(pd%1000/100==1||pd%1000/100==2||pd%1000/100==3||pd%1000/100==4)
        &&(pd%100==1||pd%100==3)
    )
        parton=true;
    if (p->status()==4)  parton=true;
    return parton;
}

char*  write_event(char* m_cursor,const HepMC3::GenEvent &evt)
{
    int m_style=1;

    m_cursor += sprintf(m_cursor, "digraph graphname%d {\n",evt.event_number());
    m_cursor += sprintf(m_cursor, "v0[label=\"Machine\"];\n");
    for(auto v: evt.vertices() )
    {
        if (m_style!=0)
        {
            if (m_style==1) //paint decay and fragmentation vertices in green
            {
                if (v->status()==2) m_cursor += sprintf(m_cursor, "node [color=\"green\"];\n");
                else  m_cursor += sprintf(m_cursor, "node [color=\"black\"];\n");
            }
        }
        HepMC3::FourVector in=HepMC3::FourVector(0,0,0,0);
        HepMC3::FourVector out=HepMC3::FourVector(0,0,0,0);
        double energy=0;
        for(auto p1: v->particles_in()  ) {
            in+=p1->momentum();
            energy+=std::abs(p1->momentum().e());
        }
        for(auto p2: v->particles_out() ) {
            out+=p2->momentum();
            energy+=std::abs(p2->momentum().e());
        }
        HepMC3::FourVector momviolation(0,0,0,0);
        momviolation+=in;
        momviolation-=out;
        double energyviolation=std::sqrt(momviolation.length2()  +momviolation.e()*momviolation.e()       );
        bool violation=false;
        if (energyviolation>1e-8*energy) violation=true;

        if(violation)
        {
            m_cursor += sprintf(m_cursor, "node [shape=rectangle];\n");
            m_cursor += sprintf(m_cursor, "v%d [label=\"%d\ndE=%4.2f\"];\n", -v->id(),v->id(),energyviolation);
        }
        else
        {
            m_cursor += sprintf(m_cursor, "node [shape=ellipse];\n");
            m_cursor += sprintf(m_cursor, "v%d[label=\"%d\"];\n", -v->id(),v->id());
        }

        m_cursor += sprintf(m_cursor, "node [shape=ellipse];\n");


    }
    for(auto p: evt.beams() )
    {
        if (!p->end_vertex()) continue;
        m_cursor += sprintf(m_cursor, "node [shape=point];\n");
        m_cursor += sprintf(m_cursor, "v0 -> v%d [label=\"%d(%d)\"];\n", -p->end_vertex()->id(),p->id(),p->pid());
    }

    for(auto v: evt.vertices() )
    {

        for(auto p: v->particles_out() )
        {
            {
                if (m_style!=0)
                {
                    if (m_style==1) //paint suspected partons and 81/82 in red
                    {
                        if (show_as_parton(p)&&p->status()!=1) m_cursor += sprintf(m_cursor, "edge [color=\"red\"];\n");
                        else        m_cursor +=sprintf(m_cursor, "edge [color=\"black\"];\n");
                    }
                }
                if (!p->end_vertex())
                {
                    m_cursor += sprintf(m_cursor, "node [shape=point];\n");
                    m_cursor += sprintf(m_cursor, "v%d -> o%d [label=\"%d(%d)\"];\n", -v->id(),p->id(),p->id(),p->pid());
                    continue;
                }
                else
                    m_cursor += sprintf(m_cursor, "v%d -> v%d [label=\"%d(%d)\"];\n", -v->id(),-p->end_vertex()->id(),p->id(),p->pid());
            }
        }
    }
    m_cursor += sprintf(m_cursor, "labelloc=\"t\";\nlabel=\"Event %d; Vertices %lu; Particles %lu;\";\n", evt.event_number(), evt.vertices().size(), evt.particles().size());
    m_cursor += sprintf(m_cursor,"}\n\n");

    return m_cursor;
}

char*  read_from_buf(char* m_buffer)
{
    GVC_t * gvc=gvContext();
    Agraph_t *g= agmemread(m_buffer);
    gvLayout(gvc,g,"dot");

    int err;
    char *data;
    unsigned int length;

    if (!g)
        return NULL;
    err = gvRenderData(gvc, g, "png", &data, &length);
    if (err)
        return NULL;
    data = (char*)realloc(data, length + 1);
    delete g;
    delete gvc;
    return data;
}
