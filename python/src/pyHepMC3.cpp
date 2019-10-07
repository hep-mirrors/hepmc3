// File: HepMC3/Setup.cpp
#include <HepMC3/Setup.h>
#include <sstream> // __str__

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_Setup(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::Setup file:HepMC3/Setup.h line:22
		pybind11::class_<HepMC3::Setup, HepMC3::Setup*> cl(M("HepMC3"), "Setup", "Configuration for HepMC\n\n Contains macro definitions for printing debug output, feature deprecation, etc.\n Static class - configuration is shared among all HepMC events\n and program threads");
		pybind11::handle cl_type = cl;

		cl.def_static("print_errors", (bool (*)()) &HepMC3::Setup::print_errors, "Get error messages printing flag\n\nC++: HepMC3::Setup::print_errors() --> bool");
		cl.def_static("set_print_errors", (void (*)(const bool)) &HepMC3::Setup::set_print_errors, "set error messages printing flag\n\nC++: HepMC3::Setup::set_print_errors(const bool) --> void", pybind11::arg("flag"));
		cl.def_static("print_warnings", (bool (*)()) &HepMC3::Setup::print_warnings, "Get warning messages printing flag\n\nC++: HepMC3::Setup::print_warnings() --> bool");
		cl.def_static("set_print_warnings", (void (*)(const bool)) &HepMC3::Setup::set_print_warnings, "Set warning messages printing flag\n\nC++: HepMC3::Setup::set_print_warnings(const bool) --> void", pybind11::arg("flag"));
		cl.def_static("debug_level", (int (*)()) &HepMC3::Setup::debug_level, "Get debug level\n\nC++: HepMC3::Setup::debug_level() --> int");
		cl.def_static("set_debug_level", (void (*)(const int)) &HepMC3::Setup::set_debug_level, "Set debug level\n\nC++: HepMC3::Setup::set_debug_level(const int) --> void", pybind11::arg("level"));
	}
}


// File: HepMC3/FourVector.cpp
#include <HepMC3/FourVector.h>
#include <sstream> // __str__

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_FourVector(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::FourVector file:HepMC3/FourVector.h line:35
		pybind11::class_<HepMC3::FourVector, std::shared_ptr<HepMC3::FourVector>> cl(M("HepMC3"), "FourVector", "Generic 4-vector\n\n  Interpretation of its content depends on accessors used: it's much simpler to do this\n  than to distinguish between space and momentum vectors via the type system (especially\n  given the need for backward compatibility with HepMC2). Be sensible and don't call\n  energy functions on spatial vectors! To avoid duplication, most definitions are only\n  implemented on the spatial function names, with the energy-momentum functions as aliases.\n\n  This is  intended to be a fully featured 4-vector, but does contain the majority\n  of common non-boosting functionality, as well as a few support operations on\n  4-vectors.\n\n  The implementations in this class are fully inlined.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::FourVector(); } ) );
		cl.def( pybind11::init<double, double, double, double>(), pybind11::arg("xx"), pybind11::arg("yy"), pybind11::arg("zz"), pybind11::arg("ee") );

		cl.def( pybind11::init( [](HepMC3::FourVector const &o){ return new HepMC3::FourVector(o); } ) );
		cl.def("set", (void (HepMC3::FourVector::*)(double, double, double, double)) &HepMC3::FourVector::set, "Set all FourVector fields, in order x,y,z,t \n\nC++: HepMC3::FourVector::set(double, double, double, double) --> void", pybind11::arg("x1"), pybind11::arg("x2"), pybind11::arg("x3"), pybind11::arg("x4"));
		cl.def("set_component", (void (HepMC3::FourVector::*)(const int, const double)) &HepMC3::FourVector::set_component, "set component of position/displacement\n\nC++: HepMC3::FourVector::set_component(const int, const double) --> void", pybind11::arg("i"), pybind11::arg("x"));
		cl.def("get_component", (double (HepMC3::FourVector::*)(const int) const) &HepMC3::FourVector::get_component, "get component of position/displacement\n\nC++: HepMC3::FourVector::get_component(const int) const --> double", pybind11::arg("i"));
		cl.def("x", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::x, "x-component of position/displacement\n\nC++: HepMC3::FourVector::x() const --> double");
		cl.def("set_x", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_x, "Set x-component of position/displacement\n\nC++: HepMC3::FourVector::set_x(double) --> void", pybind11::arg("xx"));
		cl.def("setX", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setX, "Prefer the HepMC-style set_x() function\n\nC++: HepMC3::FourVector::setX(double) --> void", pybind11::arg("xx"));
		cl.def("y", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::y, "y-component of position/displacement\n\nC++: HepMC3::FourVector::y() const --> double");
		cl.def("set_y", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_y, "Set y-component of position/displacement\n\nC++: HepMC3::FourVector::set_y(double) --> void", pybind11::arg("yy"));
		cl.def("setY", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setY, "Prefer the HepMC-style set_y() function\n\nC++: HepMC3::FourVector::setY(double) --> void", pybind11::arg("yy"));
		cl.def("z", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::z, "z-component of position/displacement\n\nC++: HepMC3::FourVector::z() const --> double");
		cl.def("set_z", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_z, "Set z-component of position/displacement\n\nC++: HepMC3::FourVector::set_z(double) --> void", pybind11::arg("zz"));
		cl.def("setZ", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setZ, "Prefer the HepMC-style set_z() function\n\nC++: HepMC3::FourVector::setZ(double) --> void", pybind11::arg("zz"));
		cl.def("t", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::t, "Time component of position/displacement\n\nC++: HepMC3::FourVector::t() const --> double");
		cl.def("set_t", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_t, "Set time component of position/displacement\n\nC++: HepMC3::FourVector::set_t(double) --> void", pybind11::arg("tt"));
		cl.def("setT", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setT, "Prefer the HepMC-style set_t() function\n\nC++: HepMC3::FourVector::setT(double) --> void", pybind11::arg("tt"));
		cl.def("px", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::px, "x-component of momentum\n\nC++: HepMC3::FourVector::px() const --> double");
		cl.def("set_px", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_px, "Set x-component of momentum\n\nC++: HepMC3::FourVector::set_px(double) --> void", pybind11::arg("pxx"));
		cl.def("setPx", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setPx, "Prefer the HepMC-style set_px() function\n\nC++: HepMC3::FourVector::setPx(double) --> void", pybind11::arg("pxx"));
		cl.def("py", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::py, "y-component of momentum\n\nC++: HepMC3::FourVector::py() const --> double");
		cl.def("set_py", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_py, "Set y-component of momentum\n\nC++: HepMC3::FourVector::set_py(double) --> void", pybind11::arg("pyy"));
		cl.def("setPy", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setPy, "Prefer the HepMC-style set_py() function\n\nC++: HepMC3::FourVector::setPy(double) --> void", pybind11::arg("pyy"));
		cl.def("pz", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::pz, "z-component of momentum\n\nC++: HepMC3::FourVector::pz() const --> double");
		cl.def("set_pz", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_pz, "Set z-component of momentum\n\nC++: HepMC3::FourVector::set_pz(double) --> void", pybind11::arg("pzz"));
		cl.def("setPz", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setPz, "Prefer the HepMC-style set_pz() function\n\nC++: HepMC3::FourVector::setPz(double) --> void", pybind11::arg("pzz"));
		cl.def("e", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::e, "Energy component of momentum\n\nC++: HepMC3::FourVector::e() const --> double");
		cl.def("set_e", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::set_e, "Set energy component of momentum\n\nC++: HepMC3::FourVector::set_e(double) --> void", pybind11::arg("ee"));
		cl.def("setE", (void (HepMC3::FourVector::*)(double)) &HepMC3::FourVector::setE, "Prefer the HepMC-style set_y() function\n\nC++: HepMC3::FourVector::setE(double) --> void", pybind11::arg("ee"));
		cl.def("length2", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::length2, "Squared magnitude of (x, y, z) 3-vector\n\nC++: HepMC3::FourVector::length2() const --> double");
		cl.def("length", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::length, "Magnitude of spatial (x, y, z) 3-vector\n\nC++: HepMC3::FourVector::length() const --> double");
		cl.def("perp2", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::perp2, "Squared magnitude of (x, y) vector\n\nC++: HepMC3::FourVector::perp2() const --> double");
		cl.def("perp", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::perp, "Magnitude of (x, y) vector\n\nC++: HepMC3::FourVector::perp() const --> double");
		cl.def("interval", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::interval, "Spacetime invariant interval s^2 = t^2 - x^2 - y^2 - z^2\n\nC++: HepMC3::FourVector::interval() const --> double");
		cl.def("p3mod2", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::p3mod2, "Squared magnitude of p3 = (px, py, pz) vector\n\nC++: HepMC3::FourVector::p3mod2() const --> double");
		cl.def("p3mod", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::p3mod, "Magnitude of p3 = (px, py, pz) vector\n\nC++: HepMC3::FourVector::p3mod() const --> double");
		cl.def("pt2", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::pt2, "Squared transverse momentum px^2 + py^2\n\nC++: HepMC3::FourVector::pt2() const --> double");
		cl.def("pt", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::pt, "Transverse momentum\n\nC++: HepMC3::FourVector::pt() const --> double");
		cl.def("m2", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::m2, "Squared invariant mass m^2 = E^2 - px^2 - py^2 - pz^2\n\nC++: HepMC3::FourVector::m2() const --> double");
		cl.def("m", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::m, "Invariant mass. Returns -sqrt(-m) if e^2 - P^2 is negative\n\nC++: HepMC3::FourVector::m() const --> double");
		cl.def("phi", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::phi, "Azimuthal angle\n\nC++: HepMC3::FourVector::phi() const --> double");
		cl.def("theta", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::theta, "Polar angle w.r.t. z direction\n\nC++: HepMC3::FourVector::theta() const --> double");
		cl.def("eta", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::eta, "Pseudorapidity\n\nC++: HepMC3::FourVector::eta() const --> double");
		cl.def("rap", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::rap, "Rapidity\n\nC++: HepMC3::FourVector::rap() const --> double");
		cl.def("abs_eta", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::abs_eta, "Absolute pseudorapidity\n\nC++: HepMC3::FourVector::abs_eta() const --> double");
		cl.def("abs_rap", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::abs_rap, "Absolute rapidity\n\nC++: HepMC3::FourVector::abs_rap() const --> double");
		cl.def("pseudoRapidity", (double (HepMC3::FourVector::*)() const) &HepMC3::FourVector::pseudoRapidity, "Same as eta()\n \n\n Prefer 'only one way to do it', and we don't have equivalent long names for e.g. pid, phi or eta\n\nC++: HepMC3::FourVector::pseudoRapidity() const --> double");
		cl.def("is_zero", (bool (HepMC3::FourVector::*)() const) &HepMC3::FourVector::is_zero, "Check if the length of this vertex is zero\n\nC++: HepMC3::FourVector::is_zero() const --> bool");
		cl.def("delta_phi", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_phi, "Signed azimuthal angle separation in [-pi, pi]\n\nC++: HepMC3::FourVector::delta_phi(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_eta", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_eta, "Pseudorapidity separation\n\nC++: HepMC3::FourVector::delta_eta(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_rap", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_rap, "Rapidity separation\n\nC++: HepMC3::FourVector::delta_rap(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_r2_eta", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_r2_eta, "R_eta^2-distance separation dR^2 = dphi^2 + deta^2\n\nC++: HepMC3::FourVector::delta_r2_eta(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_r_eta", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_r_eta, "R_eta-distance separation dR = sqrt(dphi^2 + deta^2)\n\nC++: HepMC3::FourVector::delta_r_eta(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_r2_rap", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_r2_rap, "R_rap^2-distance separation dR^2 = dphi^2 + drap^2\n\nC++: HepMC3::FourVector::delta_r2_rap(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("delta_r_rap", (double (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::delta_r_rap, "R-rap-distance separation dR = sqrt(dphi^2 + drap^2)\n\nC++: HepMC3::FourVector::delta_r_rap(const class HepMC3::FourVector &) const --> double", pybind11::arg("v"));
		cl.def("__eq__", (bool (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::operator==, "Equality\n\nC++: HepMC3::FourVector::operator==(const class HepMC3::FourVector &) const --> bool", pybind11::arg("rhs"));
		cl.def("__ne__", (bool (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::operator!=, "Inequality\n\nC++: HepMC3::FourVector::operator!=(const class HepMC3::FourVector &) const --> bool", pybind11::arg("rhs"));
		cl.def("__add__", (class HepMC3::FourVector (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::operator+, "Arithmetic operator +\n\nC++: HepMC3::FourVector::operator+(const class HepMC3::FourVector &) const --> class HepMC3::FourVector", pybind11::arg("rhs"));
		cl.def("__sub__", (class HepMC3::FourVector (HepMC3::FourVector::*)(const class HepMC3::FourVector &) const) &HepMC3::FourVector::operator-, "Arithmetic operator -\n\nC++: HepMC3::FourVector::operator-(const class HepMC3::FourVector &) const --> class HepMC3::FourVector", pybind11::arg("rhs"));
		cl.def("__mul__", (class HepMC3::FourVector (HepMC3::FourVector::*)(const double) const) &HepMC3::FourVector::operator*, "Arithmetic operator * by scalar\n\nC++: HepMC3::FourVector::operator*(const double) const --> class HepMC3::FourVector", pybind11::arg("rhs"));
		cl.def("__div__", (class HepMC3::FourVector (HepMC3::FourVector::*)(const double) const) &HepMC3::FourVector::operator/, "Arithmetic operator / by scalar\n\nC++: HepMC3::FourVector::operator/(const double) const --> class HepMC3::FourVector", pybind11::arg("rhs"));
		cl.def("__iadd__", (void (HepMC3::FourVector::*)(const class HepMC3::FourVector &)) &HepMC3::FourVector::operator+=, "Arithmetic operator +=\n\nC++: HepMC3::FourVector::operator+=(const class HepMC3::FourVector &) --> void", pybind11::arg("rhs"));
		cl.def("__isub__", (void (HepMC3::FourVector::*)(const class HepMC3::FourVector &)) &HepMC3::FourVector::operator-=, "Arithmetic operator -=\n\nC++: HepMC3::FourVector::operator-=(const class HepMC3::FourVector &) --> void", pybind11::arg("rhs"));
		cl.def("__imul__", (void (HepMC3::FourVector::*)(const double)) &HepMC3::FourVector::operator*=, "Arithmetic operator *= by scalar\n\nC++: HepMC3::FourVector::operator*=(const double) --> void", pybind11::arg("rhs"));
		cl.def("__idiv__", (void (HepMC3::FourVector::*)(const double)) &HepMC3::FourVector::operator/=, "Arithmetic operator /= by scalar\n\nC++: HepMC3::FourVector::operator/=(const double) --> void", pybind11::arg("rhs"));
		cl.def_static("ZERO_VECTOR", (const class HepMC3::FourVector & (*)()) &HepMC3::FourVector::ZERO_VECTOR, "Static null FourVector = (0,0,0,0)\n\nC++: HepMC3::FourVector::ZERO_VECTOR() --> const class HepMC3::FourVector &", pybind11::return_value_policy::automatic);
		cl.def("assign", (class HepMC3::FourVector & (HepMC3::FourVector::*)(const class HepMC3::FourVector &)) &HepMC3::FourVector::operator=, "C++: HepMC3::FourVector::operator=(const class HepMC3::FourVector &) --> class HepMC3::FourVector &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_FourVector_binder(cl);
	}
	// HepMC3::delta_phi(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:312
	M("HepMC3").def("delta_phi", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_phi, "Signed azimuthal angle separation in [-pi, pi] between vecs  and \n\nC++: HepMC3::delta_phi(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

	// HepMC3::delta_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:315
	M("HepMC3").def("delta_eta", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_eta, "Pseudorapidity separation between vecs  and \n\nC++: HepMC3::delta_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

	// HepMC3::delta_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:318
	M("HepMC3").def("delta_rap", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_rap, "Rapidity separation between vecs  and \n\nC++: HepMC3::delta_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

}


// File: HepMC3/FourVector_1.cpp
#include <HepMC3/FourVector.h>

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_FourVector_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// HepMC3::delta_r2_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:321
	M("HepMC3").def("delta_r2_eta", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_r2_eta, "R_eta^2-distance separation dR^2 = dphi^2 + deta^2 between vecs  and \n\nC++: HepMC3::delta_r2_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

	// HepMC3::delta_r_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:324
	M("HepMC3").def("delta_r_eta", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_r_eta, "R_eta-distance separation dR = sqrt(dphi^2 + deta^2) between vecs  and \n\nC++: HepMC3::delta_r_eta(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

	// HepMC3::delta_r2_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:327
	M("HepMC3").def("delta_r2_rap", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_r2_rap, "R_rap^2-distance separation dR^2 = dphi^2 + drap^2 between vecs  and \n\nC++: HepMC3::delta_r2_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

	// HepMC3::delta_r_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) file:HepMC3/FourVector.h line:330
	M("HepMC3").def("delta_r_rap", (double (*)(const class HepMC3::FourVector &, const class HepMC3::FourVector &)) &HepMC3::delta_r_rap, "R_rap-distance separation dR = sqrt(dphi^2 + drap^2) between vecs  and \n\nC++: HepMC3::delta_r_rap(const class HepMC3::FourVector &, const class HepMC3::FourVector &) --> double", pybind11::arg("a"), pybind11::arg("b"));

}


// File: std/stl_map.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/LHEF.h>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream> // __str__
#include <string>
#include <utility>

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_std_stl_map(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// std::map file:bits/stl_map.h line:96
	binder::map_binder<std::string,std::shared_ptr<HepMC3::Attribute>,std::less<std::string >,std::allocator<std::pair<const std::string, std::shared_ptr<HepMC3::Attribute> > >>(M("std"), "std_string", "std_shared_ptr_HepMC3_Attribute_t", "std_less_std_string_t", "std_allocator_std_pair_const_std_string_std_shared_ptr_HepMC3_Attribute_t");

	// std::map file:bits/stl_map.h line:96
	binder::map_binder<std::string,std::map<int, std::shared_ptr<HepMC3::Attribute>, std::less<int>, std::allocator<std::pair<const int, std::shared_ptr<HepMC3::Attribute> > > >,std::less<std::string >,std::allocator<std::pair<const std::string, std::map<int, std::shared_ptr<HepMC3::Attribute>, std::less<int>, std::allocator<std::pair<const int, std::shared_ptr<HepMC3::Attribute> > > > > >>(M("std"), "std_string", "std_map_int_std_shared_ptr_HepMC3_Attribute_std_less_int_std_allocator_std_pair_const_int_std_shared_ptr_HepMC3_Attribute_t", "std_less_std_string_t", "std_allocator_std_pair_const_std_string_std_map_int_std_shared_ptr_HepMC3_Attribute_std_less_int_std_allocator_std_pair_const_int_std_shared_ptr_HepMC3_Attribute_t");

	// std::map file:bits/stl_map.h line:96
	binder::map_binder<std::string,std::string,std::less<std::string >,std::allocator<std::pair<const std::string, std::string > >>(M("std"), "std_string", "std_string", "std_less_std_string_t", "std_allocator_std_pair_const_std_string_std_string_t");

	// std::map file:bits/stl_map.h line:96
	binder::map_binder<std::string,std::set<long, std::less<long>, std::allocator<long> >,std::less<std::string >,std::allocator<std::pair<const std::string, std::set<long, std::less<long>, std::allocator<long> > > >>(M("std"), "std_string", "std_set_long_std_less_long_std_allocator_long_t", "std_less_std_string_t", "std_allocator_std_pair_const_std_string_std_set_long_std_less_long_std_allocator_long_t");

}


// File: std/stl_vector.cpp
#include <HepMC3/Data/GenParticleData.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/LHEF.h>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <set>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<std::shared_ptr<HepMC3::GenParticle>,std::allocator<std::shared_ptr<HepMC3::GenParticle> >>(M("std"), "std_shared_ptr_HepMC3_GenParticle_t", "std_allocator_std_shared_ptr_HepMC3_GenParticle_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<double,std::allocator<double>>(M("std"), "double", "std_allocator_double_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<std::string,std::allocator<std::string >>(M("std"), "std_string", "std_allocator_std_string_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<std::shared_ptr<HepMC3::GenVertex>,std::allocator<std::shared_ptr<HepMC3::GenVertex> >>(M("std"), "std_shared_ptr_HepMC3_GenVertex_t", "std_allocator_std_shared_ptr_HepMC3_GenVertex_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<LHEF::XMLTag *,std::allocator<LHEF::XMLTag *>>(M("std"), "LHEF_XMLTag_*", "std_allocator_LHEF_XMLTag_*_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<long,std::allocator<long>>(M("std"), "long", "std_allocator_long_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<std::vector<double, std::allocator<double> >,std::allocator<std::vector<double, std::allocator<double> > >>(M("std"), "std_vector_double_std_allocator_double_t", "std_allocator_std_vector_double_std_allocator_double_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<LHEF::WeightInfo,std::allocator<LHEF::WeightInfo>>(M("std"), "LHEF_WeightInfo", "std_allocator_LHEF_WeightInfo_t");

	// std::vector file:bits/stl_vector.h line:210
	binder::vector_binder<LHEF::HEPEUP *,std::allocator<LHEF::HEPEUP *>>(M("std"), "LHEF_HEPEUP_*", "std_allocator_LHEF_HEPEUP_*_t");

}


// File: HepMC3/Attribute.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenEventData.h>
#include <HepMC3/Data/GenRunInfoData.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::Attribute file:HepMC3/Attribute.h line:45
struct PyCallBack_HepMC3_Attribute : public HepMC3::Attribute {
	using HepMC3::Attribute::Attribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Attribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Attribute::from_string\"");
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Attribute *>(this), "init");
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Attribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Attribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Attribute::to_string\"");
	}
};

// HepMC3::IntAttribute file:HepMC3/Attribute.h line:158
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::IntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::LongAttribute file:HepMC3/Attribute.h line:199
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::DoubleAttribute file:HepMC3/Attribute.h line:242
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::DoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::FloatAttribute file:HepMC3/Attribute.h line:287
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::FloatAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::StringAttribute file:HepMC3/Attribute.h line:336
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::StringAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::CharAttribute file:HepMC3/Attribute.h line:380
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::CharAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::LongLongAttribute file:HepMC3/Attribute.h line:425
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

void bind_HepMC3_Attribute(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::Attribute file:HepMC3/Attribute.h line:45
		pybind11::class_<HepMC3::Attribute, std::shared_ptr<HepMC3::Attribute>, PyCallBack_HepMC3_Attribute> cl(M("HepMC3"), "Attribute", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new PyCallBack_HepMC3_Attribute(); } ) );
		cl.def(pybind11::init<PyCallBack_HepMC3_Attribute const &>());
		cl.def("from_string", (bool (HepMC3::Attribute::*)(const std::string &)) &HepMC3::Attribute::from_string, "Fill class content from string.\n\nC++: HepMC3::Attribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("init", (bool (HepMC3::Attribute::*)()) &HepMC3::Attribute::init, "Optionally initialize the attribute after from_string.\n\nC++: HepMC3::Attribute::init() --> bool");
		cl.def("init", (bool (HepMC3::Attribute::*)(const class HepMC3::GenRunInfo &)) &HepMC3::Attribute::init, "Optionally initialize the attribute after from_string\n\n Is passed a reference to the GenRunInfo object to which the\n Attribute belongs.\n\nC++: HepMC3::Attribute::init(const class HepMC3::GenRunInfo &) --> bool", pybind11::arg(""));
		cl.def("to_string", (bool (HepMC3::Attribute::*)(std::string &) const) &HepMC3::Attribute::to_string, "Fill string from class content \n\nC++: HepMC3::Attribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("is_parsed", (bool (HepMC3::Attribute::*)() const) &HepMC3::Attribute::is_parsed, "Check if this attribute is parsed \n\nC++: HepMC3::Attribute::is_parsed() const --> bool");
		cl.def("unparsed_string", (const std::string & (HepMC3::Attribute::*)() const) &HepMC3::Attribute::unparsed_string, "Get unparsed string \n\nC++: HepMC3::Attribute::unparsed_string() const --> const std::string &", pybind11::return_value_policy::automatic);
		cl.def("event", (const class HepMC3::GenEvent * (HepMC3::Attribute::*)() const) &HepMC3::Attribute::event, "return the GenEvent to which this Attribute belongs, if at all. \n\nC++: HepMC3::Attribute::event() const --> const class HepMC3::GenEvent *", pybind11::return_value_policy::automatic);
		cl.def("particle", (class std::shared_ptr<class HepMC3::GenParticle> (HepMC3::Attribute::*)()) &HepMC3::Attribute::particle, "return the GenParticle to which this Attribute belongs, if at all. \n\nC++: HepMC3::Attribute::particle() --> class std::shared_ptr<class HepMC3::GenParticle>");
		cl.def("vertex", (class std::shared_ptr<class HepMC3::GenVertex> (HepMC3::Attribute::*)()) &HepMC3::Attribute::vertex, "return the GenVertex to which this Attribute belongs, if at all. \n\nC++: HepMC3::Attribute::vertex() --> class std::shared_ptr<class HepMC3::GenVertex>");
		cl.def("assign", (class HepMC3::Attribute & (HepMC3::Attribute::*)(const class HepMC3::Attribute &)) &HepMC3::Attribute::operator=, "C++: HepMC3::Attribute::operator=(const class HepMC3::Attribute &) --> class HepMC3::Attribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::IntAttribute file:HepMC3/Attribute.h line:158
		pybind11::class_<HepMC3::IntAttribute, std::shared_ptr<HepMC3::IntAttribute>, PyCallBack_HepMC3_IntAttribute, HepMC3::Attribute> cl(M("HepMC3"), "IntAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
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
	{ // HepMC3::LongAttribute file:HepMC3/Attribute.h line:199
		pybind11::class_<HepMC3::LongAttribute, std::shared_ptr<HepMC3::LongAttribute>, PyCallBack_HepMC3_LongAttribute, HepMC3::Attribute> cl(M("HepMC3"), "LongAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
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
	{ // HepMC3::DoubleAttribute file:HepMC3/Attribute.h line:242
		pybind11::class_<HepMC3::DoubleAttribute, std::shared_ptr<HepMC3::DoubleAttribute>, PyCallBack_HepMC3_DoubleAttribute, HepMC3::Attribute> cl(M("HepMC3"), "DoubleAttribute", "Attribute that holds a real number as a double.\n\n  \n\n ");
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
	{ // HepMC3::FloatAttribute file:HepMC3/Attribute.h line:287
		pybind11::class_<HepMC3::FloatAttribute, std::shared_ptr<HepMC3::FloatAttribute>, PyCallBack_HepMC3_FloatAttribute, HepMC3::Attribute> cl(M("HepMC3"), "FloatAttribute", "Attribute that holds a real number as a float.\n\n  \n\n ");
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
	{ // HepMC3::StringAttribute file:HepMC3/Attribute.h line:336
		pybind11::class_<HepMC3::StringAttribute, std::shared_ptr<HepMC3::StringAttribute>, PyCallBack_HepMC3_StringAttribute, HepMC3::Attribute> cl(M("HepMC3"), "StringAttribute", "Attribute that holds a string\n\n  Default attribute constructed when reading input files.\n  It can be then parsed by other attributes or left as a string.\n\n  \n\n ");
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
	{ // HepMC3::CharAttribute file:HepMC3/Attribute.h line:380
		pybind11::class_<HepMC3::CharAttribute, std::shared_ptr<HepMC3::CharAttribute>, PyCallBack_HepMC3_CharAttribute, HepMC3::Attribute> cl(M("HepMC3"), "CharAttribute", "Attribute that holds an Chareger implemented as an int\n\n  \n\n ");
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
	{ // HepMC3::LongLongAttribute file:HepMC3/Attribute.h line:425
		pybind11::class_<HepMC3::LongLongAttribute, std::shared_ptr<HepMC3::LongLongAttribute>, PyCallBack_HepMC3_LongLongAttribute, HepMC3::Attribute> cl(M("HepMC3"), "LongLongAttribute", "Attribute that holds an Integer implemented as an int\n\n  \n\n ");
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
}


// File: HepMC3/Attribute_1.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenRunInfoData.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenRunInfo.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::LongDoubleAttribute file:HepMC3/Attribute.h line:468
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::LongDoubleAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::UIntAttribute file:HepMC3/Attribute.h line:515
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::UIntAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::ULongAttribute file:HepMC3/Attribute.h line:558
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::ULongLongAttribute file:HepMC3/Attribute.h line:600
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ULongLongAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::BoolAttribute file:HepMC3/Attribute.h line:640
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::BoolAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::GenHeavyIon file:HepMC3/GenHeavyIon.h line:27
struct PyCallBack_HepMC3_GenHeavyIon : public HepMC3::GenHeavyIon {
	using HepMC3::GenHeavyIon::GenHeavyIon;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenHeavyIon *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenHeavyIon::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenHeavyIon *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenHeavyIon::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenHeavyIon *>(this), "init");
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenHeavyIon *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

void bind_HepMC3_Attribute_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::LongDoubleAttribute file:HepMC3/Attribute.h line:468
		pybind11::class_<HepMC3::LongDoubleAttribute, std::shared_ptr<HepMC3::LongDoubleAttribute>, PyCallBack_HepMC3_LongDoubleAttribute, HepMC3::Attribute> cl(M("HepMC3"), "LongDoubleAttribute", "Attribute that holds a real number as a double.\n\n  \n\n ");
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
	{ // HepMC3::UIntAttribute file:HepMC3/Attribute.h line:515
		pybind11::class_<HepMC3::UIntAttribute, std::shared_ptr<HepMC3::UIntAttribute>, PyCallBack_HepMC3_UIntAttribute, HepMC3::Attribute> cl(M("HepMC3"), "UIntAttribute", "Attribute that holds an unsigned int\n\n  \n\n ");
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
	{ // HepMC3::ULongAttribute file:HepMC3/Attribute.h line:558
		pybind11::class_<HepMC3::ULongAttribute, std::shared_ptr<HepMC3::ULongAttribute>, PyCallBack_HepMC3_ULongAttribute, HepMC3::Attribute> cl(M("HepMC3"), "ULongAttribute", "Attribute that holds an unsigned long\n\n  \n\n ");
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
	{ // HepMC3::ULongLongAttribute file:HepMC3/Attribute.h line:600
		pybind11::class_<HepMC3::ULongLongAttribute, std::shared_ptr<HepMC3::ULongLongAttribute>, PyCallBack_HepMC3_ULongLongAttribute, HepMC3::Attribute> cl(M("HepMC3"), "ULongLongAttribute", "Attribute that holds an unsigned long long\n\n  \n\n ");
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
	{ // HepMC3::BoolAttribute file:HepMC3/Attribute.h line:640
		pybind11::class_<HepMC3::BoolAttribute, std::shared_ptr<HepMC3::BoolAttribute>, PyCallBack_HepMC3_BoolAttribute, HepMC3::Attribute> cl(M("HepMC3"), "BoolAttribute", "Attribute that holds an Booleger implemented as an int\n\n  \n\n ");
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
	{ // HepMC3::GenHeavyIon file:HepMC3/GenHeavyIon.h line:27
		pybind11::class_<HepMC3::GenHeavyIon, std::shared_ptr<HepMC3::GenHeavyIon>, PyCallBack_HepMC3_GenHeavyIon, HepMC3::Attribute> cl(M("HepMC3"), "GenHeavyIon", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenHeavyIon(); }, [](){ return new PyCallBack_HepMC3_GenHeavyIon(); } ) );
		cl.def( pybind11::init( [](PyCallBack_HepMC3_GenHeavyIon const &o){ return new PyCallBack_HepMC3_GenHeavyIon(o); } ) );
		cl.def( pybind11::init( [](HepMC3::GenHeavyIon const &o){ return new HepMC3::GenHeavyIon(o); } ) );
		cl.def_readwrite("Ncoll_hard", &HepMC3::GenHeavyIon::Ncoll_hard);
		cl.def_readwrite("Npart_proj", &HepMC3::GenHeavyIon::Npart_proj);
		cl.def_readwrite("Npart_targ", &HepMC3::GenHeavyIon::Npart_targ);
		cl.def_readwrite("Ncoll", &HepMC3::GenHeavyIon::Ncoll);
		cl.def_readwrite("spectator_neutrons", &HepMC3::GenHeavyIon::spectator_neutrons);
		cl.def_readwrite("spectator_protons", &HepMC3::GenHeavyIon::spectator_protons);
		cl.def_readwrite("N_Nwounded_collisions", &HepMC3::GenHeavyIon::N_Nwounded_collisions);
		cl.def_readwrite("Nwounded_N_collisions", &HepMC3::GenHeavyIon::Nwounded_N_collisions);
		cl.def_readwrite("Nwounded_Nwounded_collisions", &HepMC3::GenHeavyIon::Nwounded_Nwounded_collisions);
		cl.def_readwrite("impact_parameter", &HepMC3::GenHeavyIon::impact_parameter);
		cl.def_readwrite("event_plane_angle", &HepMC3::GenHeavyIon::event_plane_angle);
		cl.def_readwrite("eccentricity", &HepMC3::GenHeavyIon::eccentricity);
		cl.def_readwrite("sigma_inel_NN", &HepMC3::GenHeavyIon::sigma_inel_NN);
		cl.def_readwrite("centrality", &HepMC3::GenHeavyIon::centrality);
		cl.def_readwrite("user_cent_estimate", &HepMC3::GenHeavyIon::user_cent_estimate);
		cl.def_readwrite("Nspec_proj_n", &HepMC3::GenHeavyIon::Nspec_proj_n);
		cl.def_readwrite("Nspec_targ_n", &HepMC3::GenHeavyIon::Nspec_targ_n);
		cl.def_readwrite("Nspec_proj_p", &HepMC3::GenHeavyIon::Nspec_proj_p);
		cl.def_readwrite("Nspec_targ_p", &HepMC3::GenHeavyIon::Nspec_targ_p);
		cl.def_readwrite("participant_plane_angles", &HepMC3::GenHeavyIon::participant_plane_angles);
		cl.def_readwrite("eccentricities", &HepMC3::GenHeavyIon::eccentricities);
		cl.def_readwrite("forceoldformat", &HepMC3::GenHeavyIon::forceoldformat);
		cl.def("from_string", (bool (HepMC3::GenHeavyIon::*)(const std::string &)) &HepMC3::GenHeavyIon::from_string, "Implementation of Attribute::from_string.\n\nC++: HepMC3::GenHeavyIon::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::GenHeavyIon::*)(std::string &) const) &HepMC3::GenHeavyIon::to_string, "Implementation of Attribute::to_string.\n\nC++: HepMC3::GenHeavyIon::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("__eq__", (bool (HepMC3::GenHeavyIon::*)(const class HepMC3::GenHeavyIon &) const) &HepMC3::GenHeavyIon::operator==, "Operator ==\n\nC++: HepMC3::GenHeavyIon::operator==(const class HepMC3::GenHeavyIon &) const --> bool", pybind11::arg(""));
		cl.def("__ne__", (bool (HepMC3::GenHeavyIon::*)(const class HepMC3::GenHeavyIon &) const) &HepMC3::GenHeavyIon::operator!=, "Operator !=\n\nC++: HepMC3::GenHeavyIon::operator!=(const class HepMC3::GenHeavyIon &) const --> bool", pybind11::arg(""));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5) -> void { return o.set(a0, a1, a2, a3, a4, a5); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8, const double & a9) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8, const double & a9, const double & a10) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"), pybind11::arg("pl"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8, const double & a9, const double & a10, const double & a11) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"), pybind11::arg("pl"), pybind11::arg("ec"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8, const double & a9, const double & a10, const double & a11, const double & a12) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"), pybind11::arg("pl"), pybind11::arg("ec"), pybind11::arg("s"));
		cl.def("set", [](HepMC3::GenHeavyIon &o, const int & a0, const int & a1, const int & a2, const int & a3, const int & a4, const int & a5, const int & a6, const int & a7, const int & a8, const double & a9, const double & a10, const double & a11, const double & a12, const double & a13) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13); }, "", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"), pybind11::arg("pl"), pybind11::arg("ec"), pybind11::arg("s"), pybind11::arg("cent"));
		cl.def("set", (void (HepMC3::GenHeavyIon::*)(const int &, const int &, const int &, const int &, const int &, const int &, const int &, const int &, const int &, const double &, const double &, const double &, const double &, const double &, const double &)) &HepMC3::GenHeavyIon::set, "Set all fields.\n\n HEPMC3_DEPRECATED(\"Set individual fields directly instead.\")\n \n\n Set all fields \n\nC++: HepMC3::GenHeavyIon::set(const int &, const int &, const int &, const int &, const int &, const int &, const int &, const int &, const int &, const double &, const double &, const double &, const double &, const double &, const double &) --> void", pybind11::arg("nh"), pybind11::arg("np"), pybind11::arg("nt"), pybind11::arg("nc"), pybind11::arg("ns"), pybind11::arg("nsp"), pybind11::arg("nnw"), pybind11::arg("nwn"), pybind11::arg("nwnw"), pybind11::arg("im"), pybind11::arg("pl"), pybind11::arg("ec"), pybind11::arg("s"), pybind11::arg("cent"), pybind11::arg("ucent"));
		cl.def("is_valid", (bool (HepMC3::GenHeavyIon::*)() const) &HepMC3::GenHeavyIon::is_valid, "Verify that the instance contains non-zero information.\n\n HEPMC3_DEPRECATED(\"Each filed now have default values meaning\n that they have not been set\")\n\nC++: HepMC3::GenHeavyIon::is_valid() const --> bool");
		cl.def("assign", (class HepMC3::GenHeavyIon & (HepMC3::GenHeavyIon::*)(const class HepMC3::GenHeavyIon &)) &HepMC3::GenHeavyIon::operator=, "C++: HepMC3::GenHeavyIon::operator=(const class HepMC3::GenHeavyIon &) --> class HepMC3::GenHeavyIon &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
}


// File: HepMC3/GenCrossSection.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenRunInfoData.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenRunInfo.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::GenCrossSection file:HepMC3/GenCrossSection.h line:41
struct PyCallBack_HepMC3_GenCrossSection : public HepMC3::GenCrossSection {
	using HepMC3::GenCrossSection::GenCrossSection;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenCrossSection *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenCrossSection::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenCrossSection *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenCrossSection::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenCrossSection *>(this), "init");
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenCrossSection *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

void bind_HepMC3_GenCrossSection(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::GenCrossSection file:HepMC3/GenCrossSection.h line:41
		pybind11::class_<HepMC3::GenCrossSection, std::shared_ptr<HepMC3::GenCrossSection>, PyCallBack_HepMC3_GenCrossSection, HepMC3::Attribute> cl(M("HepMC3"), "GenCrossSection", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](PyCallBack_HepMC3_GenCrossSection const &o){ return new PyCallBack_HepMC3_GenCrossSection(o); } ) );
		cl.def( pybind11::init( [](HepMC3::GenCrossSection const &o){ return new HepMC3::GenCrossSection(o); } ) );
		cl.def( pybind11::init( [](){ return new HepMC3::GenCrossSection(); }, [](){ return new PyCallBack_HepMC3_GenCrossSection(); } ) );
		cl.def("from_string", (bool (HepMC3::GenCrossSection::*)(const std::string &)) &HepMC3::GenCrossSection::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::GenCrossSection::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::GenCrossSection::*)(std::string &) const) &HepMC3::GenCrossSection::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::GenCrossSection::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("set_cross_section", [](HepMC3::GenCrossSection &o, const double & a0, const double & a1) -> void { return o.set_cross_section(a0, a1); }, "", pybind11::arg("xs"), pybind11::arg("xs_err"));
		cl.def("set_cross_section", [](HepMC3::GenCrossSection &o, const double & a0, const double & a1, const long & a2) -> void { return o.set_cross_section(a0, a1, a2); }, "", pybind11::arg("xs"), pybind11::arg("xs_err"), pybind11::arg("n_acc"));
		cl.def("set_cross_section", (void (HepMC3::GenCrossSection::*)(const double &, const double &, const long &, const long &)) &HepMC3::GenCrossSection::set_cross_section, "Set all fields \n\nC++: HepMC3::GenCrossSection::set_cross_section(const double &, const double &, const long &, const long &) --> void", pybind11::arg("xs"), pybind11::arg("xs_err"), pybind11::arg("n_acc"), pybind11::arg("n_att"));
		cl.def("set_accepted_events", (void (HepMC3::GenCrossSection::*)(const long &)) &HepMC3::GenCrossSection::set_accepted_events, "Set the number of accepted events\n\nC++: HepMC3::GenCrossSection::set_accepted_events(const long &) --> void", pybind11::arg("n_acc"));
		cl.def("set_attempted_events", (void (HepMC3::GenCrossSection::*)(const long &)) &HepMC3::GenCrossSection::set_attempted_events, "Set the number of attempted events\n\nC++: HepMC3::GenCrossSection::set_attempted_events(const long &) --> void", pybind11::arg("n_att"));
		cl.def("get_accepted_events", (long (HepMC3::GenCrossSection::*)() const) &HepMC3::GenCrossSection::get_accepted_events, "Get the number of accepted events\n\nC++: HepMC3::GenCrossSection::get_accepted_events() const --> long");
		cl.def("get_attempted_events", (long (HepMC3::GenCrossSection::*)() const) &HepMC3::GenCrossSection::get_attempted_events, "Get the number of attempted events\n\nC++: HepMC3::GenCrossSection::get_attempted_events() const --> long");
		cl.def("set_xsec", (void (HepMC3::GenCrossSection::*)(const std::string &, const double &)) &HepMC3::GenCrossSection::set_xsec, "Set the cross section  corresponding to the weight\n        named \n     \n\nC++: HepMC3::GenCrossSection::set_xsec(const std::string &, const double &) --> void", pybind11::arg("wName"), pybind11::arg("xs"));
		cl.def("set_xsec", (void (HepMC3::GenCrossSection::*)(const int &, const double &)) &HepMC3::GenCrossSection::set_xsec, "Set the cross section corresponding to the weight with\n        index \n     \n\nC++: HepMC3::GenCrossSection::set_xsec(const int &, const double &) --> void", pybind11::arg("indx"), pybind11::arg("xs"));
		cl.def("set_xsec_err", (void (HepMC3::GenCrossSection::*)(const std::string &, const double &)) &HepMC3::GenCrossSection::set_xsec_err, "Set the cross section error corresponding to the weight\n        named \n     \n\nC++: HepMC3::GenCrossSection::set_xsec_err(const std::string &, const double &) --> void", pybind11::arg("wName"), pybind11::arg("xs_err"));
		cl.def("set_xsec_err", (void (HepMC3::GenCrossSection::*)(const int &, const double &)) &HepMC3::GenCrossSection::set_xsec_err, "Set the cross section error corresponding to the weight\n        with index \n     \n\nC++: HepMC3::GenCrossSection::set_xsec_err(const int &, const double &) --> void", pybind11::arg("indx"), pybind11::arg("xs_err"));
		cl.def("xsec", (double (HepMC3::GenCrossSection::*)(const std::string &) const) &HepMC3::GenCrossSection::xsec, "Get the cross section corresponding to the weight named\n        \n     \n\nC++: HepMC3::GenCrossSection::xsec(const std::string &) const --> double", pybind11::arg("wName"));
		cl.def("xsec", [](HepMC3::GenCrossSection const &o) -> double { return o.xsec(); }, "");
		cl.def("xsec", (double (HepMC3::GenCrossSection::*)(const int &) const) &HepMC3::GenCrossSection::xsec, "Get the cross section corresponding to the weight with index\n        \n     \n\nC++: HepMC3::GenCrossSection::xsec(const int &) const --> double", pybind11::arg("indx"));
		cl.def("xsec_err", (double (HepMC3::GenCrossSection::*)(const std::string &) const) &HepMC3::GenCrossSection::xsec_err, "Get the cross section error corresponding to the weight\n        named \n     \n\nC++: HepMC3::GenCrossSection::xsec_err(const std::string &) const --> double", pybind11::arg("wName"));
		cl.def("xsec_err", [](HepMC3::GenCrossSection const &o) -> double { return o.xsec_err(); }, "");
		cl.def("xsec_err", (double (HepMC3::GenCrossSection::*)(const int &) const) &HepMC3::GenCrossSection::xsec_err, "Get the cross section error corresponding to the weight\n        with index \n     \n\nC++: HepMC3::GenCrossSection::xsec_err(const int &) const --> double", pybind11::arg("indx"));
		cl.def("__eq__", (bool (HepMC3::GenCrossSection::*)(const class HepMC3::GenCrossSection &) const) &HepMC3::GenCrossSection::operator==, "C++: HepMC3::GenCrossSection::operator==(const class HepMC3::GenCrossSection &) const --> bool", pybind11::arg(""));
		cl.def("__ne__", (bool (HepMC3::GenCrossSection::*)(const class HepMC3::GenCrossSection &) const) &HepMC3::GenCrossSection::operator!=, "C++: HepMC3::GenCrossSection::operator!=(const class HepMC3::GenCrossSection &) const --> bool", pybind11::arg(""));
		cl.def("is_valid", (bool (HepMC3::GenCrossSection::*)() const) &HepMC3::GenCrossSection::is_valid, "C++: HepMC3::GenCrossSection::is_valid() const --> bool");
		cl.def("assign", (class HepMC3::GenCrossSection & (HepMC3::GenCrossSection::*)(const class HepMC3::GenCrossSection &)) &HepMC3::GenCrossSection::operator=, "C++: HepMC3::GenCrossSection::operator=(const class HepMC3::GenCrossSection &) --> class HepMC3::GenCrossSection &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::Units file: line:26
		pybind11::class_<HepMC3::Units, std::shared_ptr<HepMC3::Units>> cl(M("HepMC3"), "Units", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::Units(); } ) );

		pybind11::enum_<HepMC3::Units::MomentumUnit>(cl, "MomentumUnit", pybind11::arithmetic(), "Momentum units ")
			.value("MEV", HepMC3::Units::MomentumUnit::MEV)
			.value("GEV", HepMC3::Units::MomentumUnit::GEV)
			.export_values();


		pybind11::enum_<HepMC3::Units::LengthUnit>(cl, "LengthUnit", pybind11::arithmetic(), "Position units ")
			.value("MM", HepMC3::Units::LengthUnit::MM)
			.value("CM", HepMC3::Units::LengthUnit::CM)
			.export_values();

		cl.def_static("momentum_unit", (enum HepMC3::Units::MomentumUnit (*)(const std::string &)) &HepMC3::Units::momentum_unit, "Get momentum unit based on its name\n\nC++: HepMC3::Units::momentum_unit(const std::string &) --> enum HepMC3::Units::MomentumUnit", pybind11::arg("name"));
		cl.def_static("length_unit", (enum HepMC3::Units::LengthUnit (*)(const std::string &)) &HepMC3::Units::length_unit, "Get length unit based on its name\n\nC++: HepMC3::Units::length_unit(const std::string &) --> enum HepMC3::Units::LengthUnit", pybind11::arg("name"));
		cl.def_static("name", (std::string (*)(enum HepMC3::Units::MomentumUnit)) &HepMC3::Units::name, "Get name of momentum unit \n\nC++: HepMC3::Units::name(enum HepMC3::Units::MomentumUnit) --> std::string", pybind11::arg("u"));
		cl.def_static("name", (std::string (*)(enum HepMC3::Units::LengthUnit)) &HepMC3::Units::name, "Get name of length unit \n\nC++: HepMC3::Units::name(enum HepMC3::Units::LengthUnit) --> std::string", pybind11::arg("u"));

		 binder::custom_Units_binder(cl);
	}
}


// File: HepMC3/GenRunInfo.cpp
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::GenPdfInfo file: line:32
struct PyCallBack_HepMC3_GenPdfInfo : public HepMC3::GenPdfInfo {
	using HepMC3::GenPdfInfo::GenPdfInfo;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenPdfInfo *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenPdfInfo::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenPdfInfo *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return GenPdfInfo::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenPdfInfo *>(this), "init");
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::GenPdfInfo *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

void bind_HepMC3_GenRunInfo(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::GenRunInfo file:HepMC3/GenRunInfo.h line:32
		pybind11::class_<HepMC3::GenRunInfo, std::shared_ptr<HepMC3::GenRunInfo>> cl(M("HepMC3"), "GenRunInfo", "Stores run-related information\n\n Manages run-related information.\n Contains run-wide attributes");
		pybind11::handle cl_type = cl;

		{ // HepMC3::GenRunInfo::ToolInfo file:HepMC3/GenRunInfo.h line:37
			auto & enclosing_class = cl;
			pybind11::class_<HepMC3::GenRunInfo::ToolInfo, std::shared_ptr<HepMC3::GenRunInfo::ToolInfo>> cl(enclosing_class, "ToolInfo", "Interrnal struct for keeping track of tools.");
			pybind11::handle cl_type = cl;

			cl.def( pybind11::init( [](HepMC3::GenRunInfo::ToolInfo const &o){ return new HepMC3::GenRunInfo::ToolInfo(o); } ) );
			cl.def( pybind11::init( [](){ return new HepMC3::GenRunInfo::ToolInfo(); } ) );
			cl.def_readwrite("name", &HepMC3::GenRunInfo::ToolInfo::name);
			cl.def_readwrite("version", &HepMC3::GenRunInfo::ToolInfo::version);
			cl.def_readwrite("description", &HepMC3::GenRunInfo::ToolInfo::description);
			cl.def("assign", (struct HepMC3::GenRunInfo::ToolInfo & (HepMC3::GenRunInfo::ToolInfo::*)(const struct HepMC3::GenRunInfo::ToolInfo &)) &HepMC3::GenRunInfo::ToolInfo::operator=, "C++: HepMC3::GenRunInfo::ToolInfo::operator=(const struct HepMC3::GenRunInfo::ToolInfo &) --> struct HepMC3::GenRunInfo::ToolInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));
		}

		cl.def( pybind11::init( [](){ return new HepMC3::GenRunInfo(); } ) );
		cl.def( pybind11::init( [](HepMC3::GenRunInfo const &o){ return new HepMC3::GenRunInfo(o); } ) );
		cl.def("attribute", (class std::shared_ptr<class HepMC3::GenHeavyIon> (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::attribute<HepMC3::GenHeavyIon>, "C++: HepMC3::GenRunInfo::attribute(const std::string &) const --> class std::shared_ptr<class HepMC3::GenHeavyIon>", pybind11::arg("name"));
		cl.def("attribute", (class std::shared_ptr<class HepMC3::GenPdfInfo> (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::attribute<HepMC3::GenPdfInfo>, "C++: HepMC3::GenRunInfo::attribute(const std::string &) const --> class std::shared_ptr<class HepMC3::GenPdfInfo>", pybind11::arg("name"));
		cl.def("attribute", (class std::shared_ptr<class HepMC3::GenCrossSection> (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::attribute<HepMC3::GenCrossSection>, "C++: HepMC3::GenRunInfo::attribute(const std::string &) const --> class std::shared_ptr<class HepMC3::GenCrossSection>", pybind11::arg("name"));
		cl.def("assign", (class HepMC3::GenRunInfo & (HepMC3::GenRunInfo::*)(const class HepMC3::GenRunInfo &)) &HepMC3::GenRunInfo::operator=, "Assignmet\n\nC++: HepMC3::GenRunInfo::operator=(const class HepMC3::GenRunInfo &) --> class HepMC3::GenRunInfo &", pybind11::return_value_policy::automatic, pybind11::arg("r"));
		cl.def("has_weight", (bool (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::has_weight, "Check if a weight name is present.\n\nC++: HepMC3::GenRunInfo::has_weight(const std::string &) const --> bool", pybind11::arg("name"));
		cl.def("weight_index", (int (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::weight_index, "Return the index corresponding to a weight name.\n \n\n -1 if name was not found\n\nC++: HepMC3::GenRunInfo::weight_index(const std::string &) const --> int", pybind11::arg("name"));
		cl.def("weight_names", (const class std::vector<std::string, class std::allocator<std::string > > & (HepMC3::GenRunInfo::*)() const) &HepMC3::GenRunInfo::weight_names, "Get the vector of weight names.\n\nC++: HepMC3::GenRunInfo::weight_names() const --> const class std::vector<std::string, class std::allocator<std::string > > &", pybind11::return_value_policy::automatic);
		cl.def("set_weight_names", (void (HepMC3::GenRunInfo::*)(const class std::vector<std::string, class std::allocator<std::string > > &)) &HepMC3::GenRunInfo::set_weight_names, "Set the names of the weights in this run.\n\n For consistency, the length of the vector should be the same as\n the number of weights in the events in the run.\n\nC++: HepMC3::GenRunInfo::set_weight_names(const class std::vector<std::string, class std::allocator<std::string > > &) --> void", pybind11::arg("names"));
		cl.def("add_attribute", (void (HepMC3::GenRunInfo::*)(const std::string &, const class std::shared_ptr<class HepMC3::Attribute> &)) &HepMC3::GenRunInfo::add_attribute, "add an attribute\n This will overwrite existing attribute if an attribute\n with the same name is present\n\nC++: HepMC3::GenRunInfo::add_attribute(const std::string &, const class std::shared_ptr<class HepMC3::Attribute> &) --> void", pybind11::arg("name"), pybind11::arg("att"));
		cl.def("remove_attribute", (void (HepMC3::GenRunInfo::*)(const std::string &)) &HepMC3::GenRunInfo::remove_attribute, "Remove attribute\n\nC++: HepMC3::GenRunInfo::remove_attribute(const std::string &) --> void", pybind11::arg("name"));
		cl.def("attribute_as_string", (std::string (HepMC3::GenRunInfo::*)(const std::string &) const) &HepMC3::GenRunInfo::attribute_as_string, "Get attribute of any type as string\n\nC++: HepMC3::GenRunInfo::attribute_as_string(const std::string &) const --> std::string", pybind11::arg("name"));
		cl.def("attribute_names", (class std::vector<std::string, class std::allocator<std::string > > (HepMC3::GenRunInfo::*)() const) &HepMC3::GenRunInfo::attribute_names, "Get list of attribute names\n\nC++: HepMC3::GenRunInfo::attribute_names() const --> class std::vector<std::string, class std::allocator<std::string > >");
		cl.def("attributes", (class std::map<std::string, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, class std::shared_ptr<class HepMC3::Attribute> > > > (HepMC3::GenRunInfo::*)() const) &HepMC3::GenRunInfo::attributes, "Get a copy of the list of attributes\n \n\n To avoid thread issues, this is returns a copy. Better solution may be needed.\n\nC++: HepMC3::GenRunInfo::attributes() const --> class std::map<std::string, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, class std::shared_ptr<class HepMC3::Attribute> > > >");
		cl.def("write_data", (void (HepMC3::GenRunInfo::*)(struct HepMC3::GenRunInfoData &) const) &HepMC3::GenRunInfo::write_data, "Fill GenRunInfoData object\n\nC++: HepMC3::GenRunInfo::write_data(struct HepMC3::GenRunInfoData &) const --> void", pybind11::arg("data"));
		cl.def("read_data", (void (HepMC3::GenRunInfo::*)(const struct HepMC3::GenRunInfoData &)) &HepMC3::GenRunInfo::read_data, "Fill GenRunInfo based on GenRunInfoData\n\nC++: HepMC3::GenRunInfo::read_data(const struct HepMC3::GenRunInfoData &) --> void", pybind11::arg("data"));

		 binder::custom_GenRunInfo_binder(cl);
	}
	{ // HepMC3::GenParticleData file:HepMC3/Data/GenParticleData.h line:24
		pybind11::class_<HepMC3::GenParticleData, std::shared_ptr<HepMC3::GenParticleData>> cl(M("HepMC3"), "GenParticleData", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenParticleData(); } ) );
		cl.def( pybind11::init( [](HepMC3::GenParticleData const &o){ return new HepMC3::GenParticleData(o); } ) );
		cl.def_readwrite("pid", &HepMC3::GenParticleData::pid);
		cl.def_readwrite("status", &HepMC3::GenParticleData::status);
		cl.def_readwrite("is_mass_set", &HepMC3::GenParticleData::is_mass_set);
		cl.def_readwrite("mass", &HepMC3::GenParticleData::mass);
		cl.def_readwrite("momentum", &HepMC3::GenParticleData::momentum);
		cl.def("assign", (struct HepMC3::GenParticleData & (HepMC3::GenParticleData::*)(const struct HepMC3::GenParticleData &)) &HepMC3::GenParticleData::operator=, "C++: HepMC3::GenParticleData::operator=(const struct HepMC3::GenParticleData &) --> struct HepMC3::GenParticleData &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::GenPdfInfo file: line:32
		pybind11::class_<HepMC3::GenPdfInfo, std::shared_ptr<HepMC3::GenPdfInfo>, PyCallBack_HepMC3_GenPdfInfo, HepMC3::Attribute> cl(M("HepMC3"), "GenPdfInfo", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](PyCallBack_HepMC3_GenPdfInfo const &o){ return new PyCallBack_HepMC3_GenPdfInfo(o); } ) );
		cl.def( pybind11::init( [](HepMC3::GenPdfInfo const &o){ return new HepMC3::GenPdfInfo(o); } ) );
		cl.def( pybind11::init( [](){ return new HepMC3::GenPdfInfo(); }, [](){ return new PyCallBack_HepMC3_GenPdfInfo(); } ) );
		cl.def_readwrite("scale", &HepMC3::GenPdfInfo::scale);
		cl.def("from_string", (bool (HepMC3::GenPdfInfo::*)(const std::string &)) &HepMC3::GenPdfInfo::from_string, "Implementation of Attribute::from_string \n\nC++: HepMC3::GenPdfInfo::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::GenPdfInfo::*)(std::string &) const) &HepMC3::GenPdfInfo::to_string, "Implementation of Attribute::to_string \n\nC++: HepMC3::GenPdfInfo::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("set", [](HepMC3::GenPdfInfo &o, const int & a0, const int & a1, const double & a2, const double & a3, const double & a4, const double & a5, const double & a6) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6); }, "", pybind11::arg("parton_id1"), pybind11::arg("parton_id2"), pybind11::arg("x1"), pybind11::arg("x2"), pybind11::arg("scale_in"), pybind11::arg("xf1"), pybind11::arg("xf2"));
		cl.def("set", [](HepMC3::GenPdfInfo &o, const int & a0, const int & a1, const double & a2, const double & a3, const double & a4, const double & a5, const double & a6, const int & a7) -> void { return o.set(a0, a1, a2, a3, a4, a5, a6, a7); }, "", pybind11::arg("parton_id1"), pybind11::arg("parton_id2"), pybind11::arg("x1"), pybind11::arg("x2"), pybind11::arg("scale_in"), pybind11::arg("xf1"), pybind11::arg("xf2"), pybind11::arg("pdf_id1"));
		cl.def("set", (void (HepMC3::GenPdfInfo::*)(const int &, const int &, const double &, const double &, const double &, const double &, const double &, const int &, const int &)) &HepMC3::GenPdfInfo::set, "Set all fields \n\nC++: HepMC3::GenPdfInfo::set(const int &, const int &, const double &, const double &, const double &, const double &, const double &, const int &, const int &) --> void", pybind11::arg("parton_id1"), pybind11::arg("parton_id2"), pybind11::arg("x1"), pybind11::arg("x2"), pybind11::arg("scale_in"), pybind11::arg("xf1"), pybind11::arg("xf2"), pybind11::arg("pdf_id1"), pybind11::arg("pdf_id2"));
		cl.def("__eq__", (bool (HepMC3::GenPdfInfo::*)(const class HepMC3::GenPdfInfo &) const) &HepMC3::GenPdfInfo::operator==, "C++: HepMC3::GenPdfInfo::operator==(const class HepMC3::GenPdfInfo &) const --> bool", pybind11::arg(""));
		cl.def("__ne__", (bool (HepMC3::GenPdfInfo::*)(const class HepMC3::GenPdfInfo &) const) &HepMC3::GenPdfInfo::operator!=, "C++: HepMC3::GenPdfInfo::operator!=(const class HepMC3::GenPdfInfo &) const --> bool", pybind11::arg(""));
		cl.def("is_valid", (bool (HepMC3::GenPdfInfo::*)() const) &HepMC3::GenPdfInfo::is_valid, "C++: HepMC3::GenPdfInfo::is_valid() const --> bool");
		cl.def("assign", (class HepMC3::GenPdfInfo & (HepMC3::GenPdfInfo::*)(const class HepMC3::GenPdfInfo &)) &HepMC3::GenPdfInfo::operator=, "C++: HepMC3::GenPdfInfo::operator=(const class HepMC3::GenPdfInfo &) --> class HepMC3::GenPdfInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::GenEvent file:HepMC3/GenEvent.h line:41
		pybind11::class_<HepMC3::GenEvent, std::shared_ptr<HepMC3::GenEvent>> cl(M("HepMC3"), "GenEvent", "Stores event-related information\n\n Manages event-related information.\n Contains lists of GenParticle and GenVertex objects");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenEvent(); } ), "doc" );
		cl.def( pybind11::init( [](enum HepMC3::Units::MomentumUnit const & a0){ return new HepMC3::GenEvent(a0); } ), "doc" , pybind11::arg("momentum_unit"));
		cl.def( pybind11::init<enum HepMC3::Units::MomentumUnit, enum HepMC3::Units::LengthUnit>(), pybind11::arg("momentum_unit"), pybind11::arg("length_unit") );

		cl.def( pybind11::init( [](class std::shared_ptr<class HepMC3::GenRunInfo> const & a0){ return new HepMC3::GenEvent(a0); } ), "doc" , pybind11::arg("run"));
		cl.def( pybind11::init( [](class std::shared_ptr<class HepMC3::GenRunInfo> const & a0, enum HepMC3::Units::MomentumUnit const & a1){ return new HepMC3::GenEvent(a0, a1); } ), "doc" , pybind11::arg("run"), pybind11::arg("momentum_unit"));
		cl.def( pybind11::init<class std::shared_ptr<class HepMC3::GenRunInfo>, enum HepMC3::Units::MomentumUnit, enum HepMC3::Units::LengthUnit>(), pybind11::arg("run"), pybind11::arg("momentum_unit"), pybind11::arg("length_unit") );

		cl.def( pybind11::init( [](HepMC3::GenEvent const &o){ return new HepMC3::GenEvent(o); } ) );
		cl.def("assign", (class HepMC3::GenEvent & (HepMC3::GenEvent::*)(const class HepMC3::GenEvent &)) &HepMC3::GenEvent::operator=, "Assignment operator\n\nC++: HepMC3::GenEvent::operator=(const class HepMC3::GenEvent &) --> class HepMC3::GenEvent &", pybind11::return_value_policy::automatic, pybind11::arg(""));
		cl.def("particles", (const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > & (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::particles, "Get/set list of particles (non-const)\n\nC++: HepMC3::GenEvent::particles() --> const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &", pybind11::return_value_policy::automatic);
		cl.def("vertices", (const class std::vector<class std::shared_ptr<class HepMC3::GenVertex>, class std::allocator<class std::shared_ptr<class HepMC3::GenVertex> > > & (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::vertices, "Get/set list of vertices (non-const)\n\nC++: HepMC3::GenEvent::vertices() --> const class std::vector<class std::shared_ptr<class HepMC3::GenVertex>, class std::allocator<class std::shared_ptr<class HepMC3::GenVertex> > > &", pybind11::return_value_policy::automatic);
		cl.def("weights", (class std::vector<double, class std::allocator<double> > & (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::weights, "Get event weights as a vector (non-const)\n\nC++: HepMC3::GenEvent::weights() --> class std::vector<double, class std::allocator<double> > &", pybind11::return_value_policy::automatic);
		cl.def("weight", [](HepMC3::GenEvent const &o) -> double { return o.weight(); }, "");
		cl.def("weight", (double (HepMC3::GenEvent::*)(const unsigned long &) const) &HepMC3::GenEvent::weight, "Get event weight accessed by index (or the canonical/first one if there is no argument)\n \n\n It's the user's responsibility to ensure that the given index exists!\n\nC++: HepMC3::GenEvent::weight(const unsigned long &) const --> double", pybind11::arg("index"));
		cl.def("weight", (double & (HepMC3::GenEvent::*)(const std::string &)) &HepMC3::GenEvent::weight, "Get event weight accessed by weight name\n \n\n Requires there to be an attached GenRunInfo, otherwise will throw an exception\n \n\n It's the user's responsibility to ensure that the given name exists!\n\nC++: HepMC3::GenEvent::weight(const std::string &) --> double &", pybind11::return_value_policy::automatic, pybind11::arg("name"));
		cl.def("weight_names", (const class std::vector<std::string, class std::allocator<std::string > > & (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::weight_names, "Get event weight names, if there are some\n \n\n Requires there to be an attached GenRunInfo with registered weight names, otherwise will throw an exception\n\nC++: HepMC3::GenEvent::weight_names() const --> const class std::vector<std::string, class std::allocator<std::string > > &", pybind11::return_value_policy::automatic);
		cl.def("run_info", (class std::shared_ptr<class HepMC3::GenRunInfo> (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::run_info, "Get a pointer to the the GenRunInfo object.\n\nC++: HepMC3::GenEvent::run_info() const --> class std::shared_ptr<class HepMC3::GenRunInfo>");
		cl.def("set_run_info", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenRunInfo>)) &HepMC3::GenEvent::set_run_info, "Set the GenRunInfo object by smart pointer.\n\nC++: HepMC3::GenEvent::set_run_info(class std::shared_ptr<class HepMC3::GenRunInfo>) --> void", pybind11::arg("run"));
		cl.def("event_number", (int (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::event_number, "Get event number\n\nC++: HepMC3::GenEvent::event_number() const --> int");
		cl.def("set_event_number", (void (HepMC3::GenEvent::*)(const int &)) &HepMC3::GenEvent::set_event_number, "Set event number\n\nC++: HepMC3::GenEvent::set_event_number(const int &) --> void", pybind11::arg("num"));
		cl.def("momentum_unit", (const enum HepMC3::Units::MomentumUnit & (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::momentum_unit, "Get momentum unit\n\nC++: HepMC3::GenEvent::momentum_unit() const --> const enum HepMC3::Units::MomentumUnit &", pybind11::return_value_policy::automatic);
		cl.def("length_unit", (const enum HepMC3::Units::LengthUnit & (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::length_unit, "Get length unit\n\nC++: HepMC3::GenEvent::length_unit() const --> const enum HepMC3::Units::LengthUnit &", pybind11::return_value_policy::automatic);
		cl.def("set_units", (void (HepMC3::GenEvent::*)(enum HepMC3::Units::MomentumUnit, enum HepMC3::Units::LengthUnit)) &HepMC3::GenEvent::set_units, "Change event units\n Converts event from current units to new ones\n\nC++: HepMC3::GenEvent::set_units(enum HepMC3::Units::MomentumUnit, enum HepMC3::Units::LengthUnit) --> void", pybind11::arg("new_momentum_unit"), pybind11::arg("new_length_unit"));
		cl.def("heavy_ion", (class std::shared_ptr<class HepMC3::GenHeavyIon> (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::heavy_ion, "Get heavy ion generator additional information\n\nC++: HepMC3::GenEvent::heavy_ion() --> class std::shared_ptr<class HepMC3::GenHeavyIon>");
		cl.def("set_heavy_ion", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenHeavyIon>)) &HepMC3::GenEvent::set_heavy_ion, "Set heavy ion generator additional information\n\nC++: HepMC3::GenEvent::set_heavy_ion(class std::shared_ptr<class HepMC3::GenHeavyIon>) --> void", pybind11::arg("hi"));
		cl.def("pdf_info", (class std::shared_ptr<class HepMC3::GenPdfInfo> (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::pdf_info, "Get PDF information\n\nC++: HepMC3::GenEvent::pdf_info() --> class std::shared_ptr<class HepMC3::GenPdfInfo>");
		cl.def("set_pdf_info", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenPdfInfo>)) &HepMC3::GenEvent::set_pdf_info, "Set PDF information\n\nC++: HepMC3::GenEvent::set_pdf_info(class std::shared_ptr<class HepMC3::GenPdfInfo>) --> void", pybind11::arg("pi"));
		cl.def("cross_section", (class std::shared_ptr<class HepMC3::GenCrossSection> (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::cross_section, "Get cross-section information\n\nC++: HepMC3::GenEvent::cross_section() --> class std::shared_ptr<class HepMC3::GenCrossSection>");
		cl.def("set_cross_section", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenCrossSection>)) &HepMC3::GenEvent::set_cross_section, "Set cross-section information\n\nC++: HepMC3::GenEvent::set_cross_section(class std::shared_ptr<class HepMC3::GenCrossSection>) --> void", pybind11::arg("cs"));
		cl.def("event_pos", (const class HepMC3::FourVector & (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::event_pos, "Vertex representing the overall event position\n\nC++: HepMC3::GenEvent::event_pos() const --> const class HepMC3::FourVector &", pybind11::return_value_policy::automatic);
		cl.def("beams", (const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > & (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::beams, "Vector of beam particles\n\nC++: HepMC3::GenEvent::beams() --> const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &", pybind11::return_value_policy::automatic);
		cl.def("shift_position_by", (void (HepMC3::GenEvent::*)(const class HepMC3::FourVector &)) &HepMC3::GenEvent::shift_position_by, "Shift position of all vertices in the event by \n\nC++: HepMC3::GenEvent::shift_position_by(const class HepMC3::FourVector &) --> void", pybind11::arg("delta"));
		cl.def("shift_position_to", (void (HepMC3::GenEvent::*)(const class HepMC3::FourVector &)) &HepMC3::GenEvent::shift_position_to, "Shift position of all vertices in the event to \n\nC++: HepMC3::GenEvent::shift_position_to(const class HepMC3::FourVector &) --> void", pybind11::arg("newpos"));
		cl.def("boost", (bool (HepMC3::GenEvent::*)(const class HepMC3::FourVector &)) &HepMC3::GenEvent::boost, "Boost event using x,y,z components of  as velocities\n\nC++: HepMC3::GenEvent::boost(const class HepMC3::FourVector &) --> bool", pybind11::arg("v"));
		cl.def("rotate", (bool (HepMC3::GenEvent::*)(const class HepMC3::FourVector &)) &HepMC3::GenEvent::rotate, "Rotate event using x,y,z components of  as rotation angles\n\nC++: HepMC3::GenEvent::rotate(const class HepMC3::FourVector &) --> bool", pybind11::arg("v"));
		cl.def("reflect", (bool (HepMC3::GenEvent::*)(const int)) &HepMC3::GenEvent::reflect, "Change sign of \n\nC++: HepMC3::GenEvent::reflect(const int) --> bool", pybind11::arg("axis"));
		cl.def("add_attribute", [](HepMC3::GenEvent &o, const class std::basic_string<char> & a0, const class std::shared_ptr<class HepMC3::Attribute> & a1) -> void { return o.add_attribute(a0, a1); }, "", pybind11::arg("name"), pybind11::arg("att"));
		cl.def("add_attribute", (void (HepMC3::GenEvent::*)(const std::string &, const class std::shared_ptr<class HepMC3::Attribute> &, const int &)) &HepMC3::GenEvent::add_attribute, "Add event attribute to event\n\n This will overwrite existing attribute if an attribute\n with the same name is present\n\nC++: HepMC3::GenEvent::add_attribute(const std::string &, const class std::shared_ptr<class HepMC3::Attribute> &, const int &) --> void", pybind11::arg("name"), pybind11::arg("att"), pybind11::arg("id"));
		cl.def("remove_attribute", [](HepMC3::GenEvent &o, const class std::basic_string<char> & a0) -> void { return o.remove_attribute(a0); }, "", pybind11::arg("name"));
		cl.def("remove_attribute", (void (HepMC3::GenEvent::*)(const std::string &, const int &)) &HepMC3::GenEvent::remove_attribute, "Remove attribute\n\nC++: HepMC3::GenEvent::remove_attribute(const std::string &, const int &) --> void", pybind11::arg("name"), pybind11::arg("id"));
		cl.def("attribute_as_string", [](HepMC3::GenEvent const &o, const class std::basic_string<char> & a0) -> std::string { return o.attribute_as_string(a0); }, "", pybind11::arg("name"));
		cl.def("attribute_as_string", (std::string (HepMC3::GenEvent::*)(const std::string &, const int &) const) &HepMC3::GenEvent::attribute_as_string, "Get attribute of any type as string\n\nC++: HepMC3::GenEvent::attribute_as_string(const std::string &, const int &) const --> std::string", pybind11::arg("name"), pybind11::arg("id"));
		cl.def("attribute_names", [](HepMC3::GenEvent const &o) -> std::vector<std::string, class std::allocator<std::string > > { return o.attribute_names(); }, "");
		cl.def("attribute_names", (class std::vector<std::string, class std::allocator<std::string > > (HepMC3::GenEvent::*)(const int &) const) &HepMC3::GenEvent::attribute_names, "Get list of attribute names\n\nC++: HepMC3::GenEvent::attribute_names(const int &) const --> class std::vector<std::string, class std::allocator<std::string > >", pybind11::arg("id"));
		cl.def("attributes", (class std::map<std::string, class std::map<int, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<int>, class std::allocator<struct std::pair<const int, class std::shared_ptr<class HepMC3::Attribute> > > >, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, class std::map<int, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<int>, class std::allocator<struct std::pair<const int, class std::shared_ptr<class HepMC3::Attribute> > > > > > > (HepMC3::GenEvent::*)() const) &HepMC3::GenEvent::attributes, "Get a copy of the list of attributes\n \n\n To avoid thread issues, this is returns a copy. Better solution may be needed.\n\nC++: HepMC3::GenEvent::attributes() const --> class std::map<std::string, class std::map<int, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<int>, class std::allocator<struct std::pair<const int, class std::shared_ptr<class HepMC3::Attribute> > > >, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, class std::map<int, class std::shared_ptr<class HepMC3::Attribute>, struct std::less<int>, class std::allocator<struct std::pair<const int, class std::shared_ptr<class HepMC3::Attribute> > > > > > >");
		cl.def("add_particle", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenEvent::add_particle, "Add particle\n\nC++: HepMC3::GenEvent::add_particle(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p"));
		cl.def("add_vertex", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenVertex>)) &HepMC3::GenEvent::add_vertex, "Add vertex\n\nC++: HepMC3::GenEvent::add_vertex(class std::shared_ptr<class HepMC3::GenVertex>) --> void", pybind11::arg("v"));
		cl.def("remove_particle", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenEvent::remove_particle, "Remove particle from the event\n\n This function  will remove whole sub-tree starting from this particle\n if it is the only incoming particle of this vertex.\n It will also production vertex of this particle if this vertex\n has no more outgoing particles\n\nC++: HepMC3::GenEvent::remove_particle(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("v"));
		cl.def("remove_particles", (void (HepMC3::GenEvent::*)(class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > >)) &HepMC3::GenEvent::remove_particles, "Remove a set of particles\n\n This function follows rules of GenEvent::remove_particle to remove\n a list of particles from the event.\n\nC++: HepMC3::GenEvent::remove_particles(class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > >) --> void", pybind11::arg("v"));
		cl.def("remove_vertex", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenVertex>)) &HepMC3::GenEvent::remove_vertex, "Remove vertex from the event\n\n This will remove all sub-trees of all outgoing particles of this vertex\n\nC++: HepMC3::GenEvent::remove_vertex(class std::shared_ptr<class HepMC3::GenVertex>) --> void", pybind11::arg("v"));
		cl.def("add_tree", (void (HepMC3::GenEvent::*)(const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &)) &HepMC3::GenEvent::add_tree, "Add whole tree in topological order\n\n This function will find the beam particles (particles\n that have no production vertices or their production vertices\n have no particles) and will add the whole decay tree starting from\n these particles.\n\n \n Any particles on this list that do not belong to the tree\n       will be ignored.\n\nC++: HepMC3::GenEvent::add_tree(const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &) --> void", pybind11::arg("particles"));
		cl.def("reserve", [](HepMC3::GenEvent &o, const unsigned long & a0) -> void { return o.reserve(a0); }, "", pybind11::arg("particles"));
		cl.def("reserve", (void (HepMC3::GenEvent::*)(const unsigned long &, const unsigned long &)) &HepMC3::GenEvent::reserve, "Reserve memory for particles and vertices\n\n Helps optimize event creation when size of the event is known beforehand\n\nC++: HepMC3::GenEvent::reserve(const unsigned long &, const unsigned long &) --> void", pybind11::arg("particles"), pybind11::arg("vertices"));
		cl.def("clear", (void (HepMC3::GenEvent::*)()) &HepMC3::GenEvent::clear, "Remove contents of this event\n\nC++: HepMC3::GenEvent::clear() --> void");
		cl.def("add_particle", (void (HepMC3::GenEvent::*)(class HepMC3::GenParticle *)) &HepMC3::GenEvent::add_particle, "Add particle by raw pointer\n \n\n Use GenEvent::add_particle( const GenParticlePtr& ) instead\n\nC++: HepMC3::GenEvent::add_particle(class HepMC3::GenParticle *) --> void", pybind11::arg("p"));
		cl.def("add_vertex", (void (HepMC3::GenEvent::*)(class HepMC3::GenVertex *)) &HepMC3::GenEvent::add_vertex, "Add vertex by raw pointer\n \n\n Use GenEvent::add_vertex( const GenVertexPtr& ) instead\n\nC++: HepMC3::GenEvent::add_vertex(class HepMC3::GenVertex *) --> void", pybind11::arg("v"));
		cl.def("set_beam_particles", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenParticle>, class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenEvent::set_beam_particles, "Set incoming beam particles\n \n\n Backward compatibility\n\nC++: HepMC3::GenEvent::set_beam_particles(class std::shared_ptr<class HepMC3::GenParticle>, class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p1"), pybind11::arg("p2"));
		cl.def("add_beam_particle", (void (HepMC3::GenEvent::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenEvent::add_beam_particle, "Add  particle to root vertex\n\nC++: HepMC3::GenEvent::add_beam_particle(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p1"));
		cl.def("write_data", (void (HepMC3::GenEvent::*)(struct HepMC3::GenEventData &) const) &HepMC3::GenEvent::write_data, "Fill GenEventData object\n\nC++: HepMC3::GenEvent::write_data(struct HepMC3::GenEventData &) const --> void", pybind11::arg("data"));
		cl.def("read_data", (void (HepMC3::GenEvent::*)(const struct HepMC3::GenEventData &)) &HepMC3::GenEvent::read_data, "Fill GenEvent based on GenEventData\n\nC++: HepMC3::GenEvent::read_data(const struct HepMC3::GenEventData &) --> void", pybind11::arg("data"));

		 binder::custom_GenEvent_binder(cl);
	}
}


// File: HepMC3/GenParticle.cpp
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_GenParticle(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::GenParticle file:HepMC3/GenParticle.h line:31
		pybind11::class_<HepMC3::GenParticle, std::shared_ptr<HepMC3::GenParticle>> cl(M("HepMC3"), "GenParticle", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenParticle(); } ), "doc" );
		cl.def( pybind11::init( [](const class HepMC3::FourVector & a0){ return new HepMC3::GenParticle(a0); } ), "doc" , pybind11::arg("momentum"));
		cl.def( pybind11::init( [](const class HepMC3::FourVector & a0, int const & a1){ return new HepMC3::GenParticle(a0, a1); } ), "doc" , pybind11::arg("momentum"), pybind11::arg("pid"));
		cl.def( pybind11::init<const class HepMC3::FourVector &, int, int>(), pybind11::arg("momentum"), pybind11::arg("pid"), pybind11::arg("status") );

		cl.def( pybind11::init<const struct HepMC3::GenParticleData &>(), pybind11::arg("data") );

		cl.def( pybind11::init( [](HepMC3::GenParticle const &o){ return new HepMC3::GenParticle(o); } ) );
		cl.def("in_event", (bool (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::in_event, "Check if this particle belongs to an event \n\nC++: HepMC3::GenParticle::in_event() const --> bool");
		cl.def("parent_event", (class HepMC3::GenEvent * (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::parent_event, "C++: HepMC3::GenParticle::parent_event() --> class HepMC3::GenEvent *", pybind11::return_value_policy::automatic);
		cl.def("id", (int (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::id, "C++: HepMC3::GenParticle::id() const --> int");
		cl.def("data", (const struct HepMC3::GenParticleData & (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::data, "C++: HepMC3::GenParticle::data() const --> const struct HepMC3::GenParticleData &", pybind11::return_value_policy::automatic);
		cl.def("production_vertex", (class std::shared_ptr<class HepMC3::GenVertex> (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::production_vertex, "C++: HepMC3::GenParticle::production_vertex() --> class std::shared_ptr<class HepMC3::GenVertex>");
		cl.def("end_vertex", (class std::shared_ptr<class HepMC3::GenVertex> (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::end_vertex, "C++: HepMC3::GenParticle::end_vertex() --> class std::shared_ptr<class HepMC3::GenVertex>");
		cl.def("parents", (class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::parents, "Convenience access to immediate incoming particles via production vertex\n \n\n Less efficient than via the vertex since return must be by value (in case there is no vertex)\n\nC++: HepMC3::GenParticle::parents() --> class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > >");
		cl.def("children", (class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::children, "Convenience access to immediate outgoing particles via end vertex\n \n\n Less efficient than via the vertex since return must be by value (in case there is no vertex)\n\nC++: HepMC3::GenParticle::children() --> class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > >");
		cl.def("pid", (int (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::pid, "C++: HepMC3::GenParticle::pid() const --> int");
		cl.def("abs_pid", (int (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::abs_pid, "C++: HepMC3::GenParticle::abs_pid() const --> int");
		cl.def("status", (int (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::status, "C++: HepMC3::GenParticle::status() const --> int");
		cl.def("momentum", (const class HepMC3::FourVector & (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::momentum, "C++: HepMC3::GenParticle::momentum() const --> const class HepMC3::FourVector &", pybind11::return_value_policy::automatic);
		cl.def("is_generated_mass_set", (bool (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::is_generated_mass_set, "C++: HepMC3::GenParticle::is_generated_mass_set() const --> bool");
		cl.def("generated_mass", (double (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::generated_mass, "Get generated mass\n\n This function will return mass as set by a generator/tool.\n If not set, it will return momentum().m()\n\nC++: HepMC3::GenParticle::generated_mass() const --> double");
		cl.def("set_pid", (void (HepMC3::GenParticle::*)(int)) &HepMC3::GenParticle::set_pid, "C++: HepMC3::GenParticle::set_pid(int) --> void", pybind11::arg("pid"));
		cl.def("set_status", (void (HepMC3::GenParticle::*)(int)) &HepMC3::GenParticle::set_status, "C++: HepMC3::GenParticle::set_status(int) --> void", pybind11::arg("status"));
		cl.def("set_momentum", (void (HepMC3::GenParticle::*)(const class HepMC3::FourVector &)) &HepMC3::GenParticle::set_momentum, "C++: HepMC3::GenParticle::set_momentum(const class HepMC3::FourVector &) --> void", pybind11::arg("momentum"));
		cl.def("set_generated_mass", (void (HepMC3::GenParticle::*)(double)) &HepMC3::GenParticle::set_generated_mass, "C++: HepMC3::GenParticle::set_generated_mass(double) --> void", pybind11::arg("m"));
		cl.def("unset_generated_mass", (void (HepMC3::GenParticle::*)()) &HepMC3::GenParticle::unset_generated_mass, "C++: HepMC3::GenParticle::unset_generated_mass() --> void");
		cl.def("add_attribute", (bool (HepMC3::GenParticle::*)(const std::string &, class std::shared_ptr<class HepMC3::Attribute>)) &HepMC3::GenParticle::add_attribute, "Add an attribute to this particle\n\n  This will overwrite existing attribute if an attribute with\n  the same name is present. The attribute will be stored in the\n  parent_event(). \n\n false if there is no parent_event();\n\nC++: HepMC3::GenParticle::add_attribute(const std::string &, class std::shared_ptr<class HepMC3::Attribute>) --> bool", pybind11::arg("name"), pybind11::arg("att"));
		cl.def("attribute_names", (class std::vector<std::string, class std::allocator<std::string > > (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::attribute_names, "Get list of names of attributes assigned to this particle\n\nC++: HepMC3::GenParticle::attribute_names() const --> class std::vector<std::string, class std::allocator<std::string > >");
		cl.def("remove_attribute", (void (HepMC3::GenParticle::*)(const std::string &)) &HepMC3::GenParticle::remove_attribute, "Remove attribute\n\nC++: HepMC3::GenParticle::remove_attribute(const std::string &) --> void", pybind11::arg("name"));
		cl.def("attribute_as_string", (std::string (HepMC3::GenParticle::*)(const std::string &) const) &HepMC3::GenParticle::attribute_as_string, "Get attribute of any type as string\n\nC++: HepMC3::GenParticle::attribute_as_string(const std::string &) const --> std::string", pybind11::arg("name"));
		cl.def("pdg_id", (int (HepMC3::GenParticle::*)() const) &HepMC3::GenParticle::pdg_id, "Get PDG ID\n \n\n Use pid() instead\n\nC++: HepMC3::GenParticle::pdg_id() const --> int");
		cl.def("set_pdg_id", (void (HepMC3::GenParticle::*)(const int &)) &HepMC3::GenParticle::set_pdg_id, "Set PDG ID\n \n\n Use set_pid() instead\n\nC++: HepMC3::GenParticle::set_pdg_id(const int &) --> void", pybind11::arg("pidin"));
		cl.def("assign", (class HepMC3::GenParticle & (HepMC3::GenParticle::*)(const class HepMC3::GenParticle &)) &HepMC3::GenParticle::operator=, "C++: HepMC3::GenParticle::operator=(const class HepMC3::GenParticle &) --> class HepMC3::GenParticle &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::GenVertexData file: line:22
		pybind11::class_<HepMC3::GenVertexData, std::shared_ptr<HepMC3::GenVertexData>> cl(M("HepMC3"), "GenVertexData", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenVertexData(); } ) );
		cl.def( pybind11::init( [](HepMC3::GenVertexData const &o){ return new HepMC3::GenVertexData(o); } ) );
		cl.def_readwrite("status", &HepMC3::GenVertexData::status);
		cl.def_readwrite("position", &HepMC3::GenVertexData::position);
		cl.def("is_zero", (bool (HepMC3::GenVertexData::*)() const) &HepMC3::GenVertexData::is_zero, "Check if this struct fields are zero\n\nC++: HepMC3::GenVertexData::is_zero() const --> bool");
		cl.def("assign", (struct HepMC3::GenVertexData & (HepMC3::GenVertexData::*)(const struct HepMC3::GenVertexData &)) &HepMC3::GenVertexData::operator=, "C++: HepMC3::GenVertexData::operator=(const struct HepMC3::GenVertexData &) --> struct HepMC3::GenVertexData &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::GenVertex file:HepMC3/GenVertex.h line:26
		pybind11::class_<HepMC3::GenVertex, std::shared_ptr<HepMC3::GenVertex>> cl(M("HepMC3"), "GenVertex", "Stores vertex-related information");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenVertex(); } ), "doc" );
		cl.def( pybind11::init<const class HepMC3::FourVector &>(), pybind11::arg("position") );

		cl.def( pybind11::init<const struct HepMC3::GenVertexData &>(), pybind11::arg("data") );

		cl.def( pybind11::init( [](HepMC3::GenVertex const &o){ return new HepMC3::GenVertex(o); } ) );
		cl.def("parent_event", (class HepMC3::GenEvent * (HepMC3::GenVertex::*)()) &HepMC3::GenVertex::parent_event, "Get parent event\n\nC++: HepMC3::GenVertex::parent_event() --> class HepMC3::GenEvent *", pybind11::return_value_policy::automatic);
		cl.def("in_event", (bool (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::in_event, "Check if this vertex belongs to an event\n\nC++: HepMC3::GenVertex::in_event() const --> bool");
		cl.def("id", (int (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::id, "Get the vertex unique identifier\n\n \n This is not the same as id() in HepMC v2, which is now \n\nC++: HepMC3::GenVertex::id() const --> int");
		cl.def("set_id", (void (HepMC3::GenVertex::*)(int)) &HepMC3::GenVertex::set_id, "set the vertex identifier\n\nC++: HepMC3::GenVertex::set_id(int) --> void", pybind11::arg("id"));
		cl.def("status", (int (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::status, "Get vertex status code\n\nC++: HepMC3::GenVertex::status() const --> int");
		cl.def("set_status", (void (HepMC3::GenVertex::*)(int)) &HepMC3::GenVertex::set_status, "Set vertex status code\n\nC++: HepMC3::GenVertex::set_status(int) --> void", pybind11::arg("stat"));
		cl.def("data", (const struct HepMC3::GenVertexData & (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::data, "Get vertex data\n\nC++: HepMC3::GenVertex::data() const --> const struct HepMC3::GenVertexData &", pybind11::return_value_policy::automatic);
		cl.def("add_particle_in", (void (HepMC3::GenVertex::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenVertex::add_particle_in, "Add incoming particle\n\nC++: HepMC3::GenVertex::add_particle_in(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p"));
		cl.def("add_particle_out", (void (HepMC3::GenVertex::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenVertex::add_particle_out, "Add outgoing particle\n\nC++: HepMC3::GenVertex::add_particle_out(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p"));
		cl.def("remove_particle_in", (void (HepMC3::GenVertex::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenVertex::remove_particle_in, "Remove incoming particle\n\nC++: HepMC3::GenVertex::remove_particle_in(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p"));
		cl.def("remove_particle_out", (void (HepMC3::GenVertex::*)(class std::shared_ptr<class HepMC3::GenParticle>)) &HepMC3::GenVertex::remove_particle_out, "Remove outgoing particle\n\nC++: HepMC3::GenVertex::remove_particle_out(class std::shared_ptr<class HepMC3::GenParticle>) --> void", pybind11::arg("p"));
		cl.def("particles_in", (const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > & (HepMC3::GenVertex::*)()) &HepMC3::GenVertex::particles_in, "Get list of incoming particles\n\nC++: HepMC3::GenVertex::particles_in() --> const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &", pybind11::return_value_policy::automatic);
		cl.def("particles_out", (const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > & (HepMC3::GenVertex::*)()) &HepMC3::GenVertex::particles_out, "Get list of outgoing particles\n\nC++: HepMC3::GenVertex::particles_out() --> const class std::vector<class std::shared_ptr<class HepMC3::GenParticle>, class std::allocator<class std::shared_ptr<class HepMC3::GenParticle> > > &", pybind11::return_value_policy::automatic);
		cl.def("position", (const class HepMC3::FourVector & (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::position, "Get vertex position\n\n Returns the position of this vertex. If a position is not set on _this_ vertex,\n the production vertices of ancestors are searched to find the inherited position.\n FourVector(0,0,0,0) is returned if no position information is found.\n\nC++: HepMC3::GenVertex::position() const --> const class HepMC3::FourVector &", pybind11::return_value_policy::automatic);
		cl.def("has_set_position", (bool (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::has_set_position, "Check if position of this vertex is set\n\nC++: HepMC3::GenVertex::has_set_position() const --> bool");
		cl.def("set_position", (void (HepMC3::GenVertex::*)(const class HepMC3::FourVector &)) &HepMC3::GenVertex::set_position, "Set vertex position\n\nC++: HepMC3::GenVertex::set_position(const class HepMC3::FourVector &) --> void", pybind11::arg("new_pos"));
		cl.def("add_attribute", (bool (HepMC3::GenVertex::*)(const std::string &, class std::shared_ptr<class HepMC3::Attribute>)) &HepMC3::GenVertex::add_attribute, "Add event attribute to this vertex\n\n This will overwrite existing attribute if an attribute with\n the same name is present. The attribute will be stored in the\n parent_event(). \n\n false if there is no parent_event();\n\nC++: HepMC3::GenVertex::add_attribute(const std::string &, class std::shared_ptr<class HepMC3::Attribute>) --> bool", pybind11::arg("name"), pybind11::arg("att"));
		cl.def("attribute_names", (class std::vector<std::string, class std::allocator<std::string > > (HepMC3::GenVertex::*)() const) &HepMC3::GenVertex::attribute_names, "Get list of names of attributes assigned to this particle\n\nC++: HepMC3::GenVertex::attribute_names() const --> class std::vector<std::string, class std::allocator<std::string > >");
		cl.def("remove_attribute", (void (HepMC3::GenVertex::*)(const std::string &)) &HepMC3::GenVertex::remove_attribute, "Remove attribute\n\nC++: HepMC3::GenVertex::remove_attribute(const std::string &) --> void", pybind11::arg("name"));
		cl.def("attribute_as_string", (std::string (HepMC3::GenVertex::*)(const std::string &) const) &HepMC3::GenVertex::attribute_as_string, "Get attribute of any type as string\n\nC++: HepMC3::GenVertex::attribute_as_string(const std::string &) const --> std::string", pybind11::arg("name"));
		cl.def("add_particle_in", (void (HepMC3::GenVertex::*)(class HepMC3::GenParticle *)) &HepMC3::GenVertex::add_particle_in, "Add incoming particle by raw pointer\n \n\n Use GenVertex::add_particle_in( const GenParticlePtr &p ) instead\n\nC++: HepMC3::GenVertex::add_particle_in(class HepMC3::GenParticle *) --> void", pybind11::arg("p"));
		cl.def("add_particle_out", (void (HepMC3::GenVertex::*)(class HepMC3::GenParticle *)) &HepMC3::GenVertex::add_particle_out, "Add outgoing particle by raw pointer\n \n\n Use GenVertex::add_particle_out( const GenParticlePtr &p ) instead\n\nC++: HepMC3::GenVertex::add_particle_out(class HepMC3::GenParticle *) --> void", pybind11::arg("p"));
		cl.def("assign", (class HepMC3::GenVertex & (HepMC3::GenVertex::*)(const class HepMC3::GenVertex &)) &HepMC3::GenVertex::operator=, "C++: HepMC3::GenVertex::operator=(const class HepMC3::GenVertex &) --> class HepMC3::GenVertex &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::GenRunInfoData file:HepMC3/Data/GenRunInfoData.h line:23
		pybind11::class_<HepMC3::GenRunInfoData, std::shared_ptr<HepMC3::GenRunInfoData>> cl(M("HepMC3"), "GenRunInfoData", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenRunInfoData(); } ) );
		cl.def_readwrite("weight_names", &HepMC3::GenRunInfoData::weight_names);
		cl.def_readwrite("tool_name", &HepMC3::GenRunInfoData::tool_name);
		cl.def_readwrite("tool_version", &HepMC3::GenRunInfoData::tool_version);
		cl.def_readwrite("tool_description", &HepMC3::GenRunInfoData::tool_description);
		cl.def_readwrite("attribute_name", &HepMC3::GenRunInfoData::attribute_name);
		cl.def_readwrite("attribute_string", &HepMC3::GenRunInfoData::attribute_string);
	}
}


// File: HepMC3/Data/GenEventData.cpp
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
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <functional>
#include <ios>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <sstream> // __str__
#include <streambuf>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::Writer file:HepMC3/Writer.h line:25
struct PyCallBack_HepMC3_Writer : public HepMC3::Writer {
	using HepMC3::Writer::Writer;

	void write_event(const class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Writer *>(this), "write_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Writer::write_event\"");
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Writer *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Writer::failed\"");
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Writer *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Writer::close\"");
	}
};

// HepMC3::Reader file:HepMC3/Reader.h line:25
struct PyCallBack_HepMC3_Reader : public HepMC3::Reader {
	using HepMC3::Reader::Reader;

	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Reader *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Reader::read_event\"");
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Reader *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Reader::failed\"");
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::Reader *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		pybind11::pybind11_fail("Tried to call pure virtual function \"Reader::close\"");
	}
};

void bind_HepMC3_Data_GenEventData(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::GenEventData file:HepMC3/Data/GenEventData.h line:26
		pybind11::class_<HepMC3::GenEventData, std::shared_ptr<HepMC3::GenEventData>> cl(M("HepMC3"), "GenEventData", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::GenEventData(); } ) );
		cl.def_readwrite("event_number", &HepMC3::GenEventData::event_number);
		cl.def_readwrite("momentum_unit", &HepMC3::GenEventData::momentum_unit);
		cl.def_readwrite("length_unit", &HepMC3::GenEventData::length_unit);
		cl.def_readwrite("particles", &HepMC3::GenEventData::particles);
		cl.def_readwrite("vertices", &HepMC3::GenEventData::vertices);
		cl.def_readwrite("weights", &HepMC3::GenEventData::weights);
		cl.def_readwrite("event_pos", &HepMC3::GenEventData::event_pos);
		cl.def_readwrite("links1", &HepMC3::GenEventData::links1);
		cl.def_readwrite("links2", &HepMC3::GenEventData::links2);
		cl.def_readwrite("attribute_id", &HepMC3::GenEventData::attribute_id);
		cl.def_readwrite("attribute_name", &HepMC3::GenEventData::attribute_name);
		cl.def_readwrite("attribute_string", &HepMC3::GenEventData::attribute_string);
	}
	// HepMC3::version() file: line:20
	M("HepMC3").def("version", (std::string (*)()) &HepMC3::version, "Get the HepMC library version string\n\nC++: HepMC3::version() --> std::string");

	// HepMC3::Print file: line:27
	binder::print_binder(M("HepMC3"));

	{ // HepMC3::Writer file:HepMC3/Writer.h line:25
		pybind11::class_<HepMC3::Writer, std::shared_ptr<HepMC3::Writer>, PyCallBack_HepMC3_Writer> cl(M("HepMC3"), "Writer", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new PyCallBack_HepMC3_Writer(); } ) );
		cl.def("write_event", (void (HepMC3::Writer::*)(const class HepMC3::GenEvent &)) &HepMC3::Writer::write_event, "Write event  to output target\n\nC++: HepMC3::Writer::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("failed", (bool (HepMC3::Writer::*)()) &HepMC3::Writer::failed, "Get file and/or stream error state \n\nC++: HepMC3::Writer::failed() --> bool");
		cl.def("close", (void (HepMC3::Writer::*)()) &HepMC3::Writer::close, "Close file and/or stream \n\nC++: HepMC3::Writer::close() --> void");
		cl.def("set_run_info", (void (HepMC3::Writer::*)(class std::shared_ptr<class HepMC3::GenRunInfo>)) &HepMC3::Writer::set_run_info, "Set the global GenRunInfo object.\n\nC++: HepMC3::Writer::set_run_info(class std::shared_ptr<class HepMC3::GenRunInfo>) --> void", pybind11::arg("run"));
		cl.def("run_info", (class std::shared_ptr<class HepMC3::GenRunInfo> (HepMC3::Writer::*)() const) &HepMC3::Writer::run_info, "Get the global GenRunInfo object.\n\nC++: HepMC3::Writer::run_info() const --> class std::shared_ptr<class HepMC3::GenRunInfo>");
	}
	{ // HepMC3::Reader file:HepMC3/Reader.h line:25
		pybind11::class_<HepMC3::Reader, std::shared_ptr<HepMC3::Reader>, PyCallBack_HepMC3_Reader> cl(M("HepMC3"), "Reader", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new PyCallBack_HepMC3_Reader(); } ) );
		cl.def("read_event", (bool (HepMC3::Reader::*)(class HepMC3::GenEvent &)) &HepMC3::Reader::read_event, "Fill next event from input into \n\nC++: HepMC3::Reader::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("failed", (bool (HepMC3::Reader::*)()) &HepMC3::Reader::failed, "Get file and/or stream error state \n\nC++: HepMC3::Reader::failed() --> bool");
		cl.def("close", (void (HepMC3::Reader::*)()) &HepMC3::Reader::close, "Close file and/or stream \n\nC++: HepMC3::Reader::close() --> void");
		cl.def("run_info", (class std::shared_ptr<class HepMC3::GenRunInfo> (HepMC3::Reader::*)() const) &HepMC3::Reader::run_info, "Get the global GenRunInfo object.\n\nC++: HepMC3::Reader::run_info() const --> class std::shared_ptr<class HepMC3::GenRunInfo>");
	}
}


// File: HepMC3/WriterAscii.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenEventData.h>
#include <HepMC3/Data/GenParticleData.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/WriterAscii.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::WriterAscii file:HepMC3/WriterAscii.h line:25
struct PyCallBack_HepMC3_WriterAscii : public HepMC3::WriterAscii {
	using HepMC3::WriterAscii::WriterAscii;

	void write_event(const class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAscii *>(this), "write_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterAscii::write_event(a0);
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAscii *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterAscii::failed();
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAscii *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterAscii::close();
	}
};

void bind_HepMC3_WriterAscii(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::WriterAscii file:HepMC3/WriterAscii.h line:25
		pybind11::class_<HepMC3::WriterAscii, std::shared_ptr<HepMC3::WriterAscii>, PyCallBack_HepMC3_WriterAscii, HepMC3::Writer> cl(M("HepMC3"), "WriterAscii", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0){ return new HepMC3::WriterAscii(a0); }, [](const class std::basic_string<char> & a0){ return new PyCallBack_HepMC3_WriterAscii(a0); } ), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterAscii::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterAscii::write_event, "Write event to file\n\n \n Event to be serialized\n\nC++: HepMC3::WriterAscii::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterAscii::*)()) &HepMC3::WriterAscii::write_run_info, "Write the GenRunInfo object to file.\n\nC++: HepMC3::WriterAscii::write_run_info() --> void");
		cl.def("failed", (bool (HepMC3::WriterAscii::*)()) &HepMC3::WriterAscii::failed, "Return status of the stream\n\nC++: HepMC3::WriterAscii::failed() --> bool");
		cl.def("close", (void (HepMC3::WriterAscii::*)()) &HepMC3::WriterAscii::close, "Close file stream\n\nC++: HepMC3::WriterAscii::close() --> void");
		cl.def("set_precision", (void (HepMC3::WriterAscii::*)(const int &)) &HepMC3::WriterAscii::set_precision, "Set output precision\n\n So far available range is [2,24]. Default is 16.\n\nC++: HepMC3::WriterAscii::set_precision(const int &) --> void", pybind11::arg("prec"));
		cl.def("precision", (int (HepMC3::WriterAscii::*)() const) &HepMC3::WriterAscii::precision, "Return output precision\n\nC++: HepMC3::WriterAscii::precision() const --> int");
	}
}


// File: HepMC3/ReaderAscii.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenEventData.h>
#include <HepMC3/Data/GenParticleData.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/ReaderAscii.h>
#include <HepMC3/ReaderAsciiHepMC2.h>
#include <HepMC3/ReaderHEPEVT.h>
#include <HepMC3/WriterAsciiHepMC2.h>
#include <HepMC3/WriterHEPEVT.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::ReaderAscii file:HepMC3/ReaderAscii.h line:29
struct PyCallBack_HepMC3_ReaderAscii : public HepMC3::ReaderAscii {
	using HepMC3::ReaderAscii::ReaderAscii;

	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAscii *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderAscii::read_event(a0);
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAscii *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderAscii::failed();
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAscii *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderAscii::close();
	}
};

// HepMC3::WriterAsciiHepMC2 file:HepMC3/WriterAsciiHepMC2.h line:26
struct PyCallBack_HepMC3_WriterAsciiHepMC2 : public HepMC3::WriterAsciiHepMC2 {
	using HepMC3::WriterAsciiHepMC2::WriterAsciiHepMC2;

	void write_event(const class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAsciiHepMC2 *>(this), "write_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterAsciiHepMC2::write_event(a0);
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAsciiHepMC2 *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterAsciiHepMC2::failed();
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterAsciiHepMC2 *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterAsciiHepMC2::close();
	}
};

// HepMC3::ReaderAsciiHepMC2 file:HepMC3/ReaderAsciiHepMC2.h line:30
struct PyCallBack_HepMC3_ReaderAsciiHepMC2 : public HepMC3::ReaderAsciiHepMC2 {
	using HepMC3::ReaderAsciiHepMC2::ReaderAsciiHepMC2;

	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAsciiHepMC2 *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderAsciiHepMC2::read_event(a0);
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAsciiHepMC2 *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderAsciiHepMC2::failed();
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderAsciiHepMC2 *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderAsciiHepMC2::close();
	}
};

// HepMC3::WriterHEPEVT file:HepMC3/WriterHEPEVT.h line:27
struct PyCallBack_HepMC3_WriterHEPEVT : public HepMC3::WriterHEPEVT {
	using HepMC3::WriterHEPEVT::WriterHEPEVT;

	void write_hepevt_particle(int a0, bool a1) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterHEPEVT *>(this), "write_hepevt_particle");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0, a1);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
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
				static pybind11::detail::overload_caster_t<void> caster;
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
				static pybind11::detail::overload_caster_t<void> caster;
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
				static pybind11::detail::overload_caster_t<void> caster;
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
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterHEPEVT::failed();
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
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::read_hepevt_event_header();
	}
	bool read_hepevt_particle(int a0, bool a1) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "read_hepevt_particle");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0, a1);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::read_hepevt_particle(a0, a1);
	}
	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderHEPEVT *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
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
				static pybind11::detail::overload_caster_t<void> caster;
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
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderHEPEVT::failed();
	}
};

void bind_HepMC3_ReaderAscii(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::ReaderAscii file:HepMC3/ReaderAscii.h line:29
		pybind11::class_<HepMC3::ReaderAscii, std::shared_ptr<HepMC3::ReaderAscii>, PyCallBack_HepMC3_ReaderAscii, HepMC3::Reader> cl(M("HepMC3"), "ReaderAscii", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_event", (bool (HepMC3::ReaderAscii::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderAscii::read_event, "Load event from file\n\n \n Event to be filled\n\nC++: HepMC3::ReaderAscii::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("failed", (bool (HepMC3::ReaderAscii::*)()) &HepMC3::ReaderAscii::failed, "Return status of the stream\n\nC++: HepMC3::ReaderAscii::failed() --> bool");
		cl.def("close", (void (HepMC3::ReaderAscii::*)()) &HepMC3::ReaderAscii::close, "Close file stream\n\nC++: HepMC3::ReaderAscii::close() --> void");
	}
	{ // HepMC3::WriterAsciiHepMC2 file:HepMC3/WriterAsciiHepMC2.h line:26
		pybind11::class_<HepMC3::WriterAsciiHepMC2, std::shared_ptr<HepMC3::WriterAsciiHepMC2>, PyCallBack_HepMC3_WriterAsciiHepMC2, HepMC3::Writer> cl(M("HepMC3"), "WriterAsciiHepMC2", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0){ return new HepMC3::WriterAsciiHepMC2(a0); }, [](const class std::basic_string<char> & a0){ return new PyCallBack_HepMC3_WriterAsciiHepMC2(a0); } ), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterAsciiHepMC2::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterAsciiHepMC2::write_event, "Write event to file\n\n \n Event to be serialized\n\nC++: HepMC3::WriterAsciiHepMC2::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterAsciiHepMC2::*)()) &HepMC3::WriterAsciiHepMC2::write_run_info, "Write the GenRunInfo object to file.\n\nC++: HepMC3::WriterAsciiHepMC2::write_run_info() --> void");
		cl.def("failed", (bool (HepMC3::WriterAsciiHepMC2::*)()) &HepMC3::WriterAsciiHepMC2::failed, "Return status of the stream\n\nC++: HepMC3::WriterAsciiHepMC2::failed() --> bool");
		cl.def("close", (void (HepMC3::WriterAsciiHepMC2::*)()) &HepMC3::WriterAsciiHepMC2::close, "Close file stream\n\nC++: HepMC3::WriterAsciiHepMC2::close() --> void");
		cl.def("set_precision", (void (HepMC3::WriterAsciiHepMC2::*)(const int &)) &HepMC3::WriterAsciiHepMC2::set_precision, "Set output precision\n\n Available range is [2,24]. Default is 16.\n\nC++: HepMC3::WriterAsciiHepMC2::set_precision(const int &) --> void", pybind11::arg("prec"));
		cl.def("precision", (int (HepMC3::WriterAsciiHepMC2::*)() const) &HepMC3::WriterAsciiHepMC2::precision, "Return output precision\n\nC++: HepMC3::WriterAsciiHepMC2::precision() const --> int");
	}
	{ // HepMC3::ReaderAsciiHepMC2 file:HepMC3/ReaderAsciiHepMC2.h line:30
		pybind11::class_<HepMC3::ReaderAsciiHepMC2, std::shared_ptr<HepMC3::ReaderAsciiHepMC2>, PyCallBack_HepMC3_ReaderAsciiHepMC2, HepMC3::Reader> cl(M("HepMC3"), "ReaderAsciiHepMC2", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_event", (bool (HepMC3::ReaderAsciiHepMC2::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderAsciiHepMC2::read_event, "Implementation of Reader::read_event \n\nC++: HepMC3::ReaderAsciiHepMC2::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("failed", (bool (HepMC3::ReaderAsciiHepMC2::*)()) &HepMC3::ReaderAsciiHepMC2::failed, "Return status of the stream\n\nC++: HepMC3::ReaderAsciiHepMC2::failed() --> bool");
		cl.def("close", (void (HepMC3::ReaderAsciiHepMC2::*)()) &HepMC3::ReaderAsciiHepMC2::close, "Close file stream\n\nC++: HepMC3::ReaderAsciiHepMC2::close() --> void");
	}
	{ // HepMC3::WriterHEPEVT file:HepMC3/WriterHEPEVT.h line:27
		pybind11::class_<HepMC3::WriterHEPEVT, std::shared_ptr<HepMC3::WriterHEPEVT>, PyCallBack_HepMC3_WriterHEPEVT, HepMC3::Writer> cl(M("HepMC3"), "WriterHEPEVT", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("write_hepevt_particle", [](HepMC3::WriterHEPEVT &o, int const & a0) -> void { return o.write_hepevt_particle(a0); }, "", pybind11::arg("index"));
		cl.def("write_hepevt_particle", (void (HepMC3::WriterHEPEVT::*)(int, bool)) &HepMC3::WriterHEPEVT::write_hepevt_particle, "Write particle to file\n\n  \n Particle to be serialized\n  \n\n Format of record\n\nC++: HepMC3::WriterHEPEVT::write_hepevt_particle(int, bool) --> void", pybind11::arg("index"), pybind11::arg("iflong"));
		cl.def("write_hepevt_event_header", (void (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::write_hepevt_event_header, "Write event header to file\n\n     \n\nC++: HepMC3::WriterHEPEVT::write_hepevt_event_header() --> void");
		cl.def("write_event", (void (HepMC3::WriterHEPEVT::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterHEPEVT::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterHEPEVT::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::close, "Close file stream \n\nC++: HepMC3::WriterHEPEVT::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterHEPEVT::*)()) &HepMC3::WriterHEPEVT::failed, "Get stream error state flag \n\nC++: HepMC3::WriterHEPEVT::failed() --> bool");
		cl.def("set_vertices_positions_present", (void (HepMC3::WriterHEPEVT::*)(bool)) &HepMC3::WriterHEPEVT::set_vertices_positions_present, "set flag if vertex positions are available \n\nC++: HepMC3::WriterHEPEVT::set_vertices_positions_present(bool) --> void", pybind11::arg("iflong"));
		cl.def("get_vertices_positions_present", (bool (HepMC3::WriterHEPEVT::*)() const) &HepMC3::WriterHEPEVT::get_vertices_positions_present, "get flag if vertex positions are available \n\nC++: HepMC3::WriterHEPEVT::get_vertices_positions_present() const --> bool");
	}
	{ // HepMC3::ReaderHEPEVT file:HepMC3/ReaderHEPEVT.h line:32
		pybind11::class_<HepMC3::ReaderHEPEVT, std::shared_ptr<HepMC3::ReaderHEPEVT>, PyCallBack_HepMC3_ReaderHEPEVT, HepMC3::Reader> cl(M("HepMC3"), "ReaderHEPEVT", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_hepevt_event_header", (bool (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::read_hepevt_event_header, "Find and read event header line  from file\n\n    \n\nC++: HepMC3::ReaderHEPEVT::read_hepevt_event_header() --> bool");
		cl.def("read_hepevt_particle", [](HepMC3::ReaderHEPEVT &o, int const & a0) -> bool { return o.read_hepevt_particle(a0); }, "", pybind11::arg("i"));
		cl.def("read_hepevt_particle", (bool (HepMC3::ReaderHEPEVT::*)(int, bool)) &HepMC3::ReaderHEPEVT::read_hepevt_particle, "read particle from file\n\n \n Particle id\n \n\n Event style\n\nC++: HepMC3::ReaderHEPEVT::read_hepevt_particle(int, bool) --> bool", pybind11::arg("i"), pybind11::arg("iflong"));
		cl.def("read_event", (bool (HepMC3::ReaderHEPEVT::*)(class HepMC3::GenEvent &, bool)) &HepMC3::ReaderHEPEVT::read_event, "Read event from file\n\n  \n Event style\n  \n\n Contains parsed even\n\nC++: HepMC3::ReaderHEPEVT::read_event(class HepMC3::GenEvent &, bool) --> bool", pybind11::arg("evt"), pybind11::arg("iflong"));
		cl.def("read_event", (bool (HepMC3::ReaderHEPEVT::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderHEPEVT::read_event, "Read event from file\n\n  \n Contains parsed even\n\nC++: HepMC3::ReaderHEPEVT::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::close, "Close file stream \n\nC++: HepMC3::ReaderHEPEVT::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderHEPEVT::*)()) &HepMC3::ReaderHEPEVT::failed, "Get stream error state \n\nC++: HepMC3::ReaderHEPEVT::failed() --> bool");
		cl.def("set_vertices_positions_present", (void (HepMC3::ReaderHEPEVT::*)(bool)) &HepMC3::ReaderHEPEVT::set_vertices_positions_present, "set flag if vertex positions are available \n\nC++: HepMC3::ReaderHEPEVT::set_vertices_positions_present(bool) --> void", pybind11::arg("iflong"));
		cl.def("get_vertices_positions_present", (bool (HepMC3::ReaderHEPEVT::*)() const) &HepMC3::ReaderHEPEVT::get_vertices_positions_present, "get flag if vertex positions are available \n\nC++: HepMC3::ReaderHEPEVT::get_vertices_positions_present() const --> bool");
	}
}


// File: HepMC3/HEPEVT_Wrapper.cpp
#include <HepMC3/Attribute.h>
#include <HepMC3/Data/GenEventData.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenHeavyIon.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenRunInfo.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/HEPEVT_Wrapper.h>
#include <functional>
#include <ios>
#include <map>
#include <memory>
#include <ostream>
#include <sstream> // __str__
#include <streambuf>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_HEPEVT_Wrapper(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::HEPEVT_Wrapper file:HepMC3/HEPEVT_Wrapper.h line:80
		pybind11::class_<HepMC3::HEPEVT_Wrapper, std::shared_ptr<HepMC3::HEPEVT_Wrapper>> cl(M("HepMC3"), "HEPEVT_Wrapper", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::HEPEVT_Wrapper(); } ) );
		cl.def_static("zero_everything", (void (*)()) &HepMC3::HEPEVT_Wrapper::zero_everything, "Set all entries in HEPEVT to zero \n\nC++: HepMC3::HEPEVT_Wrapper::zero_everything() --> void");
		cl.def_static("GenEvent_to_HEPEVT", (bool (*)(const class HepMC3::GenEvent *)) &HepMC3::HEPEVT_Wrapper::GenEvent_to_HEPEVT, "Convert GenEvent to HEPEVT\n\nC++: HepMC3::HEPEVT_Wrapper::GenEvent_to_HEPEVT(const class HepMC3::GenEvent *) --> bool", pybind11::arg("evt"));
		cl.def_static("HEPEVT_to_GenEvent", (bool (*)(class HepMC3::GenEvent *)) &HepMC3::HEPEVT_Wrapper::HEPEVT_to_GenEvent, "Convert HEPEVT to GenEvent\n\nC++: HepMC3::HEPEVT_Wrapper::HEPEVT_to_GenEvent(class HepMC3::GenEvent *) --> bool", pybind11::arg("evt"));
		cl.def_static("fix_daughters", (bool (*)()) &HepMC3::HEPEVT_Wrapper::fix_daughters, "Tries to fix list of daughters \n\nC++: HepMC3::HEPEVT_Wrapper::fix_daughters() --> bool");
		cl.def_static("set_hepevt_address", (void (*)(char *)) &HepMC3::HEPEVT_Wrapper::set_hepevt_address, "C++: HepMC3::HEPEVT_Wrapper::set_hepevt_address(char *) --> void", pybind11::arg("c"));
		cl.def_static("max_number_entries", (int (*)()) &HepMC3::HEPEVT_Wrapper::max_number_entries, "C++: HepMC3::HEPEVT_Wrapper::max_number_entries() --> int");
		cl.def_static("event_number", (int (*)()) &HepMC3::HEPEVT_Wrapper::event_number, "C++: HepMC3::HEPEVT_Wrapper::event_number() --> int");
		cl.def_static("number_entries", (int (*)()) &HepMC3::HEPEVT_Wrapper::number_entries, "C++: HepMC3::HEPEVT_Wrapper::number_entries() --> int");
		cl.def_static("status", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::status, "C++: HepMC3::HEPEVT_Wrapper::status(const int &) --> int", pybind11::arg("index"));
		cl.def_static("id", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::id, "C++: HepMC3::HEPEVT_Wrapper::id(const int &) --> int", pybind11::arg("index"));
		cl.def_static("first_parent", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::first_parent, "C++: HepMC3::HEPEVT_Wrapper::first_parent(const int &) --> int", pybind11::arg("index"));
		cl.def_static("last_parent", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::last_parent, "C++: HepMC3::HEPEVT_Wrapper::last_parent(const int &) --> int", pybind11::arg("index"));
		cl.def_static("first_child", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::first_child, "C++: HepMC3::HEPEVT_Wrapper::first_child(const int &) --> int", pybind11::arg("index"));
		cl.def_static("last_child", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::last_child, "C++: HepMC3::HEPEVT_Wrapper::last_child(const int &) --> int", pybind11::arg("index"));
		cl.def_static("px", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::px, "C++: HepMC3::HEPEVT_Wrapper::px(const int &) --> double", pybind11::arg("index"));
		cl.def_static("py", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::py, "C++: HepMC3::HEPEVT_Wrapper::py(const int &) --> double", pybind11::arg("index"));
		cl.def_static("pz", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::pz, "C++: HepMC3::HEPEVT_Wrapper::pz(const int &) --> double", pybind11::arg("index"));
		cl.def_static("e", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::e, "C++: HepMC3::HEPEVT_Wrapper::e(const int &) --> double", pybind11::arg("index"));
		cl.def_static("m", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::m, "C++: HepMC3::HEPEVT_Wrapper::m(const int &) --> double", pybind11::arg("index"));
		cl.def_static("x", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::x, "C++: HepMC3::HEPEVT_Wrapper::x(const int &) --> double", pybind11::arg("index"));
		cl.def_static("y", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::y, "C++: HepMC3::HEPEVT_Wrapper::y(const int &) --> double", pybind11::arg("index"));
		cl.def_static("z", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::z, "C++: HepMC3::HEPEVT_Wrapper::z(const int &) --> double", pybind11::arg("index"));
		cl.def_static("t", (double (*)(const int &)) &HepMC3::HEPEVT_Wrapper::t, "C++: HepMC3::HEPEVT_Wrapper::t(const int &) --> double", pybind11::arg("index"));
		cl.def_static("number_parents", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::number_parents, "C++: HepMC3::HEPEVT_Wrapper::number_parents(const int &) --> int", pybind11::arg("index"));
		cl.def_static("number_children", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::number_children, "C++: HepMC3::HEPEVT_Wrapper::number_children(const int &) --> int", pybind11::arg("index"));
		cl.def_static("number_children_exact", (int (*)(const int &)) &HepMC3::HEPEVT_Wrapper::number_children_exact, "C++: HepMC3::HEPEVT_Wrapper::number_children_exact(const int &) --> int", pybind11::arg("index"));
		cl.def_static("set_event_number", (void (*)(const int &)) &HepMC3::HEPEVT_Wrapper::set_event_number, "C++: HepMC3::HEPEVT_Wrapper::set_event_number(const int &) --> void", pybind11::arg("evtno"));
		cl.def_static("set_number_entries", (void (*)(const int &)) &HepMC3::HEPEVT_Wrapper::set_number_entries, "C++: HepMC3::HEPEVT_Wrapper::set_number_entries(const int &) --> void", pybind11::arg("noentries"));
		cl.def_static("set_status", (void (*)(const int &, const int &)) &HepMC3::HEPEVT_Wrapper::set_status, "C++: HepMC3::HEPEVT_Wrapper::set_status(const int &, const int &) --> void", pybind11::arg("index"), pybind11::arg("status"));
		cl.def_static("set_id", (void (*)(const int &, const int &)) &HepMC3::HEPEVT_Wrapper::set_id, "C++: HepMC3::HEPEVT_Wrapper::set_id(const int &, const int &) --> void", pybind11::arg("index"), pybind11::arg("id"));
		cl.def_static("set_parents", (void (*)(const int &, const int &, const int &)) &HepMC3::HEPEVT_Wrapper::set_parents, "C++: HepMC3::HEPEVT_Wrapper::set_parents(const int &, const int &, const int &) --> void", pybind11::arg("index"), pybind11::arg("firstparent"), pybind11::arg("lastparent"));
		cl.def_static("set_children", (void (*)(const int &, const int &, const int &)) &HepMC3::HEPEVT_Wrapper::set_children, "C++: HepMC3::HEPEVT_Wrapper::set_children(const int &, const int &, const int &) --> void", pybind11::arg("index"), pybind11::arg("firstchild"), pybind11::arg("lastchild"));
		cl.def_static("set_momentum", (void (*)(const int &, const double &, const double &, const double &, const double &)) &HepMC3::HEPEVT_Wrapper::set_momentum, "C++: HepMC3::HEPEVT_Wrapper::set_momentum(const int &, const double &, const double &, const double &, const double &) --> void", pybind11::arg("index"), pybind11::arg("px"), pybind11::arg("py"), pybind11::arg("pz"), pybind11::arg("e"));
		cl.def_static("set_mass", (void (*)(const int &, double)) &HepMC3::HEPEVT_Wrapper::set_mass, "C++: HepMC3::HEPEVT_Wrapper::set_mass(const int &, double) --> void", pybind11::arg("index"), pybind11::arg("mass"));
		cl.def_static("set_position", (void (*)(const int &, const double &, const double &, const double &, const double &)) &HepMC3::HEPEVT_Wrapper::set_position, "C++: HepMC3::HEPEVT_Wrapper::set_position(const int &, const double &, const double &, const double &, const double &) --> void", pybind11::arg("index"), pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("z"), pybind11::arg("t"));

		 binder::custom_HEPEVT_Wrapper_binder(cl);
	}
}


// File: HepMC3/LHEF.cpp
#include <HepMC3/LHEF.h>
#include <functional>
#include <ios>
#include <iterator>
#include <locale>
#include <map>
#include <memory>
#include <ostream>
#include <sstream> // __str__
#include <streambuf>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_LHEF(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // LHEF::OAttr file:HepMC3/LHEF.h line:45
		pybind11::class_<LHEF::OAttr<std::string>, std::shared_ptr<LHEF::OAttr<std::string>>> cl(M("LHEF"), "OAttr_std_string_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string, const std::string &>(), pybind11::arg("n"), pybind11::arg("v") );

		cl.def( pybind11::init( [](LHEF::OAttr<std::string> const &o){ return new LHEF::OAttr<std::string>(o); } ) );
		cl.def_readwrite("name", &LHEF::OAttr<std::string>::name);
		cl.def_readwrite("val", &LHEF::OAttr<std::string>::val);
		cl.def("assign", (struct LHEF::OAttr<std::string > & (LHEF::OAttr<std::string>::*)(const struct LHEF::OAttr<std::string > &)) &LHEF::OAttr<std::string >::operator=, "C++: LHEF::OAttr<std::basic_string<char> >::operator=(const struct LHEF::OAttr<std::string > &) --> struct LHEF::OAttr<std::string > &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		cl.def("__str__", [](LHEF::OAttr<std::string> const &o) -> std::string { std::ostringstream s; s << o; return s.str(); } );
	}
	{ // LHEF::OAttr file:HepMC3/LHEF.h line:45
		pybind11::class_<LHEF::OAttr<long>, std::shared_ptr<LHEF::OAttr<long>>> cl(M("LHEF"), "OAttr_long_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string, const long &>(), pybind11::arg("n"), pybind11::arg("v") );

		cl.def( pybind11::init( [](LHEF::OAttr<long> const &o){ return new LHEF::OAttr<long>(o); } ) );
		cl.def_readwrite("name", &LHEF::OAttr<long>::name);
		cl.def_readwrite("val", &LHEF::OAttr<long>::val);
		cl.def("assign", (struct LHEF::OAttr<long> & (LHEF::OAttr<long>::*)(const struct LHEF::OAttr<long> &)) &LHEF::OAttr<long>::operator=, "C++: LHEF::OAttr<long>::operator=(const struct LHEF::OAttr<long> &) --> struct LHEF::OAttr<long> &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		cl.def("__str__", [](LHEF::OAttr<long> const &o) -> std::string { std::ostringstream s; s << o; return s.str(); } );
	}
	{ // LHEF::OAttr file:HepMC3/LHEF.h line:45
		pybind11::class_<LHEF::OAttr<double>, std::shared_ptr<LHEF::OAttr<double>>> cl(M("LHEF"), "OAttr_double_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string, const double &>(), pybind11::arg("n"), pybind11::arg("v") );

		cl.def( pybind11::init( [](LHEF::OAttr<double> const &o){ return new LHEF::OAttr<double>(o); } ) );
		cl.def_readwrite("name", &LHEF::OAttr<double>::name);
		cl.def_readwrite("val", &LHEF::OAttr<double>::val);
		cl.def("assign", (struct LHEF::OAttr<double> & (LHEF::OAttr<double>::*)(const struct LHEF::OAttr<double> &)) &LHEF::OAttr<double>::operator=, "C++: LHEF::OAttr<double>::operator=(const struct LHEF::OAttr<double> &) --> struct LHEF::OAttr<double> &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		cl.def("__str__", [](LHEF::OAttr<double> const &o) -> std::string { std::ostringstream s; s << o; return s.str(); } );
	}
	{ // LHEF::OAttr file:HepMC3/LHEF.h line:45
		pybind11::class_<LHEF::OAttr<int>, std::shared_ptr<LHEF::OAttr<int>>> cl(M("LHEF"), "OAttr_int_t", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string, const int &>(), pybind11::arg("n"), pybind11::arg("v") );

		cl.def( pybind11::init( [](LHEF::OAttr<int> const &o){ return new LHEF::OAttr<int>(o); } ) );
		cl.def_readwrite("name", &LHEF::OAttr<int>::name);
		cl.def_readwrite("val", &LHEF::OAttr<int>::val);
		cl.def("assign", (struct LHEF::OAttr<int> & (LHEF::OAttr<int>::*)(const struct LHEF::OAttr<int> &)) &LHEF::OAttr<int>::operator=, "C++: LHEF::OAttr<int>::operator=(const struct LHEF::OAttr<int> &) --> struct LHEF::OAttr<int> &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		cl.def("__str__", [](LHEF::OAttr<int> const &o) -> std::string { std::ostringstream s; s << o; return s.str(); } );
	}
	// LHEF::oattr(std::string, const std::string &) file:HepMC3/LHEF.h line:68
	M("LHEF").def("oattr", (struct LHEF::OAttr<std::string > (*)(std::string, const std::string &)) &LHEF::oattr<std::string>, "C++: LHEF::oattr(std::string, const std::string &) --> struct LHEF::OAttr<std::string >", pybind11::arg("name"), pybind11::arg("value"));

	// LHEF::oattr(std::string, const long &) file:HepMC3/LHEF.h line:68
	M("LHEF").def("oattr", (struct LHEF::OAttr<long> (*)(std::string, const long &)) &LHEF::oattr<long>, "C++: LHEF::oattr(std::string, const long &) --> struct LHEF::OAttr<long>", pybind11::arg("name"), pybind11::arg("value"));

	// LHEF::oattr(std::string, const double &) file:HepMC3/LHEF.h line:68
	M("LHEF").def("oattr", (struct LHEF::OAttr<double> (*)(std::string, const double &)) &LHEF::oattr<double>, "C++: LHEF::oattr(std::string, const double &) --> struct LHEF::OAttr<double>", pybind11::arg("name"), pybind11::arg("value"));

	// LHEF::oattr(std::string, const int &) file:HepMC3/LHEF.h line:68
	M("LHEF").def("oattr", (struct LHEF::OAttr<int> (*)(std::string, const int &)) &LHEF::oattr<int>, "C++: LHEF::oattr(std::string, const int &) --> struct LHEF::OAttr<int>", pybind11::arg("name"), pybind11::arg("value"));

	{ // LHEF::XMLTag file:HepMC3/LHEF.h line:87
		pybind11::class_<LHEF::XMLTag, std::shared_ptr<LHEF::XMLTag>> cl(M("LHEF"), "XMLTag", "The XMLTag struct is used to represent all information within an\n XML tag. It contains the attributes as a map, any sub-tags as a\n vector of pointers to other XMLTag objects, and any other\n information as a single string.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::XMLTag(); } ) );
		cl.def( pybind11::init( [](LHEF::XMLTag const &o){ return new LHEF::XMLTag(o); } ) );
		cl.def_readwrite("name", &LHEF::XMLTag::name);
		cl.def_readwrite("attr", &LHEF::XMLTag::attr);
		cl.def_readwrite("tags", &LHEF::XMLTag::tags);
		cl.def_readwrite("contents", &LHEF::XMLTag::contents);
		cl.def("getattr", (bool (LHEF::XMLTag::*)(std::string, double &) const) &LHEF::XMLTag::getattr, "Find an attribute named  and set the double variable  to\n the corresponding value. \n\n false if no attribute was found.\n\nC++: LHEF::XMLTag::getattr(std::string, double &) const --> bool", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::XMLTag::*)(std::string, bool &) const) &LHEF::XMLTag::getattr, "Find an attribute named  and set the bool variable  to\n true if the corresponding value is \"yes\". \n\n false if no\n attribute was found.\n\nC++: LHEF::XMLTag::getattr(std::string, bool &) const --> bool", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::XMLTag::*)(std::string, long &) const) &LHEF::XMLTag::getattr, "Find an attribute named  and set the long variable  to\n the corresponding value. \n\n false if no attribute was found.\n\nC++: LHEF::XMLTag::getattr(std::string, long &) const --> bool", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::XMLTag::*)(std::string, int &) const) &LHEF::XMLTag::getattr, "Find an attribute named  and set the long variable  to\n the corresponding value. \n\n false if no attribute was found.\n\nC++: LHEF::XMLTag::getattr(std::string, int &) const --> bool", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::XMLTag::*)(std::string, std::string &) const) &LHEF::XMLTag::getattr, "Find an attribute named  and set the string variable  to\n the corresponding value. \n\n false if no attribute was found.\n\nC++: LHEF::XMLTag::getattr(std::string, std::string &) const --> bool", pybind11::arg("n"), pybind11::arg("v"));
		cl.def_static("findXMLTags", [](class std::basic_string<char> const & a0) -> std::vector<struct LHEF::XMLTag *, class std::allocator<struct LHEF::XMLTag *> > { return LHEF::XMLTag::findXMLTags(a0); }, "", pybind11::arg("str"));
		cl.def_static("findXMLTags", (class std::vector<struct LHEF::XMLTag *, class std::allocator<struct LHEF::XMLTag *> > (*)(std::string, std::string *)) &LHEF::XMLTag::findXMLTags, "Scan the given string and return all XML tags found as a vector\n of pointers to XMLTag objects. Text which does not belong to any\n tag is stored in tags without name and in the string pointed to\n by leftover (if not null).\n\nC++: LHEF::XMLTag::findXMLTags(std::string, std::string *) --> class std::vector<struct LHEF::XMLTag *, class std::allocator<struct LHEF::XMLTag *> >", pybind11::arg("str"), pybind11::arg("leftover"));
		cl.def_static("deleteAll", (void (*)(class std::vector<struct LHEF::XMLTag *, class std::allocator<struct LHEF::XMLTag *> > &)) &LHEF::XMLTag::deleteAll, "Delete all tags in a vector.\n\nC++: LHEF::XMLTag::deleteAll(class std::vector<struct LHEF::XMLTag *, class std::allocator<struct LHEF::XMLTag *> > &) --> void", pybind11::arg("tags"));
		cl.def("assign", (struct LHEF::XMLTag & (LHEF::XMLTag::*)(const struct LHEF::XMLTag &)) &LHEF::XMLTag::operator=, "C++: LHEF::XMLTag::operator=(const struct LHEF::XMLTag &) --> struct LHEF::XMLTag &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::XMLTag>(cl);
	}
	// LHEF::hashline(std::string) file:HepMC3/LHEF.h line:328
	M("LHEF").def("hashline", (std::string (*)(std::string)) &LHEF::hashline, "Helper function to make sure that each line in the string  starts with a\n #-character and that the string ends with a new-line.\n\nC++: LHEF::hashline(std::string) --> std::string", pybind11::arg("s"));

	{ // LHEF::TagBase file:HepMC3/LHEF.h line:345
		pybind11::class_<LHEF::TagBase, std::shared_ptr<LHEF::TagBase>> cl(M("LHEF"), "TagBase", "This is the base class of all classes representing xml tags.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::TagBase(); } ) );
		cl.def( pybind11::init( [](const class std::map<class std::basic_string<char>, class std::basic_string<char>, struct std::less<class std::basic_string<char> >, class std::allocator<struct std::pair<const class std::basic_string<char>, class std::basic_string<char> > > > & a0){ return new LHEF::TagBase(a0); } ), "doc" , pybind11::arg("attr"));
		cl.def( pybind11::init<const class std::map<std::string, std::string, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, std::string > > > &, std::string>(), pybind11::arg("attr"), pybind11::arg("conts") );

		cl.def( pybind11::init( [](LHEF::TagBase const &o){ return new LHEF::TagBase(o); } ) );
		cl.def_readwrite("attributes", &LHEF::TagBase::attributes);
		cl.def_readwrite("contents", &LHEF::TagBase::contents);
		cl.def("getattr", [](LHEF::TagBase &o, class std::basic_string<char> const & a0, double & a1) -> bool { return o.getattr(a0, a1); }, "", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::TagBase::*)(std::string, double &, bool)) &LHEF::TagBase::getattr, "Find an attribute named  and set the double variable  to\n the corresponding value. Remove the correspondig attribute from\n the list if found and  is true. \n\n false if no\n attribute was found.\n\nC++: LHEF::TagBase::getattr(std::string, double &, bool) --> bool", pybind11::arg("n"), pybind11::arg("v"), pybind11::arg("erase"));
		cl.def("getattr", [](LHEF::TagBase &o, class std::basic_string<char> const & a0, bool & a1) -> bool { return o.getattr(a0, a1); }, "", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::TagBase::*)(std::string, bool &, bool)) &LHEF::TagBase::getattr, "Find an attribute named  and set the bool variable  to\n true if the corresponding value is \"yes\". Remove the correspondig\n attribute from the list if found and  is true. \n\n\n false if no attribute was found.\n\nC++: LHEF::TagBase::getattr(std::string, bool &, bool) --> bool", pybind11::arg("n"), pybind11::arg("v"), pybind11::arg("erase"));
		cl.def("getattr", [](LHEF::TagBase &o, class std::basic_string<char> const & a0, long & a1) -> bool { return o.getattr(a0, a1); }, "", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::TagBase::*)(std::string, long &, bool)) &LHEF::TagBase::getattr, "Find an attribute named  and set the long variable  to\n the corresponding value. Remove the correspondig attribute from\n the list if found and  is true. \n\n false if no\n attribute was found.\n\nC++: LHEF::TagBase::getattr(std::string, long &, bool) --> bool", pybind11::arg("n"), pybind11::arg("v"), pybind11::arg("erase"));
		cl.def("getattr", [](LHEF::TagBase &o, class std::basic_string<char> const & a0, int & a1) -> bool { return o.getattr(a0, a1); }, "", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::TagBase::*)(std::string, int &, bool)) &LHEF::TagBase::getattr, "Find an attribute named  and set the long variable  to\n the corresponding value. Remove the correspondig attribute from\n the list if found and  is true. \n\n false if no\n attribute was found.\n\nC++: LHEF::TagBase::getattr(std::string, int &, bool) --> bool", pybind11::arg("n"), pybind11::arg("v"), pybind11::arg("erase"));
		cl.def("getattr", [](LHEF::TagBase &o, class std::basic_string<char> const & a0, class std::basic_string<char> & a1) -> bool { return o.getattr(a0, a1); }, "", pybind11::arg("n"), pybind11::arg("v"));
		cl.def("getattr", (bool (LHEF::TagBase::*)(std::string, std::string &, bool)) &LHEF::TagBase::getattr, "Find an attribute named  and set the string variable  to\n the corresponding value. Remove the correspondig attribute from\n the list if found and  is true. \n\n false if no\n attribute was found.\n\nC++: LHEF::TagBase::getattr(std::string, std::string &, bool) --> bool", pybind11::arg("n"), pybind11::arg("v"), pybind11::arg("erase"));
		cl.def_static("yes", (std::string (*)()) &LHEF::TagBase::yes, "Static string token for truth values.\n\nC++: LHEF::TagBase::yes() --> std::string");
		cl.def("assign", (struct LHEF::TagBase & (LHEF::TagBase::*)(const struct LHEF::TagBase &)) &LHEF::TagBase::operator=, "C++: LHEF::TagBase::operator=(const struct LHEF::TagBase &) --> struct LHEF::TagBase &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_LHEFTagBase_binder(cl);
	}
	{ // LHEF::Generator file:HepMC3/LHEF.h line:474
		pybind11::class_<LHEF::Generator, std::shared_ptr<LHEF::Generator>, LHEF::TagBase> cl(M("LHEF"), "Generator", "The Generator class contains information about a generator used in a run.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::Generator const &o){ return new LHEF::Generator(o); } ) );
		cl.def_readwrite("name", &LHEF::Generator::name);
		cl.def_readwrite("version", &LHEF::Generator::version);
		cl.def("assign", (struct LHEF::Generator & (LHEF::Generator::*)(const struct LHEF::Generator &)) &LHEF::Generator::operator=, "C++: LHEF::Generator::operator=(const struct LHEF::Generator &) --> struct LHEF::Generator &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		  binder::custom_T_binder<LHEF::Generator>(cl);
	}
	{ // LHEF::XSecInfo file:HepMC3/LHEF.h line:511
		pybind11::class_<LHEF::XSecInfo, std::shared_ptr<LHEF::XSecInfo>, LHEF::TagBase> cl(M("LHEF"), "XSecInfo", "The XSecInfo class contains information given in the xsecinfo tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::XSecInfo(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::XSecInfo const &o){ return new LHEF::XSecInfo(o); } ) );
		cl.def_readwrite("neve", &LHEF::XSecInfo::neve);
		cl.def_readwrite("ntries", &LHEF::XSecInfo::ntries);
		cl.def_readwrite("totxsec", &LHEF::XSecInfo::totxsec);
		cl.def_readwrite("xsecerr", &LHEF::XSecInfo::xsecerr);
		cl.def_readwrite("maxweight", &LHEF::XSecInfo::maxweight);
		cl.def_readwrite("meanweight", &LHEF::XSecInfo::meanweight);
		cl.def_readwrite("negweights", &LHEF::XSecInfo::negweights);
		cl.def_readwrite("varweights", &LHEF::XSecInfo::varweights);
		cl.def_readwrite("weightname", &LHEF::XSecInfo::weightname);
		cl.def("assign", (struct LHEF::XSecInfo & (LHEF::XSecInfo::*)(const struct LHEF::XSecInfo &)) &LHEF::XSecInfo::operator=, "C++: LHEF::XSecInfo::operator=(const struct LHEF::XSecInfo &) --> struct LHEF::XSecInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::XSecInfo>(cl);
	}
	{ // LHEF::EventFile file:HepMC3/LHEF.h line:617
		pybind11::class_<LHEF::EventFile, std::shared_ptr<LHEF::EventFile>, LHEF::TagBase> cl(M("LHEF"), "EventFile", "Simple struct to store information about separate eventfiles to be\n loaded.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::EventFile(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::EventFile const &o){ return new LHEF::EventFile(o); } ) );
		cl.def_readwrite("filename", &LHEF::EventFile::filename);
		cl.def_readwrite("neve", &LHEF::EventFile::neve);
		cl.def_readwrite("ntries", &LHEF::EventFile::ntries);
		cl.def("assign", (struct LHEF::EventFile & (LHEF::EventFile::*)(const struct LHEF::EventFile &)) &LHEF::EventFile::operator=, "C++: LHEF::EventFile::operator=(const struct LHEF::EventFile &) --> struct LHEF::EventFile &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::EventFile>(cl);
	}
}


// File: HepMC3/LHEF_1.cpp
#include <HepMC3/LHEF.h>
#include <functional>
#include <ios>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream> // __str__
#include <streambuf>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_LHEF_1(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // LHEF::Cut file:HepMC3/LHEF.h line:669
		pybind11::class_<LHEF::Cut, std::shared_ptr<LHEF::Cut>, LHEF::TagBase> cl(M("LHEF"), "Cut", "The Cut class represents a cut used by the Matrix Element generator.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::Cut(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &, const class std::map<std::string, class std::set<long, struct std::less<long>, class std::allocator<long> >, struct std::less<std::string >, class std::allocator<struct std::pair<const std::string, class std::set<long, struct std::less<long>, class std::allocator<long> > > > > &>(), pybind11::arg("tag"), pybind11::arg("ptypes") );

		cl.def( pybind11::init( [](LHEF::Cut const &o){ return new LHEF::Cut(o); } ) );
		cl.def_readwrite("type", &LHEF::Cut::type);
		cl.def_readwrite("p1", &LHEF::Cut::p1);
		cl.def_readwrite("np1", &LHEF::Cut::np1);
		cl.def_readwrite("p2", &LHEF::Cut::p2);
		cl.def_readwrite("np2", &LHEF::Cut::np2);
		cl.def_readwrite("min", &LHEF::Cut::min);
		cl.def_readwrite("max", &LHEF::Cut::max);
		cl.def("match", [](LHEF::Cut const &o, long const & a0) -> bool { return o.match(a0); }, "", pybind11::arg("id1"));
		cl.def("match", (bool (LHEF::Cut::*)(long, long) const) &LHEF::Cut::match, "Check if a  matches p1 and  matches p2. Only non-zero\n values are considered.\n\nC++: LHEF::Cut::match(long, long) const --> bool", pybind11::arg("id1"), pybind11::arg("id2"));
		cl.def("passCuts", (bool (LHEF::Cut::*)(const class std::vector<long, class std::allocator<long> > &, const class std::vector<class std::vector<double, class std::allocator<double> >, class std::allocator<class std::vector<double, class std::allocator<double> > > > &) const) &LHEF::Cut::passCuts, "Check if the particles given as a vector of PDG  numbers,\n and a vector of vectors of momentum components,  will pass\n the cut defined in this event.\n\nC++: LHEF::Cut::passCuts(const class std::vector<long, class std::allocator<long> > &, const class std::vector<class std::vector<double, class std::allocator<double> >, class std::allocator<class std::vector<double, class std::allocator<double> > > > &) const --> bool", pybind11::arg("id"), pybind11::arg("p"));
		cl.def_static("eta", (double (*)(const class std::vector<double, class std::allocator<double> > &)) &LHEF::Cut::eta, "Return the pseudorapidity of a particle with momentum \n   \n\nC++: LHEF::Cut::eta(const class std::vector<double, class std::allocator<double> > &) --> double", pybind11::arg("p"));
		cl.def_static("rap", (double (*)(const class std::vector<double, class std::allocator<double> > &)) &LHEF::Cut::rap, "Return the true rapidity of a particle with momentum \n   \n\nC++: LHEF::Cut::rap(const class std::vector<double, class std::allocator<double> > &) --> double", pybind11::arg("p"));
		cl.def_static("deltaR", (double (*)(const class std::vector<double, class std::allocator<double> > &, const class std::vector<double, class std::allocator<double> > &)) &LHEF::Cut::deltaR, "Return the delta-R of a particle pair with momenta  and \n   \n\nC++: LHEF::Cut::deltaR(const class std::vector<double, class std::allocator<double> > &, const class std::vector<double, class std::allocator<double> > &) --> double", pybind11::arg("p1"), pybind11::arg("p2"));
		cl.def("outside", (bool (LHEF::Cut::*)(double) const) &LHEF::Cut::outside, "Return true if the given  is outside limits.\n\nC++: LHEF::Cut::outside(double) const --> bool", pybind11::arg("value"));
		cl.def("assign", (struct LHEF::Cut & (LHEF::Cut::*)(const struct LHEF::Cut &)) &LHEF::Cut::operator=, "C++: LHEF::Cut::operator=(const struct LHEF::Cut &) --> struct LHEF::Cut &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::Cut>(cl);
	}
	{ // LHEF::ProcInfo file:HepMC3/LHEF.h line:915
		pybind11::class_<LHEF::ProcInfo, std::shared_ptr<LHEF::ProcInfo>, LHEF::TagBase> cl(M("LHEF"), "ProcInfo", "The ProcInfo class represents the information in a procinfo tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::ProcInfo(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::ProcInfo const &o){ return new LHEF::ProcInfo(o); } ) );
		cl.def_readwrite("iproc", &LHEF::ProcInfo::iproc);
		cl.def_readwrite("loops", &LHEF::ProcInfo::loops);
		cl.def_readwrite("qcdorder", &LHEF::ProcInfo::qcdorder);
		cl.def_readwrite("eworder", &LHEF::ProcInfo::eworder);
		cl.def_readwrite("fscheme", &LHEF::ProcInfo::fscheme);
		cl.def_readwrite("rscheme", &LHEF::ProcInfo::rscheme);
		cl.def_readwrite("scheme", &LHEF::ProcInfo::scheme);
		cl.def("assign", (struct LHEF::ProcInfo & (LHEF::ProcInfo::*)(const struct LHEF::ProcInfo &)) &LHEF::ProcInfo::operator=, "C++: LHEF::ProcInfo::operator=(const struct LHEF::ProcInfo &) --> struct LHEF::ProcInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::ProcInfo>(cl);
	}
	{ // LHEF::MergeInfo file:HepMC3/LHEF.h line:992
		pybind11::class_<LHEF::MergeInfo, std::shared_ptr<LHEF::MergeInfo>, LHEF::TagBase> cl(M("LHEF"), "MergeInfo", "The MergeInfo class represents the information in a mergeinfo tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::MergeInfo(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::MergeInfo const &o){ return new LHEF::MergeInfo(o); } ) );
		cl.def_readwrite("iproc", &LHEF::MergeInfo::iproc);
		cl.def_readwrite("mergingscale", &LHEF::MergeInfo::mergingscale);
		cl.def_readwrite("maxmult", &LHEF::MergeInfo::maxmult);
		cl.def("assign", (struct LHEF::MergeInfo & (LHEF::MergeInfo::*)(const struct LHEF::MergeInfo &)) &LHEF::MergeInfo::operator=, "C++: LHEF::MergeInfo::operator=(const struct LHEF::MergeInfo &) --> struct LHEF::MergeInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::MergeInfo>(cl);
	}
	{ // LHEF::WeightInfo file:HepMC3/LHEF.h line:1042
		pybind11::class_<LHEF::WeightInfo, std::shared_ptr<LHEF::WeightInfo>, LHEF::TagBase> cl(M("LHEF"), "WeightInfo", "The WeightInfo class encodes the description of a given weight\n present for all events.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::WeightInfo(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::WeightInfo const &o){ return new LHEF::WeightInfo(o); } ) );
		cl.def_readwrite("inGroup", &LHEF::WeightInfo::inGroup);
		cl.def_readwrite("isrwgt", &LHEF::WeightInfo::isrwgt);
		cl.def_readwrite("name", &LHEF::WeightInfo::name);
		cl.def_readwrite("muf", &LHEF::WeightInfo::muf);
		cl.def_readwrite("mur", &LHEF::WeightInfo::mur);
		cl.def_readwrite("pdf", &LHEF::WeightInfo::pdf);
		cl.def_readwrite("pdf2", &LHEF::WeightInfo::pdf2);
		cl.def("assign", (struct LHEF::WeightInfo & (LHEF::WeightInfo::*)(const struct LHEF::WeightInfo &)) &LHEF::WeightInfo::operator=, "C++: LHEF::WeightInfo::operator=(const struct LHEF::WeightInfo &) --> struct LHEF::WeightInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::WeightInfo>(cl);
	}
	{ // LHEF::WeightGroup file:HepMC3/LHEF.h line:1128
		pybind11::class_<LHEF::WeightGroup, std::shared_ptr<LHEF::WeightGroup>, LHEF::TagBase> cl(M("LHEF"), "WeightGroup", "The WeightGroup assigns a group-name to a set of WeightInfo objects.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::WeightGroup(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &, int, class std::vector<struct LHEF::WeightInfo, class std::allocator<struct LHEF::WeightInfo> > &>(), pybind11::arg("tag"), pybind11::arg("groupIndex"), pybind11::arg("wiv") );

		cl.def( pybind11::init( [](LHEF::WeightGroup const &o){ return new LHEF::WeightGroup(o); } ) );
		cl.def_readwrite("type", &LHEF::WeightGroup::type);
		cl.def_readwrite("combine", &LHEF::WeightGroup::combine);
		cl.def("assign", (struct LHEF::WeightGroup & (LHEF::WeightGroup::*)(const struct LHEF::WeightGroup &)) &LHEF::WeightGroup::operator=, "C++: LHEF::WeightGroup::operator=(const struct LHEF::WeightGroup &) --> struct LHEF::WeightGroup &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // LHEF::Weight file:HepMC3/LHEF.h line:1169
		pybind11::class_<LHEF::Weight, std::shared_ptr<LHEF::Weight>, LHEF::TagBase> cl(M("LHEF"), "Weight", "The Weight class represents the information in a weight tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::Weight(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::Weight const &o){ return new LHEF::Weight(o); } ) );
		cl.def_readwrite("name", &LHEF::Weight::name);
		cl.def_readwrite("iswgt", &LHEF::Weight::iswgt);
		cl.def_readwrite("born", &LHEF::Weight::born);
		cl.def_readwrite("sudakov", &LHEF::Weight::sudakov);
		cl.def_readwrite("weights", &LHEF::Weight::weights);
		cl.def_readwrite("indices", &LHEF::Weight::indices);
		cl.def("assign", (struct LHEF::Weight & (LHEF::Weight::*)(const struct LHEF::Weight &)) &LHEF::Weight::operator=, "C++: LHEF::Weight::operator=(const struct LHEF::Weight &) --> struct LHEF::Weight &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::Weight>(cl);
	}
	{ // LHEF::Clus file:HepMC3/LHEF.h line:1250
		pybind11::class_<LHEF::Clus, std::shared_ptr<LHEF::Clus>, LHEF::TagBase> cl(M("LHEF"), "Clus", "The Clus class represents a clustering of two particle entries into\n one as defined in a clustering tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::Clus(); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::Clus const &o){ return new LHEF::Clus(o); } ) );
		cl.def_readwrite("p1", &LHEF::Clus::p1);
		cl.def_readwrite("p2", &LHEF::Clus::p2);
		cl.def_readwrite("p0", &LHEF::Clus::p0);
		cl.def_readwrite("scale", &LHEF::Clus::scale);
		cl.def_readwrite("alphas", &LHEF::Clus::alphas);
		cl.def("assign", (struct LHEF::Clus & (LHEF::Clus::*)(const struct LHEF::Clus &)) &LHEF::Clus::operator=, "C++: LHEF::Clus::operator=(const struct LHEF::Clus &) --> struct LHEF::Clus &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::Clus>(cl);
	}
	{ // LHEF::Scale file:HepMC3/LHEF.h line:1313
		pybind11::class_<LHEF::Scale, std::shared_ptr<LHEF::Scale>, LHEF::TagBase> cl(M("LHEF"), "Scale", "Store special scales from within a scales tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::Scale(); } ), "doc" );
		cl.def( pybind11::init( [](class std::basic_string<char> const & a0){ return new LHEF::Scale(a0); } ), "doc" , pybind11::arg("st"));
		cl.def( pybind11::init( [](class std::basic_string<char> const & a0, int const & a1){ return new LHEF::Scale(a0, a1); } ), "doc" , pybind11::arg("st"), pybind11::arg("emtr"));
		cl.def( pybind11::init<std::string, int, double>(), pybind11::arg("st"), pybind11::arg("emtr"), pybind11::arg("sc") );

		cl.def( pybind11::init<const struct LHEF::XMLTag &>(), pybind11::arg("tag") );

		cl.def( pybind11::init( [](LHEF::Scale const &o){ return new LHEF::Scale(o); } ) );
		cl.def_readwrite("stype", &LHEF::Scale::stype);
		cl.def_readwrite("emitter", &LHEF::Scale::emitter);
		cl.def_readwrite("recoilers", &LHEF::Scale::recoilers);
		cl.def_readwrite("emitted", &LHEF::Scale::emitted);
		cl.def_readwrite("scale", &LHEF::Scale::scale);
		cl.def("assign", (struct LHEF::Scale & (LHEF::Scale::*)(const struct LHEF::Scale &)) &LHEF::Scale::operator=, "C++: LHEF::Scale::operator=(const struct LHEF::Scale &) --> struct LHEF::Scale &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::Scale>(cl);
	}
	{ // LHEF::Scales file:HepMC3/LHEF.h line:1416
		pybind11::class_<LHEF::Scales, std::shared_ptr<LHEF::Scales>, LHEF::TagBase> cl(M("LHEF"), "Scales", "Collect different scales relevant for an event.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::Scales(); } ), "doc" );
		cl.def( pybind11::init( [](double const & a0){ return new LHEF::Scales(a0); } ), "doc" , pybind11::arg("defscale"));
		cl.def( pybind11::init<double, int>(), pybind11::arg("defscale"), pybind11::arg("npart") );

		cl.def( pybind11::init( [](const struct LHEF::XMLTag & a0){ return new LHEF::Scales(a0); } ), "doc" , pybind11::arg("tag"));
		cl.def( pybind11::init( [](const struct LHEF::XMLTag & a0, double const & a1){ return new LHEF::Scales(a0, a1); } ), "doc" , pybind11::arg("tag"), pybind11::arg("defscale"));
		cl.def( pybind11::init<const struct LHEF::XMLTag &, double, int>(), pybind11::arg("tag"), pybind11::arg("defscale"), pybind11::arg("npart") );

		cl.def( pybind11::init( [](LHEF::Scales const &o){ return new LHEF::Scales(o); } ) );
		cl.def_readwrite("muf", &LHEF::Scales::muf);
		cl.def_readwrite("mur", &LHEF::Scales::mur);
		cl.def_readwrite("mups", &LHEF::Scales::mups);
		cl.def_readwrite("SCALUP", &LHEF::Scales::SCALUP);
		cl.def_readwrite("scales", &LHEF::Scales::scales);
		cl.def("hasInfo", (bool (LHEF::Scales::*)() const) &LHEF::Scales::hasInfo, "Check if this object contains useful information besides SCALUP.\n\nC++: LHEF::Scales::hasInfo() const --> bool");
		cl.def("getScale", (double (LHEF::Scales::*)(std::string, int, int, int) const) &LHEF::Scales::getScale, "Return the scale of type st for a given emission of particle type\n pdgem from the emitter with number emr and a recoiler rec. (Note\n that the indices for emr and rec starts at 1 and 0 is interpreted\n as any particle.) First it will check for Scale object with an\n exact match. If not found, it will search for an exact match for\n the emitter and recoiler with an undefined emitted particle. If\n not found, it will look for a match for only emitter and emitted,\n of if not found, a match for only the emitter. Finally a general\n Scale object will be used, or if nothing matches, the mups will\n be returned.\n\nC++: LHEF::Scales::getScale(std::string, int, int, int) const --> double", pybind11::arg("st"), pybind11::arg("pdgem"), pybind11::arg("emr"), pybind11::arg("rec"));
		cl.def("assign", (struct LHEF::Scales & (LHEF::Scales::*)(const struct LHEF::Scales &)) &LHEF::Scales::operator=, "C++: LHEF::Scales::operator=(const struct LHEF::Scales &) --> struct LHEF::Scales &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::Scales>(cl);
	}
	{ // LHEF::PDFInfo file:HepMC3/LHEF.h line:1540
		pybind11::class_<LHEF::PDFInfo, std::shared_ptr<LHEF::PDFInfo>, LHEF::TagBase> cl(M("LHEF"), "PDFInfo", "The PDFInfo class represents the information in a pdfinto tag.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::PDFInfo(); } ), "doc" );
		cl.def( pybind11::init<double>(), pybind11::arg("defscale") );

		cl.def( pybind11::init( [](const struct LHEF::XMLTag & a0){ return new LHEF::PDFInfo(a0); } ), "doc" , pybind11::arg("tag"));
		cl.def( pybind11::init<const struct LHEF::XMLTag &, double>(), pybind11::arg("tag"), pybind11::arg("defscale") );

		cl.def( pybind11::init( [](LHEF::PDFInfo const &o){ return new LHEF::PDFInfo(o); } ) );
		cl.def_readwrite("p1", &LHEF::PDFInfo::p1);
		cl.def_readwrite("p2", &LHEF::PDFInfo::p2);
		cl.def_readwrite("x1", &LHEF::PDFInfo::x1);
		cl.def_readwrite("x2", &LHEF::PDFInfo::x2);
		cl.def_readwrite("xf1", &LHEF::PDFInfo::xf1);
		cl.def_readwrite("xf2", &LHEF::PDFInfo::xf2);
		cl.def_readwrite("scale", &LHEF::PDFInfo::scale);
		cl.def_readwrite("SCALUP", &LHEF::PDFInfo::SCALUP);
		cl.def("assign", (struct LHEF::PDFInfo & (LHEF::PDFInfo::*)(const struct LHEF::PDFInfo &)) &LHEF::PDFInfo::operator=, "C++: LHEF::PDFInfo::operator=(const struct LHEF::PDFInfo &) --> struct LHEF::PDFInfo &", pybind11::return_value_policy::automatic, pybind11::arg(""));

		 binder::custom_T_binder<LHEF::PDFInfo>(cl);
	}
	{ // LHEF::HEPRUP file:HepMC3/LHEF.h line:1627
		pybind11::class_<LHEF::HEPRUP, std::shared_ptr<LHEF::HEPRUP>, LHEF::TagBase> cl(M("LHEF"), "HEPRUP", "The HEPRUP class is a simple container corresponding to the Les Houches\n accord (<A HREF=\"http://arxiv.org/abs/hep-ph/0109068\">hep-ph/0109068</A>)\n common block with the same name. The members are named in the same\n way as in the common block. However, fortran arrays are represented\n by vectors, except for the arrays of length two which are\n represented by pair objects.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::HEPRUP(); } ) );
		cl.def( pybind11::init( [](LHEF::HEPRUP const &o){ return new LHEF::HEPRUP(o); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &, int>(), pybind11::arg("tagin"), pybind11::arg("versin") );

		cl.def_readwrite("IDBMUP", &LHEF::HEPRUP::IDBMUP);
		cl.def_readwrite("EBMUP", &LHEF::HEPRUP::EBMUP);
		cl.def_readwrite("PDFGUP", &LHEF::HEPRUP::PDFGUP);
		cl.def_readwrite("PDFSUP", &LHEF::HEPRUP::PDFSUP);
		cl.def_readwrite("IDWTUP", &LHEF::HEPRUP::IDWTUP);
		cl.def_readwrite("NPRUP", &LHEF::HEPRUP::NPRUP);
		cl.def_readwrite("XSECUP", &LHEF::HEPRUP::XSECUP);
		cl.def_readwrite("XERRUP", &LHEF::HEPRUP::XERRUP);
		cl.def_readwrite("XMAXUP", &LHEF::HEPRUP::XMAXUP);
		cl.def_readwrite("LPRUP", &LHEF::HEPRUP::LPRUP);
		cl.def_readwrite("xsecinfos", &LHEF::HEPRUP::xsecinfos);
		cl.def_readwrite("eventfiles", &LHEF::HEPRUP::eventfiles);
		cl.def_readwrite("cuts", &LHEF::HEPRUP::cuts);
		cl.def_readwrite("ptypes", &LHEF::HEPRUP::ptypes);
		cl.def_readwrite("procinfo", &LHEF::HEPRUP::procinfo);
		cl.def_readwrite("mergeinfo", &LHEF::HEPRUP::mergeinfo);
		cl.def_readwrite("generators", &LHEF::HEPRUP::generators);
		cl.def_readwrite("weightinfo", &LHEF::HEPRUP::weightinfo);
		cl.def_readwrite("weightmap", &LHEF::HEPRUP::weightmap);
		cl.def_readwrite("weightgroup", &LHEF::HEPRUP::weightgroup);
		cl.def_readwrite("junk", &LHEF::HEPRUP::junk);
		cl.def_readwrite("version", &LHEF::HEPRUP::version);
		cl.def_readwrite("dprec", &LHEF::HEPRUP::dprec);
		cl.def("assign", (class LHEF::HEPRUP & (LHEF::HEPRUP::*)(const class LHEF::HEPRUP &)) &LHEF::HEPRUP::operator=, "Assignment operator.\n\nC++: LHEF::HEPRUP::operator=(const class LHEF::HEPRUP &) --> class LHEF::HEPRUP &", pybind11::return_value_policy::automatic, pybind11::arg("x"));
		cl.def("weightNameHepMC", (std::string (LHEF::HEPRUP::*)(int) const) &LHEF::HEPRUP::weightNameHepMC, "Return the name of the weight with given index suitable to ne\n used for HepMC3 output.\n\nC++: LHEF::HEPRUP::weightNameHepMC(int) const --> std::string", pybind11::arg("i"));
		cl.def("clear", (void (LHEF::HEPRUP::*)()) &LHEF::HEPRUP::clear, "Clear all information. \n\nC++: LHEF::HEPRUP::clear() --> void");
		cl.def("resize", (void (LHEF::HEPRUP::*)(int)) &LHEF::HEPRUP::resize, "Set the NPRUP variable, corresponding to the number of\n sub-processes, to  and resize all relevant vectors\n accordingly.\n\nC++: LHEF::HEPRUP::resize(int) --> void", pybind11::arg("nrup"));
		cl.def("resize", (void (LHEF::HEPRUP::*)()) &LHEF::HEPRUP::resize, "Assuming the NPRUP variable, corresponding to the number of\n sub-processes, is correctly set, resize the relevant vectors\n accordingly.\n\nC++: LHEF::HEPRUP::resize() --> void");
		cl.def("weightIndex", (int (LHEF::HEPRUP::*)(std::string) const) &LHEF::HEPRUP::weightIndex, "the index of the weight with the given \n   \n\nC++: LHEF::HEPRUP::weightIndex(std::string) const --> int", pybind11::arg("name"));
		cl.def("nWeights", (int (LHEF::HEPRUP::*)() const) &LHEF::HEPRUP::nWeights, "the number of weights (including the nominial one).\n\nC++: LHEF::HEPRUP::nWeights() const --> int");
		cl.def("getXSecInfo", [](LHEF::HEPRUP &o) -> LHEF::XSecInfo & { return o.getXSecInfo(); }, "", pybind11::return_value_policy::automatic);
		cl.def("getXSecInfo", (struct LHEF::XSecInfo & (LHEF::HEPRUP::*)(std::string)) &LHEF::HEPRUP::getXSecInfo, "the XSecInfo object corresponding to the named weight \n If no such object exists, it will be created.\n\nC++: LHEF::HEPRUP::getXSecInfo(std::string) --> struct LHEF::XSecInfo &", pybind11::return_value_policy::automatic, pybind11::arg("weightname"));

		 binder::custom_T_binder<LHEF::HEPRUP>(cl);
	}
}


// File: HepMC3/LHEF_2.cpp
#include <HepMC3/LHEF.h>
#include <ios>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream> // __str__
#include <streambuf>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

void bind_HepMC3_LHEF_2(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // LHEF::EventGroup file:HepMC3/LHEF.h line:2070
		pybind11::class_<LHEF::EventGroup, std::shared_ptr<LHEF::EventGroup>, std::vector<LHEF::HEPEUP *>> cl(M("LHEF"), "EventGroup", "The EventGroup represents a set of events which are to be\n considered together.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::EventGroup(); } ) );
		cl.def( pybind11::init( [](LHEF::EventGroup const &o){ return new LHEF::EventGroup(o); } ) );
		cl.def_readwrite("nreal", &LHEF::EventGroup::nreal);
		cl.def_readwrite("ncounter", &LHEF::EventGroup::ncounter);
		cl.def("assign", (struct LHEF::EventGroup & (LHEF::EventGroup::*)(const struct LHEF::EventGroup &)) &LHEF::EventGroup::operator=, "The assignment also copies the included HEPEUP object.\n\nC++: LHEF::EventGroup::operator=(const struct LHEF::EventGroup &) --> struct LHEF::EventGroup &", pybind11::return_value_policy::automatic, pybind11::arg(""));
		cl.def("clear", (void (LHEF::EventGroup::*)()) &LHEF::EventGroup::clear, "Remove all subevents.\n\nC++: LHEF::EventGroup::clear() --> void");
	}
	{ // LHEF::HEPEUP file:HepMC3/LHEF.h line:2118
		pybind11::class_<LHEF::HEPEUP, std::shared_ptr<LHEF::HEPEUP>, LHEF::TagBase> cl(M("LHEF"), "HEPEUP", "The HEPEUP class is a simple container corresponding to the Les Houches accord\n (<A HREF=\"http://arxiv.org/abs/hep-ph/0109068\">hep-ph/0109068</A>)\n common block with the same name. The members are named in the same\n way as in the common block. However, fortran arrays are represented\n by vectors, except for the arrays of length two which are\n represented by pair objects.");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new LHEF::HEPEUP(); } ) );
		cl.def( pybind11::init( [](LHEF::HEPEUP const &o){ return new LHEF::HEPEUP(o); } ) );
		cl.def( pybind11::init<const struct LHEF::XMLTag &, class LHEF::HEPRUP &>(), pybind11::arg("tagin"), pybind11::arg("heprupin") );

		cl.def_readwrite("NUP", &LHEF::HEPEUP::NUP);
		cl.def_readwrite("IDPRUP", &LHEF::HEPEUP::IDPRUP);
		cl.def_readwrite("XWGTUP", &LHEF::HEPEUP::XWGTUP);
		cl.def_readwrite("XPDWUP", &LHEF::HEPEUP::XPDWUP);
		cl.def_readwrite("SCALUP", &LHEF::HEPEUP::SCALUP);
		cl.def_readwrite("AQEDUP", &LHEF::HEPEUP::AQEDUP);
		cl.def_readwrite("AQCDUP", &LHEF::HEPEUP::AQCDUP);
		cl.def_readwrite("IDUP", &LHEF::HEPEUP::IDUP);
		cl.def_readwrite("ISTUP", &LHEF::HEPEUP::ISTUP);
		cl.def_readwrite("MOTHUP", &LHEF::HEPEUP::MOTHUP);
		cl.def_readwrite("ICOLUP", &LHEF::HEPEUP::ICOLUP);
		cl.def_readwrite("PUP", &LHEF::HEPEUP::PUP);
		cl.def_readwrite("VTIMUP", &LHEF::HEPEUP::VTIMUP);
		cl.def_readwrite("SPINUP", &LHEF::HEPEUP::SPINUP);
		cl.def_readwrite("namedweights", &LHEF::HEPEUP::namedweights);
		cl.def_readwrite("weights", &LHEF::HEPEUP::weights);
		cl.def_readwrite("clustering", &LHEF::HEPEUP::clustering);
		cl.def_readwrite("pdfinfo", &LHEF::HEPEUP::pdfinfo);
		cl.def_readwrite("PDFGUPsave", &LHEF::HEPEUP::PDFGUPsave);
		cl.def_readwrite("PDFSUPsave", &LHEF::HEPEUP::PDFSUPsave);
		cl.def_readwrite("scales", &LHEF::HEPEUP::scales);
		cl.def_readwrite("ntries", &LHEF::HEPEUP::ntries);
		cl.def_readwrite("isGroup", &LHEF::HEPEUP::isGroup);
		cl.def_readwrite("subevents", &LHEF::HEPEUP::subevents);
		cl.def_readwrite("junk", &LHEF::HEPEUP::junk);
		cl.def("setEvent", (class LHEF::HEPEUP & (LHEF::HEPEUP::*)(const class LHEF::HEPEUP &)) &LHEF::HEPEUP::setEvent, "Copy information from the given HEPEUP. Sub event information is\n left untouched.\n\nC++: LHEF::HEPEUP::setEvent(const class LHEF::HEPEUP &) --> class LHEF::HEPEUP &", pybind11::return_value_policy::automatic, pybind11::arg("x"));
		cl.def("assign", (class LHEF::HEPEUP & (LHEF::HEPEUP::*)(const class LHEF::HEPEUP &)) &LHEF::HEPEUP::operator=, "Assignment operator.\n\nC++: LHEF::HEPEUP::operator=(const class LHEF::HEPEUP &) --> class LHEF::HEPEUP &", pybind11::return_value_policy::automatic, pybind11::arg("x"));
		cl.def("reset", (void (LHEF::HEPEUP::*)()) &LHEF::HEPEUP::reset, "Reset the HEPEUP object (does not touch the sub events).\n\nC++: LHEF::HEPEUP::reset() --> void");
		cl.def("clear", (void (LHEF::HEPEUP::*)()) &LHEF::HEPEUP::clear, "Clear the HEPEUP object.\n\nC++: LHEF::HEPEUP::clear() --> void");
		cl.def("resize", (void (LHEF::HEPEUP::*)(int)) &LHEF::HEPEUP::resize, "Set the NUP variable, corresponding to the number of particles in\n the current event, to  and resize all relevant vectors\n accordingly.\n\nC++: LHEF::HEPEUP::resize(int) --> void", pybind11::arg("nup"));
		cl.def("totalWeight", [](LHEF::HEPEUP const &o) -> double { return o.totalWeight(); }, "");
		cl.def("totalWeight", (double (LHEF::HEPEUP::*)(int) const) &LHEF::HEPEUP::totalWeight, "Return the total weight for this event (including all sub\n evenets) for the given index.\n\nC++: LHEF::HEPEUP::totalWeight(int) const --> double", pybind11::arg("i"));
		cl.def("totalWeight", (double (LHEF::HEPEUP::*)(std::string) const) &LHEF::HEPEUP::totalWeight, "Return the total weight for this event (including all sub\n evenets) for the given weight name.\n\nC++: LHEF::HEPEUP::totalWeight(std::string) const --> double", pybind11::arg("name"));
		cl.def("weight", [](LHEF::HEPEUP const &o) -> double { return o.weight(); }, "");
		cl.def("weight", (double (LHEF::HEPEUP::*)(int) const) &LHEF::HEPEUP::weight, "Return the weight for the given index.\n\nC++: LHEF::HEPEUP::weight(int) const --> double", pybind11::arg("i"));
		cl.def("weight", (double (LHEF::HEPEUP::*)(std::string) const) &LHEF::HEPEUP::weight, "Return the weight for the given weight name.\n\nC++: LHEF::HEPEUP::weight(std::string) const --> double", pybind11::arg("name"));
		cl.def("setWeight", (void (LHEF::HEPEUP::*)(int, double)) &LHEF::HEPEUP::setWeight, "Set the weight with the given index.\n\nC++: LHEF::HEPEUP::setWeight(int, double) --> void", pybind11::arg("i"), pybind11::arg("w"));
		cl.def("setWeight", (bool (LHEF::HEPEUP::*)(std::string, double)) &LHEF::HEPEUP::setWeight, "Set the weight with the given name.\n\nC++: LHEF::HEPEUP::setWeight(std::string, double) --> bool", pybind11::arg("name"), pybind11::arg("w"));
		cl.def("resize", (void (LHEF::HEPEUP::*)()) &LHEF::HEPEUP::resize, "Assuming the NUP variable, corresponding to the number of\n particles in the current event, is correctly set, resize the\n relevant vectors accordingly.\n\nC++: LHEF::HEPEUP::resize() --> void");
		cl.def("setWeightInfo", (bool (LHEF::HEPEUP::*)(unsigned int)) &LHEF::HEPEUP::setWeightInfo, "Setup the current event to use weight i. If zero, the default\n weight will be used.\n\nC++: LHEF::HEPEUP::setWeightInfo(unsigned int) --> bool", pybind11::arg("i"));
		cl.def("setSubEvent", (bool (LHEF::HEPEUP::*)(unsigned int)) &LHEF::HEPEUP::setSubEvent, "Setup the current event to use sub event i. If zero, no sub event\n will be chsen.\n\nC++: LHEF::HEPEUP::setSubEvent(unsigned int) --> bool", pybind11::arg("i"));

		 binder::custom_T_binder<LHEF::HEPEUP>(cl);
	}
	{ // LHEF::Reader file:HepMC3/LHEF.h line:2742
		pybind11::class_<LHEF::Reader, std::shared_ptr<LHEF::Reader>> cl(M("LHEF"), "Reader", "The Reader class is initialized with a stream from which to read a\n version 1/2 Les Houches Accord event file. In the constructor of\n the Reader object the optional header information is read and then\n the mandatory init is read. After this the whole header block\n including the enclosing lines with tags are available in the public\n headerBlock member variable. Also the information from the init\n block is available in the heprup member variable and any additional\n comment lines are available in initComments. After each successful\n call to the readEvent() function the standard Les Houches Accord\n information about the event is available in the hepeup member\n variable and any additional comments in the eventComments\n variable. A typical reading sequence would look as follows:\n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string>(), pybind11::arg("filename") );

		cl.def_readwrite("version", &LHEF::Reader::version);
		cl.def_readwrite("outsideBlock", &LHEF::Reader::outsideBlock);
		cl.def_readwrite("headerBlock", &LHEF::Reader::headerBlock);
		cl.def_readwrite("heprup", &LHEF::Reader::heprup);
		cl.def_readwrite("initComments", &LHEF::Reader::initComments);
		cl.def_readwrite("hepeup", &LHEF::Reader::hepeup);
		cl.def_readwrite("eventComments", &LHEF::Reader::eventComments);
		cl.def_readwrite("currevent", &LHEF::Reader::currevent);
		cl.def_readwrite("curreventfile", &LHEF::Reader::curreventfile);
		cl.def_readwrite("currfileevent", &LHEF::Reader::currfileevent);
		cl.def_readwrite("dirpath", &LHEF::Reader::dirpath);
		cl.def("readEvent", (bool (LHEF::Reader::*)()) &LHEF::Reader::readEvent, "Read an event from the file and store it in the hepeup\n object. Optional comment lines are stored i the eventComments\n member variable. \n \n\n true if the read sas successful.\n\nC++: LHEF::Reader::readEvent() --> bool");
		cl.def("openeventfile", (void (LHEF::Reader::*)(int)) &LHEF::Reader::openeventfile, "Open the efentfile with index ifile. If another eventfile is\n being read, its remaining contents is discarded. This is a noop\n if current read session is not a multi-file run.\n\nC++: LHEF::Reader::openeventfile(int) --> void", pybind11::arg("ifile"));
	}
	{ // LHEF::Writer file:HepMC3/LHEF.h line:3090
		pybind11::class_<LHEF::Writer, std::shared_ptr<LHEF::Writer>> cl(M("LHEF"), "Writer", "The Writer class is initialized with a stream to which to write a\n version 1.0 Les Houches Accord event file. In the constructor of\n the Writer object the main XML tag is written out, with the\n corresponding end tag is written in the destructor. After a Writer\n object has been created, it is possible to assign standard init\n information in the heprup member variable. In addition any XML\n formatted information can be added to the headerBlock member\n variable (directly or via the addHeader() function). Further\n comment line (beginning with a # character) can be\n added to the initComments variable (directly or with the\n addInitComment() function). After this information is set, it\n should be written out to the file with the init() function.\n\n Before each event is written out with the writeEvent() function,\n the standard event information can then be assigned to the hepeup\n variable and optional comment lines (beginning with a\n # character) may be given to the eventComments\n variable (directly or with the addEventComment() function).\n\n ");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<std::string>(), pybind11::arg("filename") );

		cl.def_readwrite("heprup", &LHEF::Writer::heprup);
		cl.def_readwrite("hepeup", &LHEF::Writer::hepeup);
		cl.def("headerBlock", (void (LHEF::Writer::*)(const std::string &)) &LHEF::Writer::headerBlock, "Add header lines consisting of XML code with this stream.\n\nC++: LHEF::Writer::headerBlock(const std::string &) --> void", pybind11::arg("a"));
		cl.def("initComments", (void (LHEF::Writer::*)(const std::string &)) &LHEF::Writer::initComments, "Add comment lines to the init block with this stream.\n\nC++: LHEF::Writer::initComments(const std::string &) --> void", pybind11::arg("a"));
		cl.def("eventComments", (void (LHEF::Writer::*)(const std::string &)) &LHEF::Writer::eventComments, "Add comment lines to the next event to be written out with this stream.\n\nC++: LHEF::Writer::eventComments(const std::string &) --> void", pybind11::arg("a"));
		cl.def("init", (void (LHEF::Writer::*)()) &LHEF::Writer::init, "Initialize the writer.\n\nC++: LHEF::Writer::init() --> void");
		cl.def("openeventfile", (bool (LHEF::Writer::*)(int)) &LHEF::Writer::openeventfile, "Open a new event file, possibly closing a previous opened one.\n\nC++: LHEF::Writer::openeventfile(int) --> bool", pybind11::arg("ifile"));
		cl.def("writeinit", (void (LHEF::Writer::*)()) &LHEF::Writer::writeinit, "Write out an optional header block followed by the standard init\n block information together with any comment lines.\n\nC++: LHEF::Writer::writeinit() --> void");
		cl.def("writeEvent", (void (LHEF::Writer::*)()) &LHEF::Writer::writeEvent, "Write the current HEPEUP object to the stream;\n\nC++: LHEF::Writer::writeEvent() --> void");
	}
}


// File: HepMC3/LHEFAttributes.cpp
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
#include <HepMC3/LHEFAttributes.h>
#include <HepMC3/ReaderLHEF.h>
#include <HepMC3/ReaderPlugin.h>
#include <HepMC3/WriterPlugin.h>
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
#include <src/binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*);
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>);
#endif

// HepMC3::HEPRUPAttribute file:HepMC3/LHEFAttributes.h line:26
struct PyCallBack_HepMC3_HEPRUPAttribute : public HepMC3::HEPRUPAttribute {
	using HepMC3::HEPRUPAttribute::HEPRUPAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPRUPAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPRUPAttribute::from_string(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPRUPAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPRUPAttribute::to_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPRUPAttribute *>(this), "init");
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
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPRUPAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return Attribute::init(a0);
	}
};

// HepMC3::HEPEUPAttribute file:HepMC3/LHEFAttributes.h line:68
struct PyCallBack_HepMC3_HEPEUPAttribute : public HepMC3::HEPEUPAttribute {
	using HepMC3::HEPEUPAttribute::HEPEUPAttribute;

	bool from_string(const class std::basic_string<char> & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPEUPAttribute *>(this), "from_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPEUPAttribute::from_string(a0);
	}
	bool init() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPEUPAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPEUPAttribute::init();
	}
	bool init(const class HepMC3::GenRunInfo & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPEUPAttribute *>(this), "init");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPEUPAttribute::init(a0);
	}
	bool to_string(class std::basic_string<char> & a0) const override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::HEPEUPAttribute *>(this), "to_string");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return HEPEUPAttribute::to_string(a0);
	}
};

// HepMC3::ReaderLHEF file:HepMC3/ReaderLHEF.h line:34
struct PyCallBack_HepMC3_ReaderLHEF : public HepMC3::ReaderLHEF {
	using HepMC3::ReaderLHEF::ReaderLHEF;

	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderLHEF *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderLHEF::read_event(a0);
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderLHEF *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderLHEF::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderLHEF *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderLHEF::failed();
	}
};

// HepMC3::ReaderPlugin file:HepMC3/ReaderPlugin.h line:23
struct PyCallBack_HepMC3_ReaderPlugin : public HepMC3::ReaderPlugin {
	using HepMC3::ReaderPlugin::ReaderPlugin;

	bool read_event(class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderPlugin *>(this), "read_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderPlugin::read_event(a0);
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderPlugin *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return ReaderPlugin::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::ReaderPlugin *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return ReaderPlugin::failed();
	}
};

// HepMC3::WriterPlugin file:HepMC3/WriterPlugin.h line:23
struct PyCallBack_HepMC3_WriterPlugin : public HepMC3::WriterPlugin {
	using HepMC3::WriterPlugin::WriterPlugin;

	void write_event(const class HepMC3::GenEvent & a0) override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterPlugin *>(this), "write_event");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>(a0);
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterPlugin::write_event(a0);
	}
	void close() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterPlugin *>(this), "close");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<void>::value) {
				static pybind11::detail::overload_caster_t<void> caster;
				return pybind11::detail::cast_ref<void>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<void>(std::move(o));
		}
		return WriterPlugin::close();
	}
	bool failed() override { 
		pybind11::gil_scoped_acquire gil;
		pybind11::function overload = pybind11::get_overload(static_cast<const HepMC3::WriterPlugin *>(this), "failed");
		if (overload) {
			auto o = overload.operator()<pybind11::return_value_policy::reference>();
			if (pybind11::detail::cast_is_temporary_value_reference<bool>::value) {
				static pybind11::detail::overload_caster_t<bool> caster;
				return pybind11::detail::cast_ref<bool>(std::move(o), caster);
			}
			else return pybind11::detail::cast_safe<bool>(std::move(o));
		}
		return WriterPlugin::failed();
	}
};

void bind_HepMC3_LHEFAttributes(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // HepMC3::HEPRUPAttribute file:HepMC3/LHEFAttributes.h line:26
		pybind11::class_<HepMC3::HEPRUPAttribute, std::shared_ptr<HepMC3::HEPRUPAttribute>, PyCallBack_HepMC3_HEPRUPAttribute, HepMC3::Attribute> cl(M("HepMC3"), "HEPRUPAttribute", "Class for storing data for LHEF run information");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::HEPRUPAttribute(); }, [](){ return new PyCallBack_HepMC3_HEPRUPAttribute(); } ) );
		cl.def( pybind11::init<std::string>(), pybind11::arg("s") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_HEPRUPAttribute const &o){ return new PyCallBack_HepMC3_HEPRUPAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::HEPRUPAttribute const &o){ return new HepMC3::HEPRUPAttribute(o); } ) );
		cl.def_readwrite("heprup", &HepMC3::HEPRUPAttribute::heprup);
		cl.def_readwrite("tags", &HepMC3::HEPRUPAttribute::tags);
		cl.def("from_string", (bool (HepMC3::HEPRUPAttribute::*)(const std::string &)) &HepMC3::HEPRUPAttribute::from_string, "Fill class content from string \n\nC++: HepMC3::HEPRUPAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("to_string", (bool (HepMC3::HEPRUPAttribute::*)(std::string &) const) &HepMC3::HEPRUPAttribute::to_string, "Fill string from class content \n\nC++: HepMC3::HEPRUPAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("clear", (void (HepMC3::HEPRUPAttribute::*)()) &HepMC3::HEPRUPAttribute::clear, "Clear this object. \n\nC++: HepMC3::HEPRUPAttribute::clear() --> void");
		cl.def("assign", (class HepMC3::HEPRUPAttribute & (HepMC3::HEPRUPAttribute::*)(const class HepMC3::HEPRUPAttribute &)) &HepMC3::HEPRUPAttribute::operator=, "C++: HepMC3::HEPRUPAttribute::operator=(const class HepMC3::HEPRUPAttribute &) --> class HepMC3::HEPRUPAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::HEPEUPAttribute file:HepMC3/LHEFAttributes.h line:68
		pybind11::class_<HepMC3::HEPEUPAttribute, std::shared_ptr<HepMC3::HEPEUPAttribute>, PyCallBack_HepMC3_HEPEUPAttribute, HepMC3::Attribute> cl(M("HepMC3"), "HEPEUPAttribute", "Class for storing data for LHEF run information");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](){ return new HepMC3::HEPEUPAttribute(); }, [](){ return new PyCallBack_HepMC3_HEPEUPAttribute(); } ) );
		cl.def( pybind11::init<std::string>(), pybind11::arg("s") );

		cl.def( pybind11::init( [](PyCallBack_HepMC3_HEPEUPAttribute const &o){ return new PyCallBack_HepMC3_HEPEUPAttribute(o); } ) );
		cl.def( pybind11::init( [](HepMC3::HEPEUPAttribute const &o){ return new HepMC3::HEPEUPAttribute(o); } ) );
		cl.def_readwrite("hepeup", &HepMC3::HEPEUPAttribute::hepeup);
		cl.def_readwrite("tags", &HepMC3::HEPEUPAttribute::tags);
		cl.def("from_string", (bool (HepMC3::HEPEUPAttribute::*)(const std::string &)) &HepMC3::HEPEUPAttribute::from_string, "Fill class content from string \n\nC++: HepMC3::HEPEUPAttribute::from_string(const std::string &) --> bool", pybind11::arg("att"));
		cl.def("init", (bool (HepMC3::HEPEUPAttribute::*)()) &HepMC3::HEPEUPAttribute::init, "Parse the XML-tags. \n\nC++: HepMC3::HEPEUPAttribute::init() --> bool");
		cl.def("init", (bool (HepMC3::HEPEUPAttribute::*)(const class HepMC3::GenRunInfo &)) &HepMC3::HEPEUPAttribute::init, "Dummy function. \n\nC++: HepMC3::HEPEUPAttribute::init(const class HepMC3::GenRunInfo &) --> bool", pybind11::arg(""));
		cl.def("to_string", (bool (HepMC3::HEPEUPAttribute::*)(std::string &) const) &HepMC3::HEPEUPAttribute::to_string, "Fill string from class content \n\nC++: HepMC3::HEPEUPAttribute::to_string(std::string &) const --> bool", pybind11::arg("att"));
		cl.def("momentum", (class HepMC3::FourVector (HepMC3::HEPEUPAttribute::*)(int) const) &HepMC3::HEPEUPAttribute::momentum, "Get momentum \n\nC++: HepMC3::HEPEUPAttribute::momentum(int) const --> class HepMC3::FourVector", pybind11::arg("i"));
		cl.def("clear", (void (HepMC3::HEPEUPAttribute::*)()) &HepMC3::HEPEUPAttribute::clear, "Clear this object. \n\nC++: HepMC3::HEPEUPAttribute::clear() --> void");
		cl.def("assign", (class HepMC3::HEPEUPAttribute & (HepMC3::HEPEUPAttribute::*)(const class HepMC3::HEPEUPAttribute &)) &HepMC3::HEPEUPAttribute::operator=, "C++: HepMC3::HEPEUPAttribute::operator=(const class HepMC3::HEPEUPAttribute &) --> class HepMC3::HEPEUPAttribute &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // HepMC3::ReaderLHEF file:HepMC3/ReaderLHEF.h line:34
		pybind11::class_<HepMC3::ReaderLHEF, std::shared_ptr<HepMC3::ReaderLHEF>, PyCallBack_HepMC3_ReaderLHEF, HepMC3::Reader> cl(M("HepMC3"), "ReaderLHEF", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("read_event", (bool (HepMC3::ReaderLHEF::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderLHEF::read_event, "Reading event \n\nC++: HepMC3::ReaderLHEF::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("ev"));
		cl.def("close", (void (HepMC3::ReaderLHEF::*)()) &HepMC3::ReaderLHEF::close, "Close \n\nC++: HepMC3::ReaderLHEF::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderLHEF::*)()) &HepMC3::ReaderLHEF::failed, "State \n\nC++: HepMC3::ReaderLHEF::failed() --> bool");
	}
	{ // HepMC3::ReaderPlugin file:HepMC3/ReaderPlugin.h line:23
		pybind11::class_<HepMC3::ReaderPlugin, std::shared_ptr<HepMC3::ReaderPlugin>, PyCallBack_HepMC3_ReaderPlugin, HepMC3::Reader> cl(M("HepMC3"), "ReaderPlugin", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &, const std::string &, const std::string &>(), pybind11::arg("filename"), pybind11::arg("libname"), pybind11::arg("newreader") );

		cl.def("read_event", (bool (HepMC3::ReaderPlugin::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderPlugin::read_event, "Reading event \n\nC++: HepMC3::ReaderPlugin::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("ev"));
		cl.def("close", (void (HepMC3::ReaderPlugin::*)()) &HepMC3::ReaderPlugin::close, "Close \n\nC++: HepMC3::ReaderPlugin::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderPlugin::*)()) &HepMC3::ReaderPlugin::failed, "State \n\nC++: HepMC3::ReaderPlugin::failed() --> bool");
	}
	{ // HepMC3::WriterPlugin file:HepMC3/WriterPlugin.h line:23
		pybind11::class_<HepMC3::WriterPlugin, std::shared_ptr<HepMC3::WriterPlugin>, PyCallBack_HepMC3_WriterPlugin, HepMC3::Writer> cl(M("HepMC3"), "WriterPlugin", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0, const class std::basic_string<char> & a1, const class std::basic_string<char> & a2){ return new HepMC3::WriterPlugin(a0, a1, a2); }, [](const class std::basic_string<char> & a0, const class std::basic_string<char> & a1, const class std::basic_string<char> & a2){ return new PyCallBack_HepMC3_WriterPlugin(a0, a1, a2); } ), "doc");
		cl.def( pybind11::init<const std::string &, const std::string &, const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("libname"), pybind11::arg("newwriter"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterPlugin::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterPlugin::write_event, "Reading event \n\nC++: HepMC3::WriterPlugin::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("ev"));
		cl.def("close", (void (HepMC3::WriterPlugin::*)()) &HepMC3::WriterPlugin::close, "Close \n\nC++: HepMC3::WriterPlugin::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterPlugin::*)()) &HepMC3::WriterPlugin::failed, "State \n\nC++: HepMC3::WriterPlugin::failed() --> bool");
	}
}


#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include <pybind11/pybind11.h>

typedef std::function< pybind11::module & (std::string const &) > ModuleGetter;

void bind_HepMC3_Setup(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_FourVector(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_FourVector_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_map(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_Attribute(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_Attribute_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_GenCrossSection(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_GenRunInfo(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_GenParticle(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_Data_GenEventData(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_WriterAscii(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_ReaderAscii(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_HEPEVT_Wrapper(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_LHEF(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_LHEF_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_LHEF_2(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_HepMC3_LHEFAttributes(std::function< pybind11::module &(std::string const &namespace_) > &M);


PYBIND11_MODULE(pyHepMC3, root_module) {
	root_module.doc() = "pyHepMC3 module";

	std::map <std::string, pybind11::module> modules;
	ModuleGetter M = [&](std::string const &namespace_) -> pybind11::module & {
		auto it = modules.find(namespace_);
		if( it == modules.end() ) throw std::runtime_error("Attempt to access pybind11::module for namespace " + namespace_ + " before it was created!!!");
		return it->second;
	};

	modules[""] = root_module;

	std::vector< std::pair<std::string, std::string> > sub_modules {
		{"", "HepMC3"},
		{"", "LHEF"},
		{"", "std"},
	};
	for(auto &p : sub_modules ) modules[p.first.size() ? p.first+"::"+p.second : p.second] = modules[p.first].def_submodule(p.second.c_str(), ("Bindings for " + p.first + "::" + p.second + " namespace").c_str() );

	//pybind11::class_<std::shared_ptr<void>>(M(""), "_encapsulated_data_");

	bind_HepMC3_Setup(M);
	bind_HepMC3_FourVector(M);
	bind_HepMC3_FourVector_1(M);
	bind_std_stl_map(M);
	bind_std_stl_vector(M);
	bind_HepMC3_Attribute(M);
	bind_HepMC3_Attribute_1(M);
	bind_HepMC3_GenCrossSection(M);
	bind_HepMC3_GenRunInfo(M);
	bind_HepMC3_GenParticle(M);
	bind_HepMC3_Data_GenEventData(M);
	bind_HepMC3_WriterAscii(M);
	bind_HepMC3_ReaderAscii(M);
	bind_HepMC3_HEPEVT_Wrapper(M);
	bind_HepMC3_LHEF(M);
	bind_HepMC3_LHEF_1(M);
	bind_HepMC3_LHEF_2(M);
	bind_HepMC3_LHEFAttributes(M);

}

// Source list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3.sources
// pyHepMC3.cpp
// HepMC3/Setup.cpp
// HepMC3/FourVector.cpp
// HepMC3/FourVector_1.cpp
// std/stl_map.cpp
// std/stl_vector.cpp
// HepMC3/Attribute.cpp
// HepMC3/Attribute_1.cpp
// HepMC3/GenCrossSection.cpp
// HepMC3/GenRunInfo.cpp
// HepMC3/GenParticle.cpp
// HepMC3/Data/GenEventData.cpp
// HepMC3/WriterAscii.cpp
// HepMC3/ReaderAscii.cpp
// HepMC3/HEPEVT_Wrapper.cpp
// HepMC3/LHEF.cpp
// HepMC3/LHEF_1.cpp
// HepMC3/LHEF_2.cpp
// HepMC3/LHEFAttributes.cpp

// Modules list file: /ptmp/mpp/andriish/HOME/HepMC3/python/src/pyHepMC3.modules
// HepMC3 LHEF std 
