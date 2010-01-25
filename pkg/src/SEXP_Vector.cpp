// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SEXP_Vector.cpp: Rcpp R/C++ interface class library -- template for expression and generic vectors
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

#include <Rcpp/SEXP_Vector.h>

namespace Rcpp{
	
template<> SEXP converter<VECSXP>(SEXP x){
	return convert_using_rfunction( x, "as.list" ) ;
}
template<> SEXP converter<EXPRSXP>(SEXP x){
	return convert_using_rfunction( x, "as.expression" ) ;
}

SEXP convert_using_rfunction(SEXP x, const char* fun){
	SEXP res = R_NilValue ; 
	try{
		res = Evaluator::run( Rf_lang2( Rf_install( fun ), x ) );
	} catch( const Evaluator::eval_error& ex){
		throw RObject::not_compatible( "not compatible" ) ;
	}
	return res ;
}


} // namespace Rcpp

