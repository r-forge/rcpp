// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppSexp.h: Rcpp R/C++ interface class library -- SEXP support
//
// Copyright (C) 2009 Dirk Eddelbuettel
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

RcppSexp::RcppSexp(const double & v) {
    logTxt("RcppSexp from double\n");
    m_sexp = Rf_allocVector(REALSXP, 1);
    R_PreserveObject(m_sexp);
    REAL(m_sexp)[0] = v;
}

RcppSexp::RcppSexp(const int & v) {
    logTxt("RcppSexp from int\n");
    m_sexp = Rf_allocVector(INTSXP, 1);
    R_PreserveObject(m_sexp);
    INTEGER(m_sexp)[0] = v;
}

RcppSexp::RcppSexp(const Rbyte & v) {
    logTxt("RcppSexp from raw\n");
    m_sexp = Rf_ScalarRaw(v);
    R_PreserveObject(m_sexp);
}

RcppSexp::RcppSexp(const std::string & v) {
    logTxt("RcppSexp from std::string\n");
    m_sexp = Rf_allocVector(STRSXP, 1);
    R_PreserveObject(m_sexp);
    SET_STRING_ELT(m_sexp, 0, Rf_mkChar(v.c_str()));
}

RcppSexp::RcppSexp(const std::vector<int> & v) {
    logTxt("RcppSexp from int vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(INTSXP, n);
    R_PreserveObject(m_sexp);
    for (int i = 0; i < n; i++) {
	INTEGER(m_sexp)[i] = v[i];
    }	
}

RcppSexp::RcppSexp(const std::vector<double> & v) {
    logTxt("RcppSexp from double vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(REALSXP, n);
    R_PreserveObject(m_sexp);
    for (int i = 0; i < n; i++) {
	REAL(m_sexp)[i] = v[i];
    }	
}

RcppSexp::RcppSexp(const std::vector<Rbyte> & v) {
    logTxt("RcppSexp from vector<Rbyte> \n");
    int n = v.size();
    m_sexp = Rf_allocVector(RAWSXP, n);
    R_PreserveObject(m_sexp);
    for (int i = 0; i < n; i++) {
	RAW(m_sexp)[i] = v[i];
    }	
}

RcppSexp::RcppSexp(const std::vector<std::string> & v) {
    logTxt("RcppSexp from std::string vector\n");
    int n = v.size();
    m_sexp = Rf_allocVector(STRSXP, n);
    R_PreserveObject(m_sexp);
    for (int i = 0; i < n; i++) {
	SET_STRING_ELT(m_sexp, i, Rf_mkChar(v[i].c_str()));
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
    if (!Rf_isNumeric(m_sexp)) {
	throw std::range_error("RcppSexp::asDouble expect numeric type");
    }
    if (Rf_isInteger(m_sexp)) {
	return (double)INTEGER(m_sexp)[0];
    } else if (Rf_isReal(m_sexp)) {
	return REAL(m_sexp)[0];
    } else {
	throw std::range_error("RcppSexp::asDouble invalid type");
    }
    return 0; 	// never reached
}

int RcppSexp::asInt() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asInt expects single value");
    }
    if (!Rf_isNumeric(m_sexp)) {
	throw std::range_error("RcppSexp::asInt expects numeric type");
    }
    if (Rf_isInteger(m_sexp)) {
	return INTEGER(m_sexp)[0];
    } else	if (Rf_isReal(m_sexp)) {
	return (int)REAL(m_sexp)[0];
    } else {
	std::string mesg = "RcppParams::asInt unknown type";
    }
    return 0; 	// never reached
}

Rbyte RcppSexp::asRaw() const {
    if (Rf_length(m_sexp) != 1) {
	throw std::range_error("RcppSexp::asRaw expects single value");
    }
    switch( TYPEOF(m_sexp) ){
    	case RAWSXP:
    		return RAW(m_sexp)[0] ;
    	case INTSXP:
    		return (Rbyte)INTEGER(m_sexp)[0] ;
    	case REALSXP:
    		return (Rbyte)REAL(m_sexp)[0] ;
    	default:
    		throw std::range_error("RcppSexp::asRaw expects raw, double or int");
    }
    return (Rbyte)0; 	// never reached
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

std::vector<int> RcppSexp::asStdVectorInt() const {
    int n = Rf_length(m_sexp);
    std::vector<int> v(n);
    if (Rf_isInteger(m_sexp)) {
	for (int i = 0; i < n; i++) {
	    v[i] = INTEGER(m_sexp)[i];
	}
    } else if (Rf_isReal(m_sexp)) {
	for (int i = 0; i < n; i++) {
	    v[i] = (int)REAL(m_sexp)[i];
	}
    }
    return v;
}

std::vector<Rbyte> RcppSexp::asStdVectorRaw() const {
    int n = Rf_length(m_sexp);
    std::vector<Rbyte> v(n);
    switch( TYPEOF(m_sexp) ){
    case RAWSXP:
    	{
    		v.assign( RAW(m_sexp), RAW(m_sexp)+n ) ;
    		break ;
    	}
    case REALSXP:
    	{
    		for (int i = 0; i < n; i++) {
			    v[i] = (Rbyte)REAL(m_sexp)[i];
			}
			break ;
    	}
    case INTSXP:
    	{
    		for (int i = 0; i < n; i++) {
			    v[i] = (Rbyte)INTEGER(m_sexp)[i];
			}
    		break;
    	}
    default:
    	std::range_error("RcppSexp::asStdVectorRaw expects raw, double or int");
    }
    return v;
}

std::vector<double> RcppSexp::asStdVectorDouble() const {
    int n = Rf_length(m_sexp);
    std::vector<double> v(n);
    if (Rf_isInteger(m_sexp)) {
	for (int i = 0; i < n; i++) {
	    v[i] = (double)INTEGER(m_sexp)[i];
	}
    } else if (Rf_isReal(m_sexp)) {
	for (int i = 0; i < n; i++) {
	    v[i] = REAL(m_sexp)[i];
	}
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

