
#include "wls.h"

using namespace Rcpp ;

class Wls{
public:

    //  Wls() {}

    Wls(SEXP xr, SEXP yr) {
	x = NumericMatrix(xr);
	y = NumericVector(yr);
	xw = NumericVector(x.nrow()*x.ncol());
	yw = NumericVector(y.size());
	wrt = NumericVector(y.size());
    }

    NumericVector fit(SEXP wR) {
	compute_and_apply_weights(wR);
	return do_fit();
    }

private:
    NumericMatrix x;
    NumericVector y, yw, wrt, xw;

	
    NumericVector do_fit() {
	int n = x.nrow(), p = x.ncol();
	arma::mat X(xw.begin(), n, p, false);
	arma::colvec y(yw.begin(), n, false);
	arma::vec coefa = arma::solve(X, y);
	return wrap(coefa);
    }

    void compute_and_apply_weights(SEXP wR) {
	int n = x.nrow(), p = x.ncol();
	wrt = sqrt(check_weights(wR));
	yw = y * wrt;
	xw = x * rep(wrt, p);
    }

    NumericVector check_weights(SEXP wR) {
	//BEGIN_RCPP
	NumericVector w(wR);
	if(w.size() != y.size())
	    throw std::invalid_argument("Weight vector wrong length");
	if(as<bool>(any( is_na(w) )))
	    throw std::domain_error("Missing values not allowed in weights");
	if(as<bool>(any(w < -0.0)))
	    throw std::domain_error("Negative weights found");
	return w;
	//END_RCPP
    }

};

RCPP_MODULE(mwls) {

class_<Wls>( "cppWls" )
    // .default_constructor()
    .constructor(init_2<NumericMatrix,NumericVector>())
    .method("fit", &Wls::fit)
    ;

}
