library(RcppModels)

## Dobson (1990) Page 93: Randomized Controlled Trial :
d.AD <- data.frame(treatment = gl(3,3), outcome = gl(3,1,9),
                   counts = c(18,17,15,20,10,20,25,13,12))
res <- fastGlm(counts ~ outcome + treatment, poisson, d.AD)
res$pmod$coef0                          # estimated coefficients
res$rmod$wrkResids                      # working residuals
res$rmod$wtres                          # Pearson residuals
res$rmod$residDeviance                  # residual deviance

# A Gamma example, from McCullagh & Nelder (1989, pp. 300-2)
clotting <-
    data.frame(u = c(5,10,15,20,30,40,60,80,100),
               lot1 = c(118,58,42,35,27,25,21,19,18),
               lot2 = c(69,35,26,21,18,16,13,12,12))
res1 <- fastGlm(lot1 ~ log(u), Gamma, clotting)
res1$rmod$wrkResids                      # working residuals
res1$rmod$wtres                          # Pearson residuals
res1$rmod$residDeviance                  # residual deviance

res2 <- fastGlm(lot2 ~ log(u), Gamma, clotting)
res2$rmod$wrkResids                      # working residuals
res2$rmod$wtres                          # Pearson residuals
res2$rmod$residDeviance                  # residual deviance
