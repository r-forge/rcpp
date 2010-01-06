// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// wrap.cpp: Rcpp R/C++ interface class library -- general R object wrapper
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
#include <Rcpp/wrap.h>
#include <Rcpp/Symbol.h>
#include <Rcpp/Environment.h>
#include <algorithm>
#ifdef HAS_INIT_LISTS
#include <initializer_list>
#endif
#include <Rcpp/IntegerVector.h>
#include <Rcpp/RawVector.h>
#include <Rcpp/NumericVector.h>
#include <Rcpp/LogicalVector.h>
#include <Rcpp/GenericVector.h>

namespace Rcpp{

RObject wrap(SEXP m_sexp=R_NilValue){
	switch( TYPEOF(m_sexp) ){
		case ENVSXP:
			return Environment(m_sexp); 
		case SYMSXP:
			return Symbol(m_sexp) ;
		default:
			break ;
	}
	return RObject(m_sexp) ;
}

RObject wrap(const bool & v){
    logTxt("RObject from bool\n");
    RObject o(Rf_ScalarLogical(v));
    return o ;
}

RObject wrap(const double & v){
    logTxt("RObject from double\n");
    RObject o(Rf_ScalarReal(v));
    return o ;
}

RObject wrap(const int & v){
    logTxt("RObject from int\n");
    RObject o(Rf_ScalarInteger(v));
    return o ;
}

RObject wrap(const Rbyte & v){
    logTxt("RObject from raw\n");
    RObject o(Rf_ScalarRaw(v));
    return o ;
}

RObject wrap( const char * const v){
	return wrap( std::string(v) ) ;
}

RObject wrap(const std::string & v){
    logTxt("RObject from std::string\n");
    RObject o(Rf_mkString(v.c_str()));
    return o ;
}

RObject wrap(const std::vector<bool> & v){
    logTxt("RObject from bool vector\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(LGLSXP, n) );
    copy( v.begin(), v.end(), LOGICAL(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ; /* m_sexp now preserved by o */
    return o ;
}

RObject wrap(const std::vector<int> & v){
    logTxt("RObject from int vector\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(INTSXP, n) );
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::vector<double> & v){
    logTxt("RObject from double vector\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(REALSXP, n) );
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::vector<Rbyte> & v){
    logTxt("RObject from vector<Rbyte> \n");
    int n = v.size();
    SEXP m_sexp = PROTECT(Rf_allocVector(RAWSXP, n));
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::vector<std::string> & v){
    logTxt("RObject from std::string vector\n");
    int n = v.size();
    SEXP m_sexp = PROTECT(Rf_allocVector(STRSXP, n));
    int i=0; 
    std::vector<std::string>::const_iterator it = v.begin() ;
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

/* sets */

RObject wrap(const std::set<int> & v){
    logTxt("RObject from set<int>\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(INTSXP, n) );
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::set<double> & v){
    logTxt("RObject from set<double>\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(REALSXP, n) );
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::set<Rbyte> & v){
    logTxt("RObject from set<Rbyte> \n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(RAWSXP, n) );
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

RObject wrap(const std::set<std::string> & v){
    logTxt("RObject from set<string>\n");
    int n = v.size();
    SEXP m_sexp = PROTECT( Rf_allocVector(STRSXP, n) );
    int i=0;
    std::set<std::string>::iterator it = v.begin(); 
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
    RObject o(m_sexp) ;
    UNPROTECT(1) ;
    return o ;
}

#ifdef HAS_INIT_LISTS
IntegerVector wrap( std::initializer_list<int> list) {
	SEXP x = PROTECT( Rf_allocVector( INTSXP, list.size() ) ) ;
	std::copy( list.begin(), list.end(), INTEGER(x) ) ;
	UNPROTECT(1) ;
	return IntegerVector( x ) ;
}
NumericVector wrap( std::initializer_list<double> list) {
	SEXP x = PROTECT( Rf_allocVector( REALSXP, list.size() ) ) ;
	std::copy( list.begin(), list.end(), REAL(x) ) ;
	UNPROTECT(1) ;
	return NumericVector( x ) ;
}
LogicalVector wrap( std::initializer_list<bool> list) {
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, list.size() ) ) ;
	std::copy( list.begin(), list.end(), LOGICAL(x) ) ;
	UNPROTECT(1) ;
	return LogicalVector( x ) ;
}
RawVector wrap(std::initializer_list<Rbyte> list){
	SEXP x = PROTECT( Rf_allocVector( RAWSXP, list.size() ) ) ;
	std::copy( list.begin(), list.end(), RAW(x) ) ;
	UNPROTECT(1) ;
	return RawVector( x ) ;
}
// List wrap( std::initializer_list<RObject> list){
// 	SEXP x = PROTECT( Rf_allocVector( VECSXP, list.size() ) ) ;
// 	std::copy( list.begin(), list.end(), VECTOR_PTR(x) ) ;
// 	UNPROTECT(1) ;
// 	return List( x ) ;
// }
#endif


} // namespace Rcpp
