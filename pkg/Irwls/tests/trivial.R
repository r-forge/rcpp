require(Irwls)
irwls <- mIrwls$cppIrwls
set.seed(400)
y = rnorm(10)
x = rnorm(10)
xx <- cbind(rep(1,10), x, x^2)
irxly = irwls$new(xx, y)
coef = irxly$fit(rep(1,10))
coef2 = lm(y ~ x + I(x^2))$coef
stopifnot(all.equal(as.vector(coef), as.vector(coef2)))
wt = rep(c(1,2), 5)
coef = irxly$fit(wt)
coef2 = lm(y ~ x + I(x^2), weights = wt)$coef
stopifnot(all.equal(as.vector(coef), as.vector(coef2)))
wt[1] <- -1
msg <- tryCatch(irxly$fit(wt), error = function(e)e)
stopifnot(is(msg,"error"), grepl("negative", msg$message, ignore.case = TRUE))
wt <- wt[-1]
msg <- tryCatch(irxly$fit(wt), error = function(e)e)
stopifnot(is(msg,"error"), grepl("length", msg$message, ignore.case = TRUE))
wt <- c(NA, wt)
msg <- tryCatch(irxly$fit(wt), error = function(e)e)
stopifnot(is(msg,"error"), grepl("missing", msg$message, ignore.case = TRUE))
