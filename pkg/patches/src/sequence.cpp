// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// sequence.cpp: patch for base::sequence
//
// Copyright (C) 2010	Romain Francois
//
// This file is part of patches.
//
// patches is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// patches is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with patches.  If not, see <http://www.gnu.org/licenses/>.

#include "patches.h"

using namespace Rcpp ;

IntegerVector sequence(IntegerVector nvec){
BEGIN_RCPP
    
    IntegerVector x(nvec ) ;
    R_len_t n = x.size(), s = 0 ;
    IntegerVector::iterator px = x.begin() ;
    int x_i ;
    
    /* error checking */
    for( int i=0; i<n; i++){
        x_i = px[i] ;
        if( x_i == NA_INTEGER || x_i <= 0 ) 
            throw std::range_error( "argument must be coercible to non-negative integer" ) ;
        s += x_i ;
    }
        
    IntegerVector out( s ) ;
    int end ;
    for( int i=0, start=0; i<n; i++){
        x_i = x[i] ;
        end = start + x_i - 1;
        out[ Range( start, end ) ] = seq_len( x_i ) ;
        start = end + 1;
    } 
    return out ;
    
END_RCPP
}                  

