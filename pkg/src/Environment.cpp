// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Environment.cpp: Rcpp R/C++ interface class library -- Environments
//
// Copyright (C) 2009 - 2010	Romain Francois
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

#include <Rcpp/Environment.h>

namespace Rcpp {

    Environment::Environment( SEXP m_sexp = R_GlobalEnv) : RObject::RObject(m_sexp){
	if( TYPEOF(m_sexp) != ENVSXP ){
	    throw std::runtime_error( "not an environment" ) ;
	}
	is_user_database = IS_USER_DATABASE(m_sexp) ;
    }
	
    Environment::~Environment(){
	logTxt( "~Environment" ) ;
    }
	
    SEXP Environment::ls( bool all = true) const {
	if( is_user_database ){
	    R_ObjectTable *tb = (R_ObjectTable*)
		R_ExternalPtrAddr(HASHTAB(m_sexp));
	    return tb->objects(tb) ;
	} else{
	    Rboolean get_all = all ? TRUE : FALSE ;
	    return R_lsInternal( m_sexp, get_all ) ;
	}
	return R_NilValue ;
    }
	
} // namespace Rcpp

