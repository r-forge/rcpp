##' Create the response module from the model frame
##'
##' Create a glmResp object from the model frame and the glm family.
##' The initialization of the family as applied to the response takes
##' place here.
##' @title Create a glmResp object
##' @param fr a model frame
##' @param family a glm family object
##' @return a glmResp object
mkRespMod <- function(fr, family)
{
    N <- n <- nrow(fr)
                                        # components of the model frame
    y <- model.response(fr)
    if(length(dim(y)) == 1) { # avoid problems with 1D arrays, but keep names
        nm <- rownames(y)
        dim(y) <- NULL
        if(!is.null(nm)) names(y) <- nm
    }
    weights <- unname(model.weights(fr))
    if (is.null(weights)) weights <- rep.int(1, n)
    offset <- unname(model.offset(fr))
    if (is.null(offset)) offset <- rep(0, n)
    rho <- new.env()
    rho$etastart <- model.extract(fr, "etastart")
    rho$mustart <- model.extract(fr, "mustart")
    rho$nobs <- n
    rho$y <- y
    if (!"family" %in% class(family))
        stop("family should be a list with class \"family\"")
    eval(family$initialize, rho)
    family$initialize <- NULL       # remove clutter from str output
    rr <- new(glmResp, family, rho$y, weights, offset, rho$n)
    rr$updateMu(family$linkfun(rho$mustart))
    rr$updateWts()
    rr
}

fastGlm <-
    function(formula, family, data, weights, subset,
             na.action, start = NULL, etastart, mustart, offset,
             drop.unused.levels = FALSE, doFit = TRUE,
             control = list(...), ...)
{
    call <- match.call()
    maxit <- if(!is.null(control$maxit)) control$maxit else 8
                                        # check the family argument
    if(is.character(family))
        family <- get(family, mode = "function", envir = parent.frame())
    if(is.function(family)) family <- family()
    if(is.null(family$family)) stop("'family' not recognized")
                                        # create the model frame
    if(missing(data)) data <- environment(formula)
    mf <- match.call(expand.dots = FALSE)
    m <- match(c("formula", "data", "subset", "weights", "na.action",
                 "etastart", "mustart", "offset"), names(mf), 0L)
    mf <- mf[c(1L, m)]
    mf$drop.unused.levels <- TRUE
    mf[[1L]] <- as.name("model.frame")
    mf <- eval(mf, parent.frame())
                          # create the response and predictor modules
    rr <- RcppModels:::mkRespMod(mf, family)
    pp <- new(densePred, model.matrix(formula, mf))

    ## one iteration of the fixed-point algorithm to establish
    ## a baseline coefficient vector
    wts <- rr$sqrtWrkWt
    rr$updateMu(pp$gamma(wts, wts * rr$wrkResp))
    pp$installCoef0()
    print(pp$coef0)
    for (i in seq_len(maxit)) {
        oldwrss <- rr$updateWts()
        if ((newwrss <- rr$updateMu(pp$gamma(rr$sqrtXwt, rr$wtres)))
            < oldwrss) pp$installCoef0()
        print(pp$coef0)
        if ((oldwrss - newwrss)/newwrss < 1.e-8) break
    }
    list(rmod = rr, pmod = pp)
}
