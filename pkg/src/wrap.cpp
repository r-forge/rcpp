// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// wrap.cpp: Rcpp R/C++ interface class library -- general R object wrapper
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

#include <Rcpp.h>

namespace Rcpp{

// RObject wrap(SEXP m_sexp){
// 	switch( TYPEOF(m_sexp) ){
// 		case SYMSXP:
// 			return Symbol(m_sexp) ;
// 		case ENVSXP:
// 			return Environment(m_sexp);
// 		case LISTSXP:
// 		case DOTSXP: /* maybe change this later */
// 			return Pairlist(m_sexp);
// 		case CLOSXP:
// 		case SPECIALSXP: /* maybe change this later */
// 		case BUILTINSXP: /* maybe change this later */
// 			return Function(m_sexp);
// 		case LANGSXP:
// 			return Language(m_sexp);
// 		case LGLSXP:
// 			return LogicalVector(m_sexp);
// 		case INTSXP:
// 			return IntegerVector(m_sexp);
// 		case REALSXP:
// 			return NumericVector(m_sexp);
// 		case CPLXSXP:
// 			return ComplexVector(m_sexp);
// 		case STRSXP:
// 			return CharacterVector(m_sexp);
// 		case VECSXP:
// 			return List(m_sexp);
// 		case EXPRSXP:
// 			return ExpressionVector(m_sexp);
// 		case WEAKREFSXP:
// 			return WeakReference(m_sexp);
// 		case RAWSXP:
// 			return RawVector(m_sexp);
// 		default:
// 			break ;
// 	}
// 	/* for all other, just use RObject */
// 	return RObject(m_sexp) ;
// }
 
// template<> SEXP wrap(const bool & v){
//     logTxt("RObject from bool\n");
//     LogicalVector o(Rf_ScalarLogical(v));
//     return o ;
// }
// 
// template<> SEXP wrap(const double & v){
//     logTxt("RObject from double\n");
//     NumericVector o(Rf_ScalarReal(v));
//     return o ;
// }
// 
// template<> SEXP wrap(const int & v){
//     logTxt("RObject from int\n");
//     IntegerVector o(Rf_ScalarInteger(v));
//     return o ;
// }
// 
// template<> SEXP wrap(const size_t & v){
//     logTxt("RObject from size_t\n");
//     IntegerVector o(Rf_ScalarInteger(static_cast<int>(v)));
//     return o ;
// }
//    
// template<> SEXP wrap(const Rbyte & v){
//     logTxt("RObject from raw\n");
//     RawVector o(Rf_ScalarRaw(v));
//     return o ;
// }

// template<> SEXP wrap(const std::vector<bool> & v){ return internal::range_wrap( v.begin(), v.end() ) ; }
// template<> SEXP wrap(const std::vector<int> & v){ return internal::range_wrap( v.begin(), v.end() ) ; }
// template<> SEXP wrap(const std::vector<double> & v){ return internal::range_wrap( v.begin(), v.end() ) ; }
// template<> SEXP wrap(const std::vector<Rbyte> & v){ return internal::range_wrap( v.begin(), v.end() ) ; }
// template<> SEXP wrap(const std::vector<std::string> & v){ return internal::range_wrap(v.begin(), v.end()); }

/* sets */

// template<> SEXP wrap(const std::set<int> & v){
//     logTxt("RObject from set<int>\n");
//     size_t n = v.size();
//     SEXP m_sexp = PROTECT( Rf_allocVector(INTSXP, n) );
//     copy( v.begin(), v.end(), INTEGER(m_sexp) ) ;
//     IntegerVector o(m_sexp) ;
//     UNPROTECT(1) ;
//     return o ;
// }
// 
// template<> SEXP wrap(const std::set<double> & v){
//     logTxt("RObject from set<double>\n");
//     size_t n = v.size();
//     SEXP m_sexp = PROTECT( Rf_allocVector(REALSXP, n) );
//     copy( v.begin(), v.end(), REAL(m_sexp) ) ;
//     NumericVector o(m_sexp) ;
//     UNPROTECT(1) ;
//     return o ;
// }
// 
// template<> SEXP wrap(const std::set<Rbyte> & v){
//     logTxt("RObject from set<Rbyte> \n");
//     size_t n = v.size();
//     SEXP m_sexp = PROTECT( Rf_allocVector(RAWSXP, n) );
//     copy( v.begin(), v.end(), RAW(m_sexp) ) ;
//     RawVector o(m_sexp) ;
//     UNPROTECT(1) ;
//     return o ;
// }
// 
// template<> SEXP wrap(const std::set<std::string> & v){
//     logTxt("RObject from set<string>\n");
//     size_t n = v.size();
//     SEXP m_sexp = PROTECT( Rf_allocVector(STRSXP, n) );
//     size_t i=0;
//     std::set<std::string>::iterator it = v.begin(); 
//     while( i<n ){
//     	SET_STRING_ELT(m_sexp, i, Rf_mkChar(it->c_str()));
//     	i++ ;
//     	it++; 
//     }
//     CharacterVector o(m_sexp) ;
//     UNPROTECT(1) ;
//     return o ;
// }

} // namespace Rcpp
