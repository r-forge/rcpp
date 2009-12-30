#!/usr/bin/r -t
#
# Copyright (C) 2009 - 2010	Romain Francois
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

test.environment.ls <- function(){
	funx <- cfunction(signature(x="environment"), '
	Rcpp::Environment env(x) ; 
	return env.ls(true) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	e$.c <- "hidden"
	checkEquals( funx(e), c("a","b", ".c"), msg = "Environment::ls(true)" )
	checkEquals( funx(asNamespace("Rcpp")), ls(envir=asNamespace("Rcpp"), all = TRUE), 
		msg = "Environment(namespace)::ls()" )
	
	funx <- cfunction(signature(x="environment"), '
	Rcpp::Environment env(x) ; 
	return env.ls(false) ;
	', Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(e), c("a","b"), msg = "Environment::ls(false)" )
	checkEquals( funx(asNamespace("Rcpp")), ls(envir=asNamespace("Rcpp"), all = FALSE), 
		msg = "Environment(namespace)::ls()" )
	
}




