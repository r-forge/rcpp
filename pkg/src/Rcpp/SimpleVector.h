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
#include <Rcpp/VectorBase.h>

namespace Rcpp{
	
template <int RTYPE, typename CTYPE>
class SimpleVector : public VectorBase {
public:
	SimpleVector() : VectorBase(), start(0){}
	
	SimpleVector(SEXP x) throw(not_compatible) : VectorBase(), start(0) {
		int type = TYPEOF(x) ;
		switch( type ){
			case RTYPE:
				setSEXP( x) ;
				break ;
			default:
				if( type == INTSXP || type == LGLSXP || type == CPLXSXP || type == RAWSXP || type == REALSXP ){
					setSEXP( Rf_coerceVector(x, RTYPE ) );
					break ;
				} else{
					/* TODO : include RTYPE in the message  */
					throw not_compatible( "cannot convert to simple vector" ) ;
				}
		}
	}

	SimpleVector( const size_t& size): VectorBase(), start(0){
		setSEXP( Rf_allocVector( RTYPE, size ) ) ;
	}
	
#ifdef HAS_INIT_LISTS
	SimpleVector( std::initializer_list<CTYPE> list ) : VectorBase(), start(0){
		simple_fill( list.begin() , list.end() ) ;
	}
#endif

	inline CTYPE& operator[]( const int& i ){ return start[i] ; }
	inline CTYPE* begin() const{ return start ; }
	inline CTYPE* end() const{ return start+Rf_length(m_sexp); }
	
	CTYPE& operator()( const size_t& i) throw(index_out_of_bounds){
		if( i >= static_cast<size_t>(Rf_length(m_sexp)) ) throw index_out_of_bounds() ;
		return start[i] ;
	}
	
	CTYPE& operator()( const size_t& i, const size_t& j) throw(not_a_matrix,index_out_of_bounds){
		/* TODO: factor this code out into a Offset class otr something */
		if( !Rf_isMatrix(m_sexp) ) throw not_a_matrix() ;
		int *dim = INTEGER( Rf_getAttrib( m_sexp, R_DimSymbol ) ) ;
		size_t nrow = static_cast<size_t>(dim[0]) ;
		size_t ncol = static_cast<size_t>(dim[1]) ;
		if( i >= nrow || j >= ncol ) throw index_out_of_bounds() ;
		return start[ i + nrow*j ] ;
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
	
} ;

}// namespace Rcpp

#endif
