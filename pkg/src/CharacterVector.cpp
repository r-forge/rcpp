// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// CharacterVector.cpp: Rcpp R/C++ interface class library -- character vectors
//
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
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

#include <RcppCommon.h>
#include <Rcpp/RObject.h>
#include <Rcpp/CharacterVector.h>
#include <algorithm>

namespace Rcpp{
	
	CharacterVector::CharacterVector(SEXP x) throw(not_compatible) : RObject() {
		switch( TYPEOF( x ) ){
			case STRSXP:
				setSEXP( x ) ;
				break ;
			case SYMSXP:
				setSEXP( Rf_ScalarString(PRINTNAME(x)) ) ;
				break ;
			case CHARSXP:
				setSEXP( Rf_ScalarString( x ) ) ;
			default:
				/* TODO: try coercion */
				throw not_compatible( "not compatible with character vector" ) ;
		}
	}
	
	CharacterVector::CharacterVector(int size) : RObject() {
		setSEXP( Rf_allocVector(STRSXP, size) ) ;
	}

#ifdef HAS_INIT_LISTS
	CharacterVector::CharacterVector( std::initializer_list<std::string> list ) {
		SEXP x = PROTECT( Rf_allocVector( STRSXP, list.size() ) ) ;
		const std::string *p = list.begin() ;
		int n = list.size() ;
		for( int i=0; i<n ; i++, p++){
			SET_STRING_ELT( x, i, Rf_mkChar(p->c_str()) ) ;
		}
		setSEXP( x ) ;
		UNPROTECT( 1 ); /* x */
	}
#endif

SEXP CharacterVector::get( const int& i ) const { 
	return STRING_ELT(m_sexp, i) ;
}

void CharacterVector::set( const int& i, const std::string& value ){
	SET_STRING_ELT(m_sexp,i, Rf_mkChar(value.c_str()) ) ;
}

SEXP* CharacterVector::begin(){
	return RCPP_VECTOR_PTR(m_sexp) ;
}

SEXP* CharacterVector::end(){
	return RCPP_VECTOR_PTR(m_sexp) + LENGTH(m_sexp) ;
}

CharacterVector::StringProxy::StringProxy(CharacterVector& v, int i) :
	parent(v), index(i){}

CharacterVector::StringProxy::operator SEXP() const{
	return STRING_ELT( parent, index ) ;
}

CharacterVector::StringProxy::operator char*() const {
	return const_cast<char*>( CHAR(STRING_ELT( parent, index )) );
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator=( const StringProxy& rhs){
	SET_STRING_ELT( parent, index, STRING_ELT( rhs.parent, rhs.index) ) ;
}

CharacterVector::StringProxy& CharacterVector::StringProxy::operator=( const std::string& rhs){
	SET_STRING_ELT( parent, index, Rf_mkChar( rhs.c_str() ) ) ;
}

const CharacterVector::StringProxy CharacterVector::operator[](int i) const {
	return StringProxy(const_cast<CharacterVector&>(*this), i) ;
}

CharacterVector::StringProxy CharacterVector::operator[](int i) {
	return StringProxy(*this, i ) ;
}



} // namespace 
