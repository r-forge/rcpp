// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppNumList.cpp: Rcpp R/C++ interface class library -- numlist support
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
//
// This library is free software; you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published by 
// the Free Software Foundation; either version 2.1 of the License, or (at 
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but 
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License 
// along with this library; if not, write to the Free Software Foundation, 
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

#include <RcppNumList.h>

RcppNumList::RcppNumList(SEXP theList) {
    if (!Rf_isNewList(theList))
	throw std::range_error("RcppNumList: non-list passed to constructor");
    len = Rf_length(theList);
    names = Rf_getAttrib(theList, R_NamesSymbol);
    namedList = theList;
}

std::string RcppNumList::getName(int i) const {
    if (i < 0 || i >= len) {
	std::ostringstream oss;
	oss << "RcppNumList::getName: index out of bounds: " << i;
	throw std::range_error(oss.str());
    }
    return std::string(CHAR(STRING_ELT(names,i)));
}

double RcppNumList::getValue(int i) const {
    if (i < 0 || i >= len) {
	std::ostringstream oss;
	oss << "RcppNumList::getValue: index out of bounds: " << i;
	throw std::range_error(oss.str());
    }
    SEXP elt = VECTOR_ELT(namedList, i);
    if (Rf_isReal(elt))
	return REAL(elt)[0];
    else if (Rf_isInteger(elt))
	return (double)INTEGER(elt)[0];
    else
	throw std::range_error("RcppNumList: contains non-numeric value");
    return 0; // never get here
}

int RcppNumList::size() const { 
    return len; 
}
