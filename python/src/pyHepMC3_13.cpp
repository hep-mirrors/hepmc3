#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenEventData.h>
#include <HepMC3/Data/GenParticleData.h>
#include <HepMC3/Data/GenRunInfoData.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/HEPEVT_Wrapper_Runtime.h>
#include <HepMC3/Reader.h>
#include <HepMC3/ReaderHEPEVT.h>
#include <HepMC3/Writer.h>
#include <HepMC3/WriterHEPEVT.h>
#include <functional>
#include <ios>
#include <istream>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <sstream> // __str__
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

#include <functional>
#include <pybind11/pybind11.h>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.h>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>)
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*)
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>)
#endif

// HepMC3::WriterHEPEVT file:HepMC3/WriterHEPEVT.h line:27
struct PyCallBack_HepMC3_WriterHEPEVT : public HepMC3::WriterHEPEVT {
	using HepMC3::WriterHEPEVT::WriterHEPEVT;

	void write_hepevt_particle(int a0, bool a1) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "write_hepevt_particle");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0, a1);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterHEPEVT::write_hepevt_particle(a0, a1);
	}
	void write_hepevt_event_header() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "write_hepevt_event_header");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterHEPEVT::write_hepevt_event_header();
	}
	void write_event(const class HepMC3::GenEvent & a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "write_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterHEPEVT::write_event(a0);
	}
	void close() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterHEPEVT::close();
	}
	bool failed() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterHEPEVT::failed();
	}
	void set_run_info(class std::shared_ptr<class HepMC3::GenRunInfo> a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "set_run_info");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return Writer::set_run_info(a0);
	}
	class std::shared_ptr<class HepMC3::GenRunInfo> run_info() const override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "run_info");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<class std::shared_ptr<class HepMC3::GenRunInfo>>::value) {
				static pybind11::detail::override_caster_t<class std::shared_ptr<class HepMC3::GenRunInfo>> caster;
				return pybind11::detail::cast_ref<class std::shared_ptr<class HepMC3::GenRunInfo>>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<class std::shared_ptr<class HepMC3::GenRunInfo>>(std::move(o));
		}
		return Writer::run_info();
	}
	void set_options(const class std::map<std::string, std::string > & a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "set_options");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return Writer::set_options(a0);
	}
	using _binder_ret_0 = std::map<std::string, std::string >;
	_binder_ret_0 get_options() const override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "get_options");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<_binder_ret_0>::value) {
				static pybind11::detail::override_caster_t<_binder_ret_0> caster;
				return pybind11::detail::cast_ref<_binder_ret_0>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<_binder_ret_0>(std::move(o));
		}
		return Writer::get_options();
	}
};

// HepMC3::ReaderHEPEVT file:HepMC3/ReaderHEPEVT.h line:32
struct PyCallBack_HepMC3_ReaderHEPEVT : public HepMC3::ReaderHEPEVT {
	using HepMC3::ReaderHEPEVT::ReaderHEPEVT;

	bool read_hepevt_event_header() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "read_hepevt_event_header");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::read_hepevt_event_header();
	}
	bool read_hepevt_particle(int a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "read_hepevt_particle");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::read_hepevt_particle(a0);
	}
	bool skip(const int a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "skip");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::skip(a0);
	}
	bool read_event(class HepMC3::GenEvent & a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::read_event(a0);
	}
	void close() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderHEPEVT::close();
	}
	bool failed() override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::override_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::failed();
	}
	class std::shared_ptr<class HepMC3::GenRunInfo> run_info() const override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "run_info");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<class std::shared_ptr<class HepMC3::GenRunInfo>>::value) {
				static pybind11::detail::override_caster_t<class std::shared_ptr<class HepMC3::GenRunInfo>> caster;
				return pybind11::detail::cast_ref<class std::shared_ptr<class HepMC3::GenRunInfo>>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<class std::shared_ptr<class HepMC3::GenRunInfo>>(std::move(o));
		}
		return Reader::run_info();
	}
	void set_options(const class std::map<std::string, std::string > & a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "set_options");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return Reader::set_options(a0);
	}
	using _binder_ret_0 = std::map<std::string, std::string >;
	_binder_ret_0 get_options() const override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "get_options");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<_binder_ret_0>::value) {
				static pybind11::detail::override_caster_t<_binder_ret_0> caster;
				return pybind11::detail::cast_ref<_binder_ret_0>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<_binder_ret_0>(std::move(o));
		}
		return Reader::get_options();
	}
	void set_run_info(class std::shared_ptr<class HepMC3::GenRunInfo> a0) override {
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "set_run_info");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::override_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return Reader::set_run_info(a0);
	}
};

void bind_pyHepMC3_13(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::HEPEVT_Pointers file: line:59
		pybind11::class_<HepMC3::HEPEVT_Pointers<double>, std::shared_ptr<HepMC3::HEPEVT_Pointers<double>>> cl(M("HepMC3"), "HEPEVT_Pointers_double_t", "");
		cl.def( pybind11::init( [](){ return new HepMC3::HEPEVT_Pointers<double>(); } ) );
	}
	{ // HepMC3::GenParticlePtr_greater file: line:72
		pybind11::class_<HepMC3::GenParticlePtr_greater, std::shared_ptr<HepMC3::GenParticlePtr_greater>> cl(M("HepMC3"), "GenParticlePtr_greater", "comparison of two particles ");
		cl.def( pybind11::init( [](){ return new HepMC3::GenParticlePtr_greater(); } ) );
		cl.def( pybind11::init( [](HepMC3::GenParticlePtr_greater const &o){ return new HepMC3::GenParticlePtr_greater(o); } ) );
		cl.def("__call__", (bool (HepMC3::GenParticlePtr_greater::*)(class std::shared_ptr<const class HepMC3::GenParticle>, class std::shared_ptr<const class HepMC3::GenParticle>) const) &HepMC3::GenParticlePtr_greater::operator(), "comparison of two particles \n\nC++: HepMC3::GenParticlePtr_greater::operator()(class std::shared_ptr<const class HepMC3::GenParticle>, class std::shared_ptr<const class HepMC3::GenParticle>) const --> bool", pybind11::arg("lx"), pybind11::arg("rx"));
	}
	{ // HepMC3::pair_GenVertexPtr_int_greater file: line:78
		pybind11::class_<HepMC3::pair_GenVertexPtr_int_greater, std::shared_ptr<HepMC3::pair_GenVertexPtr_int_greater>> cl(M("HepMC3"), "pair_GenVertexPtr_int_greater", "Order vertices with equal paths. ");
		cl.def( pybind11::init( [](){ return new HepMC3::pair_GenVertexPtr_int_greater(); } ) );
		cl.def( pybind11::init( [](HepMC3::pair_GenVertexPtr_int_greater const &o){ return new HepMC3::pair_GenVertexPtr_int_greater(o); } ) );
		cl.def("__call__", (bool (HepMC3::pair_GenVertexPtr_int_greater::*)(const struct std::pair<class std::shared_ptr<const class HepMC3::GenVertex>, int> &, const struct std::pair<class std::shared_ptr<const class HepMC3::GenVertex>, int> &) const) &HepMC3::pair_GenVertexPtr_int_greater::operator(), "Order vertices with equal paths. If the paths are equal, order in other quantities.\n We cannot use id, as it can be assigned in different way\n\nC++: HepMC3::pair_GenVertexPtr_int_greater::operator()(const struct std::pair<class std::shared_ptr<const class HepMC3::GenVertex>, int> &, const struct std::pair<class std::shared_ptr<const class HepMC3::GenVertex>, int> &) const --> bool", pybind11::arg("lx"), pybind11::arg("rx"));
	}
	// HepMC3::calculate_longest_path_to_top(class std::shared_ptr<const class HepMC3::GenVertex>, class std::map<class std::shared_ptr<const class HepMC3::GenVertex>, int> &) file: line:85
	M("HepMC3").def("calculate_longest_path_to_top", (void (*)(class std::shared_ptr<const class HepMC3::GenVertex>, class std::map<class std::shared_ptr<const class HepMC3::GenVertex>, int> &)) &HepMC3::calculate_longest_path_to_top, "Calculates the path to the top (beam) particles \n\nC++: HepMC3::calculate_longest_path_to_top(class std::shared_ptr<const class HepMC3::GenVertex>, class std::map<class std::shared_ptr<const class HepMC3::GenVertex>, int> &) --> void", pybind11::arg("v"), pybind11::arg("pathl"));

	// HepMC3::HEPEVT_to_GenEvent_nonstatic(class HepMC3::GenEvent *, const class HepMC3::HEPEVT_Wrapper_Runtime *) file: line:90
	M("HepMC3").def("HEPEVT_to_GenEvent_nonstatic", (bool (*)(class HepMC3::GenEvent *, const class HepMC3::HEPEVT_Wrapper_Runtime *)) &HepMC3::HEPEVT_to_GenEvent_nonstatic<const HepMC3::HEPEVT_Wrapper_Runtime>, "C++: HepMC3::HEPEVT_to_GenEvent_nonstatic(class HepMC3::GenEvent *, const class HepMC3::HEPEVT_Wrapper_Runtime *) --> bool", pybind11::arg("evt"), pybind11::arg("A"));

	// HepMC3::GenEvent_to_HEPEVT_nonstatic(const class HepMC3::GenEvent *, class HepMC3::HEPEVT_Wrapper_Runtime *) file: line:161
	M("HepMC3").def("GenEvent_to_HEPEVT_nonstatic", (bool (*)(const class HepMC3::GenEvent *, class HepMC3::HEPEVT_Wrapper_Runtime *)) &HepMC3::GenEvent_to_HEPEVT_nonstatic<HepMC3::HEPEVT_Wrapper_Runtime>, "C++: HepMC3::GenEvent_to_HEPEVT_nonstatic(const class HepMC3::GenEvent *, class HepMC3::HEPEVT_Wrapper_Runtime *) --> bool", pybind11::arg("evt"), pybind11::arg("A"));

	{ // HepMC3::HEPEVT_Wrapper_Template file: line:31
		pybind11::class_<HepMC3::HEPEVT_Wrapper_Template<100000,double>, std::shared_ptr<HepMC3::HEPEVT_Wrapper_Template<100000,double>>> cl(M("HepMC3"), "HEPEVT_Wrapper_Template_100000_double_t", "");
		cl.def( pybind11::init( [](){ return new HepMC3::HEPEVT_Wrapper_Template<100000,double>(); } ) );
		cl.def( pybind11::init( [](HepMC3::HEPEVT_Wrapper_Template<100000,double> const &o){ return new HepMC3::HEPEVT_Wrapper_Template<100000,double>(o); } ) );
		cl.def("zero_everything", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)()) &HepMC3::HEPEVT_Wrapper_Template<100000>::zero_everything, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::zero_everything() --> void");
		cl.def("GenEvent_to_HEPEVT", (bool (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const class HepMC3::GenEvent *)) &HepMC3::HEPEVT_Wrapper_Template<100000>::GenEvent_to_HEPEVT, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::GenEvent_to_HEPEVT(const class HepMC3::GenEvent *) --> bool", pybind11::arg("evt"));
		cl.def("HEPEVT_to_GenEvent", (bool (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(class HepMC3::GenEvent *) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::HEPEVT_to_GenEvent, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::HEPEVT_to_GenEvent(class HepMC3::GenEvent *) const --> bool", pybind11::arg("evt"));
		cl.def("fix_daughters", (bool (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)()) &HepMC3::HEPEVT_Wrapper_Template<100000>::fix_daughters, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::fix_daughters() --> bool");
		cl.def("allocate_internal_storage", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)()) &HepMC3::HEPEVT_Wrapper_Template<100000>::allocate_internal_storage, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::allocate_internal_storage() --> void");
		cl.def("copy_to_internal_storage", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(char *, int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::copy_to_internal_storage, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::copy_to_internal_storage(char *, int) --> void", pybind11::arg("c"), pybind11::arg("N"));
		cl.def("set_max_number_entries", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(unsigned int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_max_number_entries, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_max_number_entries(unsigned int) --> void", pybind11::arg("size"));
		cl.def("set_hepevt_address", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(char *)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_hepevt_address, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_hepevt_address(char *) --> void", pybind11::arg("c"));
		cl.def("max_number_entries", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)() const) &HepMC3::HEPEVT_Wrapper_Template<100000>::max_number_entries, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::max_number_entries() const --> int");
		cl.def("event_number", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)() const) &HepMC3::HEPEVT_Wrapper_Template<100000>::event_number, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::event_number() const --> int");
		cl.def("number_entries", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)() const) &HepMC3::HEPEVT_Wrapper_Template<100000>::number_entries, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::number_entries() const --> int");
		cl.def("status", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::status, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::status(const int) const --> int", pybind11::arg("index"));
		cl.def("id", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::id, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::id(const int) const --> int", pybind11::arg("index"));
		cl.def("first_parent", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::first_parent, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::first_parent(const int) const --> int", pybind11::arg("index"));
		cl.def("last_parent", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::last_parent, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::last_parent(const int) const --> int", pybind11::arg("index"));
		cl.def("first_child", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::first_child, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::first_child(const int) const --> int", pybind11::arg("index"));
		cl.def("last_child", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::last_child, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::last_child(const int) const --> int", pybind11::arg("index"));
		cl.def("px", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::px, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::px(const int) const --> double", pybind11::arg("index"));
		cl.def("py", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::py, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::py(const int) const --> double", pybind11::arg("index"));
		cl.def("pz", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::pz, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::pz(const int) const --> double", pybind11::arg("index"));
		cl.def("e", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::e, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::e(const int) const --> double", pybind11::arg("index"));
		cl.def("m", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::m, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::m(const int) const --> double", pybind11::arg("index"));
		cl.def("x", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::x, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::x(const int) const --> double", pybind11::arg("index"));
		cl.def("y", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::y, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::y(const int) const --> double", pybind11::arg("index"));
		cl.def("z", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::z, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::z(const int) const --> double", pybind11::arg("index"));
		cl.def("t", (double (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::t, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::t(const int) const --> double", pybind11::arg("index"));
		cl.def("number_parents", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::number_parents, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::number_parents(const int) const --> int", pybind11::arg("index"));
		cl.def("number_children", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::number_children, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::number_children(const int) const --> int", pybind11::arg("index"));
		cl.def("number_children_exact", (int (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int) const) &HepMC3::HEPEVT_Wrapper_Template<100000>::number_children_exact, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::number_children_exact(const int) const --> int", pybind11::arg("index"));
		cl.def("set_event_number", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_event_number, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_event_number(const int) --> void", pybind11::arg("evtno"));
		cl.def("set_number_entries", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_number_entries, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_number_entries(const int) --> void", pybind11::arg("noentries"));
		cl.def("set_status", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_status, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_status(const int, const int) --> void", pybind11::arg("index"), pybind11::arg("status"));
		cl.def("set_id", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_id, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_id(const int, const int) --> void", pybind11::arg("index"), pybind11::arg("id"));
		cl.def("set_parents", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const int, const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_parents, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_parents(const int, const int, const int) --> void", pybind11::arg("index"), pybind11::arg("firstparent"), pybind11::arg("lastparent"));
		cl.def("set_children", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const int, const int)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_children, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_children(const int, const int, const int) --> void", pybind11::arg("index"), pybind11::arg("firstchild"), pybind11::arg("lastchild"));
		cl.def("set_momentum", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const double, const double, const double, const double)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_momentum, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_momentum(const int, const double, const double, const double, const double) --> void", pybind11::arg("index"), pybind11::arg("px"), pybind11::arg("py"), pybind11::arg("pz"), pybind11::arg("e"));
		cl.def("set_mass", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, double)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_mass, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_mass(const int, double) --> void", pybind11::arg("index"), pybind11::arg("mass"));
		cl.def("set_position", (void (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const int, const double, const double, const double, const double)) &HepMC3::HEPEVT_Wrapper_Template<100000>::set_position, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::set_position(const int, const double, const double, const double, const double) --> void", pybind11::arg("index"), pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("z"), pybind11::arg("t"));
		cl.def("assign", (class HepMC3::HEPEVT_Wrapper_Template<100000> & (HepMC3::HEPEVT_Wrapper_Template<100000,double>::*)(const class HepMC3::HEPEVT_Wrapper_Template<100000> &)) &HepMC3::HEPEVT_Wrapper_Template<100000>::operator=, "C++: HepMC3::HEPEVT_Wrapper_Template<100000>::operator=(const class HepMC3::HEPEVT_Wrapper_Template<100000> &) --> class HepMC3::HEPEVT_Wrapper_Template<100000> &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::WriterHEPEVT file:HepMC3/WriterHEPEVT.h line:27
		pybind11::class_<HepMC3::WriterHEPEVT, std::shared_ptr<HepMC3::WriterHEPEVT>, PyCallBack_HepMC3_WriterHEPEVT, HepMC3::Writer> cl(M("HepMC3"), "WriterHEPEVT", "");
		cl.def( pybind11::init( [](const std::string & a0){ return new HepMC3::WriterHEPEVT(a0); }, [](const std::string & a0){ return new PyCallBack_HepMC3_WriterHEPEVT(a0); } ), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def("write_hepevt_particle", [](HepMC3::WriterHEPEVT &o, int const & a0) -> void { return o.write_hepevt_particle(a0); }, "", pybind11::arg("index"));
		cl.def("write_hepevt_particle", (void (HepMC3::WriterHEPEVT::*)(int, bool)) &HepMC3::WriterHEPEVT::write_hepevt_particle, "Write particle to file\n\n  \n Particle to be serialized\n  \n\n Format of record\n\nC++: HepMC3::WriterHEPEVT::write_hepevt_particle(int, bool) --> void", pybind11::arg("index"), pybind11::arg("iflong"));
		cl.def("write_hepevt_event_header", (void (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::write_hepevt_event_header, "Write event header to file\n\n     \n\nC++: HepMC3::WriterHEPEVT::write_hepevt_event_header() --> void");
		cl.def("write_event", (void (HepMC3::WriterHEPEVT::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterHEPEVT::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterHEPEVT::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::close, "Close file stream \n\nC++: HepMC3::WriterHEPEVT::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::failed, "Get stream error state flag \n\nC++: HepMC3::WriterHEPEVT::failed() --> bool");
		cl.def("set_vertices_positions_present", (void (HepMC3::WriterHEPEVT::*)(bool)) &HepMC3::WriterHEPEVT::set_vertices_positions_present, "set flag if vertex positions are available.\n  Effectively this adds or removes key \"vertices_positions_are_absent\"\n  to/from the m_options.\n\nC++: HepMC3::WriterHEPEVT::set_vertices_positions_present(bool) --> void", pybind11::arg("iflong"));
		cl.def("get_vertices_positions_present", (bool (HepMC3::WriterHEPEVT::*)() const) &HepMC3::WriterHEPEVT::get_vertices_positions_present, "get flag if vertex positions are available.\n The flag is deduced from m_options. If the m_options have the key\n \"vertices_positions_are_absent\" the result if false. True otherwise. \n\nC++: HepMC3::WriterHEPEVT::get_vertices_positions_present() const --> bool");
	}
	{ // HepMC3::ReaderHEPEVT file:HepMC3/ReaderHEPEVT.h line:32
		pybind11::class_<HepMC3::ReaderHEPEVT, std::shared_ptr<HepMC3::ReaderHEPEVT>, PyCallBack_HepMC3_ReaderHEPEVT, HepMC3::Reader> cl(M("HepMC3"), "ReaderHEPEVT", "");
		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_hepevt_event_header", (bool (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::read_hepevt_event_header, "Find and read event header line  from file\n\n    \n\nC++: HepMC3::ReaderHEPEVT::read_hepevt_event_header() --> bool");
		cl.def("read_hepevt_particle", (bool (HepMC3::ReaderHEPEVT::*)(int)) &HepMC3::ReaderHEPEVT::read_hepevt_particle, "read particle from file\n\n \n Particle id\n\nC++: HepMC3::ReaderHEPEVT::read_hepevt_particle(int) --> bool", pybind11::arg("i"));
		cl.def("skip", (bool (HepMC3::ReaderHEPEVT::*)(const int)) &HepMC3::ReaderHEPEVT::skip, "skip events\n\nC++: HepMC3::ReaderHEPEVT::skip(const int) --> bool", pybind11::arg(""));
		cl.def("read_event", (bool (HepMC3::ReaderHEPEVT::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderHEPEVT::read_event, "Read event from file\n\nC++: HepMC3::ReaderHEPEVT::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::close, "Close file stream \n\nC++: HepMC3::ReaderHEPEVT::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::failed, "Get stream error state \n\nC++: HepMC3::ReaderHEPEVT::failed() --> bool");
	}
}
