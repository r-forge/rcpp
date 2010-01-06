// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RObject.cpp: Rcpp R/C++ interface class library -- R Object support
//
// Copyright (C) 2009 Dirk Eddelbuettel and Romain Francois
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

#include <Rcpp/RObject.h>
#include <Rcpp/Environment.h>
#include <Rcpp/Symbol.h>
#include <algorithm>
#include <Rcpp/as.h>

namespace Rcpp {

void RObject::setSEXP(SEXP x){
	/* if we are setting to the same SEXP as we already have, do nothing */
	if( x != m_sexp ){
		
		/* the previous SEXP was not NULL, so release it */
		release() ;
		
		/* set the SEXP */
		m_sexp = x ;
		
		/* the new SEXP is not NULL, so preserve it */
		preserve() ;
	}
}

/* copy constructor */
RObject::RObject( const RObject& other ){
	SEXP x = other.asSexp() ;	
	setSEXP( x ) ; 
}

RObject& RObject::operator=( const RObject& other){
	SEXP x = other.asSexp() ;	
	setSEXP( x ) ; 
	return *this ;
}

RObject& RObject::operator=( SEXP other ){
	setSEXP( other ) ; 
	return *this ;
}

RObject::~RObject() {
	release() ;
	logTxt("~RObject");
}

double RObject::asDouble() const { return as<double>( m_sexp ) ; }
int RObject::asInt() const { return as<int>( m_sexp ) ; }
Rbyte RObject::asRaw() const { return as<Rbyte>( m_sexp ) ; }
bool RObject::asBool() const { return as<bool>(m_sexp) ; }
std::string RObject::asStdString() const { return as< std::string >( m_sexp ) ; }
std::vector<bool> RObject::asStdVectorBool() const { return as< std::vector<bool> >( m_sexp ) ; }
std::vector<int> RObject::asStdVectorInt() const { return as< std::vector<int> >( m_sexp ) ; }
std::vector<Rbyte> RObject::asStdVectorRaw() const { return as< std::vector<Rbyte> >( m_sexp ) ; }
std::vector<double> RObject::asStdVectorDouble() const { return as< std::vector<double> >( m_sexp ) ; }
std::vector<std::string> RObject::asStdVectorString() const { return as< std::vector<std::string> >( m_sexp ) ; }

std::vector<std::string> RObject::attributeNames() const {
	/* inspired from do_attributes@attrib.c */
	
	std::vector<std::string> v ;
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	v.push_back( std::string(CHAR(PRINTNAME(TAG(attrs)))) ) ;
    	attrs = CDR( attrs ) ;
    }
    return v ;
}

bool RObject::hasAttribute( const std::string& attr) const {
	SEXP attrs = ATTRIB(m_sexp);
    while( attrs != R_NilValue ){
    	if( attr == CHAR(PRINTNAME(TAG(attrs))) ){
    		return true ;
    	}
    	attrs = CDR( attrs ) ;
    }
    return false; /* give up */
}

SEXP RObject::attr( const std::string& name) const{
	return Rf_getAttrib( m_sexp, Rf_install( name.c_str() ) );
}

const char* RObject::not_compatible::what( ) const throw() {
	return message.c_str() ;
}
RObject::not_compatible::~not_compatible() throw() {}
			

} // namespace Rcpp

