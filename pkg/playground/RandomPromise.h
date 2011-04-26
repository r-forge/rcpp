// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 4 -*-
//
// RandomPromise.h: Rcpp R/C++ interface class library -- 
//
// Copyright (C) 2010 - 2011 Douglas Bates, Dirk Eddelbuettel and Romain Francois
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
                            
#ifndef Rcpp__stats__RandomPromise_h
#define Rcpp__stats__RandomPromise_h

namespace Rcpp {
	namespace stats {
	  
	    template < double Func(void) >
	    class RandomPromise_0 : public VectorBase<REALSXP,true,RandomPromise_0<Func> > {
	    public:
	        RandomPromise_0(int n_) : n(n_){}
	        
	        inline double operator[]( int i ) const {
	            return Func() ;
	        }
	        inline int size() const { return n ; }
	        
	    private:
	        int n ;
	    } ;
	    
	    template < double Func(double) >
	    class RandomPromise_1 : public VectorBase<REALSXP,true,RandomPromise_1<Func> > {
	    public:
	        RandomPromise_1(int n_, double x0_ ) : n(n_), x0(x0_){}
	        
	        inline double operator[]( int i ) const {
	            return Func(x0) ;
	        }
	        inline int size() const { return n ; }
	        
	    private:
	        int n ;
	        double x0 ;
	    } ;
	    
	} // stats
} // Rcpp

#endif
