// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Language.h: Rcpp R/C++ interface class library -- language objects (calls)
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

#ifndef Rcpp_Language_h
#define Rcpp_Language_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Symbol.h>
#include <Rcpp/Pairlist.h>

namespace Rcpp{ 

/** 
 * C++ wrapper around calls (LANGSXP SEXP)
 *
 * This represents calls that can be evaluated
 */
class Language : public RObject{
public:

	/**
	 * Attempts to convert the SEXP to a call
	 *
	 * @throw not_compatible if the SEXP could not be converted
	 * to a call using as.call
	 */
	Language(SEXP lang) throw(not_compatible) ;

	/**
	 * Creates a call using the given symbol as the function name
	 *
	 * @param symbol symbol name to call
	 *
	 * Language( "rnorm" ) makes a SEXP similar to this (expressed in R)
	 * > as.call( as.list( as.name( "rnorm") ) )
	 * > call( "rnorm" )
	 */
	explicit Language( const std::string& symbol ); 

	/**
	 * Creates a call using the given symbol as the function name
	 *
	 * @param symbol symbol name to call
	 *
	 * Language( Symbol("rnorm") ) makes a SEXP similar to this: 
	 * > call( "rnorm" )
	 */
	explicit Language( const Symbol& symbol ); 

	/**
	 * Creates a call to the given symbol using variable number of 
	 * arguments
	 *
	 * @param symbol symbol
	 * @param ...Args variable length argument list. The type of each 
	 *        argument must be wrappable, meaning there need to be 
	 *        a wrap function that takes this type as its parameter
	 * 
	 * @example Language( "rnorm", 10, 0.0 ) 
	 * will create the same call as 
	 * > call( "rnorm", 10L, 0.0 )
	 *
	 * 10 is wrapped as an integer vector using wrap( const& int )
	 * 0.0 is wrapped as a numeric vector using wrap( const& double )
	 * ...
	 */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args> 
	Language( const std::string& symbol, const Args&... args) : RObject() {
		/* TODO: should we first allocate and protect the list  ?*/
		setSEXP( Rf_lcons( Symbol(symbol), pairlist( args... ) ) );
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
	 * wraps an object and add it in front of the pairlist
	 *
	 * @param object anything that can be wrapped by one 
	 * of the wrap functions, or an object of class Named
	 */
	template <typename T>
	void push_front( const T& object){
		setSEXP( grow(object, m_sexp) ) ;
		SET_TAG(m_sexp, R_NilValue);
		SET_TYPEOF(m_sexp, LANGSXP);
	}


	template <typename T>
	void insert( const int& index, const T& object) throw(index_out_of_bounds) {
		if( index == 0 ) {
			push_front( object ) ;
		} else{
			if( index <  0 ) throw index_out_of_bounds() ;
			if( isNULL( ) ) throw index_out_of_bounds() ;
			
			if( index < 0 || index > Rf_length(m_sexp) ) throw index_out_of_bounds() ;
			
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
	 * sets the symbol of the call
	 */
	void setSymbol( const std::string& symbol);

	/**
	 * sets the symbol of the call
	 */
	void setSymbol( const Symbol& symbol ) ;

	~Language() ;
};

} // namespace Rcpp

#endif
