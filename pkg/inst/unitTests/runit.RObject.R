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

test.RObject.asDouble <- function(){
	foo <- '
	double d = Rcpp::RObject(x).asDouble();
	return(Rcpp::RObject( 2*d ) );
	'
	funx <- cfunction(signature(x="numeric"), foo, 
		Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(2.123), 4.246, msg = "RObject.asDouble()" )
	checkEquals( funx(2), 4, msg = "RObject.asDouble()" )
	checkException( funx(x='2'), msg = "RObject.asDouble() can not convert character" )
	checkException( funx(x=2:3), msg = "RObject.asDouble() expects the vector to be of length 1" )
	checkEquals( funx(2L), 4.0, msg = "RObject.asDouble()" )
}

test.RObject.asInt <- function(){
	foo <- '
	int i = Rcpp::RObject(x).asInt();
	return(Rcpp::RObject( 2*i ) ); '
	funx <- cfunction(signature(x="numeric"), foo, 
		Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(2.123), 4L, msg = "RObject.asInt()" )
	checkEquals( funx(2), 4L, msg = "RObject.asInt()" )
	checkEquals( funx(2L), 4.0, msg = "RObject.asInt()" )
	checkEquals( funx(as.raw(2L)), 4.0, msg = "RObject.asInt()" )
	checkException( funx(x='2'), msg = "RObject.asInt() can not convert character" )
	checkException( funx(x=2:3), msg = "RObject.asInt() expects the vector to be of length 1" )
	
}

test.RObject.asStdString <- function(){
	foo <- '
	std::string s = Rcpp::RObject(x).asStdString();
	return(Rcpp::RObject( s+s ) );'
	funx <- cfunction(signature(x="character"), foo, 
		Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx("abc"), "abcabc", msg = "RObject.asStdString()" )
	checkException( funx(NULL), msg = "RObject.asStdString expects string" )
	checkException( funx(0L), msg = "RObject.asStdString expects string" )
	checkException( funx(0.1), msg = "RObject.asStdString expects string" )
	checkException( funx(as.raw(0L)), msg = "RObject.asStdString expects string" )
	
	checkException( funx(letters), msg = "RObject.asStdString expects single string" )
	
}

test.RObject.asRaw <- function(){
	foo <- '
	Rbyte i = Rcpp::RObject(x).asRaw();
	return(Rcpp::RObject( (Rbyte)(2*i) ) ); '
	funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
	checkEquals( funx(1L), as.raw(2L), msg = "RObject.asRaw(integer)" )
	checkEquals( funx(1.3), as.raw(2L), msg = "RObject.asRaw(numeric)" )
	checkEquals( funx(as.raw(1)), as.raw(2L), msg = "RObject.asRaw(raw)" )
	checkException( funx(NULL) , msg = "RObject.asRaw(NULL) -> exception" )
	checkException( funx("foo") , msg = "RObject.asRaw(character) -> exception" )
	checkException( funx(1:2), msg = "RObject.asRaw(>1 integer) -> exception" )
	checkException( funx(1.3), msg = "RObject.asRaw(>1 numeric) -> exception" )
	checkException( funx(as.raw(1:3)), msg = "RObject.asRaw(>1 raw) -> exception" )
	checkException( funx(integer(0)), msg = "RObject.asRaw(0 integer) -> exception" )
	checkException( funx(numeric(0)), msg = "RObject.asRaw(0 numeric) -> exception" )
	checkException( funx(raw(0)), msg = "RObject.asRaw(0 raw) -> exception" )
}

test.RObject.asLogical <- function(){
	foo <- '
	bool b = Rcpp::RObject(x).asBool();
	return(Rcpp::RObject( !b ));'
	funx <- cfunction(signature(x="logical"), foo, Rcpp=TRUE, verbose=FALSE)
	checkTrue( !funx(TRUE), msg = "RObject::asBool(TRUE) -> true" )
	checkTrue( funx(FALSE), msg = "RObject::asBool(FALSE) -> false" )
	checkTrue( !funx(1L), msg = "RObject::asBool(1L) -> true" )
	checkTrue( funx(0L), msg = "RObject::asBool(0L) -> false" )
	checkTrue( !funx(1.0), msg = "RObject::asBool(1.0) -> true" )
	checkTrue( funx(0.0), msg = "RObject::asBool(0.0) -> false" )
	checkTrue( !funx(as.raw(1)), msg = "RObject::asBool(aw.raw(1)) -> true" )
	checkTrue( funx(as.raw(0)), msg = "RObject::asBool(as.raw(0)) -> false" )
	
	checkException( funx(NULL), msg = "RObject::asBool(NULL) -> exception" )
	checkException( funx(c(TRUE,FALSE)), msg = "RObject::asBool(>1 logical) -> exception" )
	checkException( funx(1:2), msg = "RObject::asBool(>1 integer) -> exception" )
	checkException( funx(1:2+.1), msg = "RObject::asBool(>1 numeric) -> exception" )
	checkException( funx(as.raw(1:2)), msg = "RObject::asBool(>1 raw) -> exception" )
	
	checkException( funx(integer(0)), msg = "RObject.asBool(0 integer) -> exception" )
	checkException( funx(numeric(0)), msg = "RObject.asBool(0 numeric) -> exception" )
	checkException( funx(raw(0)), msg = "RObject.asBool(0 raw) -> exception" )
}



