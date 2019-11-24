// File: HepMC3/AttributeFeature.cpp
#include <HepMC3/AttributeFeature.h>
#include <HepMC3/Relatives.h>
#include <HepMC3/Selector.h>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <sstream> // __str__
#include <string>
#include <utility>
#include <vector>

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Relatives.h>
#include <HepMC3/Selector.h>
#include <src/search_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_AttributeFeature(std::function< pybind11::module &(std::string const &namespace_) > &M)
{

	 binder::search_binder(M("HepMC3"));
	{ // HepMC3::AttributeFeature file:HepMC3/AttributeFeature.h line:20
		pybind11::class_<HepMC3::AttributeFeature, std::shared_ptr<HepMC3::AttributeFeature>> cl(M("HepMC3"), "AttributeFeature", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("name") );

		cl.def( pybind11::init( [](HepMC3::AttributeFeature const &o){ return new HepMC3::AttributeFeature(o); } ) );
		cl.def("exists", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::AttributeFeature::*)() const) &HepMC3::AttributeFeature::exists, "C++: HepMC3::AttributeFeature::exists() const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>");
		cl.def("__eq__", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::AttributeFeature::*)(std::string) const) &HepMC3::AttributeFeature::operator==, "C++: HepMC3::AttributeFeature::operator==(std::string) const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>", pybind11::arg("rhs"));
		cl.def("assign", (class HepMC3::AttributeFeature & (HepMC3::AttributeFeature::*)(const class HepMC3::AttributeFeature &)) &HepMC3::AttributeFeature::operator=, "C++: HepMC3::AttributeFeature::operator=(const class HepMC3::AttributeFeature &) --> class HepMC3::AttributeFeature &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::Relatives file:HepMC3/Relatives.h line:51
		pybind11::class_<HepMC3::Relatives, std::shared_ptr<HepMC3::Relatives>> cl(M("HepMC3"), "Relatives", "Define a common interface that all Relatives objects will satisfy\n         Relatives provides an operator to get the relatives of a range of different\n         GenObject types.  The following are examples\n\n         Relatives::ANCESTORS(GenParticlePtr);// returns ancestors of the particle\n         Descendants descendants;\n         descendants(GenVertexPtr);// descendants of the vertex\n         vector<Relatives*> relations = {&Relatives::CHILDREN, &Relatives::DESCENDANTS, &Relatives::PARENTS, new Ancestors()}; // make a vector of Relatives\n\n         You can also define your own relation and wrap it in the Relatives interface using\n         Relatives * relo = new RelativesInterface<MyRelationClass>();");
		pybind11::handle cl_type = cl;

		cl.def("assign", (class HepMC3::Relatives & (HepMC3::Relatives::*)(const class HepMC3::Relatives &)) &HepMC3::Relatives::operator=, "C++: HepMC3::Relatives::operator=(const class HepMC3::Relatives &) --> class HepMC3::Relatives &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::_parents file:HepMC3/Relatives.h line:169
		pybind11::class_<HepMC3::_parents, std::shared_ptr<HepMC3::_parents>> cl(M("HepMC3"), "_parents", "Provides operator to find the parent particles of a Vertex or Particle\n\n Note you would usually not instantiate this directly, but wrap it in a RelativesInterface");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::_parents(); } ) );
	}
	{ // HepMC3::_children file:HepMC3/Relatives.h line:191
		pybind11::class_<HepMC3::_children, std::shared_ptr<HepMC3::_children>> cl(M("HepMC3"), "_children", "Provides operator to find the child particles of a Vertex or Particle\n\n Note you would usually not instantiate this directly, but wrap it in a RelativesInterface");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::_children(); } ) );
	}
	{ // HepMC3::Selector file:HepMC3/Selector.h line:55
		pybind11::class_<HepMC3::Selector, std::shared_ptr<HepMC3::Selector>> cl(M("HepMC3"), "Selector", "Selector is an interface to \"standard\" Features that are valid\n  for both integral and floating point comparisons\n\n  You would use this in preference to the more general\n  Feature<> templated type.  A Selector is constructed from a\n  function to extract features from particles, e.g.\n\n  ConstSelectorPtr status = std::make_shared<SelectorWrapper<int> >([](ConstParticlePtr p)->int{return p->status();});\n  ConstSelectorPtr pt = std::make_shared<SelectorWrapper<double> >([](ConstParticlePtr p)->double{return p->momentum().pt();});\n\n  You can then use the Selector to construct Filter functions that\n  evaluate on particles, e.g.\n  Filter is_stable = (*status) == 1;\n  bool stable = is_stable(p);\n  bool beam = (*status == 4)(p);\n\n  Selector contains a few standard Selectors already defined, e.g.\n\n  ConstGenParticlePtr p;\n  (Selector::STATUS == 1)(p);\n  (Selector::PT > 15.)(p);\n  (abs(Selector::RAPIDITY) < 2.5)(p);\n\n  you can also combined them e.g.\n\n  Filter myCuts = (Selector::PT > 15.) && (*abs(Selector::RAPIDITY) < 2.5) || (Selector::PT > 100.);\n  bool passCuts = myCuts(p);");
		pybind11::handle cl_type = cl;

		cl.def("__eq__", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::Selector::*)(int) const) &HepMC3::Selector::operator==, "C++: HepMC3::Selector::operator==(int) const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>", pybind11::arg("value"));
		cl.def("__eq__", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::Selector::*)(double) const) &HepMC3::Selector::operator==, "C++: HepMC3::Selector::operator==(double) const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>", pybind11::arg("value"));
		cl.def("__ne__", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::Selector::*)(int) const) &HepMC3::Selector::operator!=, "C++: HepMC3::Selector::operator!=(int) const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>", pybind11::arg("value"));
		cl.def("__ne__", (class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)> (HepMC3::Selector::*)(double) const) &HepMC3::Selector::operator!=, "C++: HepMC3::Selector::operator!=(double) const --> class std::function<bool (class std::shared_ptr<const class HepMC3::GenParticle>)>", pybind11::arg("value"));
		cl.def("abs", (class std::shared_ptr<const class HepMC3::Selector> (HepMC3::Selector::*)() const) &HepMC3::Selector::abs, "C++: HepMC3::Selector::abs() const --> class std::shared_ptr<const class HepMC3::Selector>");
		cl.def_static("ATTRIBUTE", (class HepMC3::AttributeFeature (*)(const std::string &)) &HepMC3::Selector::ATTRIBUTE, "C++: HepMC3::Selector::ATTRIBUTE(const std::string &) --> class HepMC3::AttributeFeature", pybind11::arg("name"));
		cl.def("assign", (class HepMC3::Selector & (HepMC3::Selector::*)(const class HepMC3::Selector &)) &HepMC3::Selector::operator=, "C++: HepMC3::Selector::operator=(const class HepMC3::Selector &) --> class HepMC3::Selector &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		binder::custom_Selector_binder(cl);
	}
	// HepMC3::abs(const class HepMC3::Selector &) file:HepMC3/Selector.h line:151
	M("HepMC3").def("abs", (class std::shared_ptr<const class HepMC3::Selector> (*)(const class HepMC3::Selector &)) &HepMC3::abs, "ConstSelectorPtr abs\n\nC++: HepMC3::abs(const class HepMC3::Selector &) --> class std::shared_ptr<const class HepMC3::Selector>", pybind11::arg("input"));

	{ // HepMC3::StandardSelector file:HepMC3/Selector.h line:166
		pybind11::class_<HepMC3::StandardSelector, std::shared_ptr<HepMC3::StandardSelector>, HepMC3::Selector> cl(M("HepMC3"), "StandardSelector", "StandardSelector ");
		pybind11::handle cl_type = cl;

		cl.def("assign", (class HepMC3::StandardSelector & (HepMC3::StandardSelector::*)(const class HepMC3::StandardSelector &)) &HepMC3::StandardSelector::operator=, "C++: HepMC3::StandardSelector::operator=(const class HepMC3::StandardSelector &) --> class HepMC3::StandardSelector &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
}


#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include <pybind11/pybind11.h>

typedef std::function< pybind11::module & (std::string const &) > ModuleGetter;

void bind_HepMC3_AttributeFeature(std::function< pybind11::module &(std::string const &namespace_) > &M);


PYBIND11_MODULE(pyHepMC3search, root_module) {
	root_module.doc() = "pyHepMC3search module";

	std::map <std::string, pybind11::module> modules;
	ModuleGetter M = [&](std::string const &namespace_) -> pybind11::module & {
		auto it = modules.find(namespace_);
		if( it == modules.end() ) throw std::runtime_error("Attempt to access pybind11::module for namespace " + namespace_ + " before it was created!!!");
		return it->second;
	};

	modules[""] = root_module;

	std::vector< std::pair<std::string, std::string> > sub_modules {
		{"", "HepMC3"},
	};
	for(auto &p : sub_modules ) modules[p.first.size() ? p.first+"::"+p.second : p.second] = modules[p.first].def_submodule(p.second.c_str(), ("Bindings for " + p.first + "::" + p.second + " namespace").c_str() );

	//pybind11::class_<std::shared_ptr<void>>(M(""), "_encapsulated_data_");

	bind_HepMC3_AttributeFeature(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.sources
// pyHepMC3search.cpp
// HepMC3/AttributeFeature.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.modules
// HepMC3 
