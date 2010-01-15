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
	
	/* grab the RCPP namespace */
	SEXP RCPP = PROTECT( R_FindNamespace( Rf_mkString( "Rcpp")  ) );
	
	/* reset the error cache */
	Rf_eval( Rf_lang1( Rf_install( "resetCurrentError" ) ), RCPP ) ; 
	
	/* grab the error handler from the Rcpp namespace */
	SEXP handler = PROTECT( Rf_findVarInFrame( RCPP, Rf_install(".rcpp_error_recorder") ) ) ;
    	
	/* call to tryCatch, we can probably do better by looking into what tryCatch does */
	SEXP trycatchcall = PROTECT( Rf_lcons( Rf_install( "tryCatch" ), 
		Rf_cons( expr,  Rf_cons( handler , R_NilValue ) ) ) ) ;
	SET_TAG( CDDR(trycatchcall), Rf_install( "error" ) ) ;
	
	/* call the tryCatch call */
	SEXP res = PROTECT( Rf_eval( trycatchcall, R_GlobalEnv ) );
	
	/* was there an error ? */
	int error = LOGICAL( Rf_eval( Rf_lang1( Rf_install("errorOccured") ), RCPP ) )[0];
	
	if( error ){
		SEXP err_msg = PROTECT( Rf_eval( 
			Rf_lang1( Rf_install("getCurrentErrorMessage")), 
			RCPP ) );
		std::string message = CHAR(STRING_ELT(err_msg,0)) ;
		UNPROTECT( 5 ) ;
		throw eval_error(message) ;
	} else {
		UNPROTECT(4) ;
		return res ;
	}
    }
    
    SEXP Evaluator::run( SEXP expr) throw(eval_error){
    	return run(expr, R_GlobalEnv );
    }
} // namespace Rcpp
