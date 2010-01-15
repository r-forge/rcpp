// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// IntegerVector.h: Rcpp R/C++ interface class library -- integer vectors
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

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/IntegerVector.h>
#include <algorithm>

namespace Rcpp{
	
	IntegerVector::IntegerVector(SEXP x) throw(not_compatible) : VectorBase(), start(0) {
		switch( TYPEOF( x ) ){
			case INTSXP:
				setSEXP( x ) ;
				update();
				break ;
			case REALSXP:
			case LGLSXP:
			case RAWSXP:
				setSEXP( Rf_coerceVector( x, INTSXP) ) ;
				update() ;
				break ;
			default:
				throw not_compatible( "cannot convert to intrger vector" ) ;
		}
	}
	
	IntegerVector::IntegerVector(int size) : VectorBase(), start(0) {
		setSEXP( Rf_allocVector(INTSXP, size) ) ;
		update() ;
	}

#ifdef HAS_INIT_LISTS	
	IntegerVector::IntegerVector( std::initializer_list<int> list ) : VectorBase(), start(0) {
		SEXP x = PROTECT( Rf_allocVector( INTSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), INTEGER(x) ); 
		setSEXP(x) ;
		update() ;
		UNPROTECT( 1 ); /* x */
	}
	IntegerVector::IntegerVector( std::initializer_list<double> list ) : VectorBase(), start(0) {
		SEXP x = PROTECT( Rf_allocVector( INTSXP, list.size() ) ) ;
		std::copy( list.begin(), list.end(), INTEGER(x) ); 
		setSEXP(x) ;
		update() ;
		UNPROTECT( 1 ); /* x */
	}
#endif

} // namespace 
