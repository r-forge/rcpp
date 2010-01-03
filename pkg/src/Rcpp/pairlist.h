// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// pairlist.h: Rcpp R/C++ interface class library -- variadic templates to create pairlists
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

#ifndef Rcpp_pairlist_h
#define Rcpp_pairlist_h

#include <RcppCommon.h>
#include <Rcpp/Named.h>
#include <Rcpp/grow.h>

namespace Rcpp{
	/* recursive packing of the arguments into a list, 
	  use first as the CAR and build the CDR from the remaining args recursively */
#ifdef CXX0X
	SEXP pairlist() ;
	template<typename T, typename... Args>
	SEXP pairlist( const T& first, const Args&... args ){
		return grow(first, pairlist(args...) ) ;
	}
 	/* end of the recursion, wrap first to make the CAR and use 
 	   R_NilValue as the CDR of the list */
	template<typename T>
	SEXP pairlist( const T& first){
		return grow(first, R_NilValue ) ; 
	}
#endif

} // namespace Rcpp

#endif
