#!/usr/bin/r -t
#
# A faster lm() replacement based on GNU GSL
#
# This first appeared in the 'Intro to HPC tutorials'
# but has been wrapped in inline::cfunction() here
#
# Copyright (C) 2010 Dirk Eddelbuettel and Romain Francois
#
# This file is part of Rcpp.
#
# Rcpp is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

suppressMessages(library(Rcpp))
suppressMessages(library(inline))

lmViaGSL <- function() {
    src <- '

    SEXP rl = R_NilValue;
    char *exceptionMesg = NULL;

    try {
        RcppMatrixView<double> Xr(Xsexp);
        RcppVectorView<double> Yr(Ysexp);

        int i,j,n = Xr.dim1(), k = Xr.dim2();
        double chisq;

        gsl_matrix *X = gsl_matrix_alloc (n, k);
        gsl_vector *y = gsl_vector_alloc (n);
        gsl_vector *c = gsl_vector_alloc (k);
        gsl_matrix *cov = gsl_matrix_alloc (k, k);
        for (i = 0; i < n; i++) {
            for (j = 0; j < k; j++)
                gsl_matrix_set (X, i, j, Xr(i,j));
            gsl_vector_set (y, i, Yr(i));
        }

        gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc (n, k);
        gsl_multifit_linear (X, y, c, cov, &chisq, work);
        gsl_multifit_linear_free (work);

        RcppMatrix<double> CovMat(k, k);
        RcppVector<double> Coef(k);
        for (i = 0; i < k; i++) {
            for (j = 0; j < k; j++)
                CovMat(i,j) = gsl_matrix_get(cov,i,j);
            Coef(i) = gsl_vector_get(c,i);
        }
        gsl_matrix_free (X);
        gsl_vector_free (y);
        gsl_vector_free (c);
        gsl_matrix_free (cov);

        RcppResultSet rs;
        rs.add("coef", Coef);
        rs.add("covmat", CovMat);

        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    if (exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    return rl;
    '

    ## turn into a function that R can call
    ## compileargs redundant on Debian/Ubuntu as gsl headers are found anyway
    fun <- cfunction(signature(Ysexp="numeric", Xsexp="numeric"),
                     src,
                     includes="#include <gsl/gsl_multifit.h>",
                     Rcpp=TRUE,
                     cppargs="-I/usr/include",
                     libargs="-lgsl -lgslcblas")
}

checkLmViaGSL <- function(y, X) {
    fun <- lmViaGSL()
    res <- fun(y, X)
    fit <- lm(y ~ X - 1)
    rc <- all.equal( res[[1]], as.numeric(coef(fit))) &
          all.equal( res[[2]], matrix(as.numeric(vcov(fit)),ncol=10,byrow=FALSE))
    invisible(rc)
}

timeLmViaGSL <- function(y, X, N) {
    fun <- lmViaGSL();
    meantime <- mean(replicate(N, system.time(fun(y, X))["elapsed"]), trim=0.05)
}

set.seed(42)
n <- 5000
k <- 9
X <- cbind( rep(1,n), matrix(rnorm(n*k), ncol=k) )
truecoef <- 1:(k+1)
y <- as.numeric(X %*% truecoef + rnorm(n))

N <- 100

stopifnot(checkLmViaGSL(y, X))
mt <- timeLmViaGSL(y, X, N)
cat("GSL: Running", N, "simulations yields (trimmed) mean time", mt, "\n")
