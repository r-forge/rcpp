// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Language.cpp: Rcpp R/C++ interface class library -- Language objects ( calls )
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

#include <Rcpp/Language.h>
#include <Rcpp/Evaluator.h>
#include <RcppCommon.h>

namespace Rcpp {
	
	Language::Language( SEXP lang = R_NilValue ) throw(not_compatible) : RObject::RObject( ){
		/* if this is not trivially a call, then try to convert it to one */
		if( lang != R_NilValue && TYPEOF(lang) != LANGSXP ){
	    		
	    		/* taken from do_ascall */
	    		switch( TYPEOF(lang) ){
	    		case LISTSXP :
	    			Rf_duplicate( lang ) ;
	    			break ;
	    		case VECSXP:
	    		case EXPRSXP:
	    			{
	    				int n = Rf_length(lang) ;
	    				if( n == 0 ) throw not_compatible() ;
	    				SEXP names = GET_NAMES(lang) ; 
	    				SEXP res, ap;
	    				PROTECT( ap = res = Rf_allocList( n ) ) ;
	    				for( int i=0; i<n; i++){
	    					SETCAR(ap, VECTOR_ELT(lang, i));
	    					if (names != R_NilValue && !Rf_StringBlank(STRING_ELT(names, i))){
	    						SET_TAG(ap, Rf_install(Rf_translateChar(STRING_ELT(names, i))));
	    					}
	    					ap = CDR( ap) ;
	    				}
	    				UNPROTECT(1) ;
	    				setSEXP(res) ; 
	    			}
	    		default:
	    			throw not_compatible() ;
	    		}
	    		SET_TYPEOF(m_sexp, LANGSXP);
	    		SET_TAG(m_sexp, R_NilValue);
		} else{
			setSEXP( lang ) ;
		}
		
	};
	
	Language::Language( const std::string& symbol ): RObject::RObject(R_NilValue) {
		setSEXP( Rf_lcons( Symbol(symbol), R_NilValue ) );
	}
	
	Language::Language( const Symbol& symbol ){
		setSEXP( Rf_lcons( symbol, R_NilValue ) ) ;
	}
	
	Language::~Language(){}
	
	void Language::setSymbol( const std::string& symbol){
		setSymbol( Symbol( symbol ) ) ;
	}
	
	void Language::setSymbol( const Symbol& symbol){
		SETCAR( m_sexp, symbol ) ;
		SET_TAG(m_sexp, R_NilValue);
	}
	
	Language::not_compatible::not_compatible() throw() {}
    	const char* Language::not_compatible::what() const throw(){
    		return "cannot convert to call" ;
    	}
    	Language::not_compatible::~not_compatible() throw() {}
    
	
	
} // namespace Rcpp
