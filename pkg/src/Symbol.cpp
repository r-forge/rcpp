// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Symbol.cpp: Rcpp R/C++ interface class library -- Symbols
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

#include <Rcpp/Symbol.h>

namespace Rcpp {

	Symbol::not_compatible::not_compatible(int type) throw() {}
	Symbol::not_compatible::~not_compatible() throw() {}
	const char* Symbol::not_compatible::what() const throw() {
		return "not compatible with Symbol, excepting SYMSXP, CHARSXP or STRSXP" ;
	}
	
	Symbol::Symbol( SEXP x = R_NilValue ) throw(not_compatible) : RObject::RObject() {
		if( x != R_NilValue ){
			int type = TYPEOF(x) ;
			switch( type ){
			case SYMSXP:
				break; /* nothing to do */
			case CHARSXP:
				setSEXP( Rf_install(CHAR(x)) ) ;
				break ;
			case STRSXP:
				{
					/* FIXME: check that there is at least one element */
					setSEXP( Rf_install( CHAR(STRING_ELT(x, 0 )) ) );
					break ;
				}
			default:
				throw not_compatible(type) ;
			}
		} else {
			setSEXP( x ) ;
		}
	}
	
	Symbol::Symbol(const std::string& symbol){
		setSEXP( Rf_install(symbol.c_str()) );
	}
	
	Symbol::~Symbol(){}
	
} // namespace Rcpp

