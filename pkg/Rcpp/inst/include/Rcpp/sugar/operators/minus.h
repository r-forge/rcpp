// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// minus.h: Rcpp R/C++ interface class library -- operator-
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

#ifndef Rcpp__sugar__minus_h
#define Rcpp__sugar__minus_h

namespace Rcpp{
namespace sugar{

	template <int RTYPE, bool LHS_NA, typename LHS_T, bool RHS_NA, typename RHS_T >
	class Minus_Vector_Vector : public Rcpp::VectorBase<RTYPE,true, Minus_Vector_Vector<RTYPE,LHS_NA,LHS_T,RHS_NA,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T> RHS_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Minus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE x = lhs[i] ; 
			if( Rcpp::traits::is_na<RTYPE>( x ) ) return x ;
			STORAGE y = rhs[i] ; 
			return Rcpp::traits::is_na<RTYPE>( y ) ? y : ( x - y ) ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;
	
	
	template <int RTYPE, typename LHS_T, bool RHS_NA, typename RHS_T >
	class Minus_Vector_Vector<RTYPE,false,LHS_T,RHS_NA,RHS_T> : public Rcpp::VectorBase<RTYPE,true, Minus_Vector_Vector<RTYPE,false,LHS_T,RHS_NA,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T> RHS_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Minus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE y = rhs[i] ; 
			if( Rcpp::traits::is_na<RTYPE>( y ) ) return y ;
			return lhs[i] - y ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;

	
	template <int RTYPE, bool LHS_NA, typename LHS_T, typename RHS_T >
	class Minus_Vector_Vector<RTYPE,LHS_NA,LHS_T,false,RHS_T> : public Rcpp::VectorBase<RTYPE,true, Minus_Vector_Vector<RTYPE,LHS_NA,LHS_T,false,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,false,RHS_T> RHS_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Minus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE x = lhs[i] ; 
			if( Rcpp::traits::is_na<RTYPE>( x ) ) return x ;
			return x - rhs[i] ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;
	
	
	template <int RTYPE, typename LHS_T, typename RHS_T >
	class Minus_Vector_Vector<RTYPE,false,LHS_T,false,RHS_T> : public Rcpp::VectorBase<RTYPE,false, Minus_Vector_Vector<RTYPE,false,LHS_T,false,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,false,RHS_T> RHS_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Minus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			return lhs[i] - rhs[i] ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;

	
	
	
	template <int RTYPE, bool NA, typename T>
	class Minus_Vector_Primitive : public Rcpp::VectorBase<RTYPE,true, Minus_Vector_Primitive<RTYPE,NA,T> > {
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef typename Rcpp::VectorBase<RTYPE,NA,T> VEC_TYPE ;
		typedef STORAGE (Minus_Vector_Primitive::*METHOD)(int) const ;
		
		Minus_Vector_Primitive( const VEC_TYPE& lhs_, STORAGE rhs_ ) : 
			lhs(lhs_), rhs(rhs_){
			
			m = Rcpp::traits::is_na<RTYPE>(rhs) ? 
				&Minus_Vector_Primitive::rhs_is_na :
				&Minus_Vector_Primitive::rhs_is_not_na ;
			
		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i) ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const VEC_TYPE& lhs ;
		STORAGE rhs ;
		METHOD m ;
		
		inline STORAGE rhs_is_na(int i) const { return rhs ; }
		inline STORAGE rhs_is_not_na(int i) const { 
			STORAGE x = lhs[i] ;
			return Rcpp::traits::is_na<RTYPE>(x) ? x : (x - rhs) ;
		}
		
	} ;
	

	template <int RTYPE, typename T>
	class Minus_Vector_Primitive<RTYPE,false,T> : public Rcpp::VectorBase<RTYPE,true, Minus_Vector_Primitive<RTYPE,false,T> > {
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef typename Rcpp::VectorBase<RTYPE,false,T> VEC_TYPE ;
		typedef STORAGE (Minus_Vector_Primitive::*METHOD)(int) const ;
		
		Minus_Vector_Primitive( const VEC_TYPE& lhs_, STORAGE rhs_ ) : 
			lhs(lhs_), rhs(rhs_){

			m = Rcpp::traits::is_na<RTYPE>(rhs) ? 
				&Minus_Vector_Primitive::rhs_is_na :
				&Minus_Vector_Primitive::rhs_is_not_na ;
				
		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i) ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const VEC_TYPE& lhs ;
		STORAGE rhs ;
		METHOD m ;
		
		inline STORAGE rhs_is_na(int i) const { return rhs ; }
		inline STORAGE rhs_is_not_na(int i) const { 
			STORAGE x = rhs[i] ;
			return Rcpp::traits::is_na<RTYPE>(x) ? x : (lhs - x) ;
		}
		
	} ;

	
	
	
	
	
	template <int RTYPE, bool NA, typename T>                                                   
	class Minus_Primitive_Vector : public Rcpp::VectorBase<RTYPE,true, Minus_Primitive_Vector<RTYPE,NA,T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,NA,T> VEC_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ; 
		typedef STORAGE (Minus_Primitive_Vector::*METHOD)(int) const ;
		
		Minus_Primitive_Vector( STORAGE lhs_, const VEC_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {
				
			m = Rcpp::traits::is_na<RTYPE>(lhs) ? 
				&Minus_Primitive_Vector::lhs_is_na :
				&Minus_Primitive_Vector::lhs_is_not_na ;

		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i) ;
		}
		
		inline int size() const { return rhs.size() ; }
	
		
	private:
		STORAGE lhs ;
		const VEC_TYPE& rhs ;
		METHOD m ;
		
		inline STORAGE lhs_is_na(int i) const { return lhs ; }
		inline STORAGE lhs_is_not_na(int i) const { 
			return lhs - rhs[i] ;
		}
		
	} ;

	
	template <int RTYPE, typename T>                                                   
	class Minus_Primitive_Vector<RTYPE,false,T> : public Rcpp::VectorBase<RTYPE,true, Minus_Primitive_Vector<RTYPE,false,T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,T> VEC_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ; 
		typedef STORAGE (Minus_Primitive_Vector::*METHOD)(int) const ;
		
		Minus_Primitive_Vector( STORAGE lhs_, const VEC_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {
				
			m = Rcpp::traits::is_na<RTYPE>(rhs) ? 
				&Minus_Primitive_Vector::lhs_is_na :
				&Minus_Primitive_Vector::lhs_is_not_na ;

		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i) ;
		}
		
		inline int size() const { return rhs.size() ; }
	
		
	private:
		STORAGE lhs ;
		const VEC_TYPE& rhs ;
		METHOD m ;
		
		inline STORAGE lhs_is_na(int i) const { return lhs ; }
		inline STORAGE lhs_is_not_na(int i) const { 
			return lhs - rhs[i] ;
		}
		
	} ;


}
}

template <int RTYPE,bool NA, typename T>
inline Rcpp::sugar::Minus_Vector_Primitive< RTYPE , NA, T >
operator-( 
	const Rcpp::VectorBase<RTYPE,NA,T>& lhs, 
	typename Rcpp::traits::storage_type<RTYPE>::type rhs 
) {
	return Rcpp::sugar::Minus_Vector_Primitive<RTYPE,NA,T>( lhs, rhs ) ;
}


template <int RTYPE,bool NA, typename T>
inline Rcpp::sugar::Minus_Primitive_Vector< RTYPE , NA,T>
operator-( 
	typename Rcpp::traits::storage_type<RTYPE>::type lhs, 
	const Rcpp::VectorBase<RTYPE,NA,T>& rhs
) {
	return Rcpp::sugar::Minus_Primitive_Vector<RTYPE,NA,T>( lhs, rhs ) ;
}

template <int RTYPE,bool LHS_NA, typename LHS_T, bool RHS_NA, typename RHS_T>
inline Rcpp::sugar::Minus_Vector_Vector< 
	RTYPE , 
	LHS_NA, LHS_T, 
	RHS_NA, RHS_T
	>
operator-( 
	const Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T>& lhs,
	const Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T>& rhs
) {
	return Rcpp::sugar::Minus_Vector_Vector<
		RTYPE, 
		LHS_NA,LHS_T,
		RHS_NA,RHS_T
		>( lhs, rhs ) ;
}

#endif
