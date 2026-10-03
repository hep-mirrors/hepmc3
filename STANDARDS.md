# HepMC event-coding standards

HepMC (all versions) is a general framework for encoding event graphs
simulated/calculated via Monte-Carlo generators of all types. To be
useful for further processing in "downstream" codes, though -- for
additions and amendments to the modelling, for connection to
experimental detector-simulation, and for MC "truth" analysis --
events must respect several standard conventions.


## Graph structure

Generators are free to represent their calculations in more or less
whichever graph topologies they wish. However, to avoid creating
infinite-loop traps for processing code, writers of HepMC events
should avoid creating closed loops within the graph structure, e.g.
by decay-chains of particles which both leave and (perhaps indirectly)
re-enter a vertex (i.e. compliant HepMC events should be [directed
acyclic
graphs](https://en.wikipedia.org/wiki/Directed_acyclic_graph)).


## Particle ID codes

All particle IDs used in HepMC records *must* use the standard
[Particle Data Group MC-Particle Numbering
scheme](https://pdg.lbl.gov/2026/web/viewer.html?file=../reviews/rpp2026-rev-monte-carlo-numbering.pdf)
for particle-species identification, for access via the
`GenParticle::pdg_id()` member function.


## Particle status codes

`GenParticle` objects carry an integer status code, inherited and
expanded from the legacy HEPEVT event record, and accessed via the
`GenParticle::status()` member function.. These codes are crucial to
distinguish unphysical calculation artifacts from physical particles
to be used downstream.

The standard scheme is as follows, with "physical" codes boldened:

- 0: an unset status; particles with this status are inferred to carry
  no meaningful information and can be skipped or removed without
  consequence; in practice, it is recommended to explicitly set the
  status for all particles.
- **1:** a stable final-state particle from the perspective of the
  current generator (it may also include physically unstable particles
  that are to be decayed by later processing).
- **2:** a decayed particle with well-defined momentum and flavour
  state, i.e. which is consistent with further status=1,2 decay
  products.
- 3: historically a "documentation entry", though this term was never
  defined. It is often taken to mean a "matrix-element" particle
  directly connected to the hard-process calculation. Particles with
  this status should generally not be used in physics analysis.
- **4:** an incoming beam/collision particle. This status code should
  be set to disambiguate unusual beam configurations and cases where
  the beam particles cannot be the first two particles in the event
  table.
- 5–10: Reserved for future standards, and should not be used.
- 11-200: a model/generator-dependent particle that does not fulfill
  the criteria of status=2. Properties and physicality may vary,
  particles with this status should generally not be used in physics
  analysis though may be useful for generator-specific studies.
- 200+: not currently used by generator models, may be used for
  simulation- or user-specific purposes.

### Note on the physical status=1,2,4 codes

Status codes 1, 2, or 4 *must* be set on all physical `GenParticle`
objects intended for downstream use. These must have well-defined
lab-frame momenta and a pure quantum or semi-classical state.

Disjoint mass and lifetime eigenstates like $K^0_{S/L}$ vs
$K^0/\bar{K^0}$ may both be given status=2, but may require care in
downstream processing; event-writers should be aware of potential
issues for users reliant on mass or mean-lifetime checks against the
[PDG database](https://pdg.lbl.gov/).

### Note on status=3

Note that the graph structure of matrix elements is undefined and in
particular their internal structure is typically a sum of interfering
amplitudes. Matrix elements are also not physically distinguished from
perturbative corrections such as parton-shower algorithms. Use with
care.

### Note on status=4

The beam specification is not 100% defined for ion and nuclear
physics, i.e. whether the "beam" should be at ion or nucleon level.

In the interests of providing full information for event analysers, it
is recommended that the largest incoming system is included in the
event, i.e. the ion is encoded in the event graph even for models
which only simulate a single-nucleon collision within it.

Proposals for further standardisation should be made to the HepMC
development team and/or community MC forums such as the [LPCC MC
Working Group](https://lpcc.web.cern.ch/content/monte-carlo-wg).


## Event weights

HepMC events generally carry lists of weights, corresponding to named
*streams* of weight-origin. Weights may be physical, used to propagate
the effects of model variations, or unphysical for tracking of
generator-specific behaviours.

A community standard for encoding and decoding these meanings to
e.g. construct systematic-uncertainty bands or use in profiling fits
is [documented here]( https://arxiv.org/abs/2203.08230). The key
elements are: that

- weight names are restricted to ASCII alphanumeric characters and the
  punctuation set `=`, `_`, `.`, `+`, and `-`; slashes,
  brackets/parentheses, and colons/semicolons should not be used.
- the nominal weight (corresponding to the distribution from which the
  event was drawn) should be in the first position in the vector and
  titled `NOMINAL`, `DEFAULT`, `WEIGHT`, `0`, or the empty string.
- weight streams *not* to be propagated as cross-section variations
  should be prefixed with `EXTRA` or `IRREG`; see the standard for
  details.


## Vertex status codes

`GenVertex` objects also carry an integer status code, by analogy with
the `GenParticle` ones and accessed via the `GenParticle::status()`
member function. Vertex codes are optional, and most usually left in
undefined state. One historically agreed vertex-status numbering is:

- 0: undefined/unset.
- 1: primary/signal-process vertex: the "main", usually largest
  momentum-transfer interaction of the event. There may be multiple
  such vertices for modelling of multiple hard scatterings.
- 2: multiparton interaction (MPI) / underlying-event vertex: for
  model-dependent identification of secondary hard or semi-hard
  partonic scatterings within the same hadronic collision.
- 3: resonance decays: for heavy resonances (such as W, Z, t, H)
  decaying before hadronization.
- 4: parton-shower and hadronization vertices: for labelling of
  partonic evolution and its evolution into hadrons.
- 5: hadronic decays: vertices where unstable hadrons decay into
  further unstable or final-state particles.

This scheme is notably incomplete, for example missing codes for
pile-up interactions (HepMC is usually but not *necessarily* used for
single beam-particle interactions) and electroweak
corrections. Collider-physics users should not currently rely on vertex
status being consistently set, or set at all, between generators.

Note that the [NuHepMC](https://arxiv.org/abs/2310.13211) standard for
neutrino physics uses an alternative vertex-status scheme.

Proposals for further standardisation should be made to the HepMC
development team and/or community MC forums such as the [LPCC MC
Working Group](https://lpcc.web.cern.ch/content/monte-carlo-wg).
