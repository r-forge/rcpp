
#include <Rcpp.h>

// faster hybrid version:
//  -- uses reference for l_nfeval
//  -- uses NumericVector for par, no copying or alloc needed
//  -- uses SEXPs for function and env

RcppExport double evaluate5(long & l_nfeval, Rcpp::NumericVector & par, SEXP fcall, SEXP env)
{
  l_nfeval++;  //increment function evaluation count

  // without PROTECT -- safe enough as we do not return fn or sexp_fvec
  SEXP fn = Rf_lang2(fcall, Rcpp::wrap(par));
  SETCADR(fn, par);
  SEXP sexp_fvec = Rf_eval(fn, env);

  double f_result = Rcpp::as<double>(sexp_fvec);
  if (ISNAN(f_result))
    Rf_error("NaN value of objective function! \nPerhaps adjust the bounds.");
   
  return(f_result); 
}
