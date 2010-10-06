
#include <Rcpp.h>

// version from DEoptim

//------objective function---------------------------------------

RcppExport double evaluate5(long & l_nfeval, Rcpp::NumericVector & par, SEXP fcall, SEXP env)
{
#if 0 
  // with PROTECT
  SEXP sexp_fvec, fn;
  double f_result;  
  
  PROTECT(fn = Rf_lang2(fcall, Rcpp::wrap(par)));
  l_nfeval++;  //increment function evaluation count
  
  SETCADR(fn, par);
 
  PROTECT(sexp_fvec = Rf_eval(fn, env));
  f_result = Rcpp::as<double>(sexp_fvec);
 
  UNPROTECT(2);	
 #endif

  // without PROTECT -- safe enough?
  SEXP fn = Rf_lang2(fcall, Rcpp::wrap(par));
  l_nfeval++;  //increment function evaluation count
  
  SETCADR(fn, par);
 
  SEXP sexp_fvec = Rf_eval(fn, env);
  double f_result = Rcpp::as<double>(sexp_fvec);
 
  if(ISNAN(f_result))
    Rf_error("NaN value of objective function! \nPerhaps adjust the bounds.");
   
  return(f_result); 
}
