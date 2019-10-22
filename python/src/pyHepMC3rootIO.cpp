// File: HepMC3/WriterRootTree.cpp
#include <HepMC3/WriterRootTree.h>
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
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/WriterRoot.h>
#include <HepMC3/WriterRootTree.h>
#include <HepMC3/ReaderRoot.h>
#include <HepMC3/ReaderRootTree.h>
#include <src/root_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_WriterRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// HepMC3::WriterRootTree file:HepMC3/WriterRootTree.h line:38
	 binder::WriterRootTree_binder(M("HepMC3"));

}


// File: HepMC3/ReaderRootTree.cpp
#include <HepMC3/ReaderRoot.h>
#include <HepMC3/ReaderRootTree.h>
#include <HepMC3/WriterRoot.h>
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
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/WriterRoot.h>
#include <HepMC3/WriterRootTree.h>
#include <HepMC3/ReaderRoot.h>
#include <HepMC3/ReaderRootTree.h>
#include <src/root_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_ReaderRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// HepMC3::ReaderRootTree file:HepMC3/ReaderRootTree.h line:34
	 binder::ReaderRootTree_binder(M("HepMC3"));

	// HepMC3::WriterRoot file:HepMC3/WriterRoot.h line:35
	 binder::WriterRoot_binder(M("HepMC3"));

	// HepMC3::ReaderRoot file:HepMC3/ReaderRoot.h line:32
	 binder::ReaderRoot_binder(M("HepMC3"));

}


#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include <pybind11/pybind11.h>

typedef std::function< pybind11::module & (std::string const &) > ModuleGetter;

void bind_HepMC3_WriterRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_ReaderRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M);


PYBIND11_MODULE(pyHepMC3rootIO, root_module) {
	root_module.doc() = "pyHepMC3rootIO module";

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

	bind_HepMC3_WriterRootTree(M);
	bind_HepMC3_ReaderRootTree(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3rootIO.sources
// pyHepMC3rootIO.cpp
// HepMC3/WriterRootTree.cpp
// HepMC3/ReaderRootTree.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3rootIO.modules
// HepMC3 
