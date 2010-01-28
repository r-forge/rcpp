// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// DottedPair.cpp: Rcpp R/C++ interface class library -- dotted pair lists
// base class of Language and Pairlist
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

#include <Rcpp/DottedPair.h>

namespace Rcpp {
	DottedPair::~DottedPair(){}
	DottedPair::DottedPair() : RObject(){}
	
	void DottedPair::remove( const size_t& index ) throw(index_out_of_bounds) {
		if( index < 0 || index >= static_cast<size_t>(Rf_length(m_sexp)) ) throw index_out_of_bounds() ;
		if( index == 0 ){
			setSEXP( CDR( m_sexp) ) ;
		} else{
			SEXP x = m_sexp ;
			size_t i=1;
			while( i<index ){ x = CDR(x) ; i++; }
			SETCDR( x, CDDR(x) ) ;
		}
	}
	
	DottedPair::Proxy::Proxy( DottedPair& v, const size_t& index_ ) : 
		parent(v), index(index_){}
	
	DottedPair::Proxy& DottedPair::Proxy::operator=(const Proxy& rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		size_t i = 0 ;
		SEXP x = parent.asSexp() ; 
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SEXP y = rhs ; /* implicit conversion */
		SETCAR( x, y ) ;
		// if( index != 0 ) SET_TAG( x, Rf_install( rhs.getTag() ) ) ;
		return *this ;
	}
	
	DottedPair::Proxy& DottedPair::Proxy::operator=(SEXP rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SETCAR( x, rhs) ;
		return *this ;
	}
	
	DottedPair::Proxy& DottedPair::Proxy::operator=(const Named& rhs){
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		size_t i = 0 ;
		SEXP x = parent.asSexp() ; 
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		SEXP y = rhs.getSEXP() ;
		SETCAR( x, y ) ;
		if( index != 0 ) SET_TAG( x, Symbol( rhs.getTag() ) ) ;
		return *this ;
	}
		
	DottedPair::Proxy::operator SEXP() {
		if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
		SEXP x = parent.asSexp() ; 
		size_t i = 0 ;
		while( i < index ) {
			x = CDR(x) ;
			i++ ;
		}
		return CAR(x) ;
	}
		
	const DottedPair::Proxy DottedPair::operator[]( int i ) const {
		return Proxy( const_cast<DottedPair&>(*this), i) ;
	}
	DottedPair::Proxy DottedPair::operator[]( int i ) {
		return Proxy( *this, i );
	}
	
	
} // namespace Rcpp
