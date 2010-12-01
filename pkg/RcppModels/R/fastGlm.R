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
             control = list(...))
{
    call <- match.call()
    if (missing(family)) {
        family <- NULL
    } else {
        if(is.character(family))
            family <- get(family, mode = "function", envir = parent.frame())
        if(is.function(family)) family <- family()
        if(is.null(family$family)) {
            print(family)
            stop("'family' not recognized")
        }
    }
    ## extract x, y, etc from the model formula and frame
    if(missing(data)) data <- environment(formula)
    mf <- match.call(expand.dots = FALSE)
    m <- match(c("formula", "data", "subset", "weights", "na.action",
                 "etastart", "mustart", "offset"), names(mf), 0L)
    mf <- mf[c(1L, m)]
    mf$drop.unused.levels <- TRUE
    mf[[1L]] <- as.name("model.frame")
    mf <- eval(mf, parent.frame())

    rr <- mkRespMod(mf, family)
    pp <- new(densePred, model.matrix(formula, mf))
    ## one iteration of the fixed-point algorithm to establish
    ## a baseline coefficient vector
    wts <- rr$sqrtWrkWt
    rr$updateMu(pp$gamma(wts, wts * rr$wrkResp))
    rr$updateWts()
    pp$installCoef0()
    list(rmod = rr, pmod = pp)
}
