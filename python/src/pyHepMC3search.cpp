// File: unknown/unknown.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
#include <string>

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

// HepMC3::IntAttribute file: line:158
struct PyCallBack_HepMC3_IntAttribute : public HepMC3::IntAttribute {
	using HepMC3::IntAttribute::IntAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::IntAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return IntAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::IntAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return IntAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::IntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::LongAttribute file: line:199
struct PyCallBack_HepMC3_LongAttribute : public HepMC3::LongAttribute {
	using HepMC3::LongAttribute::LongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::DoubleAttribute file: line:242
struct PyCallBack_HepMC3_DoubleAttribute : public HepMC3::DoubleAttribute {
	using HepMC3::DoubleAttribute::DoubleAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::DoubleAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return DoubleAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::DoubleAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return DoubleAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::DoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::FloatAttribute file: line:287
struct PyCallBack_HepMC3_FloatAttribute : public HepMC3::FloatAttribute {
	using HepMC3::FloatAttribute::FloatAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::FloatAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return FloatAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::FloatAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return FloatAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::FloatAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::StringAttribute file: line:336
struct PyCallBack_HepMC3_StringAttribute : public HepMC3::StringAttribute {
	using HepMC3::StringAttribute::StringAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::StringAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return StringAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::StringAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return StringAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::StringAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::CharAttribute file: line:380
struct PyCallBack_HepMC3_CharAttribute : public HepMC3::CharAttribute {
	using HepMC3::CharAttribute::CharAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::CharAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return CharAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::CharAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return CharAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::CharAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::LongLongAttribute file: line:425
struct PyCallBack_HepMC3_LongLongAttribute : public HepMC3::LongLongAttribute {
	using HepMC3::LongLongAttribute::LongLongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongLongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongLongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongLongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongLongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::LongDoubleAttribute file: line:468
struct PyCallBack_HepMC3_LongDoubleAttribute : public HepMC3::LongDoubleAttribute {
	using HepMC3::LongDoubleAttribute::LongDoubleAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongDoubleAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongDoubleAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongDoubleAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return LongDoubleAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongDoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M)
{

	 binder::search_binder(M("HepMC3"));
	{ // HepMC3::IntAttribute file: line:158
		pybind11::class_<HepMC3::IntAttribute, std::shared_ptr<HepMC3::IntAttribute>, PyCallBack_HepMC3_IntAttribute> cl(M("HepMC3"), "IntAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::IntAttribute(); }, [](){ return new PyCallBack_HepMC3_IntAttribute(); } ) );
		cl.def( pybind11::init<int>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_IntAttribute const &o){ return new PyCallBack_HepMC3_IntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::IntAttribute const &o){ return new HepMC3::IntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::IntAttribute::*)(const std::string &)) &HepMC3::IntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::IntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::IntAttribute::*)(std::string &) const) &HepMC3::IntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::IntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (int (HepMC3::IntAttribute::*)() const) &HepMC3::IntAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::IntAttribute::value() const --> int");
		cl.def("set_value", (void (HepMC3::IntAttribute::*)(const int &)) &HepMC3::IntAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::IntAttribute::set_value(const int &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::IntAttribute & (HepMC3::IntAttribute::*)(const class HepMC3::IntAttribute &)) &HepMC3::IntAttribute::operator=, "C++: HepMC3::IntAttribute::operator=(const class HepMC3::IntAttribute &) --> class HepMC3::IntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::LongAttribute file: line:199
		pybind11::class_<HepMC3::LongAttribute, std::shared_ptr<HepMC3::LongAttribute>, PyCallBack_HepMC3_LongAttribute> cl(M("HepMC3"), "LongAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::LongAttribute(); }, [](){ return new PyCallBack_HepMC3_LongAttribute(); } ) );
		cl.def( pybind11::init<long>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_LongAttribute const &o){ return new PyCallBack_HepMC3_LongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::LongAttribute const &o){ return new HepMC3::LongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::LongAttribute::*)(const std::string &)) &HepMC3::LongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::LongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::LongAttribute::*)(std::string &) const) &HepMC3::LongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::LongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (long (HepMC3::LongAttribute::*)() const) &HepMC3::LongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::LongAttribute::value() const --> long");
		cl.def("set_value", (void (HepMC3::LongAttribute::*)(const long &)) &HepMC3::LongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::LongAttribute::set_value(const long &) --> void", pybind11::arg("l"));
		cl.def("assign", (class HepMC3::LongAttribute & (HepMC3::LongAttribute::*)(const class HepMC3::LongAttribute &)) &HepMC3::LongAttribute::operator=, "C++: HepMC3::LongAttribute::operator=(const class HepMC3::LongAttribute &) --> class HepMC3::LongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::DoubleAttribute file: line:242
		pybind11::class_<HepMC3::DoubleAttribute, std::shared_ptr<HepMC3::DoubleAttribute>, PyCallBack_HepMC3_DoubleAttribute> cl(M("HepMC3"), "DoubleAttribute", "Attribute that holds a real number as a double.\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::DoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_DoubleAttribute(); } ) );
		cl.def( pybind11::init<double>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_DoubleAttribute const &o){ return new PyCallBack_HepMC3_DoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::DoubleAttribute const &o){ return new HepMC3::DoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::DoubleAttribute::*)(const std::string &)) &HepMC3::DoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::DoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::DoubleAttribute::*)(std::string &) const) &HepMC3::DoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::DoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (double (HepMC3::DoubleAttribute::*)() const) &HepMC3::DoubleAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::DoubleAttribute::value() const --> double");
		cl.def("set_value", (void (HepMC3::DoubleAttribute::*)(const double &)) &HepMC3::DoubleAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::DoubleAttribute::set_value(const double &) --> void", pybind11::arg("d"));
		cl.def("assign", (class HepMC3::DoubleAttribute & (HepMC3::DoubleAttribute::*)(const class HepMC3::DoubleAttribute &)) &HepMC3::DoubleAttribute::operator=, "C++: HepMC3::DoubleAttribute::operator=(const class HepMC3::DoubleAttribute &) --> class HepMC3::DoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::FloatAttribute file: line:287
		pybind11::class_<HepMC3::FloatAttribute, std::shared_ptr<HepMC3::FloatAttribute>, PyCallBack_HepMC3_FloatAttribute> cl(M("HepMC3"), "FloatAttribute", "Attribute that holds a real number as a float.\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::FloatAttribute(); }, [](){ return new PyCallBack_HepMC3_FloatAttribute(); } ) );
		cl.def( pybind11::init<float>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_FloatAttribute const &o){ return new PyCallBack_HepMC3_FloatAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::FloatAttribute const &o){ return new HepMC3::FloatAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::FloatAttribute::*)(const std::string &)) &HepMC3::FloatAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::FloatAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::FloatAttribute::*)(std::string &) const) &HepMC3::FloatAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::FloatAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (float (HepMC3::FloatAttribute::*)() const) &HepMC3::FloatAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::FloatAttribute::value() const --> float");
		cl.def("set_value", (void (HepMC3::FloatAttribute::*)(const float &)) &HepMC3::FloatAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::FloatAttribute::set_value(const float &) --> void", pybind11::arg("f"));
		cl.def("assign", (class HepMC3::FloatAttribute & (HepMC3::FloatAttribute::*)(const class HepMC3::FloatAttribute &)) &HepMC3::FloatAttribute::operator=, "C++: HepMC3::FloatAttribute::operator=(const class HepMC3::FloatAttribute &) --> class HepMC3::FloatAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::StringAttribute file: line:336
		pybind11::class_<HepMC3::StringAttribute, std::shared_ptr<HepMC3::StringAttribute>, PyCallBack_HepMC3_StringAttribute> cl(M("HepMC3"), "StringAttribute", "Attribute that holds a string\n\n  Default attribute constructed when reading input files.\n  It can be then parsed by other attributes or left as a string.\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::StringAttribute(); }, [](){ return new PyCallBack_HepMC3_StringAttribute(); } ) );
		cl.def( pybind11::init<const std::string &>(), pybind11::arg("st") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_StringAttribute const &o){ return new PyCallBack_HepMC3_StringAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::StringAttribute const &o){ return new HepMC3::StringAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::StringAttribute::*)(const std::string &)) &HepMC3::StringAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::StringAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::StringAttribute::*)(std::string &) const) &HepMC3::StringAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::StringAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (std::string (HepMC3::StringAttribute::*)() const) &HepMC3::StringAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::StringAttribute::value() const --> std::string");
		cl.def("set_value", (void (HepMC3::StringAttribute::*)(const std::string &)) &HepMC3::StringAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::StringAttribute::set_value(const std::string &) --> void", pybind11::arg("s"));
		cl.def("assign", (class HepMC3::StringAttribute & (HepMC3::StringAttribute::*)(const class HepMC3::StringAttribute &)) &HepMC3::StringAttribute::operator=, "C++: HepMC3::StringAttribute::operator=(const class HepMC3::StringAttribute &) --> class HepMC3::StringAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::CharAttribute file: line:380
		pybind11::class_<HepMC3::CharAttribute, std::shared_ptr<HepMC3::CharAttribute>, PyCallBack_HepMC3_CharAttribute> cl(M("HepMC3"), "CharAttribute", "Attribute that holds an Chareger implemented as an int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::CharAttribute(); }, [](){ return new PyCallBack_HepMC3_CharAttribute(); } ) );
		cl.def( pybind11::init<char>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_CharAttribute const &o){ return new PyCallBack_HepMC3_CharAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::CharAttribute const &o){ return new HepMC3::CharAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::CharAttribute::*)(const std::string &)) &HepMC3::CharAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::CharAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::CharAttribute::*)(std::string &) const) &HepMC3::CharAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::CharAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (char (HepMC3::CharAttribute::*)() const) &HepMC3::CharAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::CharAttribute::value() const --> char");
		cl.def("set_value", (void (HepMC3::CharAttribute::*)(const char &)) &HepMC3::CharAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::CharAttribute::set_value(const char &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::CharAttribute & (HepMC3::CharAttribute::*)(const class HepMC3::CharAttribute &)) &HepMC3::CharAttribute::operator=, "C++: HepMC3::CharAttribute::operator=(const class HepMC3::CharAttribute &) --> class HepMC3::CharAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::LongLongAttribute file: line:425
		pybind11::class_<HepMC3::LongLongAttribute, std::shared_ptr<HepMC3::LongLongAttribute>, PyCallBack_HepMC3_LongLongAttribute> cl(M("HepMC3"), "LongLongAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::LongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_LongLongAttribute(); } ) );
		cl.def( pybind11::init<long long>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_LongLongAttribute const &o){ return new PyCallBack_HepMC3_LongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::LongLongAttribute const &o){ return new HepMC3::LongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::LongLongAttribute::*)(const std::string &)) &HepMC3::LongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::LongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::LongLongAttribute::*)(std::string &) const) &HepMC3::LongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::LongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (long long (HepMC3::LongLongAttribute::*)() const) &HepMC3::LongLongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::LongLongAttribute::value() const --> long long");
		cl.def("set_value", (void (HepMC3::LongLongAttribute::*)(const long long &)) &HepMC3::LongLongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::LongLongAttribute::set_value(const long long &) --> void", pybind11::arg("l"));
		cl.def("assign", (class HepMC3::LongLongAttribute & (HepMC3::LongLongAttribute::*)(const class HepMC3::LongLongAttribute &)) &HepMC3::LongLongAttribute::operator=, "C++: HepMC3::LongLongAttribute::operator=(const class HepMC3::LongLongAttribute &) --> class HepMC3::LongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::LongDoubleAttribute file: line:468
		pybind11::class_<HepMC3::LongDoubleAttribute, std::shared_ptr<HepMC3::LongDoubleAttribute>, PyCallBack_HepMC3_LongDoubleAttribute> cl(M("HepMC3"), "LongDoubleAttribute", "Attribute that holds a real number as a double.\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::LongDoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_LongDoubleAttribute(); } ) );
		cl.def( pybind11::init<long double>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_LongDoubleAttribute const &o){ return new PyCallBack_HepMC3_LongDoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::LongDoubleAttribute const &o){ return new HepMC3::LongDoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::LongDoubleAttribute::*)(const std::string &)) &HepMC3::LongDoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::LongDoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::LongDoubleAttribute::*)(std::string &) const) &HepMC3::LongDoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::LongDoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (long double (HepMC3::LongDoubleAttribute::*)() const) &HepMC3::LongDoubleAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::LongDoubleAttribute::value() const --> long double");
		cl.def("set_value", (void (HepMC3::LongDoubleAttribute::*)(const long double &)) &HepMC3::LongDoubleAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::LongDoubleAttribute::set_value(const long double &) --> void", pybind11::arg("d"));
		cl.def("assign", (class HepMC3::LongDoubleAttribute & (HepMC3::LongDoubleAttribute::*)(const class HepMC3::LongDoubleAttribute &)) &HepMC3::LongDoubleAttribute::operator=, "C++: HepMC3::LongDoubleAttribute::operator=(const class HepMC3::LongDoubleAttribute &) --> class HepMC3::LongDoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
}


// File: unknown/unknown_1.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
#include <string>
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

// HepMC3::UIntAttribute file: line:515
struct PyCallBack_HepMC3_UIntAttribute : public HepMC3::UIntAttribute {
	using HepMC3::UIntAttribute::UIntAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::UIntAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return UIntAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::UIntAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return UIntAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::UIntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::ULongAttribute file: line:558
struct PyCallBack_HepMC3_ULongAttribute : public HepMC3::ULongAttribute {
	using HepMC3::ULongAttribute::ULongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ULongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ULongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::ULongLongAttribute file: line:600
struct PyCallBack_HepMC3_ULongLongAttribute : public HepMC3::ULongLongAttribute {
	using HepMC3::ULongLongAttribute::ULongLongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongLongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ULongLongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongLongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ULongLongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::BoolAttribute file: line:640
struct PyCallBack_HepMC3_BoolAttribute : public HepMC3::BoolAttribute {
	using HepMC3::BoolAttribute::BoolAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::BoolAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return BoolAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::BoolAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return BoolAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::BoolAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorCharAttribute file: line:683
struct PyCallBack_HepMC3_VectorCharAttribute : public HepMC3::VectorCharAttribute {
	using HepMC3::VectorCharAttribute::VectorCharAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorCharAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorCharAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorCharAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorCharAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorCharAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorFloatAttribute file: line:728
struct PyCallBack_HepMC3_VectorFloatAttribute : public HepMC3::VectorFloatAttribute {
	using HepMC3::VectorFloatAttribute::VectorFloatAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorFloatAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorFloatAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorFloatAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorFloatAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorFloatAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorLongDoubleAttribute file: line:774
struct PyCallBack_HepMC3_VectorLongDoubleAttribute : public HepMC3::VectorLongDoubleAttribute {
	using HepMC3::VectorLongDoubleAttribute::VectorLongDoubleAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongDoubleAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongDoubleAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongDoubleAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongDoubleAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongDoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorLongLongAttribute file: line:821
struct PyCallBack_HepMC3_VectorLongLongAttribute : public HepMC3::VectorLongLongAttribute {
	using HepMC3::VectorLongLongAttribute::VectorLongLongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongLongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongLongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongLongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongLongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorUIntAttribute file: line:866
struct PyCallBack_HepMC3_VectorUIntAttribute : public HepMC3::VectorUIntAttribute {
	using HepMC3::VectorUIntAttribute::VectorUIntAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorUIntAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorUIntAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorUIntAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorUIntAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorUIntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

void bind_unknown_unknown_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::UIntAttribute file: line:515
		pybind11::class_<HepMC3::UIntAttribute, std::shared_ptr<HepMC3::UIntAttribute>, PyCallBack_HepMC3_UIntAttribute> cl(M("HepMC3"), "UIntAttribute", "Attribute that holds an unsigned int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::UIntAttribute(); }, [](){ return new PyCallBack_HepMC3_UIntAttribute(); } ) );
		cl.def( pybind11::init<unsigned int>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_UIntAttribute const &o){ return new PyCallBack_HepMC3_UIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::UIntAttribute const &o){ return new HepMC3::UIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::UIntAttribute::*)(const std::string &)) &HepMC3::UIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::UIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::UIntAttribute::*)(std::string &) const) &HepMC3::UIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::UIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (unsigned int (HepMC3::UIntAttribute::*)() const) &HepMC3::UIntAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::UIntAttribute::value() const --> unsigned int");
		cl.def("set_value", (void (HepMC3::UIntAttribute::*)(const unsigned int &)) &HepMC3::UIntAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::UIntAttribute::set_value(const unsigned int &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::UIntAttribute & (HepMC3::UIntAttribute::*)(const class HepMC3::UIntAttribute &)) &HepMC3::UIntAttribute::operator=, "C++: HepMC3::UIntAttribute::operator=(const class HepMC3::UIntAttribute &) --> class HepMC3::UIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::ULongAttribute file: line:558
		pybind11::class_<HepMC3::ULongAttribute, std::shared_ptr<HepMC3::ULongAttribute>, PyCallBack_HepMC3_ULongAttribute> cl(M("HepMC3"), "ULongAttribute", "Attribute that holds an unsigned long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::ULongAttribute(); }, [](){ return new PyCallBack_HepMC3_ULongAttribute(); } ) );
		cl.def( pybind11::init<unsigned long>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_ULongAttribute const &o){ return new PyCallBack_HepMC3_ULongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::ULongAttribute const &o){ return new HepMC3::ULongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::ULongAttribute::*)(const std::string &)) &HepMC3::ULongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::ULongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::ULongAttribute::*)(std::string &) const) &HepMC3::ULongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::ULongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (unsigned long (HepMC3::ULongAttribute::*)() const) &HepMC3::ULongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::ULongAttribute::value() const --> unsigned long");
		cl.def("set_value", (void (HepMC3::ULongAttribute::*)(const unsigned long &)) &HepMC3::ULongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::ULongAttribute::set_value(const unsigned long &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::ULongAttribute & (HepMC3::ULongAttribute::*)(const class HepMC3::ULongAttribute &)) &HepMC3::ULongAttribute::operator=, "C++: HepMC3::ULongAttribute::operator=(const class HepMC3::ULongAttribute &) --> class HepMC3::ULongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::ULongLongAttribute file: line:600
		pybind11::class_<HepMC3::ULongLongAttribute, std::shared_ptr<HepMC3::ULongLongAttribute>, PyCallBack_HepMC3_ULongLongAttribute> cl(M("HepMC3"), "ULongLongAttribute", "Attribute that holds an unsigned long long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::ULongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_ULongLongAttribute(); } ) );
		cl.def( pybind11::init<unsigned long long>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_ULongLongAttribute const &o){ return new PyCallBack_HepMC3_ULongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::ULongLongAttribute const &o){ return new HepMC3::ULongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::ULongLongAttribute::*)(const std::string &)) &HepMC3::ULongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::ULongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::ULongLongAttribute::*)(std::string &) const) &HepMC3::ULongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::ULongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (unsigned long long (HepMC3::ULongLongAttribute::*)() const) &HepMC3::ULongLongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::ULongLongAttribute::value() const --> unsigned long long");
		cl.def("set_value", (void (HepMC3::ULongLongAttribute::*)(const unsigned long long &)) &HepMC3::ULongLongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::ULongLongAttribute::set_value(const unsigned long long &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::ULongLongAttribute & (HepMC3::ULongLongAttribute::*)(const class HepMC3::ULongLongAttribute &)) &HepMC3::ULongLongAttribute::operator=, "C++: HepMC3::ULongLongAttribute::operator=(const class HepMC3::ULongLongAttribute &) --> class HepMC3::ULongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::BoolAttribute file: line:640
		pybind11::class_<HepMC3::BoolAttribute, std::shared_ptr<HepMC3::BoolAttribute>, PyCallBack_HepMC3_BoolAttribute> cl(M("HepMC3"), "BoolAttribute", "Attribute that holds an Booleger implemented as an int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::BoolAttribute(); }, [](){ return new PyCallBack_HepMC3_BoolAttribute(); } ) );
		cl.def( pybind11::init<bool>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_BoolAttribute const &o){ return new PyCallBack_HepMC3_BoolAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::BoolAttribute const &o){ return new HepMC3::BoolAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::BoolAttribute::*)(const std::string &)) &HepMC3::BoolAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::BoolAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::BoolAttribute::*)(std::string &) const) &HepMC3::BoolAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::BoolAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (bool (HepMC3::BoolAttribute::*)() const) &HepMC3::BoolAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::BoolAttribute::value() const --> bool");
		cl.def("set_value", (void (HepMC3::BoolAttribute::*)(const bool &)) &HepMC3::BoolAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::BoolAttribute::set_value(const bool &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::BoolAttribute & (HepMC3::BoolAttribute::*)(const class HepMC3::BoolAttribute &)) &HepMC3::BoolAttribute::operator=, "C++: HepMC3::BoolAttribute::operator=(const class HepMC3::BoolAttribute &) --> class HepMC3::BoolAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorCharAttribute file: line:683
		pybind11::class_<HepMC3::VectorCharAttribute, std::shared_ptr<HepMC3::VectorCharAttribute>, PyCallBack_HepMC3_VectorCharAttribute> cl(M("HepMC3"), "VectorCharAttribute", "Attribute that holds a vector of charegers of type  char\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorCharAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorCharAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorCharAttribute const &o){ return new PyCallBack_HepMC3_VectorCharAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorCharAttribute const &o){ return new HepMC3::VectorCharAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorCharAttribute::*)(const std::string &)) &HepMC3::VectorCharAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorCharAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorCharAttribute::*)(std::string &) const) &HepMC3::VectorCharAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorCharAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorCharAttribute & (HepMC3::VectorCharAttribute::*)(const class HepMC3::VectorCharAttribute &)) &HepMC3::VectorCharAttribute::operator=, "C++: HepMC3::VectorCharAttribute::operator=(const class HepMC3::VectorCharAttribute &) --> class HepMC3::VectorCharAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorFloatAttribute file: line:728
		pybind11::class_<HepMC3::VectorFloatAttribute, std::shared_ptr<HepMC3::VectorFloatAttribute>, PyCallBack_HepMC3_VectorFloatAttribute> cl(M("HepMC3"), "VectorFloatAttribute", "Attribute that holds a vector of floategers of type  float\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorFloatAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorFloatAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorFloatAttribute const &o){ return new PyCallBack_HepMC3_VectorFloatAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorFloatAttribute const &o){ return new HepMC3::VectorFloatAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorFloatAttribute::*)(const std::string &)) &HepMC3::VectorFloatAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorFloatAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorFloatAttribute::*)(std::string &) const) &HepMC3::VectorFloatAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorFloatAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorFloatAttribute & (HepMC3::VectorFloatAttribute::*)(const class HepMC3::VectorFloatAttribute &)) &HepMC3::VectorFloatAttribute::operator=, "C++: HepMC3::VectorFloatAttribute::operator=(const class HepMC3::VectorFloatAttribute &) --> class HepMC3::VectorFloatAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorLongDoubleAttribute file: line:774
		pybind11::class_<HepMC3::VectorLongDoubleAttribute, std::shared_ptr<HepMC3::VectorLongDoubleAttribute>, PyCallBack_HepMC3_VectorLongDoubleAttribute> cl(M("HepMC3"), "VectorLongDoubleAttribute", "Attribute that holds a vector of long doubleegers of type  long double\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongDoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongDoubleAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongDoubleAttribute const &o){ return new PyCallBack_HepMC3_VectorLongDoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongDoubleAttribute const &o){ return new HepMC3::VectorLongDoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongDoubleAttribute::*)(const std::string &)) &HepMC3::VectorLongDoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongDoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongDoubleAttribute::*)(std::string &) const) &HepMC3::VectorLongDoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongDoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorLongDoubleAttribute & (HepMC3::VectorLongDoubleAttribute::*)(const class HepMC3::VectorLongDoubleAttribute &)) &HepMC3::VectorLongDoubleAttribute::operator=, "C++: HepMC3::VectorLongDoubleAttribute::operator=(const class HepMC3::VectorLongDoubleAttribute &) --> class HepMC3::VectorLongDoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorLongLongAttribute file: line:821
		pybind11::class_<HepMC3::VectorLongLongAttribute, std::shared_ptr<HepMC3::VectorLongLongAttribute>, PyCallBack_HepMC3_VectorLongLongAttribute> cl(M("HepMC3"), "VectorLongLongAttribute", "Attribute that holds a vector of long longegers of type  long long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongLongAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongLongAttribute const &o){ return new PyCallBack_HepMC3_VectorLongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongLongAttribute const &o){ return new HepMC3::VectorLongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongLongAttribute::*)(const std::string &)) &HepMC3::VectorLongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongLongAttribute::*)(std::string &) const) &HepMC3::VectorLongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorLongLongAttribute & (HepMC3::VectorLongLongAttribute::*)(const class HepMC3::VectorLongLongAttribute &)) &HepMC3::VectorLongLongAttribute::operator=, "C++: HepMC3::VectorLongLongAttribute::operator=(const class HepMC3::VectorLongLongAttribute &) --> class HepMC3::VectorLongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorUIntAttribute file: line:866
		pybind11::class_<HepMC3::VectorUIntAttribute, std::shared_ptr<HepMC3::VectorUIntAttribute>, PyCallBack_HepMC3_VectorUIntAttribute> cl(M("HepMC3"), "VectorUIntAttribute", "Attribute that holds a vector of unsigned integers of type  unsigned int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorUIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorUIntAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorUIntAttribute const &o){ return new PyCallBack_HepMC3_VectorUIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorUIntAttribute const &o){ return new HepMC3::VectorUIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorUIntAttribute::*)(const std::string &)) &HepMC3::VectorUIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorUIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorUIntAttribute::*)(std::string &) const) &HepMC3::VectorUIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorUIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorUIntAttribute & (HepMC3::VectorUIntAttribute::*)(const class HepMC3::VectorUIntAttribute &)) &HepMC3::VectorUIntAttribute::operator=, "C++: HepMC3::VectorUIntAttribute::operator=(const class HepMC3::VectorUIntAttribute &) --> class HepMC3::VectorUIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
}


// File: unknown/unknown_2.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
#include <string>
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

// HepMC3::VectorULongAttribute file: line:911
struct PyCallBack_HepMC3_VectorULongAttribute : public HepMC3::VectorULongAttribute {
	using HepMC3::VectorULongAttribute::VectorULongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorULongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorULongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorULongLongAttribute file: line:957
struct PyCallBack_HepMC3_VectorULongLongAttribute : public HepMC3::VectorULongLongAttribute {
	using HepMC3::VectorULongLongAttribute::VectorULongLongAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongLongAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorULongLongAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongLongAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorULongLongAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorULongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorIntAttribute file: line:1002
struct PyCallBack_HepMC3_VectorIntAttribute : public HepMC3::VectorIntAttribute {
	using HepMC3::VectorIntAttribute::VectorIntAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorIntAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorIntAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorIntAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorIntAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorIntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorLongIntAttribute file: line:1047
struct PyCallBack_HepMC3_VectorLongIntAttribute : public HepMC3::VectorLongIntAttribute {
	using HepMC3::VectorLongIntAttribute::VectorLongIntAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongIntAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongIntAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongIntAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorLongIntAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorLongIntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorDoubleAttribute file: line:1092
struct PyCallBack_HepMC3_VectorDoubleAttribute : public HepMC3::VectorDoubleAttribute {
	using HepMC3::VectorDoubleAttribute::VectorDoubleAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorDoubleAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorDoubleAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorDoubleAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorDoubleAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorDoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

// HepMC3::VectorStringAttribute file: line:1138
struct PyCallBack_HepMC3_VectorStringAttribute : public HepMC3::VectorStringAttribute {
	using HepMC3::VectorStringAttribute::VectorStringAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorStringAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorStringAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorStringAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return VectorStringAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::VectorStringAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init();
	}
};

void bind_unknown_unknown_2(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::VectorULongAttribute file: line:911
		pybind11::class_<HepMC3::VectorULongAttribute, std::shared_ptr<HepMC3::VectorULongAttribute>, PyCallBack_HepMC3_VectorULongAttribute> cl(M("HepMC3"), "VectorULongAttribute", "Attribute that holds a vector of unsigned longegers of type  unsigned long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorULongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorULongAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorULongAttribute const &o){ return new PyCallBack_HepMC3_VectorULongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorULongAttribute const &o){ return new HepMC3::VectorULongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorULongAttribute::*)(const std::string &)) &HepMC3::VectorULongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorULongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorULongAttribute::*)(std::string &) const) &HepMC3::VectorULongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorULongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorULongAttribute & (HepMC3::VectorULongAttribute::*)(const class HepMC3::VectorULongAttribute &)) &HepMC3::VectorULongAttribute::operator=, "C++: HepMC3::VectorULongAttribute::operator=(const class HepMC3::VectorULongAttribute &) --> class HepMC3::VectorULongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorULongLongAttribute file: line:957
		pybind11::class_<HepMC3::VectorULongLongAttribute, std::shared_ptr<HepMC3::VectorULongLongAttribute>, PyCallBack_HepMC3_VectorULongLongAttribute> cl(M("HepMC3"), "VectorULongLongAttribute", "Attribute that holds a vector of unsigned long longegers of type  unsigned long long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorULongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorULongLongAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorULongLongAttribute const &o){ return new PyCallBack_HepMC3_VectorULongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorULongLongAttribute const &o){ return new HepMC3::VectorULongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorULongLongAttribute::*)(const std::string &)) &HepMC3::VectorULongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorULongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorULongLongAttribute::*)(std::string &) const) &HepMC3::VectorULongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorULongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorULongLongAttribute & (HepMC3::VectorULongLongAttribute::*)(const class HepMC3::VectorULongLongAttribute &)) &HepMC3::VectorULongLongAttribute::operator=, "C++: HepMC3::VectorULongLongAttribute::operator=(const class HepMC3::VectorULongLongAttribute &) --> class HepMC3::VectorULongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorIntAttribute file: line:1002
		pybind11::class_<HepMC3::VectorIntAttribute, std::shared_ptr<HepMC3::VectorIntAttribute>, PyCallBack_HepMC3_VectorIntAttribute> cl(M("HepMC3"), "VectorIntAttribute", "Attribute that holds a vector of integers of type  int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorIntAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorIntAttribute const &o){ return new PyCallBack_HepMC3_VectorIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorIntAttribute const &o){ return new HepMC3::VectorIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorIntAttribute::*)(const std::string &)) &HepMC3::VectorIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorIntAttribute::*)(std::string &) const) &HepMC3::VectorIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorIntAttribute & (HepMC3::VectorIntAttribute::*)(const class HepMC3::VectorIntAttribute &)) &HepMC3::VectorIntAttribute::operator=, "C++: HepMC3::VectorIntAttribute::operator=(const class HepMC3::VectorIntAttribute &) --> class HepMC3::VectorIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorLongIntAttribute file: line:1047
		pybind11::class_<HepMC3::VectorLongIntAttribute, std::shared_ptr<HepMC3::VectorLongIntAttribute>, PyCallBack_HepMC3_VectorLongIntAttribute> cl(M("HepMC3"), "VectorLongIntAttribute", "Attribute that holds a vector of integers of type  int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongIntAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongIntAttribute const &o){ return new PyCallBack_HepMC3_VectorLongIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongIntAttribute const &o){ return new HepMC3::VectorLongIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongIntAttribute::*)(const std::string &)) &HepMC3::VectorLongIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongIntAttribute::*)(std::string &) const) &HepMC3::VectorLongIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorLongIntAttribute & (HepMC3::VectorLongIntAttribute::*)(const class HepMC3::VectorLongIntAttribute &)) &HepMC3::VectorLongIntAttribute::operator=, "C++: HepMC3::VectorLongIntAttribute::operator=(const class HepMC3::VectorLongIntAttribute &) --> class HepMC3::VectorLongIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorDoubleAttribute file: line:1092
		pybind11::class_<HepMC3::VectorDoubleAttribute, std::shared_ptr<HepMC3::VectorDoubleAttribute>, PyCallBack_HepMC3_VectorDoubleAttribute> cl(M("HepMC3"), "VectorDoubleAttribute", "Attribute that holds a vector of FPs of type  double\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorDoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorDoubleAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorDoubleAttribute const &o){ return new PyCallBack_HepMC3_VectorDoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorDoubleAttribute const &o){ return new HepMC3::VectorDoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorDoubleAttribute::*)(const std::string &)) &HepMC3::VectorDoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorDoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorDoubleAttribute::*)(std::string &) const) &HepMC3::VectorDoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorDoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorDoubleAttribute & (HepMC3::VectorDoubleAttribute::*)(const class HepMC3::VectorDoubleAttribute &)) &HepMC3::VectorDoubleAttribute::operator=, "C++: HepMC3::VectorDoubleAttribute::operator=(const class HepMC3::VectorDoubleAttribute &) --> class HepMC3::VectorDoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorStringAttribute file: line:1138
		pybind11::class_<HepMC3::VectorStringAttribute, std::shared_ptr<HepMC3::VectorStringAttribute>, PyCallBack_HepMC3_VectorStringAttribute> cl(M("HepMC3"), "VectorStringAttribute", "Attribute that holds a vector of FPs of type  string\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorStringAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorStringAttribute(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorStringAttribute const &o){ return new PyCallBack_HepMC3_VectorStringAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorStringAttribute const &o){ return new HepMC3::VectorStringAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorStringAttribute::*)(const std::string &)) &HepMC3::VectorStringAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorStringAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorStringAttribute::*)(std::string &) const) &HepMC3::VectorStringAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorStringAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("assign", (class HepMC3::VectorStringAttribute & (HepMC3::VectorStringAttribute::*)(const class HepMC3::VectorStringAttribute &)) &HepMC3::VectorStringAttribute::operator=, "C++: HepMC3::VectorStringAttribute::operator=(const class HepMC3::VectorStringAttribute &) --> class HepMC3::VectorStringAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
}


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
	// HepMC3::abs(const class HepMC3::Selector &) file:HepMC3/Selector.h line:161
	M("HepMC3").def("abs", (class std::shared_ptr<const class HepMC3::Selector> (*)(const class HepMC3::Selector &)) &HepMC3::abs, "ConstSelectorPtr abs\n\nC++: HepMC3::abs(const class HepMC3::Selector &) --> class std::shared_ptr<const class HepMC3::Selector>", pybind11::arg("input"));

}


#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include <pybind11/pybind11.h>

typedef std::function< pybind11::module & (std::string const &) > ModuleGetter;

void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_2(std::function< pybind11::module &(std::string const &namespace_) > &M);
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

	bind_unknown_unknown(M);
	bind_unknown_unknown_1(M);
	bind_unknown_unknown_2(M);
	bind_HepMC3_AttributeFeature(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.sources
// pyHepMC3search.cpp
// unknown/unknown.cpp
// unknown/unknown_1.cpp
// unknown/unknown_2.cpp
// HepMC3/AttributeFeature.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.modules
// HepMC3 
