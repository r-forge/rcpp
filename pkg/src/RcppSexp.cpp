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
    m_sexp = PROTECT(Rf_allocVector(REALSXP, 1));
    m_nprot++;
    REAL(m_sexp)[0] = v;
}

RcppSexp::RcppSexp(const int & v) {
    m_sexp = PROTECT(Rf_allocVector(INTSXP, 1));
    m_nprot++;
    INTEGER(m_sexp)[0] = v;
}

RcppSexp::RcppSexp(const std::string & v) {
    m_sexp = PROTECT(Rf_allocVector(STRSXP, 1));
    m_nprot++;
    SET_STRING_ELT(m_sexp, 0, Rf_mkChar(v.c_str()));
}

RcppSexp::~RcppSexp() {
    UNPROTECT(m_nprot);
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

