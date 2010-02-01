// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
/* :tabSize=4:indentSize=4:noTabs=false:folding=explicit:collapseFolds=1: */
//
// wrap.h: Rcpp R/C++ interface class library -- wrap implementations
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

#ifndef Rcpp_internal_wrap_h
#define Rcpp_internal_wrap_h

// this is a private header, included in RcppCommon.h
// don't include it directly

namespace Rcpp{

// pre-declaring wrap :
template <typename T> SEXP wrap(const T& object) ;

namespace internal{

// pre declaring
template <typename InputIterator> SEXP range_wrap(InputIterator first, InputIterator last) ;

// {{{ information about R vectors
// welcome to template metaprogramming !!

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
	typedef ::Rcpp::traits::storage_type<RTYPE> CTYPE ;
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
// }}}

// {{{ range wrap 
// {{{ unnamed range wrap
/**
 * Range based wrap implementation that deals with iterator over
 * primitive types (int, double, etc ...)
 * 
 * This produces an unnamed vector of the appropriate type
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_primitive_tag){ 
	size_t size = std::distance( first, last ) ;
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	std::copy( first, last, r_vector_start<RTYPE, typename ::Rcpp::traits::storage_type<RTYPE>::type >(x) ) ; 
	UNPROTECT(1) ;
	return x ;
} ;

/**
 * Range based wrap implementation that deals with iterators over bool
 * 
 * This produces an unnamed logical vector
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_bool_tag){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
	std::transform( first, last, LOGICAL(x), bool_to_Rboolean ) ; 
	UNPROTECT(1) ;
	return x ;
} ;


/** 
 * range based wrap implementation that deals with iterators over 
 * some type U. each U object is itself wrapped
 * 
 * This produces an unnamed generic vector (list)
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_generic_tag ){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) );
	size_t i =0 ;
	while( i < size ){
		SET_VECTOR_ELT( x, i, ::Rcpp::wrap(*first) ) ;
		i++ ;
		++first ;
	}
	UNPROTECT(1) ;
	return x ;
} ;

/**
 * Range based wrap implementation for iterators over std::string
 * 
 * This produces an unnamed character vector
 */
template<typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_string_tag ){
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

/**
 * Dispatcher for all range based wrap implementations
 * 
 * This uses the Rcpp::traits::r_type_traits to perform further dispatch
 */
template<typename InputIterator, typename T>
SEXP range_wrap_dispatch( InputIterator first, InputIterator last ){
	return range_wrap_dispatch___impl<InputIterator,T>( first, last, typename ::Rcpp::traits::r_type_traits<T>::r_category() ) ;
}
// }}}

// {{{ named range wrap
/** 
 * range based wrap implementation that deals with iterators over
 * pair<const string,T> where T is a primitive type : int, double ...
 * 
 * This produces a named R vector of the appropriate type
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_pairstring_primitive_tag){ 
	size_t size = std::distance( first, last ) ;
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<typename T::second_type>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	SEXP names = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	typedef typename ::Rcpp::traits::storage_type<RTYPE>::type CTYPE ;
	CTYPE* start = r_vector_start<RTYPE,CTYPE>(x) ;
	size_t i =0;
	std::string buf ; 
	for( ; i<size; i++, ++first){
		start[i] = (*first).second ;
		buf = (*first).first ;
		SET_STRING_ELT( names, i, Rf_mkChar(buf.c_str()) ) ;
	}
	::Rf_setAttrib( x, R_NamesSymbol, names ) ;
	UNPROTECT(2) ; /* x, names */
	return x ;
} ;

/**
 * Range based wrap implementation that deals with iterators
 * over pair<const string,bool>
 *
 * This produces a named character vector
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_pairstring_bool_tag){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
	SEXP names = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	size_t i =0;
	std::string buf ;
	int* start = LOGICAL(x) ;
	for( ; i<size; i++, ++first){
		start[i] = bool_to_Rboolean( (*first).second );
		buf = (*first).first ;
		SET_STRING_ELT( names, i, Rf_mkChar(buf.c_str()) ) ;
	}
	::Rf_setAttrib( x, R_NamesSymbol, names ) ;
	UNPROTECT(2) ; /* x, names */
	return x ;
} ;


/**
 * Range based wrap implementation that deals with iterators over
 * pair<const string, U> where U is wrappable. This is the kind of 
 * iterators that are produced by map<string,U>
 * 
 * This produces a named generic vector (named list). The first 
 * element of the list contains the result of a call to wrap on the 
 * object of type U, etc ...
 *
 * The names are taken from the keys
 */
template <typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_pairstring_generic_tag ){ 
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) );
	SEXP names = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	size_t i =0 ;
	std::string buf ;
	SEXP element = R_NilValue ;
	while( i < size ){
		element = ::Rcpp::wrap( first->second ) ;
		buf = first->first ;
		SET_VECTOR_ELT( x, i, element ) ;
		SET_STRING_ELT( names, i, Rf_mkChar(buf.c_str()) ) ; 
		i++ ;
		++first ;
	}
	::Rf_setAttrib( x, R_NamesSymbol, names ) ;
	UNPROTECT(2) ; /* x, names */
	return x ;
} ;

/**
 * Range based wrap for iterators over std::pair<const std::string, std::string>
 *
 * This is mainly used for wrapping map<string,string> and friends 
 * which happens to produce iterators over pair<const string, string>
 *
 * This produces a character vector containing copies of the 
 * string iterated over. The names of the vector is set to the keys
 * of the pair
 */
template<typename InputIterator, typename T>
SEXP range_wrap_dispatch___impl( InputIterator first, InputIterator last, ::Rcpp::traits::r_type_pairstring_string_tag ){
	size_t size = std::distance( first, last ) ;
	SEXP x = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	SEXP names = PROTECT( Rf_allocVector( STRSXP, size ) ) ;
	size_t i = 0 ;
	std::string buffer ;
	while( i < size ){
		buffer = first->second ;
		SET_STRING_ELT( x, i, Rf_mkChar( buffer.c_str()) ) ;
		
		buffer = first->first ;
		SET_STRING_ELT( names, i, Rf_mkChar( buffer.c_str()) ) ;
		
		i++ ;
		++first ;
	}
	::Rf_setAttrib( x, R_NamesSymbol, names ) ;
	UNPROTECT(2) ; /* x, names */
	return x ;
}
// }}}

// we use the iterator trait to make the dispatch
/**
 * range based wrap. This uses the std::iterator_traits class
 * to perform further dispatch
 */
template <typename InputIterator>
SEXP range_wrap(InputIterator first, InputIterator last){
	return range_wrap_dispatch<InputIterator,typename std::iterator_traits<InputIterator>::value_type>( first, last ) ;
}
// }}}

// {{{ primitive wrap (wrapping a single primitive value)

/**
 * primitive wrap for 'easy' primitive types: int, double, Rbyte, Rcomplex
 *
 * This produces a vector of length 1 of the appropriate type
 */
template <typename T>
SEXP primitive_wrap__impl( const T& object, ::Rcpp::traits::r_type_primitive_tag ){
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, 1 ) );
	r_vector_start<RTYPE, typename ::Rcpp::traits::storage_type<RTYPE>::type >(x)[0] = object ;
	UNPROTECT(1);
	return x;
}

/**
 * primitive wrap for bool
 *
 * This produces a logical vector of length 1
 */
template <typename T>
SEXP primitive_wrap__impl( const T& object, ::Rcpp::traits::r_type_bool_tag){
	SEXP x = PROTECT( ::Rf_allocVector( LGLSXP, 1) );
	LOGICAL(x)[0] = static_cast<int>(object);
	UNPROTECT(1) ; /* x */
	return x;
}

/**
 * primitive wrap for types that can be converted implicitely to std::string
 * 
 * This produces a character vector of length 1 containing the std::string
 */
template <typename T>
SEXP primitive_wrap__impl( const T& object, ::Rcpp::traits::r_type_string_tag){
	SEXP x = PROTECT( ::Rf_allocVector( STRSXP, 1) ) ;
	std::string y = object ; /* give a chance to implicit conversion */
	SET_STRING_ELT( x, 0, Rf_mkChar(y.c_str()) ) ;
	UNPROTECT(1) ;
	return x; 
}

/**
 * called when T is a primitive type : int, bool, double, std::string, etc ...
 * This uses the Rcpp::traits::r_type_traits on the type T to perform
 * further dispatching and wrap the object into an vector of length 1
 * of the appropriate SEXP type 
 */
template <typename T>
SEXP primitive_wrap(const T& object){
	return primitive_wrap__impl( object, typename ::Rcpp::traits::r_type_traits<T>::r_category() ) ;
}
// }}}

// {{{ unknown
/**
 * Called when the type T is known to be implicitely convertible to 
 * SEXP. It uses the implicit conversion to SEXP to wrap the object
 * into a SEXP
 */
template <typename T>
SEXP wrap_dispatch_unknown( const T& object, true_type ){
	// here we know (or assume) that T is convertible to SEXP
	SEXP x = object ;
	return x ;
}

/** 
 * Called when no implicit conversion to SEXP is possible
 * This generates compile time errors
 */
template <typename T>
SEXP wrap_dispatch_unknown( const T& object, false_type){
	// here we know that T is not convertible to SEXP
#ifdef HAS_CXX0X
	static_assert( !sizeof(T), "cannot convert type to SEXP" ) ;
#else
	// leave the cryptic message
	SEXP x = object ; 
	return x ;
#endif
	return R_NilValue ; // -Wall
}
// }}}

// {{{ wrap dispatch
/** 
 * generic wrap for stl containers. This implementation is used
 * when the type T is an STL-like container, with a begin() method
 * and an end() method
 *
 * further dispatch is performed internally by the range_wrap 
 * template based on the type of object iterated over
 */
template <typename T> SEXP wrap_dispatch( const T& object, ::Rcpp::traits::wrap_type_stl_container_tag ){
	return range_wrap( object.begin(), object.end() ) ;
}

/**
 * wrapping a __single__ primitive type : int, double, std::string, size_t, 
 * Rbyte, Rcomplex
 */
template <typename T> SEXP wrap_dispatch( const T& object, ::Rcpp::traits::wrap_type_primitive_tag ){
	return primitive_wrap( object ) ;
}

/** 
 * This is called by wrap when the wrap_type_traits is wrap_type_unknown_tag
 * This tries to identify is an implicit conversion to SEXP is possible
 * ( the type T defines operator SEXP() ) and uses it, otherwise generates
 * a compile error
 */
template <typename T> SEXP wrap_dispatch( const T& object, ::Rcpp::traits::wrap_type_unknown_tag ){
	return wrap_dispatch_unknown( object, typename is_convertible<T,SEXP>::type() ) ;
}
// }}}

} // internal

/**
 * wraps an object of type T in a SEXP
 *
 * This method depends on the Rcpp::traits::wrap_type_traits trait 
 * class to dispatch to the appropriate internal implementation 
 * method
 * 
 * If your type has a begin and end method returning stl-like iterator
 * you should specialize the wrap_type_traits template so that it 
 * defines wrap_category to be ::Rcpp::traits::wrap_type_stl_container_tag
 */
template <typename T> SEXP wrap(const T& object){
	return internal::wrap_dispatch( object, typename ::Rcpp::traits::wrap_type_traits<T>::wrap_category() ) ;
}
// {{{ // explicit instanciations (not needed)
// template SEXP wrap<int>(const int& object) ;
// template SEXP wrap<double>(const double& object) ;
// template SEXP wrap<Rbyte>(const Rbyte& object) ;
// template SEXP wrap<Rcomplex>(const Rcomplex& object) ;
// template SEXP wrap<bool>(const bool& object) ;
// template SEXP wrap<std::string>(const std::string& object) ;
// template SEXP wrap< std::vector<int> >( const std::vector<int>& object ) ;
// template SEXP wrap< std::vector<double> >( const std::vector<double>& object ) ;
// template SEXP wrap< std::vector<Rbyte> >( const std::vector<Rbyte>& object ) ;
// template SEXP wrap< std::vector<Rcomplex> >( const std::vector<Rcomplex>& object ) ;
// template SEXP wrap< std::vector<bool> >( const std::vector<bool>& object ) ;
// 
// template SEXP wrap< std::set<int> >( const std::set<int>& object ) ;
// template SEXP wrap< std::set<double> >( const std::set<double>& object ) ;
// template SEXP wrap< std::set<Rbyte> >( const std::set<Rbyte>& object ) ;
// 
// template SEXP wrap< std::deque<int> >( const std::deque<int>& object ) ;
// template SEXP wrap< std::deque<double> >( const std::deque<double>& object ) ;
// template SEXP wrap< std::deque<Rbyte> >( const std::deque<Rbyte>& object ) ;
// template SEXP wrap< std::deque<Rcomplex> >( const std::deque<Rcomplex>& object ) ;
// template SEXP wrap< std::deque<bool> >( const std::deque<bool>& object ) ;
// }}}

// special cases - FIXME : these are not template specializations of wrap<>
inline SEXP wrap(const char* const v ){ return Rf_mkString(v) ; } ;

// wrap( { ... } ) : disabled for now
// #ifdef HAS_INIT_LISTS
// inline SEXP wrap(std::initializer_list<bool> v) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<std::string> v ) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<SEXP> v ) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<Rbyte> v) { return internal::range_wrap( v.begin() , v.end() ); };
// inline SEXP wrap(std::initializer_list<double> v) { return internal::range_wrap( v.begin() , v.end() ); } ; 
// inline SEXP wrap(std::initializer_list<int> v) { return internal::range_wrap( v.begin() , v.end() ); } ; 
// #endif


} // Rcpp

#endif
