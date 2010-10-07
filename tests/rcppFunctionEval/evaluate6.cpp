#include <Rcpp.h>

// faster hybrid version:
//  -- uses reference for l_nfeval
//  -- uses NumericVector for par, no copying or alloc needed
//  -- uses Rcpp objects for interface
// and that last points makes it slower that version5 
// but comparable or slightly faster that v1 (the base case)

RcppExport double evaluate6(long & l_nfeval, 
			    Rcpp::NumericVector & par, 
			    const Rcpp::Function & fcall, 
			    const Rcpp::Environment & env)
{
  l_nfeval++;  //increment function evaluation count

  // without PROTECT -- safe enough as we do not return fn or sexp_fvec
  SEXP fn = Rf_lang2(Rcpp::wrap(fcall), Rcpp::wrap(par));
  SETCADR(fn, par);
  SEXP sexp_fvec = Rf_eval(fn, Rcpp::wrap(env));

  double f_result = Rcpp::as<double>(sexp_fvec);
  if (ISNAN(f_result))
    Rf_error("NaN value of objective function! \nPerhaps adjust the bounds.");
   
  return(f_result); 
}
