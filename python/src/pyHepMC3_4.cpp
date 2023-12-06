#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <list>
#include <memory>
#include <sstream> // __str__

#include <functional>
#include <pybind11/pybind11.h>
#include <string>
#include <HepMC3/Version.h>
#include <HepMC3/Reader.h>
#include <HepMC3/Writer.h>
#include <HepMC3/Print.h>
#include <src/stl_binders.hpp>
#include <src/binders.h>
#include <src/list_binder.h>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>)
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*)
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>)
#endif

void bind_pyHepMC3_4(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	// std::list file:bits/stl_list.h line:632
	binder::list_binder<std::shared_ptr<HepMC3::GenParticle>,std::allocator<std::shared_ptr<HepMC3::GenParticle> >>(M("std"), "std_shared_ptr_HepMC3_GenParticle_t", "std_allocator_std_shared_ptr_HepMC3_GenParticle_t");

	// std::list file:bits/stl_list.h line:632
	binder::list_binder<std::shared_ptr<const HepMC3::GenParticle>,std::allocator<std::shared_ptr<const HepMC3::GenParticle> >>(M("std"), "std_shared_ptr_const_HepMC3_GenParticle_t", "std_allocator_std_shared_ptr_const_HepMC3_GenParticle_t");

	// std::list file:bits/stl_list.h line:632
	binder::list_binder<std::shared_ptr<HepMC3::GenVertex>,std::allocator<std::shared_ptr<HepMC3::GenVertex> >>(M("std"), "std_shared_ptr_HepMC3_GenVertex_t", "std_allocator_std_shared_ptr_HepMC3_GenVertex_t");

}
