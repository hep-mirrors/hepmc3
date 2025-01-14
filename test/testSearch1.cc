#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Relatives.h"

std::shared_ptr<HepMC3::GenEvent> generate_event(const size_t  n,const  size_t  iterations)
{
    auto e = std::make_shared<HepMC3::GenEvent>();
    HepMC3::GenVertexPtr v0=std::make_shared<HepMC3::GenVertex>();
    e->add_vertex(v0);
    size_t it=0;
    for (;;)
    {
        if (it>iterations)
        {
            for (auto v: e->vertices())
            {
                if (!v->particles_out().empty()) continue;
                for (size_t i=0; i<n; i++) v->add_particle_out(std::make_shared<HepMC3::GenParticle>());
            }
            break;
        }
        auto vertices=e->vertices();
        for (auto v: vertices)
        {
            if (!v->particles_out().empty()) continue;
            for (size_t i=0; i<n; i++) v->add_particle_out(std::make_shared<HepMC3::GenParticle>());
            for (auto p: v->particles_out())
            {
                HepMC3::GenVertexPtr vx=std::make_shared<HepMC3::GenVertex>();
                vx->add_particle_in(p);
                e->add_vertex(vx);
            }
        }
        it++;
    }
    return e;
}

int main() {
    auto evt = generate_event(3,4);
    size_t np = 0;
    for (auto p: evt->particles()) np += (HepMC3::Relatives::ANCESTORS(p)).size();
    return 0;
}
