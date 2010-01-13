// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RawVector.h: Rcpp R/C++ interface class library -- raw vectors
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

#ifndef Rcpp_RawVector_h
#define Rcpp_RawVector_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/VectorBase.h>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#include <algorithm>
#endif

namespace Rcpp{ 

class RawVector : public VectorBase {     
public:

	RawVector(SEXP x) throw(not_compatible);
	RawVector( int size) ;
	
#ifdef HAS_INIT_LISTS	
	RawVector( std::initializer_list<Rbyte> list ) ;
	RawVector( std::initializer_list<int> list ) ;
#endif
	
	inline Rbyte& operator[]( int i ) const { return RAW(m_sexp)[i] ; }
	inline Rbyte* begin() const { return RAW(m_sexp) ; }
	inline Rbyte* end() const { return RAW(m_sexp) + LENGTH(m_sexp) ; }
	
	typedef Rbyte* iterator ;
	
} ;

} // namespace

#endif
