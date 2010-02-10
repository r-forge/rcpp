// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SEXP_Vector.h: Rcpp R/C++ interface class library -- template for expression vector and generic vector
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

#ifndef Rcpp_SEXP_Vector_h
#define Rcpp_SEXP_Vector_h

#include <RcppCommon.h>
#include <Rcpp/VectorBase.h>
#include <Rcpp/Environment.h>
#include <Rcpp/Dimension.h>

namespace Rcpp{

class SEXP_Vector_Base : public VectorBase {
public:
	
	class iterator ;
	
	class Proxy {
	public:
		Proxy( SEXP_Vector_Base& v, size_t i ) ;
		
		Proxy& operator=(SEXP rhs) ; 
		
		Proxy& operator=(const Proxy& rhs) ;
		
		template <typename T>
		Proxy& operator=( const T& rhs){
			set( wrap(rhs) ) ;
			return *this; 
		}
		
		inline operator SEXP() const { return get() ; }
		template <typename U> operator U(){
			return as<U>( get() ) ;
		}
		
		void swap(Proxy& other) ;
		
		friend class iterator ;
	private:
		SEXP_Vector_Base& parent; 
		size_t index ;
		void move(int n) ;
		void set(SEXP x) ;
		SEXP get() const ;
	} ;

	class iterator {
	public:
		typedef Proxy& reference ;
		typedef Proxy* pointer ;
		typedef int difference_type ;
		typedef Proxy value_type;
		typedef std::random_access_iterator_tag iterator_category ;
		
		iterator( SEXP_Vector_Base& object, int index );
		
		inline iterator& operator++(){ proxy.move(1) ; return *this; }
		inline iterator& operator++(int){ proxy.move(1) ; return *this; }
		
		inline iterator& operator--() { proxy.move(-1) ; return *this; }
		inline iterator& operator--(int) { proxy.move(-1) ; return *this; }
		                    
		inline iterator operator+(difference_type n) const { return iterator( proxy.parent, proxy.index + n ) ; }
		inline iterator operator-(difference_type n) const { return iterator( proxy.parent, proxy.index - n ) ; }
		
		inline iterator& operator+=(difference_type n) { proxy.move(n) ; return *this; }
		inline iterator& operator-=(difference_type n) { proxy.move(-n) ; return *this; }

		inline reference operator*() { return proxy ; }
		inline pointer operator->(){ return &proxy ; }
		
		inline bool operator==( const iterator& y) { return this->proxy.index == y.proxy.index && this->proxy.parent == y.proxy.parent; }
		inline bool operator!=( const iterator& y) { return this->proxy.index != y.proxy.index || this->proxy.parent != y.proxy.parent; }
		inline bool operator< ( const iterator& y) { return this->proxy.index <  y.proxy.index ; }
		inline bool operator> ( const iterator& y) { return this->proxy.index >  y.proxy.index ; }
		inline bool operator<=( const iterator& y) { return this->proxy.index <= y.proxy.index ; }
		inline bool operator>=( const iterator& y) { return this->proxy.index >= y.proxy.index ; }
		
		inline difference_type operator-(const iterator& y) { return this->proxy.index - y.proxy.index ; }
		
	private:
		Proxy proxy ;
	};
	
	SEXP_Vector_Base() ; 
	
	inline const Proxy operator[]( int i ) const throw(index_out_of_bounds){
		return Proxy(const_cast<SEXP_Vector_Base&>(*this), offset(i)) ;
	}
	inline Proxy operator[]( int i ) throw(index_out_of_bounds){
		return Proxy(*this, offset(i) ) ; 
	}
	
	const Proxy operator[]( const std::string& name) const ; 
	Proxy operator[](const std::string& name) ;
    	
	inline iterator begin() { return iterator(*this, 0) ; }
	inline iterator end() { return iterator(*this, size() ) ; }
	
	Proxy operator()( const size_t& i) throw(index_out_of_bounds) ; 
	Proxy operator()( const size_t& i, const size_t& j) throw(index_out_of_bounds,not_a_matrix) ;
	
	friend class Proxy;
	friend class iterator ;
	
} ;

template <int RTYPE>
class SEXP_Vector : public SEXP_Vector_Base{
public:
	SEXP_Vector() : SEXP_Vector_Base(){} ; 
	
	SEXP_Vector(const SEXP_Vector& other) : SEXP_Vector_Base(){
		setSEXP( other.asSexp() ) ;
	} ;
	
	SEXP_Vector& operator=(const SEXP_Vector& other){
		setSEXP( other.asSexp() ) ;
		return *this ;
	}
	
	SEXP_Vector(SEXP x) : SEXP_Vector_Base() {
		SEXP y = r_cast<RTYPE>(x) ;
		setSEXP( y );
	}
	
	SEXP_Vector(const size_t& size) : SEXP_Vector_Base(){
		setSEXP( Rf_allocVector( RTYPE, size ) ) ;
	}
	
	SEXP_Vector(const Dimension& dims) : SEXP_Vector_Base(){
		setSEXP( Rf_allocVector( RTYPE, dims.prod() ) ) ;
		if( dims.size() > 1) attr( "dim" ) = dims ;
	}

	template <typename InputIterator>
	SEXP_Vector(InputIterator first, InputIterator last) : SEXP_Vector_Base() {
		assign( first, last ) ;
	}
	
#ifdef HAS_INIT_LISTS
	SEXP_Vector( std::initializer_list<SEXP> list) : SEXP_Vector_Base(){
		assign( list.begin(), list.end() ) ;
	} ;
#endif
	
	template <typename InputIterator>
	void assign( InputIterator first, InputIterator last){
		setSEXP( r_cast<RTYPE>( wrap( first, last) ) ) ;
	}

	
}   ;

typedef SEXP_Vector<VECSXP> GenericVector ;
typedef GenericVector List ;

} //namespace Rcpp

namespace std {
	template<> void swap( Rcpp::SEXP_Vector_Base::Proxy& a, Rcpp::SEXP_Vector_Base::Proxy& b) ;
}

#endif
