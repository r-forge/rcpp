// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Environment.h: Rcpp R/C++ interface class library -- access R environments
//
// Copyright (C) 2009 - 2010	Dirk Eddelbuettel and Romain Francois
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
     *
     * @param all same meaning as in ?ls
     */ 
    SEXP ls(bool all) const ;
    
    /**
     * Get an object from the environment
     *
     * @param name name of the object
     *
     * @return a SEXP (possibly R_NilValue)
     */
    SEXP get(const std::string& name) const ;
    
    /**
     * Indicates if an object called name exists in the 
     * environment
     *
     * @param name name of the object
     *
     * @return true if the object exists in the environment
     */
    bool exists( const std::string& name ) const ;
    
    /**
     * Attempts to assign x to name in this environment
     *
     * @param name name of the object to assign
     * @param x object to assign
     *
     * @return true if the assign was successfull
     */
    bool assign( const std::string& name, SEXP x ) const ;
    
    /**
     * @return true if this environment is locked
     * see ?environmentIsLocked for details of what this means
     */
    bool isLocked() const ;
    
    /**
     * @param name name of a potential binding
     *
     * @return true if the binding is locked in this environment
     * see ?bindingIsLocked
     */
    bool bindingIsLocked(const std::string& name) const ;
    
    /**
     *
     * @param name name of a binding
     * 
     * @return true if the binding is active in this environment
     * see ?bindingIsActive
     */
    bool bindingIsActive(const std::string& name) const ;
    
protected:
	
    /**
     * we cache whether this environment is a user defined database
     * or a standard environment
     */
    bool is_user_database ;
};

} // namespace Rcpp

#endif
