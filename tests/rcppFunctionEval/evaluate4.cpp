 
#include <Rcpp.h>

RcppExport double evaluate4(long *l_nfeval, 
			    double *param, int i_D,
			    const Rcpp::Function & fun, const Rcpp::Environment & env)
{
   (*l_nfeval)++;  //increment function evaluation count
   Rcpp::NumericVector par(i_D);
   for (int i = 0; i < i_D; i++) 
       par[i] = param[i];

   Rcpp::Language funcall(fun, par);
   Rcpp::NumericVector v = funcall.eval(env);
   double f_result = v[0];

   if (ISNAN(f_result))
     ::Rf_error("NaN value of objective function! \nPerhaps adjust the bounds.");
   
   return(f_result); 
}
