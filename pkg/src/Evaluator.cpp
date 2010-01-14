// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Evaluator.cpp: Rcpp R/C++ interface class library -- evaluator
//
// Copyright (C) 2009 - 2010	Dirk Eddelbuettel and Romain Francois
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

#include <Rcpp/Evaluator.h>
#include <Rcpp/Environment.h>

namespace Rcpp {

    Evaluator::eval_error::eval_error( const std::string& message) throw() :
    	message(message){}
    Evaluator::eval_error::~eval_error( ) throw(){} ;
    const char* Evaluator::eval_error::what() const throw(){ return message.c_str() ; }

   SEXP Evaluator::run(SEXP expr, SEXP env) throw(eval_error) {
	int error = 0 ;
	SEXP res = PROTECT( R_tryEval( expr, env, &error ) ) ;
	if( error ){
		UNPROTECT( 1 ) ; /* res */
		SEXP message = PROTECT( Rf_eval( Rf_lang1( Rf_install("geterrmessage") ), R_GlobalEnv ) );
		std::string err_msg ;
		if( TYPEOF( message ) == STRSXP && Rf_length( message ) ){
			err_msg = CHAR( STRING_ELT( message, 0 ) ) ;
		} else{
			err_msg = "error with no message" ;
		}
		UNPROTECT(1) ; /* message */
		throw eval_error( err_msg ) ;
	} else {
		UNPROTECT( 1) ; /* res */
		return res ;
	}
    }
    
    SEXP Evaluator::run( SEXP expr) throw(eval_error){
    	return run(expr, R_GlobalEnv );
    }
} // namespace Rcpp
