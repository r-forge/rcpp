// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// DottedPair.h: Rcpp R/C++ interface class library -- dotted pair list template
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

#ifndef Rcpp_DottedPair_h
#define Rcpp_DottedPair_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Symbol.h>
#include <Rcpp/grow.h>
#include <Rcpp/wrap.h>

namespace Rcpp{ 

template <int RTYPE> class DottedPair : public RObject{
public:

	DottedPair() : RObject(){}
	
	DottedPair(SEXP x) throw(not_compatible) : RObject(){
		setSEXP( r_cast<RTYPE>(x) ) ;
	}

#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args> 
	DottedPair( const Args&... args) : RObject() {
		setSEXP( pairlist(args...) ) ;
	}
#endif	

	/**
	 * wraps an object and add it at the end of the pairlist
	 * (this require traversing the entire pairlist)
	 *
	 * @param object anything that can be wrapped by one 
	 * of the wrap functions, or an object of class Named
	 */
	template <typename T>
	void push_back( const T& object){
		if( isNULL() ){
			setSEXP( grow( object, m_sexp ) ) ;
		} else {
			SEXP x = m_sexp ;
			/* traverse the pairlist */
			while( !Rf_isNull(CDR(x)) ){
				x = CDR(x) ;
			}
			SEXP tail = PROTECT( pairlist( object ) ); 
			SETCDR( x, tail ) ;
			UNPROTECT(1) ;
		}
	}

	/**
	 * wraps an object and add it in front of the pairlist. 
	 *
	 * @param object anything that can be wrapped by one 
	 * of the wrap functions, or an object of class Named
	 */
	template <typename T>
	void push_front( const T& object){
		setSEXP( grow(object, m_sexp) ) ;
	}

	/**
	 * insert an object at the given position, pushing other objects
	 * to the tail of the list
	 *
	 * @param index index (0-based) where to insert
	 * @param object object to wrap
	 */
	template <typename T>
	void insert( const int& index, const T& object) throw(index_out_of_bounds) {
		if( index == 0 ) {
			push_front( object ) ;
		} else{
			if( index <  0 ) throw index_out_of_bounds() ;
			if( isNULL( ) ) throw index_out_of_bounds() ;
			
			if( index < 0 || index > ::Rf_length(m_sexp) ) throw index_out_of_bounds() ;
			
			int i=1;
			SEXP x = m_sexp ;
			while( i < index ){
				x = CDR(x) ;
				i++; 
			}
			SEXP tail = PROTECT( grow( object, CDR(x) ) ) ; 
			SETCDR( x, tail ) ;
			UNPROTECT(1) ;
		}
	}
	
	/**
	 * replaces an element of the list
	 *
	 * @param index position
	 * @param object object that can be wrapped
	 */
	template <typename T>
	void replace( const int& index, const T& object ) throw(index_out_of_bounds){
 	        if( index < 0 || index >= ::Rf_length(m_sexp) ) throw index_out_of_bounds() ;
		
		/* pretend we do a pairlist so that we get Named to work for us */
		SEXP x = PROTECT(pairlist( object ));
		SEXP y = m_sexp ;
		int i=0;
		while( i<index ){ y = CDR(y) ; i++; }
		
		SETCAR( y, CAR(x) );
		SET_TAG( y, TAG(x) );
		UNPROTECT(1) ;
	}

        inline size_t length() const { return ::Rf_length(m_sexp) ; }
        inline size_t size() const { return ::Rf_length(m_sexp) ; }
	
	/**
	 * Remove the element at the given position
	 *
	 * @param index position where the element is to be removed
	 */
	void remove( const int& index ) throw(index_out_of_bounds){
		if( index < 0 || index >= Rf_length(m_sexp) ) throw index_out_of_bounds() ;
		if( index == 0 ){
			setSEXP( CDR( m_sexp) ) ;
		} else{
			SEXP x = m_sexp ;
			int i=1;
			while( i<index ){ x = CDR(x) ; i++; }
			SETCDR( x, CDDR(x) ) ;
		}
	}
	
	class Proxy {
	public:
		Proxy( DottedPair<RTYPE>& v, const size_t& index_ ) : parent(v), index(index_){}
		
		/* lvalue uses */
		Proxy& operator=(const Proxy& rhs){
			if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
			size_t i = 0 ;
			SEXP x = parent.asSexp() ; 
			while( i < index ) {
				x = CDR(x) ;
				i++ ;
			}
			SEXP y = rhs ;
			SETCAR( x, y ) ;
			// if( index != 0 ) SET_TAG( x, Rf_install( rhs.getTag() ) ) ;
			return *this ;
		}
		Proxy& operator=(SEXP rhs){
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
		
		template <typename T>
		Proxy& operator=(const T& rhs){
			parent.replace( index, rhs ) ;
			return *this ;
		}
		Proxy& operator=(const Named& rhs){
			if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
			size_t i = 0 ;
			SEXP x = parent.asSexp() ; 
			while( i < index ) {
				x = CDR(x) ;
				i++ ;
			}
			SEXP y = rhs ;
			SETCAR( x, y ) ;
			// if( index != 0 ) SET_TAG( x, Symbol( rhs.getTag() ) ) ;
			return *this ;
		}
		
		/* rvalue use */
		operator SEXP() {
			if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
			SEXP x = parent.asSexp() ; 
			size_t i = 0 ;
			while( i < index ) {
				x = CDR(x) ;
				i++ ;
			}
			return CAR(x) ;
		}
		
		template <typename T> operator T() const {
			if( index < 0 || index >= parent.length() ) throw index_out_of_bounds() ;
			SEXP x = parent.asSexp() ; 
			size_t i = 0 ;
			while( i < index ) {
				x = CDR(x) ;
				i++ ;
			}
			return as<T>( CAR(x) ) ;
		}
		
	private:
		DottedPair<RTYPE>& parent; 
		size_t index ;
	} ;
	
	const Proxy operator[]( int i ) const {
		return Proxy( const_cast<DottedPair<RTYPE>&>(*this), i) ;
	}
	Proxy operator[]( int i ) {
		return Proxy( *this, i );
	}
	
	friend class Proxy; 
	
	virtual ~DottedPair() {};
	
};

} // namespace Rcpp

#endif
