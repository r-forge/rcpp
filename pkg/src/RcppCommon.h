// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppCommon.h: Rcpp R/C++ interface class library -- common include and defines statements
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
// Copyright (C)        2009 Romain Francois
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

#ifndef RcppCommon_h
#define RcppCommon_h

#ifdef __GNUC__
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
		#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
		#if GCC_VERSION >= 40300
			#define HAS_VARIADIC_TEMPLATES
		#endif
		#if GCC_VERSION >= 40400
			#define HAS_INIT_LISTS
		#endif
	#endif
#endif

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

#ifdef HAS_INIT_LISTS
#include <initializer_list>
#endif

// include R headers, but set R_NO_REMAP and access everything via Rf_ prefixes
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Callbacks.h>
#include <R_ext/Complex.h>
#include <R_ext/Parse.h>
#include <Rversion.h>
#define RCPP_GET_NAMES(x)	Rf_getAttrib(x, R_NamesSymbol)

// #ifdef BUILDING_DLL
// #define RcppExport extern "C" __declspec(dllexport)
// #else
#define RcppExport extern "C"
// #endif

char *copyMessageToR(const char* const mesg);

// simple logging help
inline void logTxtFunction(const char* file, const int line, const char* expression); 

#ifndef logTxt
//#define logTxt(x) logTxtFunction(__FILE__, __LINE__, x);
#define logTxt(x) 
#endif

/* in exceptions.cpp */
void forward_uncaught_exceptions_to_r() ;
RcppExport SEXP initUncaughtExceptionHandler() ; 

/* just testing variadic templates */
#ifdef HAS_VARIADIC_TEMPLATES
template<typename... Args>
int variadic_length( const Args&... args) { return sizeof...(Args) ; }
#endif

#ifdef HAS_VARIADIC_TEMPLATES
RcppExport inline SEXP canUseCXX0X(){ return Rf_ScalarLogical( TRUE ); }
#else
RcppExport inline SEXP canUseCXX0X(){ return Rf_ScalarLogical( FALSE ); }
#endif

RcppExport SEXP test_named() ;
RcppExport SEXP capabilities() ;

const char * sexp_to_name(int sexp_type); 

RcppExport SEXP initRcpp() ;

namespace Rcpp{
/* internal namespace for things not intended to be used by the 
   user */
namespace internal{	
	
	/* defined in Evaluator.cpp */
	SEXP convert_using_rfunction(SEXP x, const char* const fun) ;
	
	SEXP try_catch( SEXP expr, SEXP env ) ;
	SEXP try_catch( SEXP expr ) ;
	
} // namespace internal 

/** 
 * Generic converted from SEXP to the typename. T can be any type that 
 * has a constructor taking a SEXP, which is the case for all our 
 * RObject and derived classes. 
 *
 * If it is not possible to add the SEXP constructor, e.g you don't control
 * the type, you can overload the as template to perform the 
 * requested conversion
 *
 * This is used for example in Environment, so that for example the code
 * below will work as long as there is a way to as<> the Foo type
 *
 * Environment x = ... ; // some environment
 * Foo y = x["bla"] ;    // if as<Foo> makes sense then this works !!
 */
template <typename T> T as( SEXP m_sexp) {
	T t(m_sexp);
	return t ;
}
template<> bool 			as<bool>(SEXP m_sexp) ;
template<> double                   	as<double>(SEXP m_sexp) ;
template<> int                      	as<int>(SEXP m_sexp) ;
template<> Rbyte                    	as<Rbyte>(SEXP m_sexp) ;
template<> std::string              	as<std::string>(SEXP m_sexp) ;
template<> std::vector<int>         	as< std::vector<int> >(SEXP m_sexp) ;
template<> std::vector<double>      	as< std::vector<double> >(SEXP m_sexp) ;
template<> std::vector<std::string> 	as< std::vector<std::string> >(SEXP m_sexp) ;
template<> std::vector<Rbyte>       	as< std::vector<Rbyte> >(SEXP m_sexp) ;
template<> std::vector<bool>        	as< std::vector<bool> >(SEXP m_sexp) ;


/* FIXME: turn the functions below into a template */

/* these do not take care of coercion*/
inline bool Rboolean_to_bool( int x){ return x == TRUE ; }
inline bool int_to_bool(int x){ return x != 0 ; }
inline bool double_to_bool(double x){ return x != 0.0 ; }
inline bool Rbyte_to_bool(Rbyte x){ return x != static_cast<Rbyte>(0) ; }

/* these take care of coercion */
inline int bool_to_Rboolean(bool x){ return x ? TRUE : FALSE ; }

inline int Rboolean_to_int(int x){ return (x==NA_LOGICAL) ? NA_INTEGER : x ; }
inline int double_to_int(double x){ 
	if (ISNAN(x)) return NA_INTEGER;
	else if (x > INT_MAX || x <= INT_MIN ) {
		return NA_INTEGER;
	}
	return static_cast<int>(x);
}
inline int Rbyte_to_int(Rbyte x){ return static_cast<int>(x); }

inline Rbyte Rboolean_to_Rbyte(int x){ return x == TRUE ? static_cast<Rbyte>(1) : static_cast<Rbyte>(0) ;}
inline Rbyte double_to_Rbyte(double x){ 
	if( x == NA_REAL) return (Rbyte)0 ; 
	int y = static_cast<int>(x) ;
	return (y < 0 || y > 255) ? (Rbyte)0 : (Rbyte)y ;
} 
inline Rbyte int_to_Rbyte(int x){
	return (x < 0 || x > 255) ? static_cast<Rbyte>(0) : static_cast<Rbyte>(x) ;
}

inline double Rbyte_to_double(Rbyte x){
	return static_cast<double>(x) ;
}
inline double int_to_double(int x){
	return x == NA_INTEGER ? NA_REAL : static_cast<double>(x) ;
}
inline double Rboolean_to_double(int x){
	return x == NA_LOGICAL ? NA_REAL : static_cast<double>(x) ;
}

inline int int_to_RBoolean(int x){ return ( x == NA_INTEGER ) ? NA_LOGICAL : (x!=0); }

template <typename T> SEXP wrap( const T& object){
	SEXP x = object ; /* let implicit conversion work */
	return x ;
}

template<> SEXP wrap(const int & v);
template<> SEXP wrap(const size_t& v) ; 
template<> SEXP wrap(const std::vector<int> & v);
template<> SEXP wrap(const std::set<int> & v);

template<> SEXP wrap(const double & v);
template<> SEXP wrap(const std::vector<double> & v);
template<> SEXP wrap(const std::set<double> & v);

template<> SEXP wrap(const Rbyte & v);
template<> SEXP wrap(const std::vector<Rbyte> & v);
template<> SEXP wrap(const std::set<Rbyte> & v);


template<> SEXP wrap(const bool & v); 
template<> SEXP wrap(const std::vector<bool> & v);

template<> SEXP wrap(const std::set<std::string> & v) ; 
SEXP wrap(const char* const v );
template<> SEXP wrap(const std::string & v) ;
template<> SEXP wrap(const std::vector<std::string> & v) ;

// range based wrappers

namespace internal{

// welcome to template metaprogramming !!
	
// template that returns the SEXP type that is appropriate for 
// the type T, this is allways VECSXP (lists) unless it is specialized
template <typename T> struct r_sexptype{ enum{ rtype = VECSXP }; } ;
template<> struct r_sexptype<int>{ enum{ rtype = INTSXP } ; } ;
template<> struct r_sexptype<size_t>{ enum{ rtype = INTSXP } ; } ;
template<> struct r_sexptype<double>{ enum{ rtype = REALSXP } ; } ;
template<> struct r_sexptype<bool>{ enum{ rtype = LGLSXP } ; } ;
template<> struct r_sexptype<std::string>{ enum{ rtype = STRSXP } ; } ;
template<> struct r_sexptype<Rcomplex>{ enum{ rtype = CPLXSXP } ; } ;

template<int RTYPE> struct storage_type{ typedef SEXP type ; } ;
template<> struct storage_type<INTSXP>{  typedef int type ; } ;
template<> struct storage_type<REALSXP>{ typedef double type ; } ;
template<> struct storage_type<CPLXSXP>{ typedef Rcomplex type ; } ;
template<> struct storage_type<RAWSXP>{ typedef Rbyte type ; } ;
template<> struct storage_type<LGLSXP>{ typedef int type ; } ;

template<int RTYPE, typename CTYPE> CTYPE* r_vector_start(SEXP x){ return static_cast<CTYPE*>(0) ; } ;
template<> int* r_vector_start<INTSXP,int>(SEXP x) ; 
template<> int* r_vector_start<LGLSXP,int>(SEXP x) ;
template<> double* r_vector_start<REALSXP,double>(SEXP x) ;
template<> Rbyte* r_vector_start<RAWSXP,Rbyte>(SEXP x) ; 
template<> Rcomplex* r_vector_start<CPLXSXP,Rcomplex>(SEXP x) ;                         

template <int RTYPE,typename CTYPE> CTYPE get_zero(){ return static_cast<CTYPE>(0) ; } ;
template<> Rcomplex get_zero<CPLXSXP,Rcomplex>() ;

// initializes a vector. The template does nothing because there is
template<int RTYPE> void r_init_vector(SEXP x){
	typedef storage_type<RTYPE> CTYPE ;
	CTYPE* start=r_vector_start<RTYPE>(x) ;
	std::fill( start, start + Rf_length(x), get_zero<RTYPE,CTYPE>(0) ) ;
}
// these are specifically overwritten so that they do nothing
// - character vectors : already initialized with ""
// - generic vectors   : already initialized with R_NilValue
// - expression vectors: already initialized with R_NilValue
template<> void r_init_vector<VECSXP>(SEXP x) ;
template<> void r_init_vector<EXPRSXP>(SEXP x) ;
template<> void r_init_vector<STRSXP>(SEXP x) ;


/* implementations of dispatch of range based wrap functions */

struct r_type_primitive_tag{} ;
struct r_type_string_tag{} ;
struct r_type_generic_tag{} ;
struct r_type_bool_tag{} ;

template <typename T> struct r_type_traits { typedef r_type_generic_tag category ; } ;
template<> struct r_type_traits<int>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<double>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<Rbyte>{ typedef r_type_primitive_tag category ; } ;
template<> struct r_type_traits<Rcomplex>{ typedef r_type_primitive_tag category ; } ;

template<> struct r_type_traits<bool>{ typedef r_type_bool_tag category ; } ;

template<> struct r_type_traits<std::string>{ typedef r_type_string_tag category ; } ;

template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_primitive_tag){ 
	size_t size = std::distance( first, last ) ;
	const int RTYPE = r_sexptype<T>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	std::copy( first, last, r_vector_start<RTYPE, storage_type<RTYPE> >(x) ) ; 
	UNPROTECT(1) ;
	return x ;
} ;

template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_bool_tag){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
	std::transform( first, last, LOGICAL(x), bool_to_Rboolean ) ; 
	UNPROTECT(1) ;
	return x ;
} ;


// this implementation is used when T is not a primitive type
// T needs to be wrappable though
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_generic_tag ){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) );
	size_t i =0 ;
	while( i < size ){
		SET_VECTOR_ELT( x, i, wrap(*first) ) ;
		i++ ;
		++first ;
	}
	UNPROTECT(1) ;
	return x ;
} ;

template<typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, r_type_string_tag ){
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	size_t i = 0 ;
	std::string buffer ;
	while( i < size ){
		buffer = *first ;
		SET_STRING_ELT( x, i, Rf_mkChar( buffer.c_str()) ) ;
		i++ ;
		++first ;
	}
	UNPROTECT(1) ;
	return x ;
}

template<typename InputIterator, typename T>
SEXP range_wrap_dispatch( InputIterator first, InputIterator last ){
	return range_wrap_dispatch___impl<InputIterator,T>( first, last, r_type_traits<T>::category() ) ;
}

// we use the iterator trait to make the dispatch
template <typename InputIterator>
SEXP range_wrap(InputIterator first, InputIterator last){
	return range_wrap_dispatch<InputIterator,std::iterator_traits<InputIterator>::value_type>( first, last ) ;
}

}


#ifdef HAS_INIT_LISTS
inline template<> SEXP wrap(const std::initializer_list<bool>& list) { return internal::range_wrap(list.begin(), list.end() ); }
inline template<> SEXP wrap(const std::initializer_list<std::string>& list ){ return internal::range_wrap(list.begin(), list.end()) ; }
inline template<> SEXP wrap(const std::initializer_list<SEXP>& list ){ return internal::range_wrap(list.begin(), list.end() ) ; }
inline template<> SEXP wrap(const std::initializer_list<Rbyte>& list) { return internal::range_wrap(list.begin(), list.end() ) ; }
inline template<> SEXP wrap(const std::initializer_list<double>& list) { return internal::range_wrap( list.begin(), list.end() ); } 
inline template<> SEXP wrap(const std::initializer_list<int>& list) { return internal::range_wrap( list.begin(), list.end() ) ;}
#endif



} // namespace Rcpp      



#include <Rcpp/RObject.h>

#endif
