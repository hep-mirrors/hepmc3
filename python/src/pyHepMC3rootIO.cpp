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
#include <src/stl_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::WriterRootTree file:HepMC3/WriterRootTree.h line:38
struct PyCallBack_HepMC3_WriterRootTree : public HepMC3::WriterRootTree {
	using HepMC3::WriterRootTree::WriterRootTree;

	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterRootTree *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterRootTree::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterRootTree *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterRootTree::failed();
	}
};

void bind_HepMC3_WriterRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::WriterRootTree file:HepMC3/WriterRootTree.h line:38
		pybind11::class_<HepMC3::WriterRootTree, std::shared_ptr<HepMC3::WriterRootTree>, PyCallBack_HepMC3_WriterRootTree, HepMC3::Writer> cl(M("HepMC3"), "WriterRootTree", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0){ return new HepMC3::WriterRootTree(a0); }, [](const class std::basic_string<char> & a0){ return new PyCallBack_HepMC3_WriterRootTree(a0); } ), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0, const class std::basic_string<char> & a1, const class std::basic_string<char> & a2){ return new HepMC3::WriterRootTree(a0, a1, a2); }, [](const class std::basic_string<char> & a0, const class std::basic_string<char> & a1, const class std::basic_string<char> & a2){ return new PyCallBack_HepMC3_WriterRootTree(a0, a1, a2); } ), "doc");
		cl.def( pybind11::init<const std::string &, const std::string &, const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("treename"), pybind11::arg("branchname"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterRootTree::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterRootTree::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterRootTree::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::write_run_info, "Write the GenRunInfo object to file. \n\nC++: HepMC3::WriterRootTree::write_run_info() --> void");
		cl.def("close", (void (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::close, "Close file stream \n\nC++: HepMC3::WriterRootTree::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::failed, "Get stream error state flag \n\nC++: HepMC3::WriterRootTree::failed() --> bool");
	}
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
#include <src/stl_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::ReaderRootTree file:HepMC3/ReaderRootTree.h line:34
struct PyCallBack_HepMC3_ReaderRootTree : public HepMC3::ReaderRootTree {
	using HepMC3::ReaderRootTree::ReaderRootTree;

	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderRootTree *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderRootTree::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderRootTree *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderRootTree::failed();
	}
};

// HepMC3::WriterRoot file:HepMC3/WriterRoot.h line:35
struct PyCallBack_HepMC3_WriterRoot : public HepMC3::WriterRoot {
	using HepMC3::WriterRoot::WriterRoot;

	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterRoot *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterRoot::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterRoot *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterRoot::failed();
	}
};

// HepMC3::ReaderRoot file:HepMC3/ReaderRoot.h line:32
struct PyCallBack_HepMC3_ReaderRoot : public HepMC3::ReaderRoot {
	using HepMC3::ReaderRoot::ReaderRoot;

	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderRoot *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderRoot::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderRoot *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderRoot::failed();
	}
};

void bind_HepMC3_ReaderRootTree(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::ReaderRootTree file:HepMC3/ReaderRootTree.h line:34
		pybind11::class_<HepMC3::ReaderRootTree, std::shared_ptr<HepMC3::ReaderRootTree>, PyCallBack_HepMC3_ReaderRootTree, HepMC3::Reader> cl(M("HepMC3"), "ReaderRootTree", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def( pybind11::init<const std::string &, const std::string &, const std::string &>(), pybind11::arg("filename"), pybind11::arg("treename"), pybind11::arg("branchname") );

		cl.def("read_event", (bool (HepMC3::ReaderRootTree::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderRootTree::read_event, "Read event from file\n\n  \n Contains parsed event\n\nC++: HepMC3::ReaderRootTree::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::ReaderRootTree::*)()) &HepMC3::ReaderRootTree::close, "Close file \n\nC++: HepMC3::ReaderRootTree::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderRootTree::*)()) &HepMC3::ReaderRootTree::failed, "Get file  error state \n\nC++: HepMC3::ReaderRootTree::failed() --> bool");
	}
	{ // HepMC3::WriterRoot file:HepMC3/WriterRoot.h line:35
		pybind11::class_<HepMC3::WriterRoot, std::shared_ptr<HepMC3::WriterRoot>, PyCallBack_HepMC3_WriterRoot, HepMC3::Writer> cl(M("HepMC3"), "WriterRoot", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0){ return new HepMC3::WriterRoot(a0); }, [](const class std::basic_string<char> & a0){ return new PyCallBack_HepMC3_WriterRoot(a0); } ), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterRoot::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterRoot::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterRoot::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::write_run_info, "Write the GenRunInfo object to file. \n\nC++: HepMC3::WriterRoot::write_run_info() --> void");
		cl.def("close", (void (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::close, "Close file stream \n\nC++: HepMC3::WriterRoot::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::failed, "Get stream error state flag \n\nC++: HepMC3::WriterRoot::failed() --> bool");
	}
	{ // HepMC3::ReaderRoot file:HepMC3/ReaderRoot.h line:32
		pybind11::class_<HepMC3::ReaderRoot, std::shared_ptr<HepMC3::ReaderRoot>, PyCallBack_HepMC3_ReaderRoot, HepMC3::Reader> cl(M("HepMC3"), "ReaderRoot", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_event", (bool (HepMC3::ReaderRoot::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderRoot::read_event, "Read event from file\n\n  \n Contains parsed event\n\nC++: HepMC3::ReaderRoot::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::ReaderRoot::*)()) &HepMC3::ReaderRoot::close, "Close file stream \n\nC++: HepMC3::ReaderRoot::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderRoot::*)()) &HepMC3::ReaderRoot::failed, "Get stream error state \n\nC++: HepMC3::ReaderRoot::failed() --> bool");
	}
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
