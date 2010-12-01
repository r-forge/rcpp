library(RcppModels)

## Dobson (1990) Page 93: Randomized Controlled Trial :
d.AD <- data.frame(treatment = gl(3,3), outcome = gl(3,1,9),
                   counts = c(18,17,15,20,10,20,25,13,12))
res <- fastGlm(counts ~ outcome + treatment, poisson, d.AD)
pp <- res$pmod
rr <- res$rmod
rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres))
(oldwrss <- rr$updateWts())
pp$installCoef0()
rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres))
(oldwrss <- rr$updateWts())
pp$installCoef0()
(newwrss <- rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres)))
(oldwrss <- rr$updateWts())
pp$installCoef0()
(newwrss <- rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres)))
(oldwrss <- rr$updateWts())
pp$installCoef0()
(newwrss <- rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres)))

