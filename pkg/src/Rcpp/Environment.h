// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Rcpp_RObject.h: Rcpp R/C++ interface class library -- super class of all R objects wrapped in C++ classes
//
// Copyright (C) 2009 - 2010	Dirk Eddelbuettel
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

#ifndef Rcpp_Environment_h
#define Rcpp_Environment_h

#include <RcppCommon.h>
#include <Rcpp/RObject.h>

#define IS_USER_DATABASE(rho)  OBJECT((rho)) && Rf_inherits((rho), "UserDefinedDatabase")

namespace Rcpp{ 

class Environment: public RObject{
public:
	
	/**
	 * wraps the given environment
	 *
	 * if the SEXP is not an environment, and exception is thrown
	 */
	Environment(SEXP m_sexp) ;
    
	/**
	 * Nothing specific
	 */ 
    ~Environment() ;
	
    /**
     * The list of objects in the environment
     * 
     * the same as calling this from R: 
     * > ls( envir = this, all = all )
     */ 
    SEXP ls(bool all) const ;
    
protected:
	
	/**
	 * we cache whether this environment is a user defined database
	 * or a standard environment
	 */
	bool is_user_database ;
};

} // namespace Rcpp

#endif
