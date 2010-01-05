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

#include <Rcpp/Pairlist.h>
#include <Rcpp/Evaluator.h>
#include <Rcpp/RObject.h>
#include <Rcpp/Language.h>
#include <RcppCommon.h>

namespace Rcpp {
	
	Pairlist::Pairlist( SEXP x = R_NilValue ) throw(not_compatible) : RObject::RObject( ){
		if( x != R_NilValue ){
			switch( TYPEOF(x) ){
				case LANGSXP:
				case LISTSXP:
					setSEXP( x) ; 
					break ;
				default:
					{
						Evaluator evaluator( Rf_lang2( Rf_install("as.pairlist"), x ) ) ;
						evaluator.run() ;
						if( evaluator.successfull() ){
    							setSEXP( evaluator.getResult().asSexp() ) ;
    						} else{
    							throw not_compatible( ) ; 
    						}
					}
			}
		}          
		
	};
	
	Pairlist::~Pairlist(){}
	
	Pairlist::not_compatible::not_compatible() throw() {}
    	const char* Pairlist::not_compatible::what() const throw(){
    		return "cannot convert to pair list" ;
    	}
    	Pairlist::not_compatible::~not_compatible() throw() {}
    
	
	
} // namespace Rcpp
