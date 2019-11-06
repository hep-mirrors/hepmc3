// File: std/stl_vector.cpp
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

void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<long>, std::shared_ptr<std::vector<long>>> cl(M("std"), "vector_long_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<long>(); } ) );
		cl.def( pybind11::init<const class std::allocator<long> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<long>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<long> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const long & a1){ return new std::vector<long>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const long &, const class std::allocator<long> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<long> const &o){ return new std::vector<long>(o); } ) );
		cl.def( pybind11::init<const class std::vector<long, class std::allocator<long> > &, const class std::allocator<long> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<long, class std::allocator<long> > & (std::vector<long>::*)(const class std::vector<long, class std::allocator<long> > &)) &std::vector<long, std::allocator<long> >::operator=, "C++: std::vector<long, std::allocator<long> >::operator=(const class std::vector<long, class std::allocator<long> > &) --> class std::vector<long, class std::allocator<long> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<long>::*)(unsigned long, const long &)) &std::vector<long, std::allocator<long> >::assign, "C++: std::vector<long, std::allocator<long> >::assign(unsigned long, const long &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::begin, "C++: std::vector<long, std::allocator<long> >::begin() --> class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::end, "C++: std::vector<long, std::allocator<long> >::end() --> class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::cbegin, "C++: std::vector<long, std::allocator<long> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const long *, class std::vector<long, class std::allocator<long> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::cend, "C++: std::vector<long, std::allocator<long> >::cend() const --> class __gnu_cxx::__normal_iterator<const long *, class std::vector<long, class std::allocator<long> > >");
		cl.def("size", (unsigned long (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::size, "C++: std::vector<long, std::allocator<long> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::max_size, "C++: std::vector<long, std::allocator<long> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<long>::*)(unsigned long)) &std::vector<long, std::allocator<long> >::resize, "C++: std::vector<long, std::allocator<long> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<long>::*)(unsigned long, const long &)) &std::vector<long, std::allocator<long> >::resize, "C++: std::vector<long, std::allocator<long> >::resize(unsigned long, const long &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::shrink_to_fit, "C++: std::vector<long, std::allocator<long> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::capacity, "C++: std::vector<long, std::allocator<long> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<long>::*)() const) &std::vector<long, std::allocator<long> >::empty, "C++: std::vector<long, std::allocator<long> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<long>::*)(unsigned long)) &std::vector<long, std::allocator<long> >::reserve, "C++: std::vector<long, std::allocator<long> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (long & (std::vector<long>::*)(unsigned long)) &std::vector<long, std::allocator<long> >::operator[], "C++: std::vector<long, std::allocator<long> >::operator[](unsigned long) --> long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (long & (std::vector<long>::*)(unsigned long)) &std::vector<long, std::allocator<long> >::at, "C++: std::vector<long, std::allocator<long> >::at(unsigned long) --> long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (long & (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::front, "C++: std::vector<long, std::allocator<long> >::front() --> long &", pybind11::return_value_policy::automatic);
		cl.def("back", (long & (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::back, "C++: std::vector<long, std::allocator<long> >::back() --> long &", pybind11::return_value_policy::automatic);
		cl.def("data", (long * (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::data, "C++: std::vector<long, std::allocator<long> >::data() --> long *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<long>::*)(const long &)) &std::vector<long, std::allocator<long> >::push_back, "C++: std::vector<long, std::allocator<long> >::push_back(const long &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::pop_back, "C++: std::vector<long, std::allocator<long> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, const long &)) &std::vector<long, std::allocator<long> >::insert, "C++: std::vector<long, std::allocator<long> >::insert(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, const long &) --> class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<long>::*)(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, unsigned long, const long &)) &std::vector<long, std::allocator<long> >::insert, "C++: std::vector<long, std::allocator<long> >::insert(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, unsigned long, const long &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >)) &std::vector<long, std::allocator<long> >::erase, "C++: std::vector<long, std::allocator<long> >::erase(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >) --> class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > > (std::vector<long>::*)(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >)) &std::vector<long, std::allocator<long> >::erase, "C++: std::vector<long, std::allocator<long> >::erase(class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >, class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >) --> class __gnu_cxx::__normal_iterator<long *, class std::vector<long, class std::allocator<long> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<long>::*)(class std::vector<long, class std::allocator<long> > &)) &std::vector<long, std::allocator<long> >::swap, "C++: std::vector<long, std::allocator<long> >::swap(class std::vector<long, class std::allocator<long> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<long>::*)()) &std::vector<long, std::allocator<long> >::clear, "C++: std::vector<long, std::allocator<long> >::clear() --> void");
	}
}


// File: unknown/unknown.cpp
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

void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M)
{

	 binder::search_binder(M("HepMC3"));
	{ // HepMC3::VectorLongIntAttribute file: line:1047
		pybind11::class_<HepMC3::VectorLongIntAttribute, std::shared_ptr<HepMC3::VectorLongIntAttribute>, PyCallBack_HepMC3_VectorLongIntAttribute> cl(M("HepMC3"), "VectorLongIntAttribute", "Attribute that holds a vector of integers of type  int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongIntAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<long, class std::allocator<long> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongIntAttribute const &o){ return new PyCallBack_HepMC3_VectorLongIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongIntAttribute const &o){ return new HepMC3::VectorLongIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongIntAttribute::*)(const std::string &)) &HepMC3::VectorLongIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongIntAttribute::*)(std::string &) const) &HepMC3::VectorLongIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<long, class std::allocator<long> > (HepMC3::VectorLongIntAttribute::*)() const) &HepMC3::VectorLongIntAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorLongIntAttribute::value() const --> class std::vector<long, class std::allocator<long> >");
		cl.def("set_value", (void (HepMC3::VectorLongIntAttribute::*)(const class std::vector<long, class std::allocator<long> > &)) &HepMC3::VectorLongIntAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorLongIntAttribute::set_value(const class std::vector<long, class std::allocator<long> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorLongIntAttribute & (HepMC3::VectorLongIntAttribute::*)(const class HepMC3::VectorLongIntAttribute &)) &HepMC3::VectorLongIntAttribute::operator=, "C++: HepMC3::VectorLongIntAttribute::operator=(const class HepMC3::VectorLongIntAttribute &) --> class HepMC3::VectorLongIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
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

void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M);
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
		{"", "std"},
	};
	for(auto &p : sub_modules ) modules[p.first.size() ? p.first+"::"+p.second : p.second] = modules[p.first].def_submodule(p.second.c_str(), ("Bindings for " + p.first + "::" + p.second + " namespace").c_str() );

	//pybind11::class_<std::shared_ptr<void>>(M(""), "_encapsulated_data_");

	bind_std_stl_vector(M);
	bind_unknown_unknown(M);
	bind_HepMC3_AttributeFeature(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.sources
// pyHepMC3search.cpp
// std/stl_vector.cpp
// unknown/unknown.cpp
// HepMC3/AttributeFeature.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.modules
// HepMC3 std 
