// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppStringVector.h: Rcpp R/C++ interface class library -- string vector support
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

#ifndef RcppStringVector_h
#define RcppStringVector_h

#include <RcppCommon.h>

class RcppStringVector {
public:
    RcppStringVector(SEXP vec);
    ~RcppStringVector();
    std::string& operator()(int i) const;
    int size() const;
    std::vector<std::string> stlVector() const;
private:
    std::string *v;
    int length;
};

#endif
