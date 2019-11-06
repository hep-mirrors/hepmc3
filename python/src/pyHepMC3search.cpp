// File: std/stl_vector.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
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
		pybind11::class_<std::vector<char>, std::shared_ptr<std::vector<char>>> cl(M("std"), "vector_char_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<char>(); } ) );
		cl.def( pybind11::init<const class std::allocator<char> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<char>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<char> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const char & a1){ return new std::vector<char>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const char &, const class std::allocator<char> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<char> const &o){ return new std::vector<char>(o); } ) );
		cl.def( pybind11::init<const class std::vector<char, class std::allocator<char> > &, const class std::allocator<char> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<char, class std::allocator<char> > & (std::vector<char>::*)(const class std::vector<char, class std::allocator<char> > &)) &std::vector<char, std::allocator<char> >::operator=, "C++: std::vector<char, std::allocator<char> >::operator=(const class std::vector<char, class std::allocator<char> > &) --> class std::vector<char, class std::allocator<char> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<char>::*)(unsigned long, const char &)) &std::vector<char, std::allocator<char> >::assign, "C++: std::vector<char, std::allocator<char> >::assign(unsigned long, const char &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::begin, "C++: std::vector<char, std::allocator<char> >::begin() --> class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::end, "C++: std::vector<char, std::allocator<char> >::end() --> class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::cbegin, "C++: std::vector<char, std::allocator<char> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const char *, class std::vector<char, class std::allocator<char> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::cend, "C++: std::vector<char, std::allocator<char> >::cend() const --> class __gnu_cxx::__normal_iterator<const char *, class std::vector<char, class std::allocator<char> > >");
		cl.def("size", (unsigned long (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::size, "C++: std::vector<char, std::allocator<char> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::max_size, "C++: std::vector<char, std::allocator<char> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<char>::*)(unsigned long)) &std::vector<char, std::allocator<char> >::resize, "C++: std::vector<char, std::allocator<char> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<char>::*)(unsigned long, const char &)) &std::vector<char, std::allocator<char> >::resize, "C++: std::vector<char, std::allocator<char> >::resize(unsigned long, const char &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::shrink_to_fit, "C++: std::vector<char, std::allocator<char> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::capacity, "C++: std::vector<char, std::allocator<char> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<char>::*)() const) &std::vector<char, std::allocator<char> >::empty, "C++: std::vector<char, std::allocator<char> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<char>::*)(unsigned long)) &std::vector<char, std::allocator<char> >::reserve, "C++: std::vector<char, std::allocator<char> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (char & (std::vector<char>::*)(unsigned long)) &std::vector<char, std::allocator<char> >::operator[], "C++: std::vector<char, std::allocator<char> >::operator[](unsigned long) --> char &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (char & (std::vector<char>::*)(unsigned long)) &std::vector<char, std::allocator<char> >::at, "C++: std::vector<char, std::allocator<char> >::at(unsigned long) --> char &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (char & (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::front, "C++: std::vector<char, std::allocator<char> >::front() --> char &", pybind11::return_value_policy::automatic);
		cl.def("back", (char & (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::back, "C++: std::vector<char, std::allocator<char> >::back() --> char &", pybind11::return_value_policy::automatic);
		cl.def("data", (char * (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::data, "C++: std::vector<char, std::allocator<char> >::data() --> char *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<char>::*)(const char &)) &std::vector<char, std::allocator<char> >::push_back, "C++: std::vector<char, std::allocator<char> >::push_back(const char &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::pop_back, "C++: std::vector<char, std::allocator<char> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, const char &)) &std::vector<char, std::allocator<char> >::insert, "C++: std::vector<char, std::allocator<char> >::insert(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, const char &) --> class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<char>::*)(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, unsigned long, const char &)) &std::vector<char, std::allocator<char> >::insert, "C++: std::vector<char, std::allocator<char> >::insert(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, unsigned long, const char &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >)) &std::vector<char, std::allocator<char> >::erase, "C++: std::vector<char, std::allocator<char> >::erase(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >) --> class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > > (std::vector<char>::*)(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >)) &std::vector<char, std::allocator<char> >::erase, "C++: std::vector<char, std::allocator<char> >::erase(class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >, class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >) --> class __gnu_cxx::__normal_iterator<char *, class std::vector<char, class std::allocator<char> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<char>::*)(class std::vector<char, class std::allocator<char> > &)) &std::vector<char, std::allocator<char> >::swap, "C++: std::vector<char, std::allocator<char> >::swap(class std::vector<char, class std::allocator<char> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<char>::*)()) &std::vector<char, std::allocator<char> >::clear, "C++: std::vector<char, std::allocator<char> >::clear() --> void");
	}
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<float>, std::shared_ptr<std::vector<float>>> cl(M("std"), "vector_float_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<float>(); } ) );
		cl.def( pybind11::init<const class std::allocator<float> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<float>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<float> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const float & a1){ return new std::vector<float>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const float &, const class std::allocator<float> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<float> const &o){ return new std::vector<float>(o); } ) );
		cl.def( pybind11::init<const class std::vector<float, class std::allocator<float> > &, const class std::allocator<float> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<float, class std::allocator<float> > & (std::vector<float>::*)(const class std::vector<float, class std::allocator<float> > &)) &std::vector<float, std::allocator<float> >::operator=, "C++: std::vector<float, std::allocator<float> >::operator=(const class std::vector<float, class std::allocator<float> > &) --> class std::vector<float, class std::allocator<float> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<float>::*)(unsigned long, const float &)) &std::vector<float, std::allocator<float> >::assign, "C++: std::vector<float, std::allocator<float> >::assign(unsigned long, const float &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::begin, "C++: std::vector<float, std::allocator<float> >::begin() --> class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::end, "C++: std::vector<float, std::allocator<float> >::end() --> class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::cbegin, "C++: std::vector<float, std::allocator<float> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const float *, class std::vector<float, class std::allocator<float> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::cend, "C++: std::vector<float, std::allocator<float> >::cend() const --> class __gnu_cxx::__normal_iterator<const float *, class std::vector<float, class std::allocator<float> > >");
		cl.def("size", (unsigned long (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::size, "C++: std::vector<float, std::allocator<float> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::max_size, "C++: std::vector<float, std::allocator<float> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<float>::*)(unsigned long)) &std::vector<float, std::allocator<float> >::resize, "C++: std::vector<float, std::allocator<float> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<float>::*)(unsigned long, const float &)) &std::vector<float, std::allocator<float> >::resize, "C++: std::vector<float, std::allocator<float> >::resize(unsigned long, const float &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::shrink_to_fit, "C++: std::vector<float, std::allocator<float> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::capacity, "C++: std::vector<float, std::allocator<float> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<float>::*)() const) &std::vector<float, std::allocator<float> >::empty, "C++: std::vector<float, std::allocator<float> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<float>::*)(unsigned long)) &std::vector<float, std::allocator<float> >::reserve, "C++: std::vector<float, std::allocator<float> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (float & (std::vector<float>::*)(unsigned long)) &std::vector<float, std::allocator<float> >::operator[], "C++: std::vector<float, std::allocator<float> >::operator[](unsigned long) --> float &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (float & (std::vector<float>::*)(unsigned long)) &std::vector<float, std::allocator<float> >::at, "C++: std::vector<float, std::allocator<float> >::at(unsigned long) --> float &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (float & (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::front, "C++: std::vector<float, std::allocator<float> >::front() --> float &", pybind11::return_value_policy::automatic);
		cl.def("back", (float & (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::back, "C++: std::vector<float, std::allocator<float> >::back() --> float &", pybind11::return_value_policy::automatic);
		cl.def("data", (float * (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::data, "C++: std::vector<float, std::allocator<float> >::data() --> float *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<float>::*)(const float &)) &std::vector<float, std::allocator<float> >::push_back, "C++: std::vector<float, std::allocator<float> >::push_back(const float &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::pop_back, "C++: std::vector<float, std::allocator<float> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, const float &)) &std::vector<float, std::allocator<float> >::insert, "C++: std::vector<float, std::allocator<float> >::insert(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, const float &) --> class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<float>::*)(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, unsigned long, const float &)) &std::vector<float, std::allocator<float> >::insert, "C++: std::vector<float, std::allocator<float> >::insert(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, unsigned long, const float &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >)) &std::vector<float, std::allocator<float> >::erase, "C++: std::vector<float, std::allocator<float> >::erase(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >) --> class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > > (std::vector<float>::*)(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >)) &std::vector<float, std::allocator<float> >::erase, "C++: std::vector<float, std::allocator<float> >::erase(class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >, class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >) --> class __gnu_cxx::__normal_iterator<float *, class std::vector<float, class std::allocator<float> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<float>::*)(class std::vector<float, class std::allocator<float> > &)) &std::vector<float, std::allocator<float> >::swap, "C++: std::vector<float, std::allocator<float> >::swap(class std::vector<float, class std::allocator<float> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<float>::*)()) &std::vector<float, std::allocator<float> >::clear, "C++: std::vector<float, std::allocator<float> >::clear() --> void");
	}
}


// File: std/stl_vector_1.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
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

void bind_std_stl_vector_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<long double>, std::shared_ptr<std::vector<long double>>> cl(M("std"), "vector_long_double_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<long double>(); } ) );
		cl.def( pybind11::init<const class std::allocator<long double> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<long double>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<long double> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const long double & a1){ return new std::vector<long double>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const long double &, const class std::allocator<long double> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<long double> const &o){ return new std::vector<long double>(o); } ) );
		cl.def( pybind11::init<const class std::vector<long double, class std::allocator<long double> > &, const class std::allocator<long double> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<long double, class std::allocator<long double> > & (std::vector<long double>::*)(const class std::vector<long double, class std::allocator<long double> > &)) &std::vector<long double, std::allocator<long double> >::operator=, "C++: std::vector<long double, std::allocator<long double> >::operator=(const class std::vector<long double, class std::allocator<long double> > &) --> class std::vector<long double, class std::allocator<long double> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<long double>::*)(unsigned long, const long double &)) &std::vector<long double, std::allocator<long double> >::assign, "C++: std::vector<long double, std::allocator<long double> >::assign(unsigned long, const long double &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::begin, "C++: std::vector<long double, std::allocator<long double> >::begin() --> class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::end, "C++: std::vector<long double, std::allocator<long double> >::end() --> class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::cbegin, "C++: std::vector<long double, std::allocator<long double> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const long double *, class std::vector<long double, class std::allocator<long double> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::cend, "C++: std::vector<long double, std::allocator<long double> >::cend() const --> class __gnu_cxx::__normal_iterator<const long double *, class std::vector<long double, class std::allocator<long double> > >");
		cl.def("size", (unsigned long (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::size, "C++: std::vector<long double, std::allocator<long double> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::max_size, "C++: std::vector<long double, std::allocator<long double> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<long double>::*)(unsigned long)) &std::vector<long double, std::allocator<long double> >::resize, "C++: std::vector<long double, std::allocator<long double> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<long double>::*)(unsigned long, const long double &)) &std::vector<long double, std::allocator<long double> >::resize, "C++: std::vector<long double, std::allocator<long double> >::resize(unsigned long, const long double &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::shrink_to_fit, "C++: std::vector<long double, std::allocator<long double> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::capacity, "C++: std::vector<long double, std::allocator<long double> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<long double>::*)() const) &std::vector<long double, std::allocator<long double> >::empty, "C++: std::vector<long double, std::allocator<long double> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<long double>::*)(unsigned long)) &std::vector<long double, std::allocator<long double> >::reserve, "C++: std::vector<long double, std::allocator<long double> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (long double & (std::vector<long double>::*)(unsigned long)) &std::vector<long double, std::allocator<long double> >::operator[], "C++: std::vector<long double, std::allocator<long double> >::operator[](unsigned long) --> long double &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (long double & (std::vector<long double>::*)(unsigned long)) &std::vector<long double, std::allocator<long double> >::at, "C++: std::vector<long double, std::allocator<long double> >::at(unsigned long) --> long double &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (long double & (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::front, "C++: std::vector<long double, std::allocator<long double> >::front() --> long double &", pybind11::return_value_policy::automatic);
		cl.def("back", (long double & (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::back, "C++: std::vector<long double, std::allocator<long double> >::back() --> long double &", pybind11::return_value_policy::automatic);
		cl.def("data", (long double * (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::data, "C++: std::vector<long double, std::allocator<long double> >::data() --> long double *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<long double>::*)(const long double &)) &std::vector<long double, std::allocator<long double> >::push_back, "C++: std::vector<long double, std::allocator<long double> >::push_back(const long double &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::pop_back, "C++: std::vector<long double, std::allocator<long double> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, const long double &)) &std::vector<long double, std::allocator<long double> >::insert, "C++: std::vector<long double, std::allocator<long double> >::insert(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, const long double &) --> class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<long double>::*)(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, unsigned long, const long double &)) &std::vector<long double, std::allocator<long double> >::insert, "C++: std::vector<long double, std::allocator<long double> >::insert(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, unsigned long, const long double &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >)) &std::vector<long double, std::allocator<long double> >::erase, "C++: std::vector<long double, std::allocator<long double> >::erase(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >) --> class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > > (std::vector<long double>::*)(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >)) &std::vector<long double, std::allocator<long double> >::erase, "C++: std::vector<long double, std::allocator<long double> >::erase(class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >, class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >) --> class __gnu_cxx::__normal_iterator<long double *, class std::vector<long double, class std::allocator<long double> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<long double>::*)(class std::vector<long double, class std::allocator<long double> > &)) &std::vector<long double, std::allocator<long double> >::swap, "C++: std::vector<long double, std::allocator<long double> >::swap(class std::vector<long double, class std::allocator<long double> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<long double>::*)()) &std::vector<long double, std::allocator<long double> >::clear, "C++: std::vector<long double, std::allocator<long double> >::clear() --> void");
	}
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<long long>, std::shared_ptr<std::vector<long long>>> cl(M("std"), "vector_long_long_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<long long>(); } ) );
		cl.def( pybind11::init<const class std::allocator<long long> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<long long>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<long long> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const long long & a1){ return new std::vector<long long>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const long long &, const class std::allocator<long long> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<long long> const &o){ return new std::vector<long long>(o); } ) );
		cl.def( pybind11::init<const class std::vector<long long, class std::allocator<long long> > &, const class std::allocator<long long> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<long long, class std::allocator<long long> > & (std::vector<long long>::*)(const class std::vector<long long, class std::allocator<long long> > &)) &std::vector<long long, std::allocator<long long> >::operator=, "C++: std::vector<long long, std::allocator<long long> >::operator=(const class std::vector<long long, class std::allocator<long long> > &) --> class std::vector<long long, class std::allocator<long long> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<long long>::*)(unsigned long, const long long &)) &std::vector<long long, std::allocator<long long> >::assign, "C++: std::vector<long long, std::allocator<long long> >::assign(unsigned long, const long long &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::begin, "C++: std::vector<long long, std::allocator<long long> >::begin() --> class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::end, "C++: std::vector<long long, std::allocator<long long> >::end() --> class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::cbegin, "C++: std::vector<long long, std::allocator<long long> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const long long *, class std::vector<long long, class std::allocator<long long> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::cend, "C++: std::vector<long long, std::allocator<long long> >::cend() const --> class __gnu_cxx::__normal_iterator<const long long *, class std::vector<long long, class std::allocator<long long> > >");
		cl.def("size", (unsigned long (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::size, "C++: std::vector<long long, std::allocator<long long> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::max_size, "C++: std::vector<long long, std::allocator<long long> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<long long>::*)(unsigned long)) &std::vector<long long, std::allocator<long long> >::resize, "C++: std::vector<long long, std::allocator<long long> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<long long>::*)(unsigned long, const long long &)) &std::vector<long long, std::allocator<long long> >::resize, "C++: std::vector<long long, std::allocator<long long> >::resize(unsigned long, const long long &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::shrink_to_fit, "C++: std::vector<long long, std::allocator<long long> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::capacity, "C++: std::vector<long long, std::allocator<long long> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<long long>::*)() const) &std::vector<long long, std::allocator<long long> >::empty, "C++: std::vector<long long, std::allocator<long long> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<long long>::*)(unsigned long)) &std::vector<long long, std::allocator<long long> >::reserve, "C++: std::vector<long long, std::allocator<long long> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (long long & (std::vector<long long>::*)(unsigned long)) &std::vector<long long, std::allocator<long long> >::operator[], "C++: std::vector<long long, std::allocator<long long> >::operator[](unsigned long) --> long long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (long long & (std::vector<long long>::*)(unsigned long)) &std::vector<long long, std::allocator<long long> >::at, "C++: std::vector<long long, std::allocator<long long> >::at(unsigned long) --> long long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (long long & (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::front, "C++: std::vector<long long, std::allocator<long long> >::front() --> long long &", pybind11::return_value_policy::automatic);
		cl.def("back", (long long & (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::back, "C++: std::vector<long long, std::allocator<long long> >::back() --> long long &", pybind11::return_value_policy::automatic);
		cl.def("data", (long long * (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::data, "C++: std::vector<long long, std::allocator<long long> >::data() --> long long *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<long long>::*)(const long long &)) &std::vector<long long, std::allocator<long long> >::push_back, "C++: std::vector<long long, std::allocator<long long> >::push_back(const long long &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::pop_back, "C++: std::vector<long long, std::allocator<long long> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, const long long &)) &std::vector<long long, std::allocator<long long> >::insert, "C++: std::vector<long long, std::allocator<long long> >::insert(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, const long long &) --> class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<long long>::*)(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, unsigned long, const long long &)) &std::vector<long long, std::allocator<long long> >::insert, "C++: std::vector<long long, std::allocator<long long> >::insert(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, unsigned long, const long long &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >)) &std::vector<long long, std::allocator<long long> >::erase, "C++: std::vector<long long, std::allocator<long long> >::erase(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >) --> class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > > (std::vector<long long>::*)(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >)) &std::vector<long long, std::allocator<long long> >::erase, "C++: std::vector<long long, std::allocator<long long> >::erase(class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >, class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >) --> class __gnu_cxx::__normal_iterator<long long *, class std::vector<long long, class std::allocator<long long> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<long long>::*)(class std::vector<long long, class std::allocator<long long> > &)) &std::vector<long long, std::allocator<long long> >::swap, "C++: std::vector<long long, std::allocator<long long> >::swap(class std::vector<long long, class std::allocator<long long> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<long long>::*)()) &std::vector<long long, std::allocator<long long> >::clear, "C++: std::vector<long long, std::allocator<long long> >::clear() --> void");
	}
}


// File: std/stl_vector_2.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
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

void bind_std_stl_vector_2(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<unsigned int>, std::shared_ptr<std::vector<unsigned int>>> cl(M("std"), "vector_unsigned_int_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<unsigned int>(); } ) );
		cl.def( pybind11::init<const class std::allocator<unsigned int> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<unsigned int>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<unsigned int> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const unsigned int & a1){ return new std::vector<unsigned int>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const unsigned int &, const class std::allocator<unsigned int> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<unsigned int> const &o){ return new std::vector<unsigned int>(o); } ) );
		cl.def( pybind11::init<const class std::vector<unsigned int, class std::allocator<unsigned int> > &, const class std::allocator<unsigned int> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<unsigned int, class std::allocator<unsigned int> > & (std::vector<unsigned int>::*)(const class std::vector<unsigned int, class std::allocator<unsigned int> > &)) &std::vector<unsigned int, std::allocator<unsigned int> >::operator=, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::operator=(const class std::vector<unsigned int, class std::allocator<unsigned int> > &) --> class std::vector<unsigned int, class std::allocator<unsigned int> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<unsigned int>::*)(unsigned long, const unsigned int &)) &std::vector<unsigned int, std::allocator<unsigned int> >::assign, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::assign(unsigned long, const unsigned int &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::begin, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::begin() --> class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::end, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::end() --> class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::cbegin, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::cend, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::cend() const --> class __gnu_cxx::__normal_iterator<const unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >");
		cl.def("size", (unsigned long (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::size, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::max_size, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<unsigned int>::*)(unsigned long)) &std::vector<unsigned int, std::allocator<unsigned int> >::resize, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<unsigned int>::*)(unsigned long, const unsigned int &)) &std::vector<unsigned int, std::allocator<unsigned int> >::resize, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::resize(unsigned long, const unsigned int &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::shrink_to_fit, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::capacity, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<unsigned int>::*)() const) &std::vector<unsigned int, std::allocator<unsigned int> >::empty, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<unsigned int>::*)(unsigned long)) &std::vector<unsigned int, std::allocator<unsigned int> >::reserve, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (unsigned int & (std::vector<unsigned int>::*)(unsigned long)) &std::vector<unsigned int, std::allocator<unsigned int> >::operator[], "C++: std::vector<unsigned int, std::allocator<unsigned int> >::operator[](unsigned long) --> unsigned int &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (unsigned int & (std::vector<unsigned int>::*)(unsigned long)) &std::vector<unsigned int, std::allocator<unsigned int> >::at, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::at(unsigned long) --> unsigned int &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (unsigned int & (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::front, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::front() --> unsigned int &", pybind11::return_value_policy::automatic);
		cl.def("back", (unsigned int & (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::back, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::back() --> unsigned int &", pybind11::return_value_policy::automatic);
		cl.def("data", (unsigned int * (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::data, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::data() --> unsigned int *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<unsigned int>::*)(const unsigned int &)) &std::vector<unsigned int, std::allocator<unsigned int> >::push_back, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::push_back(const unsigned int &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::pop_back, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, const unsigned int &)) &std::vector<unsigned int, std::allocator<unsigned int> >::insert, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::insert(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, const unsigned int &) --> class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<unsigned int>::*)(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, unsigned long, const unsigned int &)) &std::vector<unsigned int, std::allocator<unsigned int> >::insert, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::insert(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, unsigned long, const unsigned int &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >)) &std::vector<unsigned int, std::allocator<unsigned int> >::erase, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::erase(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >) --> class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > > (std::vector<unsigned int>::*)(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >)) &std::vector<unsigned int, std::allocator<unsigned int> >::erase, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::erase(class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >, class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >) --> class __gnu_cxx::__normal_iterator<unsigned int *, class std::vector<unsigned int, class std::allocator<unsigned int> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<unsigned int>::*)(class std::vector<unsigned int, class std::allocator<unsigned int> > &)) &std::vector<unsigned int, std::allocator<unsigned int> >::swap, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::swap(class std::vector<unsigned int, class std::allocator<unsigned int> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<unsigned int>::*)()) &std::vector<unsigned int, std::allocator<unsigned int> >::clear, "C++: std::vector<unsigned int, std::allocator<unsigned int> >::clear() --> void");
	}
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<unsigned long>, std::shared_ptr<std::vector<unsigned long>>> cl(M("std"), "vector_unsigned_long_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<unsigned long>(); } ) );
		cl.def( pybind11::init<const class std::allocator<unsigned long> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<unsigned long>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<unsigned long> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const unsigned long & a1){ return new std::vector<unsigned long>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const unsigned long &, const class std::allocator<unsigned long> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<unsigned long> const &o){ return new std::vector<unsigned long>(o); } ) );
		cl.def( pybind11::init<const class std::vector<unsigned long, class std::allocator<unsigned long> > &, const class std::allocator<unsigned long> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<unsigned long, class std::allocator<unsigned long> > & (std::vector<unsigned long>::*)(const class std::vector<unsigned long, class std::allocator<unsigned long> > &)) &std::vector<unsigned long, std::allocator<unsigned long> >::operator=, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::operator=(const class std::vector<unsigned long, class std::allocator<unsigned long> > &) --> class std::vector<unsigned long, class std::allocator<unsigned long> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<unsigned long>::*)(unsigned long, const unsigned long &)) &std::vector<unsigned long, std::allocator<unsigned long> >::assign, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::assign(unsigned long, const unsigned long &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::begin, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::begin() --> class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::end, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::end() --> class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::cbegin, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::cend, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::cend() const --> class __gnu_cxx::__normal_iterator<const unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >");
		cl.def("size", (unsigned long (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::size, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::max_size, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<unsigned long>::*)(unsigned long)) &std::vector<unsigned long, std::allocator<unsigned long> >::resize, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<unsigned long>::*)(unsigned long, const unsigned long &)) &std::vector<unsigned long, std::allocator<unsigned long> >::resize, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::resize(unsigned long, const unsigned long &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::shrink_to_fit, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::capacity, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<unsigned long>::*)() const) &std::vector<unsigned long, std::allocator<unsigned long> >::empty, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<unsigned long>::*)(unsigned long)) &std::vector<unsigned long, std::allocator<unsigned long> >::reserve, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (unsigned long & (std::vector<unsigned long>::*)(unsigned long)) &std::vector<unsigned long, std::allocator<unsigned long> >::operator[], "C++: std::vector<unsigned long, std::allocator<unsigned long> >::operator[](unsigned long) --> unsigned long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (unsigned long & (std::vector<unsigned long>::*)(unsigned long)) &std::vector<unsigned long, std::allocator<unsigned long> >::at, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::at(unsigned long) --> unsigned long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (unsigned long & (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::front, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::front() --> unsigned long &", pybind11::return_value_policy::automatic);
		cl.def("back", (unsigned long & (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::back, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::back() --> unsigned long &", pybind11::return_value_policy::automatic);
		cl.def("data", (unsigned long * (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::data, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::data() --> unsigned long *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<unsigned long>::*)(const unsigned long &)) &std::vector<unsigned long, std::allocator<unsigned long> >::push_back, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::push_back(const unsigned long &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::pop_back, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, const unsigned long &)) &std::vector<unsigned long, std::allocator<unsigned long> >::insert, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::insert(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, const unsigned long &) --> class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<unsigned long>::*)(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, unsigned long, const unsigned long &)) &std::vector<unsigned long, std::allocator<unsigned long> >::insert, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::insert(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, unsigned long, const unsigned long &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >)) &std::vector<unsigned long, std::allocator<unsigned long> >::erase, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::erase(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >) --> class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > > (std::vector<unsigned long>::*)(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >)) &std::vector<unsigned long, std::allocator<unsigned long> >::erase, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::erase(class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >, class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >) --> class __gnu_cxx::__normal_iterator<unsigned long *, class std::vector<unsigned long, class std::allocator<unsigned long> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<unsigned long>::*)(class std::vector<unsigned long, class std::allocator<unsigned long> > &)) &std::vector<unsigned long, std::allocator<unsigned long> >::swap, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::swap(class std::vector<unsigned long, class std::allocator<unsigned long> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<unsigned long>::*)()) &std::vector<unsigned long, std::allocator<unsigned long> >::clear, "C++: std::vector<unsigned long, std::allocator<unsigned long> >::clear() --> void");
	}
}


// File: std/stl_vector_3.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
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

void bind_std_stl_vector_3(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<unsigned long long>, std::shared_ptr<std::vector<unsigned long long>>> cl(M("std"), "vector_unsigned_long_long_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<unsigned long long>(); } ) );
		cl.def( pybind11::init<const class std::allocator<unsigned long long> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<unsigned long long>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<unsigned long long> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const unsigned long long & a1){ return new std::vector<unsigned long long>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const unsigned long long &, const class std::allocator<unsigned long long> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<unsigned long long> const &o){ return new std::vector<unsigned long long>(o); } ) );
		cl.def( pybind11::init<const class std::vector<unsigned long long, class std::allocator<unsigned long long> > &, const class std::allocator<unsigned long long> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<unsigned long long, class std::allocator<unsigned long long> > & (std::vector<unsigned long long>::*)(const class std::vector<unsigned long long, class std::allocator<unsigned long long> > &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::operator=, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::operator=(const class std::vector<unsigned long long, class std::allocator<unsigned long long> > &) --> class std::vector<unsigned long long, class std::allocator<unsigned long long> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<unsigned long long>::*)(unsigned long, const unsigned long long &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::assign, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::assign(unsigned long, const unsigned long long &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::begin, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::begin() --> class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::end, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::end() --> class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::cbegin, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::cend, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::cend() const --> class __gnu_cxx::__normal_iterator<const unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >");
		cl.def("size", (unsigned long (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::size, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::max_size, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<unsigned long long>::*)(unsigned long)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::resize, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<unsigned long long>::*)(unsigned long, const unsigned long long &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::resize, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::resize(unsigned long, const unsigned long long &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::shrink_to_fit, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::capacity, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<unsigned long long>::*)() const) &std::vector<unsigned long long, std::allocator<unsigned long long> >::empty, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<unsigned long long>::*)(unsigned long)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::reserve, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (unsigned long long & (std::vector<unsigned long long>::*)(unsigned long)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::operator[], "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::operator[](unsigned long) --> unsigned long long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (unsigned long long & (std::vector<unsigned long long>::*)(unsigned long)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::at, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::at(unsigned long) --> unsigned long long &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (unsigned long long & (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::front, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::front() --> unsigned long long &", pybind11::return_value_policy::automatic);
		cl.def("back", (unsigned long long & (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::back, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::back() --> unsigned long long &", pybind11::return_value_policy::automatic);
		cl.def("data", (unsigned long long * (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::data, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::data() --> unsigned long long *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<unsigned long long>::*)(const unsigned long long &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::push_back, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::push_back(const unsigned long long &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::pop_back, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, const unsigned long long &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::insert, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::insert(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, const unsigned long long &) --> class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<unsigned long long>::*)(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, unsigned long, const unsigned long long &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::insert, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::insert(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, unsigned long, const unsigned long long &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::erase, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::erase(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >) --> class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > > (std::vector<unsigned long long>::*)(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::erase, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::erase(class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >, class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >) --> class __gnu_cxx::__normal_iterator<unsigned long long *, class std::vector<unsigned long long, class std::allocator<unsigned long long> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<unsigned long long>::*)(class std::vector<unsigned long long, class std::allocator<unsigned long long> > &)) &std::vector<unsigned long long, std::allocator<unsigned long long> >::swap, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::swap(class std::vector<unsigned long long, class std::allocator<unsigned long long> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<unsigned long long>::*)()) &std::vector<unsigned long long, std::allocator<unsigned long long> >::clear, "C++: std::vector<unsigned long long, std::allocator<unsigned long long> >::clear() --> void");
	}
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<int>, std::shared_ptr<std::vector<int>>> cl(M("std"), "vector_int_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<int>(); } ) );
		cl.def( pybind11::init<const class std::allocator<int> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<int>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<int> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const int & a1){ return new std::vector<int>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const int &, const class std::allocator<int> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<int> const &o){ return new std::vector<int>(o); } ) );
		cl.def( pybind11::init<const class std::vector<int, class std::allocator<int> > &, const class std::allocator<int> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<int, class std::allocator<int> > & (std::vector<int>::*)(const class std::vector<int, class std::allocator<int> > &)) &std::vector<int, std::allocator<int> >::operator=, "C++: std::vector<int, std::allocator<int> >::operator=(const class std::vector<int, class std::allocator<int> > &) --> class std::vector<int, class std::allocator<int> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<int>::*)(unsigned long, const int &)) &std::vector<int, std::allocator<int> >::assign, "C++: std::vector<int, std::allocator<int> >::assign(unsigned long, const int &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::begin, "C++: std::vector<int, std::allocator<int> >::begin() --> class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::end, "C++: std::vector<int, std::allocator<int> >::end() --> class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::cbegin, "C++: std::vector<int, std::allocator<int> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const int *, class std::vector<int, class std::allocator<int> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::cend, "C++: std::vector<int, std::allocator<int> >::cend() const --> class __gnu_cxx::__normal_iterator<const int *, class std::vector<int, class std::allocator<int> > >");
		cl.def("size", (unsigned long (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::size, "C++: std::vector<int, std::allocator<int> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::max_size, "C++: std::vector<int, std::allocator<int> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<int>::*)(unsigned long)) &std::vector<int, std::allocator<int> >::resize, "C++: std::vector<int, std::allocator<int> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<int>::*)(unsigned long, const int &)) &std::vector<int, std::allocator<int> >::resize, "C++: std::vector<int, std::allocator<int> >::resize(unsigned long, const int &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::shrink_to_fit, "C++: std::vector<int, std::allocator<int> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::capacity, "C++: std::vector<int, std::allocator<int> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<int>::*)() const) &std::vector<int, std::allocator<int> >::empty, "C++: std::vector<int, std::allocator<int> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<int>::*)(unsigned long)) &std::vector<int, std::allocator<int> >::reserve, "C++: std::vector<int, std::allocator<int> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (int & (std::vector<int>::*)(unsigned long)) &std::vector<int, std::allocator<int> >::operator[], "C++: std::vector<int, std::allocator<int> >::operator[](unsigned long) --> int &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (int & (std::vector<int>::*)(unsigned long)) &std::vector<int, std::allocator<int> >::at, "C++: std::vector<int, std::allocator<int> >::at(unsigned long) --> int &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (int & (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::front, "C++: std::vector<int, std::allocator<int> >::front() --> int &", pybind11::return_value_policy::automatic);
		cl.def("back", (int & (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::back, "C++: std::vector<int, std::allocator<int> >::back() --> int &", pybind11::return_value_policy::automatic);
		cl.def("data", (int * (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::data, "C++: std::vector<int, std::allocator<int> >::data() --> int *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<int>::*)(const int &)) &std::vector<int, std::allocator<int> >::push_back, "C++: std::vector<int, std::allocator<int> >::push_back(const int &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::pop_back, "C++: std::vector<int, std::allocator<int> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, const int &)) &std::vector<int, std::allocator<int> >::insert, "C++: std::vector<int, std::allocator<int> >::insert(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, const int &) --> class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<int>::*)(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, unsigned long, const int &)) &std::vector<int, std::allocator<int> >::insert, "C++: std::vector<int, std::allocator<int> >::insert(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, unsigned long, const int &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >)) &std::vector<int, std::allocator<int> >::erase, "C++: std::vector<int, std::allocator<int> >::erase(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >) --> class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > > (std::vector<int>::*)(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >)) &std::vector<int, std::allocator<int> >::erase, "C++: std::vector<int, std::allocator<int> >::erase(class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >, class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >) --> class __gnu_cxx::__normal_iterator<int *, class std::vector<int, class std::allocator<int> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<int>::*)(class std::vector<int, class std::allocator<int> > &)) &std::vector<int, std::allocator<int> >::swap, "C++: std::vector<int, std::allocator<int> >::swap(class std::vector<int, class std::allocator<int> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<int>::*)()) &std::vector<int, std::allocator<int> >::clear, "C++: std::vector<int, std::allocator<int> >::clear() --> void");
	}
}


// File: std/stl_vector_4.cpp
#include <iterator>
#include <memory>
#include <sstream> // __str__
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

void bind_std_stl_vector_4(std::function< pybind11::module &(std::string const &namespace_) > &M)
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
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<double>, std::shared_ptr<std::vector<double>>> cl(M("std"), "vector_double_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<double>(); } ) );
		cl.def( pybind11::init<const class std::allocator<double> &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<double>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<double> &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const double & a1){ return new std::vector<double>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const double &, const class std::allocator<double> &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<double> const &o){ return new std::vector<double>(o); } ) );
		cl.def( pybind11::init<const class std::vector<double, class std::allocator<double> > &, const class std::allocator<double> &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<double, class std::allocator<double> > & (std::vector<double>::*)(const class std::vector<double, class std::allocator<double> > &)) &std::vector<double, std::allocator<double> >::operator=, "C++: std::vector<double, std::allocator<double> >::operator=(const class std::vector<double, class std::allocator<double> > &) --> class std::vector<double, class std::allocator<double> > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<double>::*)(unsigned long, const double &)) &std::vector<double, std::allocator<double> >::assign, "C++: std::vector<double, std::allocator<double> >::assign(unsigned long, const double &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::begin, "C++: std::vector<double, std::allocator<double> >::begin() --> class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::end, "C++: std::vector<double, std::allocator<double> >::end() --> class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::cbegin, "C++: std::vector<double, std::allocator<double> >::cbegin() const --> class __gnu_cxx::__normal_iterator<const double *, class std::vector<double, class std::allocator<double> > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::cend, "C++: std::vector<double, std::allocator<double> >::cend() const --> class __gnu_cxx::__normal_iterator<const double *, class std::vector<double, class std::allocator<double> > >");
		cl.def("size", (unsigned long (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::size, "C++: std::vector<double, std::allocator<double> >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::max_size, "C++: std::vector<double, std::allocator<double> >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<double>::*)(unsigned long)) &std::vector<double, std::allocator<double> >::resize, "C++: std::vector<double, std::allocator<double> >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<double>::*)(unsigned long, const double &)) &std::vector<double, std::allocator<double> >::resize, "C++: std::vector<double, std::allocator<double> >::resize(unsigned long, const double &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::shrink_to_fit, "C++: std::vector<double, std::allocator<double> >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::capacity, "C++: std::vector<double, std::allocator<double> >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<double>::*)() const) &std::vector<double, std::allocator<double> >::empty, "C++: std::vector<double, std::allocator<double> >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<double>::*)(unsigned long)) &std::vector<double, std::allocator<double> >::reserve, "C++: std::vector<double, std::allocator<double> >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (double & (std::vector<double>::*)(unsigned long)) &std::vector<double, std::allocator<double> >::operator[], "C++: std::vector<double, std::allocator<double> >::operator[](unsigned long) --> double &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (double & (std::vector<double>::*)(unsigned long)) &std::vector<double, std::allocator<double> >::at, "C++: std::vector<double, std::allocator<double> >::at(unsigned long) --> double &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (double & (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::front, "C++: std::vector<double, std::allocator<double> >::front() --> double &", pybind11::return_value_policy::automatic);
		cl.def("back", (double & (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::back, "C++: std::vector<double, std::allocator<double> >::back() --> double &", pybind11::return_value_policy::automatic);
		cl.def("data", (double * (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::data, "C++: std::vector<double, std::allocator<double> >::data() --> double *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<double>::*)(const double &)) &std::vector<double, std::allocator<double> >::push_back, "C++: std::vector<double, std::allocator<double> >::push_back(const double &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::pop_back, "C++: std::vector<double, std::allocator<double> >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, const double &)) &std::vector<double, std::allocator<double> >::insert, "C++: std::vector<double, std::allocator<double> >::insert(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, const double &) --> class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<double>::*)(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, unsigned long, const double &)) &std::vector<double, std::allocator<double> >::insert, "C++: std::vector<double, std::allocator<double> >::insert(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, unsigned long, const double &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >)) &std::vector<double, std::allocator<double> >::erase, "C++: std::vector<double, std::allocator<double> >::erase(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >) --> class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > > (std::vector<double>::*)(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >)) &std::vector<double, std::allocator<double> >::erase, "C++: std::vector<double, std::allocator<double> >::erase(class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >, class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >) --> class __gnu_cxx::__normal_iterator<double *, class std::vector<double, class std::allocator<double> > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<double>::*)(class std::vector<double, class std::allocator<double> > &)) &std::vector<double, std::allocator<double> >::swap, "C++: std::vector<double, std::allocator<double> >::swap(class std::vector<double, class std::allocator<double> > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<double>::*)()) &std::vector<double, std::allocator<double> >::clear, "C++: std::vector<double, std::allocator<double> >::clear() --> void");
	}
}


// File: std/stl_vector_5.cpp
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

void bind_std_stl_vector_5(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // std::vector file:bits/stl_vector.h line:210
		pybind11::class_<std::vector<std::string,std::allocator<std::string >>, std::shared_ptr<std::vector<std::string,std::allocator<std::string >>>> cl(M("std"), "vector_std_string_std_allocator_std_string_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new std::vector<std::string,std::allocator<std::string >>(); } ) );
		cl.def( pybind11::init<const class std::allocator<std::string > &>(), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0){ return new std::vector<std::string,std::allocator<std::string >>(a0); } ), "doc" , pybind11::arg("__n"));
		cl.def( pybind11::init<unsigned long, const class std::allocator<std::string > &>(), pybind11::arg("__n"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](unsigned long const & a0, const class std::basic_string<char> & a1){ return new std::vector<std::string,std::allocator<std::string >>(a0, a1); } ), "doc" , pybind11::arg("__n"), pybind11::arg("__value"));
		cl.def( pybind11::init<unsigned long, const std::string &, const class std::allocator<std::string > &>(), pybind11::arg("__n"), pybind11::arg("__value"), pybind11::arg("__a") );

		cl.def( pybind11::init( [](std::vector<std::string,std::allocator<std::string >> const &o){ return new std::vector<std::string,std::allocator<std::string >>(o); } ) );
		cl.def( pybind11::init<const class std::vector<std::string, class std::allocator<std::string > > &, const class std::allocator<std::string > &>(), pybind11::arg("__x"), pybind11::arg("__a") );

		cl.def("assign", (class std::vector<std::string, class std::allocator<std::string > > & (std::vector<std::string,std::allocator<std::string >>::*)(const class std::vector<std::string, class std::allocator<std::string > > &)) &std::vector<std::string, std::allocator<std::string > >::operator=, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::operator=(const class std::vector<std::string, class std::allocator<std::string > > &) --> class std::vector<std::string, class std::allocator<std::string > > &", pybind11::return_value_policy::automatic, pybind11::arg("__x"));
		cl.def("assign", (void (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long, const std::string &)) &std::vector<std::string, std::allocator<std::string > >::assign, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::assign(unsigned long, const std::string &) --> void", pybind11::arg("__n"), pybind11::arg("__val"));
		cl.def("begin", (class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::begin, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::begin() --> class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >");
		cl.def("end", (class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::end, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::end() --> class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >");
		cl.def("cbegin", (class __gnu_cxx::__normal_iterator<const std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::cbegin, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::cbegin() const --> class __gnu_cxx::__normal_iterator<const std::string *, class std::vector<std::string, class std::allocator<std::string > > >");
		cl.def("cend", (class __gnu_cxx::__normal_iterator<const std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::cend, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::cend() const --> class __gnu_cxx::__normal_iterator<const std::string *, class std::vector<std::string, class std::allocator<std::string > > >");
		cl.def("size", (unsigned long (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::size, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::size() const --> unsigned long");
		cl.def("max_size", (unsigned long (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::max_size, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::max_size() const --> unsigned long");
		cl.def("resize", (void (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long)) &std::vector<std::string, std::allocator<std::string > >::resize, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::resize(unsigned long) --> void", pybind11::arg("__new_size"));
		cl.def("resize", (void (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long, const std::string &)) &std::vector<std::string, std::allocator<std::string > >::resize, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::resize(unsigned long, const std::string &) --> void", pybind11::arg("__new_size"), pybind11::arg("__x"));
		cl.def("shrink_to_fit", (void (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::shrink_to_fit, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::shrink_to_fit() --> void");
		cl.def("capacity", (unsigned long (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::capacity, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::capacity() const --> unsigned long");
		cl.def("empty", (bool (std::vector<std::string,std::allocator<std::string >>::*)() const) &std::vector<std::string, std::allocator<std::string > >::empty, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::empty() const --> bool");
		cl.def("reserve", (void (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long)) &std::vector<std::string, std::allocator<std::string > >::reserve, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::reserve(unsigned long) --> void", pybind11::arg("__n"));
		cl.def("__getitem__", (std::string & (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long)) &std::vector<std::string, std::allocator<std::string > >::operator[], "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::operator[](unsigned long) --> std::string &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("at", (std::string & (std::vector<std::string,std::allocator<std::string >>::*)(unsigned long)) &std::vector<std::string, std::allocator<std::string > >::at, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::at(unsigned long) --> std::string &", pybind11::return_value_policy::automatic, pybind11::arg("__n"));
		cl.def("front", (std::string & (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::front, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::front() --> std::string &", pybind11::return_value_policy::automatic);
		cl.def("back", (std::string & (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::back, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::back() --> std::string &", pybind11::return_value_policy::automatic);
		cl.def("data", (std::string * (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::data, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::data() --> std::string *", pybind11::return_value_policy::automatic);
		cl.def("push_back", (void (std::vector<std::string,std::allocator<std::string >>::*)(const std::string &)) &std::vector<std::string, std::allocator<std::string > >::push_back, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::push_back(const std::string &) --> void", pybind11::arg("__x"));
		cl.def("pop_back", (void (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::pop_back, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::pop_back() --> void");
		cl.def("insert", (class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, const std::string &)) &std::vector<std::string, std::allocator<std::string > >::insert, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::insert(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, const std::string &) --> class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >", pybind11::arg("__position"), pybind11::arg("__x"));
		cl.def("insert", (void (std::vector<std::string,std::allocator<std::string >>::*)(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, unsigned long, const std::string &)) &std::vector<std::string, std::allocator<std::string > >::insert, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::insert(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, unsigned long, const std::string &) --> void", pybind11::arg("__position"), pybind11::arg("__n"), pybind11::arg("__x"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >)) &std::vector<std::string, std::allocator<std::string > >::erase, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::erase(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >) --> class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >", pybind11::arg("__position"));
		cl.def("erase", (class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > > (std::vector<std::string,std::allocator<std::string >>::*)(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >)) &std::vector<std::string, std::allocator<std::string > >::erase, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::erase(class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >, class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >) --> class __gnu_cxx::__normal_iterator<std::string *, class std::vector<std::string, class std::allocator<std::string > > >", pybind11::arg("__first"), pybind11::arg("__last"));
		cl.def("swap", (void (std::vector<std::string,std::allocator<std::string >>::*)(class std::vector<std::string, class std::allocator<std::string > > &)) &std::vector<std::string, std::allocator<std::string > >::swap, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::swap(class std::vector<std::string, class std::allocator<std::string > > &) --> void", pybind11::arg("__x"));
		cl.def("clear", (void (std::vector<std::string,std::allocator<std::string >>::*)()) &std::vector<std::string, std::allocator<std::string > >::clear, "C++: std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >::clear() --> void");
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

void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M)
{

	 binder::search_binder(M("HepMC3"));
	{ // HepMC3::VectorCharAttribute file: line:683
		pybind11::class_<HepMC3::VectorCharAttribute, std::shared_ptr<HepMC3::VectorCharAttribute>, PyCallBack_HepMC3_VectorCharAttribute> cl(M("HepMC3"), "VectorCharAttribute", "Attribute that holds a vector of charegers of type  char\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorCharAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorCharAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<char, class std::allocator<char> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorCharAttribute const &o){ return new PyCallBack_HepMC3_VectorCharAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorCharAttribute const &o){ return new HepMC3::VectorCharAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorCharAttribute::*)(const std::string &)) &HepMC3::VectorCharAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorCharAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorCharAttribute::*)(std::string &) const) &HepMC3::VectorCharAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorCharAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<char, class std::allocator<char> > (HepMC3::VectorCharAttribute::*)() const) &HepMC3::VectorCharAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorCharAttribute::value() const --> class std::vector<char, class std::allocator<char> >");
		cl.def("set_value", (void (HepMC3::VectorCharAttribute::*)(const class std::vector<char, class std::allocator<char> > &)) &HepMC3::VectorCharAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorCharAttribute::set_value(const class std::vector<char, class std::allocator<char> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorCharAttribute & (HepMC3::VectorCharAttribute::*)(const class HepMC3::VectorCharAttribute &)) &HepMC3::VectorCharAttribute::operator=, "C++: HepMC3::VectorCharAttribute::operator=(const class HepMC3::VectorCharAttribute &) --> class HepMC3::VectorCharAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorFloatAttribute file: line:728
		pybind11::class_<HepMC3::VectorFloatAttribute, std::shared_ptr<HepMC3::VectorFloatAttribute>, PyCallBack_HepMC3_VectorFloatAttribute> cl(M("HepMC3"), "VectorFloatAttribute", "Attribute that holds a vector of floategers of type  float\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorFloatAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorFloatAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<float, class std::allocator<float> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorFloatAttribute const &o){ return new PyCallBack_HepMC3_VectorFloatAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorFloatAttribute const &o){ return new HepMC3::VectorFloatAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorFloatAttribute::*)(const std::string &)) &HepMC3::VectorFloatAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorFloatAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorFloatAttribute::*)(std::string &) const) &HepMC3::VectorFloatAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorFloatAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<float, class std::allocator<float> > (HepMC3::VectorFloatAttribute::*)() const) &HepMC3::VectorFloatAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorFloatAttribute::value() const --> class std::vector<float, class std::allocator<float> >");
		cl.def("set_value", (void (HepMC3::VectorFloatAttribute::*)(const class std::vector<float, class std::allocator<float> > &)) &HepMC3::VectorFloatAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorFloatAttribute::set_value(const class std::vector<float, class std::allocator<float> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorFloatAttribute & (HepMC3::VectorFloatAttribute::*)(const class HepMC3::VectorFloatAttribute &)) &HepMC3::VectorFloatAttribute::operator=, "C++: HepMC3::VectorFloatAttribute::operator=(const class HepMC3::VectorFloatAttribute &) --> class HepMC3::VectorFloatAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorLongDoubleAttribute file: line:774
		pybind11::class_<HepMC3::VectorLongDoubleAttribute, std::shared_ptr<HepMC3::VectorLongDoubleAttribute>, PyCallBack_HepMC3_VectorLongDoubleAttribute> cl(M("HepMC3"), "VectorLongDoubleAttribute", "Attribute that holds a vector of long doubleegers of type  long double\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongDoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongDoubleAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<long double, class std::allocator<long double> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongDoubleAttribute const &o){ return new PyCallBack_HepMC3_VectorLongDoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongDoubleAttribute const &o){ return new HepMC3::VectorLongDoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongDoubleAttribute::*)(const std::string &)) &HepMC3::VectorLongDoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongDoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongDoubleAttribute::*)(std::string &) const) &HepMC3::VectorLongDoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongDoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<long double, class std::allocator<long double> > (HepMC3::VectorLongDoubleAttribute::*)() const) &HepMC3::VectorLongDoubleAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorLongDoubleAttribute::value() const --> class std::vector<long double, class std::allocator<long double> >");
		cl.def("set_value", (void (HepMC3::VectorLongDoubleAttribute::*)(const class std::vector<long double, class std::allocator<long double> > &)) &HepMC3::VectorLongDoubleAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorLongDoubleAttribute::set_value(const class std::vector<long double, class std::allocator<long double> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorLongDoubleAttribute & (HepMC3::VectorLongDoubleAttribute::*)(const class HepMC3::VectorLongDoubleAttribute &)) &HepMC3::VectorLongDoubleAttribute::operator=, "C++: HepMC3::VectorLongDoubleAttribute::operator=(const class HepMC3::VectorLongDoubleAttribute &) --> class HepMC3::VectorLongDoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorLongLongAttribute file: line:821
		pybind11::class_<HepMC3::VectorLongLongAttribute, std::shared_ptr<HepMC3::VectorLongLongAttribute>, PyCallBack_HepMC3_VectorLongLongAttribute> cl(M("HepMC3"), "VectorLongLongAttribute", "Attribute that holds a vector of long longegers of type  long long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorLongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorLongLongAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<long long, class std::allocator<long long> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorLongLongAttribute const &o){ return new PyCallBack_HepMC3_VectorLongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorLongLongAttribute const &o){ return new HepMC3::VectorLongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorLongLongAttribute::*)(const std::string &)) &HepMC3::VectorLongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorLongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorLongLongAttribute::*)(std::string &) const) &HepMC3::VectorLongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorLongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<long long, class std::allocator<long long> > (HepMC3::VectorLongLongAttribute::*)() const) &HepMC3::VectorLongLongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorLongLongAttribute::value() const --> class std::vector<long long, class std::allocator<long long> >");
		cl.def("set_value", (void (HepMC3::VectorLongLongAttribute::*)(const class std::vector<long long, class std::allocator<long long> > &)) &HepMC3::VectorLongLongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorLongLongAttribute::set_value(const class std::vector<long long, class std::allocator<long long> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorLongLongAttribute & (HepMC3::VectorLongLongAttribute::*)(const class HepMC3::VectorLongLongAttribute &)) &HepMC3::VectorLongLongAttribute::operator=, "C++: HepMC3::VectorLongLongAttribute::operator=(const class HepMC3::VectorLongLongAttribute &) --> class HepMC3::VectorLongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorUIntAttribute file: line:866
		pybind11::class_<HepMC3::VectorUIntAttribute, std::shared_ptr<HepMC3::VectorUIntAttribute>, PyCallBack_HepMC3_VectorUIntAttribute> cl(M("HepMC3"), "VectorUIntAttribute", "Attribute that holds a vector of unsigned integers of type  unsigned int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorUIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorUIntAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<unsigned int, class std::allocator<unsigned int> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorUIntAttribute const &o){ return new PyCallBack_HepMC3_VectorUIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorUIntAttribute const &o){ return new HepMC3::VectorUIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorUIntAttribute::*)(const std::string &)) &HepMC3::VectorUIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorUIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorUIntAttribute::*)(std::string &) const) &HepMC3::VectorUIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorUIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<unsigned int, class std::allocator<unsigned int> > (HepMC3::VectorUIntAttribute::*)() const) &HepMC3::VectorUIntAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorUIntAttribute::value() const --> class std::vector<unsigned int, class std::allocator<unsigned int> >");
		cl.def("set_value", (void (HepMC3::VectorUIntAttribute::*)(const class std::vector<unsigned int, class std::allocator<unsigned int> > &)) &HepMC3::VectorUIntAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorUIntAttribute::set_value(const class std::vector<unsigned int, class std::allocator<unsigned int> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorUIntAttribute & (HepMC3::VectorUIntAttribute::*)(const class HepMC3::VectorUIntAttribute &)) &HepMC3::VectorUIntAttribute::operator=, "C++: HepMC3::VectorUIntAttribute::operator=(const class HepMC3::VectorUIntAttribute &) --> class HepMC3::VectorUIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorULongAttribute file: line:911
		pybind11::class_<HepMC3::VectorULongAttribute, std::shared_ptr<HepMC3::VectorULongAttribute>, PyCallBack_HepMC3_VectorULongAttribute> cl(M("HepMC3"), "VectorULongAttribute", "Attribute that holds a vector of unsigned longegers of type  unsigned long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorULongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorULongAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<unsigned long, class std::allocator<unsigned long> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorULongAttribute const &o){ return new PyCallBack_HepMC3_VectorULongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorULongAttribute const &o){ return new HepMC3::VectorULongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorULongAttribute::*)(const std::string &)) &HepMC3::VectorULongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorULongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorULongAttribute::*)(std::string &) const) &HepMC3::VectorULongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorULongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<unsigned long, class std::allocator<unsigned long> > (HepMC3::VectorULongAttribute::*)() const) &HepMC3::VectorULongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorULongAttribute::value() const --> class std::vector<unsigned long, class std::allocator<unsigned long> >");
		cl.def("set_value", (void (HepMC3::VectorULongAttribute::*)(const class std::vector<unsigned long, class std::allocator<unsigned long> > &)) &HepMC3::VectorULongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorULongAttribute::set_value(const class std::vector<unsigned long, class std::allocator<unsigned long> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorULongAttribute & (HepMC3::VectorULongAttribute::*)(const class HepMC3::VectorULongAttribute &)) &HepMC3::VectorULongAttribute::operator=, "C++: HepMC3::VectorULongAttribute::operator=(const class HepMC3::VectorULongAttribute &) --> class HepMC3::VectorULongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorULongLongAttribute file: line:957
		pybind11::class_<HepMC3::VectorULongLongAttribute, std::shared_ptr<HepMC3::VectorULongLongAttribute>, PyCallBack_HepMC3_VectorULongLongAttribute> cl(M("HepMC3"), "VectorULongLongAttribute", "Attribute that holds a vector of unsigned long longegers of type  unsigned long long\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorULongLongAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorULongLongAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<unsigned long long, class std::allocator<unsigned long long> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorULongLongAttribute const &o){ return new PyCallBack_HepMC3_VectorULongLongAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorULongLongAttribute const &o){ return new HepMC3::VectorULongLongAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorULongLongAttribute::*)(const std::string &)) &HepMC3::VectorULongLongAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorULongLongAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorULongLongAttribute::*)(std::string &) const) &HepMC3::VectorULongLongAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorULongLongAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<unsigned long long, class std::allocator<unsigned long long> > (HepMC3::VectorULongLongAttribute::*)() const) &HepMC3::VectorULongLongAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorULongLongAttribute::value() const --> class std::vector<unsigned long long, class std::allocator<unsigned long long> >");
		cl.def("set_value", (void (HepMC3::VectorULongLongAttribute::*)(const class std::vector<unsigned long long, class std::allocator<unsigned long long> > &)) &HepMC3::VectorULongLongAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorULongLongAttribute::set_value(const class std::vector<unsigned long long, class std::allocator<unsigned long long> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorULongLongAttribute & (HepMC3::VectorULongLongAttribute::*)(const class HepMC3::VectorULongLongAttribute &)) &HepMC3::VectorULongLongAttribute::operator=, "C++: HepMC3::VectorULongLongAttribute::operator=(const class HepMC3::VectorULongLongAttribute &) --> class HepMC3::VectorULongLongAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
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

void bind_unknown_unknown_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::VectorIntAttribute file: line:1002
		pybind11::class_<HepMC3::VectorIntAttribute, std::shared_ptr<HepMC3::VectorIntAttribute>, PyCallBack_HepMC3_VectorIntAttribute> cl(M("HepMC3"), "VectorIntAttribute", "Attribute that holds a vector of integers of type  int\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorIntAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorIntAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<int, class std::allocator<int> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorIntAttribute const &o){ return new PyCallBack_HepMC3_VectorIntAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorIntAttribute const &o){ return new HepMC3::VectorIntAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorIntAttribute::*)(const std::string &)) &HepMC3::VectorIntAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorIntAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorIntAttribute::*)(std::string &) const) &HepMC3::VectorIntAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorIntAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<int, class std::allocator<int> > (HepMC3::VectorIntAttribute::*)() const) &HepMC3::VectorIntAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorIntAttribute::value() const --> class std::vector<int, class std::allocator<int> >");
		cl.def("set_value", (void (HepMC3::VectorIntAttribute::*)(const class std::vector<int, class std::allocator<int> > &)) &HepMC3::VectorIntAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorIntAttribute::set_value(const class std::vector<int, class std::allocator<int> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorIntAttribute & (HepMC3::VectorIntAttribute::*)(const class HepMC3::VectorIntAttribute &)) &HepMC3::VectorIntAttribute::operator=, "C++: HepMC3::VectorIntAttribute::operator=(const class HepMC3::VectorIntAttribute &) --> class HepMC3::VectorIntAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
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
	{ // HepMC3::VectorDoubleAttribute file: line:1092
		pybind11::class_<HepMC3::VectorDoubleAttribute, std::shared_ptr<HepMC3::VectorDoubleAttribute>, PyCallBack_HepMC3_VectorDoubleAttribute> cl(M("HepMC3"), "VectorDoubleAttribute", "Attribute that holds a vector of FPs of type  double\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorDoubleAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorDoubleAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<double, class std::allocator<double> >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorDoubleAttribute const &o){ return new PyCallBack_HepMC3_VectorDoubleAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorDoubleAttribute const &o){ return new HepMC3::VectorDoubleAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorDoubleAttribute::*)(const std::string &)) &HepMC3::VectorDoubleAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorDoubleAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorDoubleAttribute::*)(std::string &) const) &HepMC3::VectorDoubleAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorDoubleAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<double, class std::allocator<double> > (HepMC3::VectorDoubleAttribute::*)() const) &HepMC3::VectorDoubleAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorDoubleAttribute::value() const --> class std::vector<double, class std::allocator<double> >");
		cl.def("set_value", (void (HepMC3::VectorDoubleAttribute::*)(const class std::vector<double, class std::allocator<double> > &)) &HepMC3::VectorDoubleAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorDoubleAttribute::set_value(const class std::vector<double, class std::allocator<double> > &) --> void", pybind11::arg("i"));
		cl.def("assign", (class HepMC3::VectorDoubleAttribute & (HepMC3::VectorDoubleAttribute::*)(const class HepMC3::VectorDoubleAttribute &)) &HepMC3::VectorDoubleAttribute::operator=, "C++: HepMC3::VectorDoubleAttribute::operator=(const class HepMC3::VectorDoubleAttribute &) --> class HepMC3::VectorDoubleAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::VectorStringAttribute file: line:1138
		pybind11::class_<HepMC3::VectorStringAttribute, std::shared_ptr<HepMC3::VectorStringAttribute>, PyCallBack_HepMC3_VectorStringAttribute> cl(M("HepMC3"), "VectorStringAttribute", "Attribute that holds a vector of FPs of type  string\n\n  \n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::VectorStringAttribute(); }, [](){ return new PyCallBack_HepMC3_VectorStringAttribute(); } ) );
		cl.def( pybind11::init<class std::vector<std::string, class std::allocator<std::string > >>(), pybind11::arg("val") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_VectorStringAttribute const &o){ return new PyCallBack_HepMC3_VectorStringAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::VectorStringAttribute const &o){ return new HepMC3::VectorStringAttribute(o); } ) );
		cl.def("from_string", (bool (HepMC3::VectorStringAttribute::*)(const std::string &)) &HepMC3::VectorStringAttribute::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::VectorStringAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::VectorStringAttribute::*)(std::string &) const) &HepMC3::VectorStringAttribute::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::VectorStringAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("value", (class std::vector<std::string, class std::allocator<std::string > > (HepMC3::VectorStringAttribute::*)() const) &HepMC3::VectorStringAttribute::value, "get the value associated to this Attribute. \n\nC++: HepMC3::VectorStringAttribute::value() const --> class std::vector<std::string, class std::allocator<std::string > >");
		cl.def("set_value", (void (HepMC3::VectorStringAttribute::*)(const class std::vector<std::string, class std::allocator<std::string > > &)) &HepMC3::VectorStringAttribute::set_value, "set the value associated to this Attribute. \n\nC++: HepMC3::VectorStringAttribute::set_value(const class std::vector<std::string, class std::allocator<std::string > > &) --> void", pybind11::arg("i"));
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

void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_2(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_3(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_4(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_5(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
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
	bind_std_stl_vector_1(M);
	bind_std_stl_vector_2(M);
	bind_std_stl_vector_3(M);
	bind_std_stl_vector_4(M);
	bind_std_stl_vector_5(M);
	bind_unknown_unknown(M);
	bind_unknown_unknown_1(M);
	bind_HepMC3_AttributeFeature(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.sources
// pyHepMC3search.cpp
// std/stl_vector.cpp
// std/stl_vector_1.cpp
// std/stl_vector_2.cpp
// std/stl_vector_3.cpp
// std/stl_vector_4.cpp
// std/stl_vector_5.cpp
// unknown/unknown.cpp
// unknown/unknown_1.cpp
// HepMC3/AttributeFeature.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3search.modules
// HepMC3 std 
