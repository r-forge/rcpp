// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RObject.cpp: Rcpp R/C++ interface class library -- SEXP support
//
// Copyright (C) 2009 Dirk Eddelbuettel
// Copyright (C) 2009 Romain Francois
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

#include <Rcpp_RObject.h>
#include <algorithm>

namespace Rcpp {

RObject::RObject(const bool & v) {
    logTxt("RObject from bool\n");
    m_sexp = Rf_ScalarLogical(v);
    protect() ;
}

RObject::RObject(const double & v) {
    logTxt("RObject from double\n");
    m_sexp = Rf_ScalarReal(v);
    protect() ;
}

RObject::RObject(const int & v) {
    logTxt("RObject from int\n");
    m_sexp = Rf_ScalarInteger(v);
    protect() ;
}

RObject::RObject(const Rbyte & v) {
    logTxt("RObject from raw\n");
    m_sexp = Rf_ScalarRaw(v);
    protect() ;
}

RObject::RObject(const std::string & v) {
    logTxt("RObject from std::string\n");
    m_sexp = Rf_mkString(v.c_str());
    protect() ;
}

RObject::RObject(const std::vector<bool> & v) {
    logTxt("RObject from bool vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(LGLSXP, n);
    protect() ;
    copy( v.begin(), v.end(), LOGICAL(m_sexp) ) ;
}

RObject::RObject(const std::vector<int> & v) {
    logTxt("RObject from int vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(INTSXP, n);
    protect() ;
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
}

RObject::RObject(const std::vector<double> & v) {
    logTxt("RObject from double vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(REALSXP, n);
    protect() ;
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
}

RObject::RObject(const std::vector<Rbyte> & v) {
    logTxt("RObject from vector<Rbyte> \n");
    int n = v.size();
    m_sexp = Rf_allocVector(RAWSXP, n);
    protect() ;
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
}

RObject::RObject(const std::vector<std::string> & v) {
    logTxt("RObject from std::string vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(STRSXP, n);
    protect() ;
    int i=0; 
    std::vector<std::string>::const_iterator it = v.begin() ;
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
}

/* sets */

RObject::RObject(const std::set<int> & v) {
    logTxt("RObject from set<int>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(INTSXP, n);
    protect() ;
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
}

RObject::RObject(const std::set<double> & v) {
    logTxt("RObject from set<double>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(REALSXP, n);
    protect() ;
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
}

RObject::RObject(const std::set<Rbyte> & v) {
    logTxt("RObject from set<Rbyte> \n");
    int n = v.size();
    m_sexp = Rf_allocVector(RAWSXP, n);
    protect() ;
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
}

RObject::RObject(const std::set<std::string> & v) {
    logTxt("RObject from set<string>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(STRSXP, n);
    protect() ;
    int i=0;
    std::set<std::string>::iterator it = v.begin(); 
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
}

RObject::~RObject() {
	release() ;
    logTxt("~RObject");
}

double RObject::asDouble() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RObject::asDouble expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return LOGICAL(m_sexp)[0] ? 1.0 : 0.0 ; 
    	case REALSXP:
    		return REAL(m_sexp)[0] ; 
    	case INTSXP:
    		return (double)INTEGER(m_sexp)[0]; 
    	case RAWSXP:
    		return (double)RAW(m_sexp)[0];
    	default:
    		throw std::range_error("RObject::asDouble invalid type");
    }
    return 0.0 ; 	// never reached
}

int RObject::asInt() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RObject::asInt expects single value");
    }
    switch( TYPEOF(m_sexp)){
    	case LGLSXP:
    		return LOGICAL(m_sexp)[0] ? 1 : 0 ; 
    	case REALSXP:
    		return (int)REAL(m_sexp)[0] ; // some of this might be lost
    	case INTSXP:
    		return INTEGER(m_sexp)[0]; 
    	case RAWSXP:
    		return (int)RAW(m_sexp)[0];
    	default:
    		throw std::range_error("RObject::asInt invalid type");
    }
    return 0; 	// never reached
}

Rbyte RObject::asRaw() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RObject::asRaw expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return LOGICAL(m_sexp)[0] ? (Rbyte)1 : (Rbyte)0 ; 
    	case REALSXP:
    		return (Rbyte)REAL(m_sexp)[0] ;
    	case INTSXP:
    		return (Rbyte)INTEGER(m_sexp)[0] ;
    	case RAWSXP:
    		return RAW(m_sexp)[0] ;
    	default:
    		throw std::range_error("RObject::asRaw expects raw, double or int");
    }
    return (Rbyte)0; 	// never reached
}

bool RObject::asBool() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RObject::asRaw expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case LGLSXP:
    		return LOGICAL(m_sexp)[0] ? true : false ; 
    	case REALSXP:
    		return (bool)REAL(m_sexp)[0] ;
    	case INTSXP:
    		return (bool)INTEGER(m_sexp)[0] ;
    	case RAWSXP:
    		return (bool)RAW(m_sexp)[0] ;
    	default:
    		throw std::range_error("RObject::asRaw expects raw, double or int");
    }
    return false; 	// never reached
}

std::string RObject::asStdString() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RObject::asStdString expects single value");
    }
    if (!Rf_isString(m_sexp)) {
	throw std::range_error("RObject::asStdString expects string");
    }
    return std::string(CHAR(STRING_ELT(m_sexp,0)));
}

std::vector<bool> RObject::asStdVectorBool() const {
    int n = Rf_length(m_sexp);
    std::vector<bool> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	v.assign( LOGICAL(m_sexp), LOGICAL(m_sexp)+n ) ;
    	break ;
    case INTSXP:
    	v.assign( INTEGER(m_sexp), INTEGER(m_sexp)+n ) ;
    	break;
    case REALSXP:
    	v.assign( REAL(m_sexp), REAL(m_sexp)+n ) ;
    	break;
    case RAWSXP:
    	v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    	break;
    default:
    		throw std::range_error( "RObject::asStdVectorBool(): invalid R type" ) ; 
    }
    return v;
}


std::vector<int> RObject::asStdVectorInt() const {
    int n = Rf_length(m_sexp);
    std::vector<int> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	v.assign( LOGICAL(m_sexp), LOGICAL(m_sexp)+n ) ;
    	break;
    case INTSXP:
    	v.assign( INTEGER(m_sexp), INTEGER(m_sexp)+n ) ;
    	break;
    case REALSXP:
    	v.assign( REAL(m_sexp), REAL(m_sexp)+n ) ;
    	break;
    case RAWSXP:
    	v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    	break;
    default:
    		throw std::range_error( "RObject::asStdVectorInt(): invalid R type" ) ; 
    }
    return v;
}

std::vector<Rbyte> RObject::asStdVectorRaw() const {
    int n = Rf_length(m_sexp);
    std::vector<Rbyte> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	v.assign( LOGICAL(m_sexp), LOGICAL(m_sexp)+n ) ;
    	break ;
    case RAWSXP:
    	v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    	break ;
    case REALSXP:
    	v.assign( REAL(m_sexp), REAL(m_sexp)+n) ;
    	break;
    case INTSXP:
    	v.assign( INTEGER(m_sexp), INTEGER(m_sexp)+n) ;
    	break;
    default:
    	std::range_error("RObject::asStdVectorRaw expects raw, double or int");
    }
    return v;
}

std::vector<double> RObject::asStdVectorDouble() const {
    int n = Rf_length(m_sexp);
    std::vector<double> v(n);
    switch( TYPEOF(m_sexp) ){
    case LGLSXP:
    	v.assign( LOGICAL(m_sexp), LOGICAL(m_sexp)+n ) ;
    	break ;
    case RAWSXP:
    	v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    	break ;
    case REALSXP:
    	v.assign( REAL(m_sexp), REAL(m_sexp)+n) ;
    	break;
    case INTSXP:
    	v.assign( INTEGER(m_sexp), INTEGER(m_sexp)+n) ;
    	break;
    default:
    	std::range_error("RObject::asStdVectorDouble expects raw, double or int");
    }
    return v;
}


std::vector<std::string> RObject::asStdVectorString() const {
    int n = Rf_length(m_sexp);
    std::vector<std::string> v(n);
    if (!Rf_isString(m_sexp)) {
	throw std::range_error("RObject::asStdVectorString expects string");
    }
    for (int i = 0; i < n; i++) {
	v[i] = std::string(CHAR(STRING_ELT(m_sexp,i)));
    }
    return v;
}




void RObject::protect(){
	if( !isProtected ){
		isProtected = true ;
		R_PreserveObject( m_sexp ); 
	}
}

void RObject::release(){
	if( isProtected ){
		R_ReleaseObject(m_sexp); 
	}
}

std::vector<std::string> RObject::attributeNames() const {
	/* inspired from do_attributes@attrib.c */
	
	std::vector<std::string> v ;
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	v.push_back( std::string(CHAR(PRINTNAME(TAG(attrs)))) ) ;
    	attrs = CDR( attrs ) ;
    }
    return v ;
}

bool RObject::hasAttribute( const std::string& attr) const {
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	if( attr == CHAR(PRINTNAME(TAG(attrs))) ){
    		return true ;
    	}
    	attrs = CDR( attrs ) ;
    }
    return false; /* give up */
}

SEXP RObject::attr( const std::string& name) const{
	return Rf_getAttrib( m_sexp, Rf_install( name.c_str() ) );
}


} // namespace Rcpp

