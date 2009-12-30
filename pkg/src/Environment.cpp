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

/* this comes from JRI, where it was introduced to cope with cases
   where bindings are locked */
struct safeAssign_s {
    SEXP sym, val, rho;
};
static void safeAssign(void *data) {
    struct safeAssign_s *s = (struct safeAssign_s*) data;
    Rf_defineVar(s->sym, s->val, s->rho);
}
	
	
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
    
    SEXP Environment::get( const std::string& name) const {
    	SEXP res = Rf_findVarInFrame( m_sexp, Rf_install(name.c_str())  ) ;
    	
    	if( res == R_UnboundValue ) return R_NilValue ;
    	
    	/* We need to evaluate if it is a promise */
	if( TYPEOF(res) == PROMSXP){
    		res = Rf_eval( res, m_sexp ) ;
    	}
    	return res ;
    }
    
    bool Environment::exists( const std::string& name) const{
    	SEXP res = Rf_findVarInFrame( m_sexp, Rf_install(name.c_str())  ) ;
    	return res != R_UnboundValue ;
    }
    
    bool Environment::assign( const std::string& name, SEXP x = R_NilValue) const{
    	/* borrowed from JRI, we cannot just use defineVar since it might 
    	   crash on locked bindings */
    	struct safeAssign_s s;
    	s.sym = Rf_install( name.c_str() ) ;
    	if( !s.sym || s.sym == R_NilValue ) return false ;
    	
    	s.rho = m_sexp ;
    	s.val = x ;
    	return static_cast<bool>( R_ToplevelExec(safeAssign, (void*) &s) );
    }
    
    bool Environment::isLocked() const{
    	return static_cast<bool>(R_EnvironmentIsLocked(m_sexp));
    }
    
    bool Environment::bindingIsActive(const std::string& name) const{
    	if( !exists( name) ) return false ; /* should this be an exception instead ? */
    	return static_cast<bool>(R_BindingIsActive(Rf_install(name.c_str()), m_sexp)) ;
    }
    
    bool Environment::bindingIsLocked(const std::string& name) const{
    	if( !exists( name) ) return false ; /* should this be an exception instead ? */
    	return static_cast<bool>(R_BindingIsLocked(Rf_install(name.c_str()), m_sexp)) ;
    }
    
} // namespace Rcpp

