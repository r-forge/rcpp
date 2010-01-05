// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Function.h: Rcpp R/C++ interface class library -- functions (also primitives and builtins)
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

#ifndef Rcpp_Function_h
#define Rcpp_Function_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/pairlist.h>
#include <Rcpp/Evaluator.h>

namespace Rcpp{ 

/** 
 * functions
 */
class Function : public RObject{
public:
	/**
	 * Exception thrown when attempting build a Function from a SEXP 
	 * that is not appropriate
	 */
	class not_compatible: public std::exception{
		public:
			not_compatible() throw() ;
			
			/**
			 * The message: not a function 
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
	Function(SEXP lang) throw(not_compatible) ;
	
	
	/**
	 * calls the function with the specified arguments
	 *
	 * @param ...Args variable length argument list. The type of each 
	 *        argument must be wrappable, meaning there need to be 
	 *        a wrap function that takes this type as its parameter
	 *
	 */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args> 
	SEXP operator()( const Args&... args) {
		Evaluator evaluator( Rf_lcons( m_sexp, pairlist(args...) ) ) ; 
		evaluator.run() ;
		if( evaluator.successfull() ){
			return evaluator.getResult() ;
		} else{
			/* FIXME: need some strategy about error handling */
			/* throw an exception ? */
			return evaluator.getError() ;
		}
	}
#endif	
	
	~Function() ;
};

} // namespace Rcpp

#endif
