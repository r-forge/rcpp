// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppSexp.h: Rcpp R/C++ interface class library -- SEXP support
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

#include <RcppSexp.h>
#include <algorithm>

RcppSexp::RcppSexp(const bool & v) {
    logTxt("RcppSexp from bool\n");
    m_sexp = Rf_ScalarLogical(v);
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const double & v) {
    logTxt("RcppSexp from double\n");
    m_sexp = Rf_ScalarReal(v);
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const int & v) {
    logTxt("RcppSexp from int\n");
    m_sexp = Rf_ScalarInteger(v);
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const Rbyte & v) {
    logTxt("RcppSexp from raw\n");
    m_sexp = Rf_ScalarRaw(v);
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const std::string & v) {
    logTxt("RcppSexp from std::string\n");
    m_sexp = Rf_mkString(v.c_str());
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const std::vector<bool> & v) {
    logTxt("RcppSexp from bool vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(LGLSXP, n);
    R_PreserveObject(m_sexp);
    copy( v.begin(), v.end(), LOGICAL(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::vector<int> & v) {
    logTxt("RcppSexp from int vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(INTSXP, n);
    R_PreserveObject(m_sexp);
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::vector<double> & v) {
    logTxt("RcppSexp from double vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(REALSXP, n);
    R_PreserveObject(m_sexp);
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::vector<Rbyte> & v) {
    logTxt("RcppSexp from vector<Rbyte> \n");
    int n = v.size();
    m_sexp = Rf_allocVector(RAWSXP, n);
    R_PreserveObject(m_sexp);
    // copy the content of the byte vector 
    // into the raw vector
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::vector<std::string> & v) {
    logTxt("RcppSexp from std::string vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(STRSXP, n);
    R_PreserveObject(m_sexp);
    int i=0; 
    std::vector<std::string>::const_iterator it = v.begin() ;
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
}

/* sets */

RcppSexp::RcppSexp(const std::set<int> & v) {
    logTxt("RcppSexp from set<int>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(INTSXP, n);
    R_PreserveObject(m_sexp);
    copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::set<double> & v) {
    logTxt("RcppSexp from set<double>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(REALSXP, n);
    R_PreserveObject(m_sexp);
    copy( v.begin(), v.end(), REAL(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::set<Rbyte> & v) {
    logTxt("RcppSexp from set<Rbyte> \n");
    int n = v.size();
    m_sexp = Rf_allocVector(RAWSXP, n);
    R_PreserveObject(m_sexp);
    // copy the content of the byte vector 
    // into the raw vector
    copy( v.begin(), v.end(), RAW(m_sexp) ) ;
}

RcppSexp::RcppSexp(const std::set<std::string> & v) {
    logTxt("RcppSexp from set<string>\n");
    int n = v.size();
    m_sexp = Rf_allocVector(STRSXP, n);
    R_PreserveObject(m_sexp);
    int i=0; 
    std::set<std::string>::iterator it = v.begin(); 
    while( i<n ){
    	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
    	i++ ;
    	it++; 
    }
}





RcppSexp::~RcppSexp() {
    logTxt("dtor");
    R_ReleaseObject(m_sexp);
}

double RcppSexp::asDouble() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asDouble expects single value");
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
    		throw std::range_error("RcppSexp::asDouble invalid type");
    }
    return 0.0 ; 	// never reached
}

int RcppSexp::asInt() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asInt expects single value");
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
    		throw std::range_error("RcppSexp::asInt invalid type");
    }
    return 0; 	// never reached
}

Rbyte RcppSexp::asRaw() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asRaw expects single value");
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
    		throw std::range_error("RcppSexp::asRaw expects raw, double or int");
    }
    return (Rbyte)0; 	// never reached
}

bool RcppSexp::asBool() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asRaw expects single value");
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
    		throw std::range_error("RcppSexp::asRaw expects raw, double or int");
    }
    return false; 	// never reached
}

std::string RcppSexp::asStdString() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asStdString expects single value");
    }
    if (!Rf_isString(m_sexp)) {
	throw std::range_error("RcppSexp::asStdString expects string");
    }
    return std::string(CHAR(STRING_ELT(m_sexp,0)));
}

SEXP RcppSexp::asSexp() const {
    return m_sexp;
}

std::vector<bool> RcppSexp::asStdVectorBool() const {
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
    		throw std::range_error( "RcppSexp::asStdVectorBool(): invalid R type" ) ; 
    }
    return v;
}


std::vector<int> RcppSexp::asStdVectorInt() const {
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
    		throw std::range_error( "RcppSexp::asStdVectorInt(): invalid R type" ) ; 
    }
    return v;
}

std::vector<Rbyte> RcppSexp::asStdVectorRaw() const {
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
    	std::range_error("RcppSexp::asStdVectorRaw expects raw, double or int");
    }
    return v;
}

std::vector<double> RcppSexp::asStdVectorDouble() const {
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
    	std::range_error("RcppSexp::asStdVectorDouble expects raw, double or int");
    }
    return v;
}


std::vector<std::string> RcppSexp::asStdVectorString() const {
    int n = Rf_length(m_sexp);
    std::vector<std::string> v(n);
    if (!Rf_isString(m_sexp)) {
	throw std::range_error("RcppSexp::asStdVectorString expects string");
    }
    for (int i = 0; i < n; i++) {
	v[i] = std::string(CHAR(STRING_ELT(m_sexp,i)));
    }
    return v;
}

std::vector<std::string> RcppSexp::attributeNames() const {
	/* inspired from do_attributes@attrib.c */
	
	std::vector<std::string> v ;
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	v.push_back( std::string(CHAR(PRINTNAME(TAG(attrs)))) ) ;
    	attrs = CDR( attrs ) ;
    }
    return v ;
}

bool RcppSexp::hasAttribute( const std::string& attr) const {
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	if( attr == CHAR(PRINTNAME(TAG(attrs))) ){
    		return true ;
    	}
    	attrs = CDR( attrs ) ;
    }
    return false; /* give up */
}

RcppSexp RcppSexp::attr( const std::string& name) const{
	SEXP att = Rf_getAttrib( m_sexp, Rf_install( name.c_str() ) );
	return RcppSexp( att ) ;
}

bool RcppSexp::isNULL() const{
	return m_sexp == R_NilValue ;
}

RcppSexp::operator SEXP() const{
	return m_sexp ;
}

