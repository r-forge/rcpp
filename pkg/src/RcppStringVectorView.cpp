// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppStringVectorView.cpp: Rcpp R/C++ interface class library -- string vector view
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

#include <RcppStringVectorView.h>

RcppStringVectorView::RcppStringVectorView(SEXP vec) {
    //int i;
    if (Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppStringVectorView: invalid numeric vector in constructor");
    if (!Rf_isString(vec))
	throw std::range_error("RcppStringVectorView: invalid string");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppStringVectorView: null vector in constructor");
    //v = new (char *)[len];
    //for (i = 0; i < len; i++)
    //	v[i] = string(CHAR(STRING_ELT(vec,i)));
    length = len;
    v = vec;
}

inline const char * RcppStringVectorView::operator()(int i) const {
    if (i < 0 || i >= length) {
	std::ostringstream oss;
	oss << "RcppStringVector: subscript out of range: " << i;
	throw std::range_error(oss.str());
    }
    return CHAR(STRING_ELT(v,i));
}

int RcppStringVectorView::size() const { 
    return length; 
}
