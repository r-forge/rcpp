#!/usr/bin/r -t
#
# Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
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

.setUp <- function(){
	suppressMessages( require( inline ) )
}

test.Pairlist <- function(){
	funx <- cfunction(signature(x="ANY"), 'return Pairlist(x) ;', 
		Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
	checkEquals( funx( pairlist("rnorm") ), pairlist("rnorm" ), msg = "Pairlist( LISTSXP )" )
	checkEquals( funx( call("rnorm") ), call("rnorm" ), msg = "Pairlist( LANGSXP )" )
	checkEquals( funx(1:10), as.pairlist(1:10) , msg = "Pairlist( INTSXP) " )
	checkEquals( funx(TRUE), as.pairlist( TRUE) , msg = "Pairlist( LGLSXP )" )
	checkEquals( funx(1.3), as.pairlist(1.3), msg = "Pairlist( REALSXP) " )
	checkEquals( funx(as.raw(1) ), as.pairlist(as.raw(1)), msg = "Pairlist( RAWSXP)" )
	
	checkException( funx(funx), msg = "Pairlist not compatible with function" )
	checkException( funx(new.env()), msg = "Pairlist not compatible with environment" )
	
}

test.Pairlist.variadic <- function(){
	if( Rcpp:::canUseCXX0X() ){
		funx <- cfunction(signature(), '
		return Pairlist( "rnorm", 10, 0.0, 2.0 ) ;
		', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
		checkEquals( funx(), pairlist("rnorm", 10L, 0.0, 2.0 ), 
			msg = "variadic templates" )
			
		funx <- cfunction(signature(), '
		return Pairlist( "rnorm", 10, Named("mean",0.0), 2.0 ) ;
		', Rcpp=TRUE, verbose=FALSE, includes = "using namespace Rcpp;" )
		checkEquals( funx(), pairlist("rnorm", 10L, mean = 0.0, 2.0 ), 
			msg = "variadic templates (with names)" )
	}
}

