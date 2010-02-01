// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
/* :tabSize=4:indentSize=4:noTabs=false:folding=explicit:collapseFolds=1: */
//
// wrap_type_traits.h: Rcpp R/C++ interface class library -- traits to help wrap
//
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__traits__wrap_type_traits__h
#define Rcpp__traits__wrap_type_traits__h

namespace Rcpp{
namespace traits{

/**
 * Identifies an stl type container, with begin and end methods. 
 */
struct wrap_type_stl_container_tag{};

/**
 * primitive type : int, double, std::string, Rcomplex, size_t, Rbyte
 */
struct wrap_type_primitive_tag{};

/**
 * unknown. Implicit conversion to SEXP will be tried.
 */
struct wrap_type_unknown_tag{};          // unknown, not sure what to do with this type

/**
 * Type trait that helps the dispatch of wrap to the proper method
 *
 * This builds a struct that contains a typedef called wrap_category
 * that has to be one of "wrap_type_stl_container_tag", 
 * "wrap_type_primitive_tag" or "wrap_type_unknown_tag"
 *
 * The default is "wrap_type_unknown_tag" and many
 * partial or complete specializations
 * are defined below.
 */
template <typename T> struct wrap_type_traits { typedef wrap_type_unknown_tag wrap_category; } ;

/**
 * partial specialization for stl containers
 */ 
template <typename T> struct wrap_type_traits< std::vector<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::list<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::set<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::deque<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::multiset<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::map<std::string,T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::multimap<std::string,T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
#ifdef HAS_TR1_UNORDERED_MAP
template <typename T> struct wrap_type_traits< std::tr1::unordered_map<std::string,T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::tr1::unordered_multimap<std::string,T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
#endif
#ifdef HAS_TR1_UNORDERED_SET
template <typename T> struct wrap_type_traits< std::tr1::unordered_set<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
template <typename T> struct wrap_type_traits< std::tr1::unordered_multiset<T> > { typedef wrap_type_stl_container_tag wrap_category ; } ;
#endif

/**
 * Total specialization for primitive types
 */
template <> struct wrap_type_traits<int> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<double> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<Rbyte> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<Rcomplex> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<size_t> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<bool> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<std::string> { typedef wrap_type_primitive_tag wrap_category; } ;
template <> struct wrap_type_traits<char> { typedef wrap_type_primitive_tag wrap_category; } ;

} // namespace traits
} // namespace Rcpp
#endif
