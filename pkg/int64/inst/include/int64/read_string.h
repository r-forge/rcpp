// read_string.h : 64 bit integers
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
    
#ifndef int64__read_string__h
#define int64__read_string__h
              
namespace int64{
    namespace internal{

    template <typename LONG>
    inline LONG read_string(const char* s) ;
    
    template <>
    inline int64_t read_string<int64_t>(const char* s ){
        return strtoll( s, NULL, 0 ) ; 
    }
        
    template <>
    inline uint64_t read_string<uint64_t>(const char* s){
        return strtoull( s, NULL, 0 ) ;
    } 
        
    
    } // namespace internal
} // namespace int64

#endif
