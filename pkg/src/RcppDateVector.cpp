// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppDateVector.cpp: Rcpp R/C++ interface class library -- Date vector support
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

#include <RcppDateVector.h>

RcppDateVector::RcppDateVector(SEXP vec) {
    int i;
    if (!Rf_isNumeric(vec) || Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppDateVector: invalid numeric vector in constructor");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppDateVector: null vector in constructor");
    v = new RcppDate[len];
    for (i = 0; i < len; i++)
	v[i] = RcppDate((int)REAL(vec)[i]);
    length = len;
}

RcppDateVector::~RcppDateVector() {
    delete [] v;
}

RcppDate& RcppDateVector::operator()(int i) {
    if (i < 0 || i >= length) {
	std::ostringstream oss;
	oss << "RcppDateVector: subscript out of range: " << i;
	throw std::range_error(oss.str());
    }
    return v[i];
}

int RcppDateVector::size() const { 
    return length; 
}
