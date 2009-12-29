// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RcppXPtr.h: Rcpp R/C++ interface class library -- smart external pointers
//
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

#ifndef RcppXPtr_h
#define RcppXPtr_h

#include <RcppCommon.h>
#include <RcppSexp.h>


template <typename T>
void delete_finalizer(SEXP p){
	if( TYPEOF(p) == EXTPTRSXP ){
		T* ptr = (T*) EXTPTR_PTR(p) ;
		delete ptr ;
	}
}

template <typename T>
class RcppXPtr : public RcppSexp {
	public:
		
		/** 
		 * constructs a RcppXPtr wrapping the external pointer (EXTPTRSXP SEXP)
		 *
		 * @param xp external pointer to wrap
		 */
		 explicit RcppXPtr(SEXP m_sexp) : RcppSexp::RcppSexp(m_sexp){} ;
		
		/**
		 * creates a new external pointer wrapping the dumb pointer p. 
		 * This calls R_PreserveObject to prevent the external pointer 
		 * from R garbage collection
		 * 
		 * @param p dumb pointer to some object
		 * @param set_delete_finalizer if set to true, a finalizer will 
		 *        be registered for the external pointer. The finalizer
		 *        is called when the xp is garbage collected. The finalizer 
		 *        is merely a call to the delete operator or the pointer
		 *        so you need to make sure the pointer can be deleted. 
		 */
  		explicit RcppXPtr(T* p, bool set_delete_finalizer) ;

  		/**
  		 * Returns a reference to the object wrapped. This allows this
  		 * object to look and feel like a dumb pointer to T
  		 */
  		T& operator*() const ;
  		
  		/**
  		 * Returns the dumb pointer. This allows to call the -> operator 
  		 * on this as if it was the dumb pointer
  		 */
  		T* operator->() const ;
  		
  		/**
  		 * Returns the 'protected' part of the external pointer, this is 
  		 * the SEXP that is passed in as the third argument of the 
  		 * R_MakeExternalPointer function. See Writing R extensions
  		 */
  		SEXP getProtected() ;
  		
  		/** 
  		 * Returns the 'tag' part of the external pointer, this is the 
  		 * SEXP that is passed in as the 2nd argument of the 
  		 * R_MakeExternalPointer function. See Writing R extensions
  		 */
  		SEXP getTag() ;
  		
  		void setDeleteFinalizer() ;
  		
};


template <typename T>
void delete_finalizer(SEXP p) {
	if( TYPEOF(p) == EXTPTRSXP ){
		T* ptr = (T*) EXTPTR_PTR(p) ;
		delete ptr ;
	}
}


template<typename T>
RcppXPtr<T>::RcppXPtr(T* p, bool set_delete_finalizer = true) : RcppSexp::RcppSexp() {
	m_sexp = R_MakeExternalPtr( (void*)p , R_NilValue, R_NilValue) ;
	if( set_delete_finalizer ){
		setDeleteFinalizer() ;
	}
	protect() ;
}

template<typename T>
void RcppXPtr<T>::setDeleteFinalizer(){
	R_RegisterCFinalizerEx( m_sexp, delete_finalizer<T> , FALSE) ; 
}

template<typename T>
T& RcppXPtr<T>::operator*() const {
	return *((T*)EXTPTR_PTR( m_sexp )) ;
}

template<typename T>
T* RcppXPtr<T>::operator->() const {
	return (T*)(EXTPTR_PTR(m_sexp));
}

template<typename T>
SEXP RcppXPtr<T>::getProtected(){
	return EXTPTR_PROT(m_sexp) ;
}

template<typename T>
SEXP RcppXPtr<T>::getTag(){
	return EXTPTR_TAG(m_sexp) ;
}


#endif
