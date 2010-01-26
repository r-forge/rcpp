// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// rcast.h: Rcpp R/C++ interface class library -- cast from one SEXP type to another
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

#include <Rcpp/r_cast.h>

namespace Rcpp{
namespace internal{

template<> SEXP r_true_cast<INTSXP>(SEXP x){
	switch( TYPEOF(x) ){
	case REALSXP:
	case RAWSXP:
	case LGLSXP:
	case CPLXSXP:
		return Rf_coerceVector( x, INTSXP) ;
	default:
		throw RObject::not_compatible( "not compatible with INTSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}

template<> SEXP r_true_cast<REALSXP>( SEXP x){
	switch( TYPEOF( x ) ){
	case INTSXP:
	case LGLSXP:
	case CPLXSXP:
	case RAWSXP:
		return Rf_coerceVector( x, REALSXP );
	default:
		throw RObject::not_compatible( "not compatible with INTSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}

template<> SEXP r_true_cast<LGLSXP>( SEXP x){
	switch( TYPEOF( x ) ){
	case REALSXP:
	case INTSXP:
	case CPLXSXP:
	case RAWSXP:
		return Rf_coerceVector( x, LGLSXP );
	default:
		throw RObject::not_compatible( "not compatible with LGLSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}

template<> SEXP r_true_cast<RAWSXP>( SEXP x){
	switch( TYPEOF( x ) ){
	case LGLSXP:
	case REALSXP:
	case INTSXP:
	case CPLXSXP:
		return Rf_coerceVector( x, RAWSXP );
	default:
		throw RObject::not_compatible( "not compatible with RAWSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}


template<> SEXP r_true_cast<CPLXSXP>( SEXP x){
	switch( TYPEOF( x ) ){
	case RAWSXP:
	case LGLSXP:
	case REALSXP:
	case INTSXP:
		return Rf_coerceVector( x, CPLXSXP );
	default:
		throw RObject::not_compatible( "not compatible with CPLXSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}

template<> SEXP r_true_cast<STRSXP>( SEXP x){
	switch( TYPEOF( x ) ){
	case CPLXSXP:
	case RAWSXP:
	case LGLSXP:
	case REALSXP:
	case INTSXP:
		return Rf_coerceVector( x, STRSXP );
	case CHARSXP:
		return Rf_ScalarString( x ) ;
	case SYMSXP:
		return Rf_ScalarString( PRINTNAME( x ) ) ; 
	default:
		throw RObject::not_compatible( "not compatible with STRSXP" ) ;
	}
	return R_NilValue ; /* -Wall */
}

template<> SEXP r_true_cast<VECSXP>(SEXP x){
	return convert_using_rfunction(x, "as.list" ) ;
}
    
template<> SEXP r_true_cast<EXPRSXP>(SEXP x){
	return convert_using_rfunction(x, "as.expression" ) ;
}

template<> SEXP r_true_cast<LISTSXP>(SEXP x){
	return convert_using_rfunction(x, "as.pairlist" ) ;
}


} // namespace internal
} // namespace Rcpp

