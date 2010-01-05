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
#include <Rcpp/pairlist.h>

namespace Rcpp{ 

/** 
 * C++ wrapper around calls (LANGSXP SEXP)
 *
 * This represents calls that can be evaluated
 */
class Pairlist : public RObject{
public:
	/**
	 * Exception thrown when attempting to convert a SEXP to 
	 * a pair list using as.pairlist
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
};

} // namespace Rcpp

#endif
