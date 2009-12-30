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

test.environment.get <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	return env.get( Rcpp::RObject(name).asStdString() ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	
	checkEquals( funx( e, "a" ), e$a, msg = "Environment::get()" )
	checkEquals( funx( e, "foobar" ), NULL, msg = "Environment::get()" )
	checkEquals( funx( asNamespace("Rcpp"), "CxxFlags"), Rcpp:::CxxFlags, 
		msg = "Environment(namespace)::get() " )
	
}

test.environment.exists <- function(){
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::RObject(name).asStdString() ;
	return Rcpp::RObject( env.exists(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	e$a <- 1:10
	e$b <- "foo"
	
	checkTrue( funx( e, "a" ), msg = "Environment::get()" )
	checkTrue( !funx( e, "foobar" ), msg = "Environment::get()" )
	checkTrue( funx( asNamespace("Rcpp"), "CxxFlags"), 
		msg = "Environment(namespace)::get() " )
}

test.environment.assign <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character", object = "ANY" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::RObject(name).asStdString() ;
	return Rcpp::RObject( env.assign(st, object) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env( )
	checkTrue( funx(e, "a", 1:10 ), msg = "Environment::assign" )
	checkTrue( funx(e, "b", Rcpp:::CxxFlags ), msg = "Environment::assign" )
	checkEquals( ls(e), c("a", "b"), msg = "Environment::assign, checking names" )
	checkEquals( e$a, 1:10, msg = "Environment::assign, checking value 1" )
	checkEquals( e$b, Rcpp:::CxxFlags, msg = "Environment::assign, checking value 2" )
	
	lockBinding( "a", e ) 
	checkTrue( !funx(e, "a", letters ), msg = "Environment::assign and locked bindings" )
}

test.environment.isLocked <- function(){
	funx <- cfunction(signature(x="environment" ), '
	Rcpp::Environment env(x) ;
	return Rcpp::RObject( env.isLocked() ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	checkTrue( !funx(e), msg = "Environment::isLocked( new.env) -> false" )
	checkTrue( funx(asNamespace("Rcpp")), msg = "Environment::isLocked( namespace:Rcpp ) -> true" )
	
}

test.environment.bindingIsActive <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::RObject(name).asStdString() ;
	return Rcpp::RObject( env.bindingIsActive(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	makeActiveBinding( "b", function(x) 10, e ) 
	
	checkTrue( !funx(e, "a" ), msg = "Environment::bindingIsActive( non active ) -> false" )
	checkTrue( funx(e, "b" ), msg = "Environment::bindingIsActive( active ) -> true" )
	checkTrue( !funx(e, "xx" ), msg = "Environment::bindingIsActive( no binding ) -> false" )
	
}

test.environment.bindingIsLocked <- function(){
	
	funx <- cfunction(signature(x="environment", name = "character" ), '
	Rcpp::Environment env(x) ;
	std::string st = Rcpp::RObject(name).asStdString() ;
	return Rcpp::RObject( env.bindingIsLocked(st) ) ;
	', Rcpp=TRUE, verbose=FALSE)
	
	e <- new.env()
	e$a <- 1:10
	e$b <- letters
	lockBinding( "b", e )
	
	checkTrue( !funx(e, "a" ), msg = "Environment::bindingIsActive( non active ) -> false" )
	checkTrue( funx(e, "b" ), msg = "Environment::bindingIsActive( active ) -> true" )
	checkTrue( !funx(e, "xx" ), msg = "Environment::bindingIsActive( no binding ) -> false" )
	
}

test.environment.NotAnEnvironment <- function(){
	funx <- cfunction(signature(x="ANY"), 'Rcpp::Environment env(x) ;', 
		Rcpp=TRUE, verbose=FALSE)
	checkException( funx( funx ), msg = "not an environment" )
	checkException( funx( letters ), msg = "not an environment" )
	checkException( funx( iris ), msg = "not an environment" )
	checkException( funx( NULL ), msg = "not an environment" )
}

