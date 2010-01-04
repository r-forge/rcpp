// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// wrap.h: Rcpp R/C++ interface class library -- general R object wrapper
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

#ifndef Rcpp_wrap_h
#define Rcpp_wrap_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <set>

namespace Rcpp{ 

// factories

RObject wrap(SEXP m_sexp) ;

RObject wrap(const bool & v);
RObject wrap(const double & v);
RObject wrap(const int & v);
RObject wrap(const char* const v);
RObject wrap(const Rbyte & v);
RObject wrap(const std::string & v);

RObject wrap(const std::vector<int> & v);
RObject wrap(const std::vector<double> & v);
RObject wrap(const std::vector<std::string> & v);
RObject wrap(const std::vector<Rbyte> & v);
RObject wrap(const std::vector<bool> & v);

RObject wrap(const std::set<int> & v);
RObject wrap(const std::set<double> & v);
RObject wrap(const std::set<std::string> & v);
RObject wrap(const std::set<Rbyte> & v);

} // namespace Rcpp

#endif
