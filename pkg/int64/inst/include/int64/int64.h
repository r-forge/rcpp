// int64.h : 64 bit integers
//
// Copyright (C) 2011 Romain Francois
// Copyright (C) 2011 Google Inc.  All rights reserved.
//
// This file is part of int64.
//
// int64 is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// int64 is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License  
// along with int64.  If not, see <http://www.gnu.org/licenses/>.    
    
#ifndef int64__int64__h
#define int64__int64__h

namespace int64{
    namespace internal {
        SEXP int2(int,int) ;
    }
}

#include <int64/get_long.h>
#include <int64/get_bits.h>
#include <int64/get_class.h>

#include <int64/format_binary.h>
#include <int64/LongVector.h>
#include <int64/as_long.h>
#include <int64/as_character.h>

namespace int64{
    namespace internal {
        
        template <typename LONG>
        SEXP new_long(LONG x){
            std::string klass = get_class<LONG>() ;
            int64::LongVector<LONG> y(1) ;
            y.set(0, x) ;
            SEXP res = PROTECT( 
                R_do_slot_assign( 
                    R_do_new_object( R_do_MAKE_CLASS( klass.c_str() ) ), 
                    Rf_install(".Data"), 
                    y ) ) ;
            UNPROTECT(1) ;
            return res ;
        }
        
        template <typename LONG>
        SEXP new_long_2(LONG x, LONG y){
            std::string klass = get_class<LONG>() ;
            int64::LongVector<LONG> z(2) ;
            z.set(0, x ) ;
            z.set(1, y ) ;
            SEXP res = PROTECT( 
                R_do_slot_assign( 
                    R_do_new_object( R_do_MAKE_CLASS( klass.c_str() ) ), 
                    Rf_install(".Data"), 
                    z ) ) ;
            UNPROTECT(1) ;
            return res ;
        }
        
    }
}


#include <int64/routines.h>
#include <int64/arith.h>
#include <int64/compare.h>
#include <int64/summary.h>

#endif