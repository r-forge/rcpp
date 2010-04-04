#include <RcppGsl.h>

extern "C" SEXP test_gsl_vector(){
	gsl_vector * x = gsl_vector_alloc (10);
	gsl_vector_set_zero( x ) ;
	
	Rcpp::NumericVector xx = Rcpp::wrap( *x ) ;
	gsl_vector_free (x);
	return xx ;
}
