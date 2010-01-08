// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Pairlist.h: Rcpp R/C++ interface class library -- pair lists objects (LISTSXP)
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

#ifndef Rcpp_Pairlist_h
#define Rcpp_Pairlist_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

namespace Rcpp{ 

/** 
 * C++ wrapper around pair lists (LISTSXP SEXP)
 *
 * This represents dotted pair lists
 */
class Pairlist : public RObject{
public:
	
	/**
	 * Attempts to convert the SEXP to a pair list
	 *
	 * @throw not_compatible if the SEXP could not be converted
	 * to a pair list using as.pairlist
	 */
	Pairlist(SEXP lang) throw(not_compatible) ;
	
	/**
	 * Creates a pairlist by wrapping the variable number of arguments
	 * using the pairlist template
	 *
	 * @param ...Args variable length argument list. The type of each 
	 *        argument must be wrappable, meaning there need to be 
	 *        a wrap function that takes this type as its parameter
	 * 
	 * @example Pairlist( 10, std::string("foobar"), "rnorm" ) 
	 * will create the same pair list as
	 * > pairlist( 10L, "foobar", "rnorm" )
	 */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args> 
	Pairlist( const Args&... args) : RObject() {
		/* TODO: should we first allocate and protect the list  ?*/
		setSEXP( pairlist( args... ) );
	}
#endif	
	
	~Pairlist() ;
	
	/**
	 * wraps an object and add it in front of the pairlist
	 *
	 * @param object anything that can be wrapped by one 
	 * of the wrap functions, or an object of class Named
	 */
	template <typename T>
	void push_front( const T& object){
		setSEXP( grow(object, m_sexp) ) ;
	}

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

};

#ifdef HAS_VARIADIC_TEMPLATES
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
