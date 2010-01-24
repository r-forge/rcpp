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
	NumericVector( std::initializer_list<int> list ) : VectorBase(), start(0){
		coerce_and_fill( list.begin(), list.end() ) ;
	};
	NumericVector( std::initializer_list<double> list ) : VectorBase(), start(0){
		simple_fill( list.begin(), list.end() ) ;
	}
#endif
	/** Fast 1d 0-based indexing. no bounds checking are performed */
	inline double& operator[]( const int& i ) { return start[i] ; }
	
	/** Returns a pointer to the internal array */
	inline double* begin() const { return start ; }
	
	/** Returns a pointer to the element after the last element of the array */
	inline double* end() const   { return start + Rf_length(m_sexp);}
	
	/** convenience typedef */
	typedef double* iterator ;

	/** secure 1d 0-based indexing. 
	 * As opposed to operator[], this operator performs bounds checks
	 * to make sure that i is valid. There is however a price associated 
	 * with the check
	 * 
	 * @param i 0-based index. this indexes the object as a vector
	 * so if it is actually a matrix the order is column-major (as in R)
	 */
	double& operator()( const size_t& i ) throw(index_out_of_bounds) ;
	
	/**
	 * matrix indexing. 
	 */
	double& operator()( const size_t&i, const size_t& j) throw(not_a_matrix,index_out_of_bounds);
	
private:
	double *start ;
	virtual void update(){ start = REAL(m_sexp);}
	
	// simple is when there is no need for coercion
	// called only when the input container contains doubles
	template <typename InputIterator>
	void simple_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( REALSXP, size ) );
		std::copy( first, last, REAL(x) ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}
	
	// need to coerce to double
	template <typename InputIterator>
	void coerce_and_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( REALSXP, size ) );
		// FIXME: actually coerce
		// std::transform( first, last, REAL(x), coerce_to_double ) ;
		std::copy( first, last, REAL(x) ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}
	
	
} ;

} // namespace

#endif
