// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// pfnonlinbs.h: Rcpp integration of SMC library -- PF Nonlinear Bootstrp
//
//    The declarations and externals for an implementation of the bootstrap
//    particle filter of "Novel approaches to nonlinear non-Gaussian
//    Bayesian state estimation", Gordon Salmond and Smith, 
//    IEE PROCEEDINGS-F 140(2):107-113, 1993
//
// Copyright (C) 2012         Dirk Eddelbuettel and Adam Johansen
//
// This file is part of RcppSMC.
//
// RcppSMC is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// RcppSMC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppSMC.  If not, see <http://www.gnu.org/licenses/>.

#include "smctc.h"

namespace nonlinbs {
    double logLikelihood(long lTime, const double & X);

    smc::particle<double> fInitialise(smc::rng *pRng);
    long fSelect(long lTime, const smc::particle<double> & p, smc::rng *pRng);
    void fMove(long lTime, smc::particle<double> & pFrom, smc::rng *pRng);

    double integrand_mean_x(const double&, void*);
    double integrand_var_x(const double&, void*);
}
