#!/usr/bin/r -t
#
# A faster lm() replacement based on Armadillo
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
    ## a really simple C program calling three functions from the GSL
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

        RcppMatrix<double> CovMat(k, k);
        RcppVector<double> Coef(k);
        for (int i = 0; i < k; i++) {
            Coef(i) = coef(i);
            for (int j = 0; j < k; j++) {
                CovMat(i,j) = covmat(i,j);
            }
        }
        //Rcpp::NumericVector coefr(k);
        //Rcpp::NumericVector stderrestr(k);
        //for (int i=0; i<k; i++) {
        //    coefr[i]      = coef[i];
        //    stderrestr[i] = stderrestr[i];
        //}

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
                     includes="#include <armadillo>",
                     Rcpp=TRUE,
                     cppargs="-I/usr/include",
                     libargs="-larmadillo")
}

checkLmViaArmadillo <- function(y, X) {
    fun <- lmViaArmadillo();
    cat("Running lm via Armadillo\n")
    res <- fun(y, X)
    print(cbind(res[[1]], sqrt(diag(res[[2]]))))
    cat("Running lm()\n")
    print(summary(lm(y ~ X - 1)))
    invisible(NULL)
}

timeLmViaArmadillo <- function(y, X, N) {
    fun <- lmViaArmadillo();
    meantime <- mean(replicate(N, system.time(fun(y, X))["elapsed"]), trim=0.05)
}


set.seed(42)
n <- 5000
k <- 9
X <- cbind( rep(1,n), matrix(rnorm(n*k), ncol=k) )
truecoef <- 1:(k+1)
y <- as.numeric(X %*% truecoef + rnorm(n))

N <- 100

#checkLmViaArmadillo(y, X)
mt <- timeLmViaArmadillo(y, X, N)
cat("Armadillo: Running", N, "simulations yields (trimmed) mean time", mt, "\n")
