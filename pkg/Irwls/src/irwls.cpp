
#include "Irwls.h"

using namespace Rcpp ;

class Irwls{
public:
    NumericMatrix x, xw;
    NumericVector y, yw, wrt;

    Irwls(SEXP xr, SEXP yr) {
	x = NumericMatrix(xr);
	y = NumericVector(yr);
	xw = NumericMatrix(x.nrow(), x.ncol());
	yw = NumericVector(y.size());
	wrt = NumericVector(y.size());
    }

    SEXP fit(SEXP wR) {
	compute_and_apply_weights(wR);
	return do_fit();
    }
	
    SEXP do_fit() {
	int n = xw.nrow(), p = xw.ncol();
	arma::mat X(xw.begin(), n, p, false);
	arma::colvec y(yw.begin(), n, false);
	arma::vec coefa = arma::solve(X, y);
	return wrap( coefa);
    }

    void compute_and_apply_weights(SEXP wR) {
	int n = yw.size(), p = xw.ncol(), np = n*p;
	NumericVector w(wR);
	NumericIterator ir = wrt.begin(), iw = w.begin(),
	    ix = x.begin(), ixw = xw.begin(),
	    iy = y.begin(), iyw = yw.begin();
	for (int i = 0; i < n; i++) {
	    ir[i] = sqrt(iw[i]);
	    iyw[i] = iy[i] * ir[i];
	}
	for (int ij = 0; ij < np; )
	    for(int i = 0; i < n; i++) {
		ixw[ij] = ix[ij] * ir[i];
		ij++;
	    }
    }
    
};

RCPP_MODULE(mIrwls) {

class_<Irwls>( "cppIrwls" )

    .constructor(init_2<NumericMatrix,NumericVector>())

    .field("x", &Irwls::x)
    .field("y", &Irwls::y)
    .field("xw", &Irwls::xw)
    .field("yw", &Irwls::yw)
    .field("wrt", &Irwls::wrt)

    .method("fit", &Irwls::fit)
    ;

}
