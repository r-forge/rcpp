// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// as.h: Rcpp R/C++ interface class library -- generic converters from SEXP
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

#ifndef Rcpp_as_h
#define Rcpp_as_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <algorithm>

namespace Rcpp{ 

/** 
 * Generic converted from SEXP to the typename
 */
template <typename T> T as( SEXP m_sexp) {
	throw std::runtime_error("not implemented") ; 
}
template<> bool 			as<bool>(SEXP m_sexp) ;
template<> double                   	as<double>(SEXP m_sexp) ;
template<> int                      	as<int>(SEXP m_sexp) ;
template<> Rbyte                    	as<Rbyte>(SEXP m_sexp) ;
template<> std::string              	as<std::string>(SEXP m_sexp) ;
template<> std::vector<int>         	as< std::vector<int> >(SEXP m_sexp) ;
template<> std::vector<double>      	as< std::vector<double> >(SEXP m_sexp) ;
template<> std::vector<std::string> 	as< std::vector<std::string> >(SEXP m_sexp) ;
template<> std::vector<Rbyte>       	as< std::vector<Rbyte> >(SEXP m_sexp) ;
template<> std::vector<bool>        	as< std::vector<bool> >(SEXP m_sexp) ;

/* these do not take care of coercion*/
inline bool Rboolean_to_bool( int x){ return x == TRUE ; }
inline bool int_to_bool(int x){ return x != 0 ; }
inline bool double_to_bool(double x){ return x != 0.0 ; }
inline bool Rbyte_to_bool(Rbyte x){ return x != static_cast<Rbyte>(0) ; }

/* these take care of coercion */
inline int Rboolean_to_int(int x){ return (x==NA_LOGICAL) ? NA_INTEGER : x ; }
inline int double_to_int(double x){ 
	if (ISNAN(x)) return NA_INTEGER;
	else if (x > INT_MAX || x <= INT_MIN ) {
		return NA_INTEGER;
	}
	return static_cast<int>(x);
}
inline int Rbyte_to_int(Rbyte x){ return static_cast<int>(x); }

inline int int_to_RBoolean(int x){ return ( x == NA_INTEGER ) ? NA_LOGICAL : (x!=0); }



} // namespace Rcpp      

#endif
