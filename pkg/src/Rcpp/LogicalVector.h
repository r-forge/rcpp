// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// LogicalVector.h: Rcpp R/C++ interface class library -- logical vectors
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

#ifndef Rcpp_LogicalVector_h
#define Rcpp_LogicalVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{

class LogicalVector : public VectorBase {     
public:

	LogicalVector(SEXP x) throw(not_compatible);
	LogicalVector( int size) ;

#ifdef HAS_INIT_LISTS	
	LogicalVector( std::initializer_list<int> list ) ;
	LogicalVector( std::initializer_list<Rboolean> list ) ;
	LogicalVector( std::initializer_list<bool> list ) ;
#endif

	typedef int* iterator ;

	inline int& operator[]( int i ) const { return start[i] ;}
	inline int* begin() const { return start ; }
	inline int* end() const { return start + LENGTH(m_sexp); }

private:
	int* start ;
	inline void update(){ start=LOGICAL(m_sexp); }
} ;

} // namespace

#endif
