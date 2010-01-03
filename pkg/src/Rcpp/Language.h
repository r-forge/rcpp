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

namespace Rcpp{ 

/** 
 * C++ wrapper around calls (LANGSXP SEXP)
 *
 * This represents calls that can be evaluated
 */
class Language : public RObject{
public:
	/**
	 * Exception thrown when attempting to convert a SEXP to 
	 * a call using as.call
	 */
	class not_compatible: public std::exception{
		public:
			not_compatible() throw() ;
			
			/**
			 * The message: cannot convert to call 
			 */
			const char* what() const throw() ;
			
			~not_compatible() throw() ;
		
	} ;
  	
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
	Language( const std::string& symbol ); 
	
	/**
	 * Creates a call using the given symbol as the function name
	 *
	 * @param symbol symbol name to call
	 *
	 * Language( Symbol("rnorm") ) makes a SEXP similar to this: 
	 * > call( "rnorm" )
	 */
	Language( const Symbol& symbol ); 
	
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
#ifdef CXX0X
template<typename... Args> 
	Language( const std::string& symbol, const Args&... args) : RObject(R_NilValue) {
		/* TODO: should we first allocate and protect the list  ?*/
		setSEXP( Rf_lcons( Symbol(symbol), pack( args... ) ) );
	}
#endif	
	~Language() ;

private:
	
	/* recursive packing of the arguments into a list, 
	  use first as the CAR and build the CDR from the remaining args recursively */
#ifdef CXX0X
	template<typename T, typename... Args>
	SEXP pack( const T& first, const Args&... args ){
		return Rf_cons( wrap(first), pack( args... ) ) ;
	}
#endif	
	
	/* end of the recursion, wrap first to make the CAR and use 
	   R_NilValue as the CDR of the list */
#ifdef CXX0X
template<typename T>
	SEXP pack( const T& first){
		return Rf_cons( wrap(first), R_NilValue ) ; 
	}
#endif
};

} // namespace Rcpp

#endif
