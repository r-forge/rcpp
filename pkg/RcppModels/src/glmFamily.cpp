#include "glmFamily.h"
#include <cmath>
#include <limits>

using namespace Rcpp;
using namespace std;

namespace glm {
    // Establish the values for the class constants
    double glmFamily::epsilon = numeric_limits<double>::epsilon();
    
    // initialize the function maps (i.e. associative arrays of functions)
    drmap glmFamily::devRes = drmap();

    fmap glmFamily::linvs = fmap();
    fmap glmFamily::lnks = fmap();
    fmap glmFamily::muEtas = fmap();
    fmap glmFamily::varFuncs = fmap();
    
    void glmFamily::initMaps() {
	// initialize the static maps.  The identity link is
	// guaranteed to be initialized if any maps are initialized
	    lnks["log"]                  = &log;
	    muEtas["log"] = linvs["log"] = &exp;
	    
	    lnks["sqrt"]                 = &sqrt;
	    linvs["sqrt"]                = &sqrf;
	    muEtas["sqrt"]               = &twoxf;
	    
	    lnks["identity"]             = &identf;
	    linvs["identity"]            = &identf;
	    muEtas["identity"]           = &onef;
	    
	    lnks["inverse"]              = &inversef;
	    linvs["inverse"]             = &inversef;
	    muEtas["inverse"]            = &invderivf;
	    
	    lnks["logit"]                = &logitLink;
	    linvs["logit"]               = &logitLinkInv;
	    muEtas["logit"]              = &logitMuEta;
	    
	    lnks["probit"]               = &probitLink;
	    linvs["probit"]              = &probitLinkInv;
	    muEtas["probit"]             = &probitMuEta;
	    
//	    lnks["cloglog"]              = &cloglogLink;
	    linvs["cloglog"]             = &cloglogLinkInv;
	    muEtas["cloglog"]            = &cloglogMuEta;
	    
	    devRes["Gamma"]              = &GammaDevRes;
	    varFuncs["Gamma"]            = &sqrf;   // x^2

	    devRes["binomial"]           = &BinomialDevRes;
	    varFuncs["binomial"]         = &x1mxf;  // x * (1 - x)

	    devRes["gaussian"]           = &GaussianDevRes;
	    varFuncs["gaussian"]         = &onef;   // 1

	    varFuncs["inverse.gaussian"] = &cubef;  // x^3

	    devRes["poisson"]            = &PoissonDevRes;
	    varFuncs["poisson"]          = &identf; // x
    }
    
    glmFamily::glmFamily() {
	if (!lnks.count("identity")) initMaps();
    }
	
    glmFamily::glmFamily(List ll) : lst(ll) {
	if (as<string>(lst.attr("class")) != "family")
	    Rf_error("glmFamily only from list of (S3) class \"family\"");
	
	CharacterVector fam = lst["family"], llink = lst["link"];
	char *pt = fam[0]; family = string(pt);
	pt = llink[0]; link = string(pt);

	if (!lnks.count("identity")) initMaps();
    }

    Rcpp::NumericVector
    glmFamily::linkFun(Rcpp::NumericVector const &mu) const {
	if (lnks.count(link)) {	// sapply the known scalar function
	    return NumericVector::import_transform(mu.begin(), mu.end(), lnks[link]);
	} else {		// use the R function
	    Function linkfun = ((const_cast<glmFamily*>(this))->lst)["linkfun"];
	    // The const_cast is needed so that this member function
	    // can be const and also use the extraction of a list
	    // component. 
	    return linkfun(mu);
	}
    }
    
    Rcpp::NumericVector
    glmFamily::linkInv(Rcpp::NumericVector const &eta) const {
	if (linvs.count(link)) {
	    return NumericVector::import_transform(eta.begin(), eta.end(), linvs[link]);
	} else {
	    Function linkinv = ((const_cast<glmFamily*>(this))->lst)["linkinv"];
	    return linkinv(eta);
	}
    }
    
    Rcpp::NumericVector
    glmFamily::muEta(Rcpp::NumericVector const &eta) const {
	if (muEtas.count(link)) {
	    return NumericVector::import_transform(eta.begin(), eta.end(), muEtas[link]);
	}
	Function mu_eta = ((const_cast<glmFamily*>(this))->lst)["mu.eta"];
	return mu_eta(eta);
    }
    
    Rcpp::NumericVector
    glmFamily::variance(Rcpp::NumericVector const &mu) const {
	if (varFuncs.count(link)) {
	    return NumericVector::import_transform(mu.begin(), mu.end(), varFuncs[link]);
	}
	Function vv = ((const_cast<glmFamily*>(this))->lst)["variance"];
	return vv(mu);
    }
    
    Rcpp::NumericVector
    glmFamily::devResid(Rcpp::NumericVector const &mu,
			Rcpp::NumericVector const &weights,
			Rcpp::NumericVector const &y) const {
	if (devRes.count(family)) {
	    double (*f)(double, double, double) = devRes[family];
	    int nobs = mu.size();
	    NumericVector ans(nobs);
	    double *mm = mu.begin(), *ww = weights.begin(),
		*yy = y.begin(), *aa = ans.begin();
	    for (int i = 0; i < nobs; i++)
		aa[i] = f(yy[i], mm[i], ww[i]);
	    return ans;
	}
	Function devres =
	    ((const_cast<glmFamily*>(this))->lst)["dev.resids"];
	return devres(y, mu, weights);
    }
}
