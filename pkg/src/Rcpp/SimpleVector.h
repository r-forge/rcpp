// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SimpleVector.h: Rcpp R/C++ interface class library -- simple vectors (INTSXP,REALSXP,RAWSXP,LGLSXP,CPLXSXP)
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

#ifndef Rcpp_SimpleVector_h
#define Rcpp_SimpleVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>
#include <Rcpp/r_cast.h>
#include <Rcpp/Dimension.h>

namespace Rcpp{
	
template <int RTYPE, typename CTYPE>
class SimpleVector : public VectorBase {
public:
	SimpleVector() : VectorBase(), start(0){}
	
	SimpleVector(SEXP x) throw(RObject::not_compatible) : VectorBase(), start(0){
		SEXP y = r_cast<RTYPE>( x ) ;
		setSEXP( y );
	}
	
	SimpleVector( const size_t& size){
		setSEXP( Rf_allocVector( RTYPE, size) ) ;
		init() ;
	}
	
	SimpleVector( const Dimension& dims){
		setSEXP( Rf_allocVector( RTYPE, dims.prod() ) ) ;
		init() ;
		if( dims.size() > 1 ){
			attr( "dim" ) = dims ;
		}
	}
	
#ifdef HAS_INIT_LISTS
	SimpleVector( std::initializer_list<CTYPE> list ) : VectorBase(), start(0){
		simple_fill( list.begin() , list.end() ) ;
	}
#endif

	inline CTYPE& operator[]( const int& i ){ return start[i] ; }
	inline CTYPE* begin() const{ return start ; }
	inline CTYPE* end() const{ return start+Rf_length(m_sexp); }
	
	inline CTYPE& operator()( const size_t& i) throw(RObject::index_out_of_bounds){
		return start[ offset(i) ] ;
	}
	
	inline CTYPE& operator()( const size_t& i, const size_t& j) throw(VectorBase::not_a_matrix,RObject::index_out_of_bounds){
		return start[ offset(i,j) ] ;
	}

protected:
	/* TODO: make this private ASAP */
	CTYPE* start ;
	
private:
	virtual void update(){ start = get_pointer<RTYPE,CTYPE>(m_sexp) ; }
	
	// simple is when there is no need for coercion
	// called only when the input container contains doubles
	// TODO: use traits or something to make sure this is an 
	//       iterator over RTYPE
	template <typename InputIterator>
	void simple_fill( InputIterator first, InputIterator last){
		size_t size = std::distance( first, last) ;
		SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
		std::copy( first, last, get_pointer<RTYPE,CTYPE>(x) ) ;
		setSEXP( x ) ;
		UNPROTECT(1) ;
	}
	
	void init(){
		size_t n = static_cast<size_t>(length()) ;
		CTYPE zero = static_cast<CTYPE>(0) ;
		for( size_t i=0; i<n; i++){
			start[i] = zero;
		}
	}
	
} ;

}// namespace Rcpp

#endif
