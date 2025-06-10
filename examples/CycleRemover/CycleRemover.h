#ifndef CYCLE_REMOVER_H
#define CYCLE_REMOVER_H

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace HepMC3 {

class CycleRemover {

public:
  CycleRemover(const std::shared_ptr<GenEvent> &e) { m_original_event = std::make_shared<HepMC3::GenEvent>(*(e.get())); }
  CycleRemover(const GenEvent &e) { m_original_event = std::make_shared<HepMC3::GenEvent>(e); }
  CycleRemover(const GenEvent *e) { m_original_event = std::make_shared<HepMC3::GenEvent>(*e); }
  std::shared_ptr<GenEvent> merge_cycle(int flag);
  ~CycleRemover() {}

private:
  std::shared_ptr<GenEvent> m_original_event = nullptr;
  std::unordered_set<HepMC3::GenParticlePtr> m_loop_particles;
  std::unordered_set<HepMC3::GenVertexPtr> m_loop_vertices;

  void detect_cycles_DFS(HepMC3::GenVertexPtr v, std::unordered_set<HepMC3::GenVertexPtr> &visited, std::unordered_map<HepMC3::GenVertexPtr, HepMC3::GenVertexPtr> &parent,
                       std::vector<std::vector<HepMC3::GenVertexPtr>> &cycles, std::vector<HepMC3::GenVertexPtr> &path);

};
} // namespace HepMC3

#endif // CYCLE_REMOVER_H
