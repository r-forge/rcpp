#include "glmFamily.h"
#include "RcppModels.h"

using namespace Rcpp;

RCPP_MODULE(RcppModels) {

class_<Irwls>( "Irwls" )

    .default_constructor()
    .constructor(init_2<NumericMatrix,NumericVector>())

    .field_readonly("x", &Irwls::x)
    .field_readonly("y", &Irwls::y)

    .method("fit", &Irwls::fit)
    ;

using namespace glm;
    
class_<glmFamily>("glmFamily")

    .default_constructor()
    .constructor(init_1<List>())

    .method("linkFun",  &glmFamily::linkFun)
    .method("linkInv",  &glmFamily::linkInv)
    .method("muEta",    &glmFamily::muEta)
    .method("devResid", &glmFamily::devResid)
    .method("variance", &glmFamily::variance)
    ;

class_<lmResp>("lmResp")

    .default_constructor()
    .constructor(init_1<NumericVector>())
    .constructor(init_2<NumericVector,NumericVector>())
    .constructor(init_3<NumericVector,NumericVector,NumericVector>())

    .property("mu",      &lmResp::mu)
    .property("offset",  &lmResp::offset)
    .property("sqrtXwt", &lmResp::sqrtXwt)
    .property("sqrtrwt", &lmResp::sqrtrwt)
    .property("weights", &lmResp::weights)
    .property("wrss",    &lmResp::wrss)
    .property("wtres",   &lmResp::wtres)
    .property("y",       &lmResp::y)

    .method("updateMu",  &lmResp::updateMu)
    .method("updateWts", &lmResp::updateWts)
    ;

class_<glmResp>("glmResp")

    .default_constructor()
    .constructor(init_2<List,NumericVector>())
    .constructor(init_3<List,NumericVector,NumericVector>())
    .constructor(init_4<List,NumericVector,NumericVector,NumericVector>())
    .constructor(init_5<List,NumericVector,NumericVector,NumericVector,NumericVector>())
 
    .property("eta",         &glmResp::eta)   
    .property("mu",          &glmResp::mu)
    .property("offset",      &glmResp::offset)
    .property("sqrtXwt",     &glmResp::sqrtXwt)
    .property("sqrtrwt",     &glmResp::sqrtrwt)
    .property("weights",     &glmResp::weights)
    .property("wtres",       &glmResp::wtres)
    .property("y",           &glmResp::y)
    .property("wrss",        &glmResp::wrss)
    .property("wrkResids",   &glmResp::wrkResids)

    .method("devResid",      &glmResp::devResid)
    .method("residDeviance", &glmResp::residDeviance)
    .method("updateMu",      &glmResp::updateMu)
    .method("updateWts",     &glmResp::updateWts)
    .method("wrkResids",     &glmResp::wrkResids)
    ;

class_<predMod>("predMod")

    .default_constructor()
    .constructor(init_1<int>())
    .constructor(init_1<NumericVector>())
 
// For some reason this doesn't work.
//    .property("coef0", &predMod::coef0, &predMod::setCoef0)   
    .property("coef0", &predMod::coef0)
    ;

class_<densePred>("densePred")

    .default_constructor()
    .constructor(init_1<NumericMatrix>())
    .constructor(init_2<NumericMatrix, NumericVector>())

    .property("coef0", &densePred::coef0)
    .property("delta", &densePred::delta)
    .property("X",     &densePred::X)

    .method("gamma",   &densePred::gamma)
    ;

}
