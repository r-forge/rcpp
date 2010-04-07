// RcppGSLForward.h: Rcpp/GSL glue
//
// Copyright (C)  2010 Romain Francois and Dirk Eddelbuettel
//
// This file is part of RcppGSL.
//
// RcppGSL is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//                           
// RcppGSL is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppGSL.  If not, see <http://www.gnu.org/licenses/>.

#ifndef RCPPGSL__RCPPGSLFORWARD_H
#define RCPPGSL__RCPPGSLFORWARD_H

#include <RcppCommon.h>
#include <gsl/gsl_vector.h> 
#include <gsl/gsl_matrix.h>

/* forward declarations */
namespace Rcpp{
	
	namespace traits{
		/* support for gsl_complex */
		template<> struct r_sexptype_traits<gsl_complex>{ enum{ rtype = CPLXSXP } ; } ;
		template<> struct wrap_type_traits<gsl_complex> { typedef wrap_type_primitive_tag wrap_category; } ;
		template<> struct r_type_traits<gsl_complex>{ typedef r_type_primitive_tag r_category ; } ;
		template<> struct r_type_traits< std::pair<const std::string,gsl_complex> >{ typedef r_type_primitive_tag r_category ; } ;
	
		/* support for gsl_complex_float */
		template<> struct r_sexptype_traits<gsl_complex_float>{ enum{ rtype = CPLXSXP } ; } ;
		template<> struct wrap_type_traits<gsl_complex_float> { typedef wrap_type_primitive_tag wrap_category; } ;
		template<> struct r_type_traits<gsl_complex_float>{ typedef r_type_primitive_tag r_category ; } ;
		template<> struct r_type_traits< std::pair<const std::string,gsl_complex_float> >{ typedef r_type_primitive_tag r_category ; } ;
	
		/* support for gsl_complex_long_double */
		template<> struct r_sexptype_traits<gsl_complex_long_double>{ enum{ rtype = CPLXSXP } ; } ;
		template<> struct wrap_type_traits<gsl_complex_long_double> { typedef wrap_type_primitive_tag wrap_category; } ;
		template<> struct r_type_traits<gsl_complex_long_double>{ typedef r_type_primitive_tag r_category ; } ;
		template<> struct r_type_traits< std::pair<const std::string,gsl_complex_long_double> >{ typedef r_type_primitive_tag r_category ; } ;
	
	}
	
	namespace internal{
		template<> gsl_complex caster<Rcomplex,gsl_complex>( Rcomplex from) ;
		template<> Rcomplex caster<gsl_complex,Rcomplex>( gsl_complex from) ;
		
		template<> gsl_complex_float caster<Rcomplex,gsl_complex_float>( Rcomplex from) ;
		template<> Rcomplex caster<gsl_complex_float,Rcomplex>( gsl_complex_float from) ;
    
		template<> gsl_complex_long_double caster<Rcomplex,gsl_complex_long_double>( Rcomplex from) ;
		template<> Rcomplex caster<gsl_complex_long_double,Rcomplex>( gsl_complex_long_double from) ;
    }
    
	template <> SEXP wrap( const gsl_vector& ) ;
	template <> SEXP wrap( const gsl_vector_int& ) ;
	template <> SEXP wrap( const gsl_vector_float& ) ;
	template <> SEXP wrap( const gsl_vector_long& ) ;
	template <> SEXP wrap( const gsl_vector_char& ) ;
	template <> SEXP wrap( const gsl_vector_complex& ) ;
	template <> SEXP wrap( const gsl_vector_complex_float& ) ;
	template <> SEXP wrap( const gsl_vector_complex_long_double& ) ;
	template <> SEXP wrap( const gsl_vector_long_double& ) ;
	template <> SEXP wrap( const gsl_vector_short& ) ;
	template <> SEXP wrap( const gsl_vector_uchar& ) ;
	template <> SEXP wrap( const gsl_vector_uint& ) ;
	template <> SEXP wrap( const gsl_vector_ushort& ) ;
	template <> SEXP wrap( const gsl_vector_ulong& ) ;
	
	template <> SEXP wrap( const gsl_vector_view& ) ;
	template <> SEXP wrap( const gsl_vector_int_view& ) ;
	template <> SEXP wrap( const gsl_vector_float_view& ) ;
	template <> SEXP wrap( const gsl_vector_long_view& ) ;
	template <> SEXP wrap( const gsl_vector_char_view& ) ;
	template <> SEXP wrap( const gsl_vector_complex_view& ) ;
	template <> SEXP wrap( const gsl_vector_complex_float_view& ) ;
	template <> SEXP wrap( const gsl_vector_complex_long_double_view& ) ;
	template <> SEXP wrap( const gsl_vector_long_double_view& ) ;
	template <> SEXP wrap( const gsl_vector_short_view& ) ;
	template <> SEXP wrap( const gsl_vector_uchar_view& ) ;
	template <> SEXP wrap( const gsl_vector_uint_view& ) ;
	template <> SEXP wrap( const gsl_vector_ushort_view& ) ;
	template <> SEXP wrap( const gsl_vector_ulong_view& ) ;
	
	template <> SEXP wrap( const gsl_matrix& ) ;
	template <> SEXP wrap( const gsl_matrix_int& ) ;
	template <> SEXP wrap( const gsl_matrix_float& ) ;
	template <> SEXP wrap( const gsl_matrix_long& ) ;
	template <> SEXP wrap( const gsl_matrix_char& ) ;
	template <> SEXP wrap( const gsl_matrix_complex& ) ;
	template <> SEXP wrap( const gsl_matrix_complex_float& ) ;
	template <> SEXP wrap( const gsl_matrix_complex_long_double& ) ;
	template <> SEXP wrap( const gsl_matrix_long_double& ) ;
	template <> SEXP wrap( const gsl_matrix_short& ) ;
	template <> SEXP wrap( const gsl_matrix_uchar& ) ;
	template <> SEXP wrap( const gsl_matrix_uint& ) ;
	template <> SEXP wrap( const gsl_matrix_ushort& ) ;
	template <> SEXP wrap( const gsl_matrix_ulong& ) ;
	
}

#endif
