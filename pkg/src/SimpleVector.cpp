// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SimpleVector.h: Rcpp R/C++ interface class library -- simple vectors
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

#include <RcppCommon.h>
#include <Rcpp/SimpleVector.h>

namespace Rcpp{
    	
	template<> double* get_pointer<REALSXP,double>(SEXP x){ return REAL(x) ; }
	template<> int* get_pointer<INTSXP,int>(SEXP x){ return INTEGER(x) ; }
	template<> int* get_pointer<LGLSXP,int>(SEXP x){ return LOGICAL(x) ; }
	template<> Rcomplex* get_pointer<CPLXSXP,Rcomplex>(SEXP x){ return COMPLEX(x) ; }
	template<> Rbyte* get_pointer<RAWSXP,Rbyte>(SEXP x){ return RAW(x) ; }

	template<> Rcomplex get_zero<CPLXSXP,Rcomplex>(){
		Rcomplex x ;
		x.r = 0.0 ;
		x.i = 0.0 ;
		return x ;
	}

	
} // namespace 
