
#include <Rcpp.h>

/*------General functions-----------------------------------------*/

RcppExport double evaluate2(long *l_nfeval, 
			    double *param, int i_D,
			    SEXP fcall, SEXP rho)
{
   (*l_nfeval)++;  //increment function evaluation count
   Rcpp::NumericVector par(i_D);
   for (int i = 0; i < i_D; i++) 
       par[i] = param[i];

   Rcpp::Function fun(fcall);
   Rcpp::Environment env(rho);
  
   Rcpp::Language funcall(fun, par);
   double f_result = Rcpp::as<double>( Rcpp::Evaluator::run( funcall, env) );
   // or: Rcpp::NumericVector v = funcall.eval(env);

   if (ISNAN(f_result))
     ::Rf_error("NaN value of objective function! \nPerhaps adjust the bounds.");
   
   return(f_result); 
}


