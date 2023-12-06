// Copyright (c) 2023 Christian Holm Christensen <cholmcc@gmail.com>
//
// All rights reserved. Use of this source code is governed by a
// MIT license that can be found in the LICENSE file.

/// @file   list_binder.h
/// @brief  Support for custom binders for some std:: template classes
/// @author Christian Holm Christensen


#ifndef _INCLUDED_list_binder_h_
#define _INCLUDED_list_binder_h_

#include <pybind11/stl_bind.h>
#include "pybind11/detail/common.h"
#include "pybind11/detail/type_caster_base.h"

namespace binder {
template <typename T, class Allocator>
class list_binder
{
public:
    list_binder(pybind11::module &m, std::string const &value_name,
                std::string const & /*allocator name*/)
    {
        using List = std::list<T, Allocator>;
        using holder_type = std::shared_ptr< std::list<T, Allocator> >;
        using Class_ = pybind11::class_<List, holder_type>;
        using DiffType = typename List::difference_type;
        using ItType = typename List::iterator;
        constexpr pybind11::return_value_policy ReturnPolicy =
            pybind11::return_value_policy::reference_internal;
        
        using vtype = typename List::value_type;
        auto *vtype_info = pybind11::detail::get_type_info(typeid(vtype));
        bool local       = !vtype_info || vtype_info->module_local;
        std::string name = "list_"+value_name;
        
        Class_ cl(m, name.c_str(), pybind11::module_local(local));

        // Default constructor 
        cl.def(pybind11::init<>());
        
        // Copy constructor 
        cl.def(pybind11::init<const List &>(), "Copy constructor");
        
        // Range constructor 
        cl.def(pybind11::init([](const pybind11::iterable &it) {
            auto l = std::unique_ptr<List>(new List());
            for (auto h : it) {
                l->push_back(h.cast<T>());
            }
            return l.release();}));
        
        // String representation 
        cl.def("__repr__",
               [name](List &l) {
                 std::ostringstream s;
                 s << name << '[';
                 size_t i = 0;
                 for (auto e : l) {
                   if (i != 0) s << ", ";
                   s << e;
                   i++;
                 }
                 s << ']';
                 return s.str();
               },
               "Return the canonical string representation "
               "of this list.");

        // Clear content 
        cl.def("clear",
               [](List &l) { l.clear(); },
               "Clear the contents");

        cl.def("remove",
               [](List &l, const T &x) {
                 auto p = std::find(l.begin(), l.end(), x);
                 if (p != l.end()) {
                     l.erase(p);
                 } else {
                   throw pybind11::value_error();
                 }
               },
               pybind11::arg("x"),
               "Remove the first item from the list whose value is x. "
               "It is an error if there is no such item.");

        cl.def("find",
               [](List &l, const T &x) {
                 auto p = std::find(l.begin(), l.end(), x);
                 if (p != l.end()) {
                     return std::distance(l.begin(),p);
                 } else {
                     throw pybind11::value_error();
                 }
               },
               pybind11::arg("x"),
               "Find position of value x in the list.");
                
        // Append value 
        cl.def("append",
               [](List &l, const T &value) { l.push_back(value); },
               pybind11::arg("x"),
               "Add an item to the end of the list");
        cl.def("push_back",
               [](List &l, const T &value) { l.push_back(value); },
               pybind11::arg("x"),
               "Add an item to the end of the list");
        cl.def("push_front",
               [](List &l, const T &value) { l.push_front(value); },
               pybind11::arg("x"),
               "Add an item to the front of the list");


        // Append list 
        cl.def("extend",
               [](List &l, const List &src) {
                 l.insert(l.end(), src.begin(), src.end()); },
               pybind11::arg("L"),
               "Extend the list by appending all the items "
               "in the given list");

        // Append pybind11::iterable 
        cl.def("extend",
               [](List &l, const pybind11::iterable &it) {
                   try {
                       for (auto h : it) {
                           l.push_back(h.cast<T>());
                       }
                   } catch (const pybind11::cast_error &) {
                   }
               },
               pybind11::arg("L"),
               "Extend the list by appending all the "
               "items in the given list");

        // Insert an element 
        cl.def("insert",
               [](List &l, DiffType i, const T &x) {
                   auto pos = std::next(l.begin(), i);
                   l.insert(pos, x);
               },
               pybind11::arg("i"),
               pybind11::arg("x"),
               "Insert an item at a given position.");

        // Pop an element from the back of the list 
        cl.def("pop_back",
               [](List &l) {
                 if (l.empty()) {
                     throw pybind11::index_error();
                 }
                 T t = std::move(l.back());
                 l.pop_back();
                 return t;
               },
               "Remove and return the last item");
        // Pop an element from the back of the list 
        cl.def("pop_front",
               [](List &l) {
                 if (l.empty()) {
                     throw pybind11::index_error();
                 }
                 T t = std::move(l.front());
                 l.pop_front();
                 return t;
               },
               "Remove and return the last item");

        // Assign to element - should this be done?
        cl.def("__setitem__",
               [](List &l, DiffType i, const T &t) {
                   if (i < 0) i += l.size();
                   if (i < 0 or i >= l.size())
                       throw pybind11::index_error();

                   *std::next(l.begin(),i) = t;
               });

        // Assign to elements - should this be done?
        cl.def("__setitem__",
               [](List &l, const pybind11::slice &slice, const List &value) {
                 size_t start = 0;
                   size_t stop = 0;
                   size_t step = 0;
                   size_t slicelength = 0;
                   
                   if (!slice.compute(l.size(),
                                      &start,
                                      &stop,
                                      &step,
                                      &slicelength)) {
                       throw pybind11::error_already_set();
                   }

                   if (slicelength != value.size()) {
                       throw
                         std::runtime_error("Left and right hand "
                                            "size of slice "
                                            "assignment have "
                                            "different sizes!");
                   }

                   for (auto v : value) {
                       *std::next(l.begin(),start) = v;
                       start += step;
                   }
               },
               "Assign list elements using a slice object");

        /// Access element
        cl.def("__getitem__",
               [](const List &l, DiffType i) {
                   if (i < 0) i += l.size();
                   if (i < 0 or i >= l.size())
                       throw pybind11::index_error();

                   return *std::next(l.begin(),i);
               },
               ReturnPolicy,
               "Retrieve list elements using a slice object");
        
        /// Slicing protocol
        cl.def("__getitem__",
               [](const List &l, const pybind11::slice &slice) -> List * {
                   size_t start = 0;
                   size_t stop = 0;
                   size_t step = 0;
                   size_t slicelength = 0;
                   
                   if (!slice.compute(l.size(),
                                      &start,
                                      &stop,
                                      &step,
                                      &slicelength)) {
                       throw pybind11::error_already_set();
                   }
                   
                   auto *seq = new List();
                   
                   for (size_t i = 0; i < slicelength; ++i) {
                       seq->push_back(*std::next(l.begin(),start));
                       start += step;
                   }
                   return seq;
               },
               pybind11::arg("s"),
               "Retrieve list elements using a slice object");

        // Remove item via index 
        cl.def("__delitem__",
               [](List &l, DiffType i) {
                   if (i < 0) i += l.size();
                   if (i < 0 or i >= l.size())
                       throw pybind11::index_error();
                   
                   l.erase(std::next(l.begin(),i));
               },
               "Delete the list elements at index ``i``");

        // Remove items via slice 
        cl.def("__delitem__",
               [](List &l, const pybind11::slice &slice) {
                   size_t start = 0;
                   size_t stop = 0;
                   size_t step = 0;
                   size_t slicelength = 0;
                   
                   if (!slice.compute(l.size(),
                                      &start,
                                      &stop,
                                      &step,
                                      &slicelength)) {
                       throw pybind11::error_already_set();
                   }

                   if (step == 1 && false) {
                       l.erase(std::next(l.begin(),start),
                               std::next(l.begin(),stop));
                   } else {
                       for (size_t i = 0; i < slicelength; ++i) {
                           l.erase(std::next(l.begin(),start));
                           start += step - 1;
                       }
                   }
               },
               "Delete list elements using a slice object");

        // Iterator 
        cl.def("__iter__",
               [ReturnPolicy](List &l) {
                 return pybind11::make_iterator<ReturnPolicy,
                                                ItType,
                                                ItType, T &>(l.begin(), l.end());
               },
               /* Essential: keep list alive while iterator exists */ 
               pybind11::keep_alive<0, 1>() 
               );

        // Search for item 
        cl.def("__contains__",
               [](const List &l, const T &x) {
                 return std::find(l.begin(), l.end(), x) != l.end(); },
               pybind11::arg("x"),
               "Return true the container contains ``x``");

        // Check if not empty 
        cl.def("__bool__",
               [](const List &l) -> bool { return !l.empty(); },
               "Check whether the list is nonempty");

        // Get length 
        cl.def("__len__", &List::size);
    }
};
  
} // namespace binder

#endif // _INCLUDED_list_binder_h_

