#!/usr/bin/r -t
#
# Copyright (C) 2009 Romain Francois
#
# This file is part of Rcpp.
#
# Rcpp is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

require(Rcpp)
require(inline)

funx <- cfunction(signature(), '
	/* creating a pointer to a vector<int> */
	std::vector<int>* v = new std::vector<int> ;
	v->push_back( 1 ) ;
	v->push_back( 2 ) ;
	
	/* wrap the pointer as an external pointer */
	/* this automatically protected the external pointer from R garbage 
	   collection until p goes out of scope. */
	Rcpp::XPtr< std::vector<int> > p(v) ;
	
	/* return it back to R, since p goes out of scope after the return 
	   the external pointer is no more protected by p, but it gets 
	   protected by being on the R side */
	return( p ) ;
', Rcpp=TRUE, verbose=FALSE)
xp <- funx()
stopifnot( identical( typeof( xp ), "externalptr" ) )

# passing the pointer back to C++
funx <- cfunction(signature(x = "externalptr" ), '
	/* wrapping x as smart external pointer */
	/* The SEXP based constructor does not protect the SEXP from 
	   garbage collection automatically, it is already protected 
	   because it comes from the R side, however if you want to keep 
	   the Rcpp::XPtr object on the C(++) side
	   and return something else to R, you need to protect the external
	   pointer, by using the protect member function */
	Rcpp::XPtr< std::vector<int> > p(x) ;
	
	/* just return the front of the vector as a SEXP */
	return( Rcpp::RObject( p->front() ) ) ;
', Rcpp=TRUE, verbose=FALSE)
front <- funx(xp)
stopifnot( identical( front, 1L ) )

