#!/usr/bin/r -ti

genroseFunction <- function(x) {
    ## One generalization of the Rosenbrock banana valley function (n parameters)
    n <- length(x)
    #1.0 + sum (100 * (x[1:(n-1)]^2 - x[2:n])^2 + (x[2:n] - 1)^2)
    1.0 + sum (100 * (x[-n]^2 - x[-1])^2 + (x[-1] - 1)^2)
}

dyn.load("bm.so")
res <-.Call("benchmarkEvals",
            list("nsim"=5000),		# bmpar list
            rep(1,2),			# parameter vec.
            genroseFunction,		# function
            new.env())			# environment

print(res)
