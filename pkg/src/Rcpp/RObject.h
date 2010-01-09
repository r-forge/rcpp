// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RObject.h: Rcpp R/C++ interface class library -- general R object wrapper
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

#ifndef Rcpp_RObject_h
#define Rcpp_RObject_h

#include <RcppCommon.h>
#include <set>

namespace Rcpp{ 

class RObject{
public:

	/**
   	 * Exception thrown when attempting to convert a SEXP
   	 */
   	class not_compatible: public std::exception{
   		public:
   			not_compatible(const std::string& message) throw() : message(message){};
   			~not_compatible() throw(){} ;
   			const char* what() const throw() ; 
   		private:
   			std::string message ;
   	} ;

	/**
   	 * Exception thrown when attempting to convert a SEXP
   	 */
   	class not_s4: public std::exception{
   		public:
   			not_s4() throw(){};
   			~not_s4() throw(){} ;
   			const char* what() const throw() ; 
   	} ;
   	
   	class index_out_of_bounds: public std::exception{
   	public:
   		index_out_of_bounds() throw(){};
   		~index_out_of_bounds() throw(){};
   		const char* what() const throw() ;
   	} ;
   	
    /**
     * default constructor. uses R_NilValue
     */ 
    RObject() : m_sexp(R_NilValue) {} ;	

    /**
     * wraps a SEXP. The SEXP is automatically protected from garbage 
     * collection by this object and the protection vanishes when this 
     * object is destroyed
     */
    RObject(SEXP x) : m_sexp(R_NilValue) { setSEXP(x) ; };

    /**
     * Copy constructor. set this SEXP to the SEXP of the copied object
     */
    RObject( const RObject& other ) ;

    /**
     * Assignment operator. set this SEXP to the SEXP of the copied object
     */
    RObject& operator=( const RObject& other ) ;

    /** 
     * Assignement operator. Set this SEXP to the given SEXP
     */ 
    RObject& operator=( SEXP other ) ;

    /**
     * if this object is protected rom R's GC, then it is released
     * and become subject to garbage collection. See preserve 
     * and release member functions.
     */
    virtual ~RObject() ;

    /**
     * implicit conversion to SEXP.
     */
    inline operator SEXP() const { return m_sexp ; }

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

    inline bool isPreserved() { DEFUNCT("isPreserved") ; return m_sexp != R_NilValue ; }
    inline void forgetPreserve() { DEFUNCT("forgetPreserve") ; }

    /* attributes */

    /**
     * extracts the names of the attributes of the wrapped SEXP
     */
    std::vector<std::string> attributeNames() const ;

    /**
     * Identifies if the SEXP has the given attribute
     */
    bool hasAttribute( const std::string& attr) const ; 

    class AttributeProxy {
	public:
		AttributeProxy( const RObject& v, const std::string& attr_name) ;
		
		/* lvalue uses */
		AttributeProxy& operator=(const AttributeProxy& rhs) ;
		
		template <typename T>
		AttributeProxy& operator=(const T& rhs){
			Rf_setAttrib( parent, Rf_install(attr_name.c_str()), wrap(rhs) ) ;
			return *this ;
		}
		
		/* rvalue use */
		operator SEXP() const ;
		operator RObject() const ;
		
	private:
		const RObject& parent; 
		std::string attr_name ;
	} ;
    
    /**
     * extract the given attribute
     */
    /* TODO: implement a proxy pattern for attributes */
    AttributeProxy attr( const std::string& name) const  ;
    
    /**
     * is this object NULL
     */
    inline bool isNULL() const{ return Rf_isNull(m_sexp) ; }

    /**
     * The SEXP typeof, calls TYPEOF on the underlying SEXP
     */
    inline int sexp_type() const { return TYPEOF(m_sexp) ; }

    /** 
     * explicit conversion to SEXP
     */
    inline SEXP asSexp() const { return m_sexp ; }

    /**
     * Tests if the SEXP has the object bit set
     */
    inline bool isObject() const { return Rf_isObject(m_sexp) ;}

    /**
     * Tests if the SEXP is an S4 object
     */
    inline bool isS4() const { return Rf_isS4(m_sexp) ; }

    /**
     * Indicates if this S4 object has the given slot
     *
     * @throw not_s4 if the object is not an S4 object
     */
    bool hasSlot(const std::string& name) const throw(not_s4) ;

    /**
     * Retrieves the given slot
     *
     * @throw not_s4 if this is not an S4 object
     */
    RObject slot(const std::string& name) const throw(not_s4) ;
    /* TODO : implement the proxy pattern here so that we can get and 
              set the slot the same way */

protected:

    /**
     * sets the SEXP wrapped by this object
     *
     * @param x new SEXP to attach to this object
     */
    void setSEXP(SEXP x) ;

    inline void DEFUNCT(const std::string& method ){ Rf_warning("method %s is defunct", method.c_str() )  ; }

    /**
     * The SEXP this is wrapping. This has to be considered read only.
     * to change it, use setSEXP
     */
    SEXP m_sexp ;

private:

    void preserve(){ if( m_sexp != R_NilValue ) R_PreserveObject(m_sexp) ; } 
    void release() { if( m_sexp != R_NilValue ) R_ReleaseObject(m_sexp) ; } 

};

} // namespace Rcpp

#endif
