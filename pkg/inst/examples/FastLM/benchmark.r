#!/usr/bin/r -t
#
# Comparison benchmark
#
# This improves on the previous version using GNU GSL
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

lmViaArmadillo <- function() {
    src <- '

    SEXP rl = R_NilValue;
    char *exceptionMesg = NULL;

    try {
        Rcpp::NumericVector yr(Ysexp);
	Rcpp::NumericVector Xr(Xsexp);
        std::vector<int> dims = Xr.attr("dim") ;
        int n = dims[0];
        int k = dims[1];
        // use advanced armadillo constructors:

        arma::mat X(Xr.begin(), n, k, false);
        arma::colvec y(yr.begin(), yr.size());

        //std::cout << ay << std::endl;
        //std::cout << aX << std::endl;

        arma::colvec coef = solve(X, y);
        //std::cout << coef << std::endl;

        // compute std. error of the coefficients
        arma::colvec resid = y - X*coef;
        double rss  = trans(resid)*resid;
        double sig2 = rss/(n-k);
        arma::mat covmat = sig2 * arma::inv(arma::trans(X)*X);
        //arma::colvec stderrest = sqrt(arma::diagview(covmat));

        RcppVector<double> StdErr(k);
        RcppVector<double> Coef(k);
        for (int i = 0; i < k; i++) {
            Coef(i) = coef(i);
            StdErr(i) = covmat(i,i);
        }
        //Rcpp::NumericVector coefr(k);
        //Rcpp::NumericVector stderrestr(k);
        //for (int i=0; i<k; i++) {
        //    coefr[i]      = coef[i];
        //    stderrestr[i] = stderrestr[i];
        //}

        RcppResultSet rs;
        rs.add("coef", Coef);
        rs.add("stderr", StdErr);

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
                     includes="#include <armadillo>",
                     Rcpp=TRUE,
                     cppargs="-I/usr/include",
                     libargs="-larmadillo")
}

lmViaGSL <- function() {
    src <- '

    SEXP rl = R_NilValue;
    char *exceptionMesg = NULL;

    try {
        RcppVectorView<double> Yr(Ysexp);
        RcppMatrixView<double> Xr(Xsexp);

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

        RcppVector<double> StdErr(k);
        RcppVector<double> Coef(k);
        for (i = 0; i < k; i++) {
            Coef(i) = gsl_vector_get(c,i);
            StdErr(i) = gsl_matrix_get(cov,i,i);
        }
        gsl_matrix_free (X);
        gsl_vector_free (y);
        gsl_vector_free (c);
        gsl_matrix_free (cov);

        RcppResultSet rs;
        rs.add("coef", Coef);
        rs.add("stderr", StdErr);

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

set.seed(42)
n <- 100
k <- 9
X <- cbind( rep(1,n), matrix(rnorm(n*k), ncol=k) )
truecoef <- 1:(k+1)
y <- as.numeric(X %*% truecoef + rnorm(n))

N <- 500

lmgsl <- lmViaGSL()
lmarma <- lmViaArmadillo()

tlm <- mean(replicate(N, system.time( lmfit <- lm(y ~ X - 1) )["elapsed"]), trim=0.05)
tlmfit <- mean(replicate(N, system.time(lmfitfit <- lm.fit(X, y))["elapsed"]), trim=0.05)
tlmgsl <- mean(replicate(N, system.time(lmgsl(y, X))["elapsed"]), trim=0.05)
tlmarma <- mean(replicate(N, system.time(lmarma(y, X))["elapsed"]), trim=0.05)
print(c(tlm, tlmfit, tlmgsl, tlmarma))
