// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppSexp.h: Rcpp R/C++ interface class library -- SEXP support
//
// Copyright (C) 2009 Dirk Eddelbuettel
// Copyright (C) 2009 Romain Francois
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

#ifndef RcppSexp_h
#define RcppSexp_h

#include <RcppCommon.h>
#include <RcppSuperClass.h>
#include <set>

class RcppSexp: public RcppSuperClass {
public:
	
	/**
	 * wraps a SEXP. The SEXP is not automatically 
	 * protected from garbage collection because it might be 
	 * protected from elsewhere (e.g. if it comes from the 
	 * R side). See protect and release for ways to protect
	 * the SEXP from garbage collection, and release to 
	 * remove the protection
	 */
	RcppSexp(SEXP m_sexp = R_NilValue) : m_sexp(m_sexp) {};
    
    /**
	 * if this object is protected rom R's GC, then it is released
	 * and become subject to garbage collection. See protect 
	 * and release member functions.
	 */
    ~RcppSexp() ;
	
    RcppSexp(const double & v);
    RcppSexp(const int & v);
    RcppSexp(const Rbyte & v);
    RcppSexp(const std::string & v);
    RcppSexp(const bool & v);
    
    RcppSexp(const std::vector<int> & v);
    RcppSexp(const std::vector<double> & v);
    RcppSexp(const std::vector<std::string> & v);
    RcppSexp(const std::vector<Rbyte> & v);
    RcppSexp(const std::vector<bool> & v);
    
    RcppSexp(const std::set<int> & v);
    RcppSexp(const std::set<double> & v);
    RcppSexp(const std::set<std::string> & v);
    RcppSexp(const std::set<Rbyte> & v);
    
    
    /* we don't provide implicit converters because 
       of Item 5 in More Effective C++ */
    bool                     asBool() const;
    double                   asDouble() const;
    int                      asInt() const;
    Rbyte                    asRaw() const;
    std::string              asStdString() const;
    std::vector<int>         asStdVectorInt() const;
    std::vector<double>      asStdVectorDouble() const;
    std::vector<std::string> asStdVectorString() const;
    std::vector<Rbyte>       asStdVectorRaw() const;
    std::vector<bool>        asStdVectorBool() const;
    
    
    /**
	 * protects the wrapped SEXP from garbage collection. This 
	 * calls the R_PreserveObject function on the underlying SEXP.
	 *
	 * Note that this does not use the PROTECT/UNPROTECT dance
	 */
	void protect();
	
	/**
	 * explicitely release this object to R garbage collection. This
	 * calls the R_ReleaseObject function on the underlying SEXP. 
	 * This is automatically done by the destructor if we protected 
	 * the SEXP (using the protect member function)
	 */
	void release();
	
	/**
	 * implicit conversion to SEXP
	 */
	inline operator SEXP() const {
		return m_sexp ;
	}
	
	
	/* attributes */
	
	/**
	 * extracts the names of the attributes of the wrapped SEXP
	 */
    std::vector<std::string> attributeNames() const ;
    
    /**
     * Identifies if the SEXP has the given attribute
     */
	bool hasAttribute( const std::string& attr) const ; 
    
    /**
     * extract the given attribute
     */
    SEXP attr( const std::string& name) const  ;
    
    /**
     * is this object NULL
     */
    inline bool isNULL() const{
    	return m_sexp == R_NilValue ;
    }
    
    /**
     * The SEXP typeof, calls TYPEOF on the underlying SEXP
     */
    inline int sexp_type() const {
    	return TYPEOF(m_sexp) ;
    }
    
    /** 
	 * explicit conversion to SEXP
	 */
	inline SEXP asSexp() const {
		return m_sexp ;
	}
	
protected:
	
	/**
	 * The SEXP this is wrapping
	 */
	SEXP m_sexp ;
	
	/**
	 * true if this protects the SEXP from garbage collection
	 * using R_ReleaseObject/R_PreserveObject strategy
	 *
	 * if this is true then the object will be release and become
	 * subject to R garbage collection when this object is deleted
	 */
	bool isProtected ;    
    
};

#endif
