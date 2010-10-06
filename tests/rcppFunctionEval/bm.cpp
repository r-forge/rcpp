
#include <Rcpp.h>

class Timer {
public:
    Timer() : sys_time("Sys.time") { Reset(); }
    void Start()  { start_t = getFractionalSeconds() ; }
    void Stop() { 
	end_t = getFractionalSeconds(); 
	elapsed = end_t - start_t;		// Calculate elapsed time in seconds
	cumul += elapsed;
    }
    void Reset() { end_t = start_t = elapsed = cumul = 0.0; }
    double ElapsedTime() { return elapsed; }
    double CumulativeTime() { return cumul; }


private:
    Rcpp::Function sys_time ;
    double start_t, end_t, elapsed, cumul;

    double getFractionalSeconds(void) {
        return Rcpp::as<double>( sys_time() ) ;
    }
};

RcppExport double evaluate1(long *l_nfeval, double *param, int i_D, SEXP fcall, SEXP env);
RcppExport double evaluate2(long *l_nfeval, double *param, int i_D, SEXP fcall, SEXP env);
RcppExport double evaluate3(long *l_nfeval, double *param, int i_D,
 			    const Rcpp::Function & fun, const Rcpp::Environment & env);
RcppExport double evaluate4(long *l_nfeval, double *param, int i_D,
			    const Rcpp::Function & fun, const Rcpp::Environment & env);
RcppExport double evaluate5(long & l_nfeval, Rcpp::NumericVector & par, SEXP fcall, SEXP env);

RcppExport SEXP benchmarkEvals(SEXP bmS, SEXP parS, SEXP funS, SEXP envS) {
    
    Rcpp::List bm(bmS);
    int nsim = Rcpp::as<int>(bm["nsim"]);

    long neval = 0;
    Rcpp::NumericVector x(parS);

    Timer t;
    double v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0;
    t.Start();
    for (int i=0; i<nsim; i++)
	v1 = evaluate1(&neval, x.begin(), x.size(), funS, envS);
    t.Stop();
    double t1 = t.ElapsedTime();

    t.Reset();
    neval = 0;
    t.Start();
    for (int i=0; i<nsim; i++)
	v2 = evaluate2(&neval, x.begin(), x.size(), funS, envS);
    t.Stop();
    double t2 = t.ElapsedTime();

    Rcpp::Function fun(funS);
    Rcpp::Environment env(envS);

    t.Reset();
    neval = 0;
    t.Start();
    for (int i=0; i<nsim; i++)
	v3 = evaluate3(&neval, x.begin(), x.size(), fun, env);
    t.Stop();
    double t3 = t.ElapsedTime();

    t.Reset();
    neval = 0;
    t.Start();
    for (int i=0; i<nsim; i++)
	v4 = evaluate4(&neval, x.begin(), x.size(), fun, env);
    t.Stop();
    double t4 = t.ElapsedTime();

    t.Reset();
    neval = 0;
    t.Start();
    for (int i=0; i<nsim; i++)
	v5 = evaluate5(neval, x, fun, env);
    t.Stop();
    double t5 = t.ElapsedTime();

    return Rcpp::DataFrame::create(Rcpp::Named("times", Rcpp::NumericVector::create(t1, t2, t3, t4, t5)),
				   Rcpp::Named("values", Rcpp::NumericVector::create(v1, v2, v3, v4, v5)));

}
