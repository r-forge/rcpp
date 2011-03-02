// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 4 -*-
//
// Col_meat.h: Rcpp/Armadillo glue
//
// Copyright (C)  2011 Dirk Eddelbuettel, Romain Francois and Douglas Bates
//
// This file is part of RcppArmadillo.
//
// RcppArmadillo is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// RcppArmadillo is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppArmadillo.  If not, see <http://www.gnu.org/licenses/>.

#ifndef RCPPARMADILLO_COL_MEAT_H
#define RCPPARMADILLO_COL_MEAT_H

template <typename eT>
template <int RTYPE, bool NA, typename VECTOR>
inline Col<eT>::Col( const Rcpp::VectorBase<RTYPE,NA,VECTOR>& X )
    : Mat<eT>( X ) 
{	
    arma_extra_debug_sigprint(this);
}

template <typename eT>
template <int RTYPE, bool NA, typename MATRIX>
inline Col<eT>::Col( const Rcpp::MatrixBase<RTYPE,NA,MATRIX>& X ) 
    : Mat<eT>( X ) 
{	
    arma_extra_debug_sigprint(this);
	
    arma_debug_check( (Mat<eT>::n_cols > 1), "Col(): incompatible dimensions" );

    access::rw(Mat<eT>::vec_state) = 1;
}

#endif
