// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// NumericVector.h: Rcpp R/C++ interface class library -- numeric vectors
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

#ifndef Rcpp_NumericVector_h
#define Rcpp_NumericVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class NumericVector : public VectorBase {     
public:

	NumericVector(SEXP x) throw(not_compatible);
	NumericVector( int size) ;
	
#ifdef HAS_INIT_LISTS	
	NumericVector( std::initializer_list<int> list ) ;
	NumericVector( std::initializer_list<double> list ) ;
#endif

	inline double& operator[]( const int& i ) { return start[i] ; }
	inline const double& operator[]( const int& i ) const { return start[i]; }
	inline double* begin() const { return start ; } 
	inline double* end() const   { return start+LENGTH(m_sexp);}
	
	typedef double* iterator ;

private:
	double *start ;
	inline void update(){ start = REAL(m_sexp);}
} ;

} // namespace

#endif
