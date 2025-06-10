#include "CycleRemover.h"
#include "HepMC3/Print.h"

namespace HepMC3 {

void CycleRemover::detect_cycles_DFS(HepMC3::GenVertexPtr v, std::unordered_set<HepMC3::GenVertexPtr> &visited, std::unordered_map<HepMC3::GenVertexPtr, HepMC3::GenVertexPtr> &parent,
                                   std::vector<std::vector<HepMC3::GenVertexPtr>> &cycles, std::vector<HepMC3::GenVertexPtr> &path) {

  if (!v || visited.find(v) != visited.end())
    return;

  visited.insert(v);
  path.push_back(v);

  for (auto &p : v->particles_out()) {
    auto nextVertex = p->end_vertex();
    if (nextVertex) {
      if (visited.find(nextVertex) != visited.end()) {
        // Cycle detected, extract the cycle path
        std::vector<HepMC3::GenVertexPtr> cycle;
        auto it = std::find(path.begin(), path.end(), nextVertex);
        if (it != path.end()) {
          cycle.assign(it, path.end());
          cycles.push_back(cycle);
        }
      } else {
        parent[nextVertex] = v;
        detect_cycles_DFS(nextVertex, visited, parent, cycles, path);
      }
    }
  }

  path.pop_back();
}

std::shared_ptr<GenEvent> CycleRemover::merge_cycle(int flag) {
  auto m_event = std::make_shared<HepMC3::GenEvent>(*(m_original_event.get()));

  std::unordered_set<HepMC3::GenVertexPtr> visited;
  std::unordered_map<HepMC3::GenVertexPtr, HepMC3::GenVertexPtr> parent;
  std::vector<std::vector<HepMC3::GenVertexPtr>> cycles;

  for (auto &vertex : m_event->vertices()) {
    std::vector<HepMC3::GenVertexPtr> path;
    if (visited.find(vertex) == visited.end()) {
      detect_cycles_DFS(vertex, visited, parent, cycles, path);
    }
  }
  // One should merge one path per pass
  for (const auto &cycle : cycles) {
    for (const auto &v : cycle) {
      m_loop_vertices.insert(v);
    }
  }

  auto mergedVertex = std::make_shared<GenVertex>();
  m_event->add_vertex(mergedVertex);

  std::unordered_set<HepMC3::GenParticlePtr> newin;
  std::unordered_set<HepMC3::GenParticlePtr> newout;
  for (auto v : m_loop_vertices) {
    for (auto p : v->particles_out()) {
      if (m_loop_vertices.find(p->end_vertex()) == m_loop_vertices.end()) {
        newout.insert(p);
      } else {
        m_loop_particles.insert(p);
      }
    }
    for (auto p : v->particles_in()) {
      if (m_loop_vertices.find(p->production_vertex()) == m_loop_vertices.end()) {
        newin.insert(p);
      } else {
        m_loop_particles.insert(p);
      }
    }
  }
  for (auto p : newin) {
    if (p) {
      mergedVertex->add_particle_in(p);
    }
  }
  for (auto p : newout) {
    if (p) {
      mergedVertex->add_particle_out(p);
    }
  }
  if (flag == 1) {
    for (auto p : m_loop_particles)
      if (p) {
        mergedVertex->add_particle_out(p);
        mergedVertex->add_particle_in(p);
      }
  }
  for (auto v : m_loop_vertices) {
    m_event->remove_vertex(v);
  }
  return m_event;
}

} // namespace HepMC3
