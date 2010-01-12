#!/usr/bin/r

set.seed(42)
a <- rnorm(100)
b <- rnorm(100)

## load shared library with wrapper code and callback class
dyn.load("convolve2_c.so")

## call the wrapper function provided in the shared library
v1 <- .Call("convolve2", a, b)
t1 <- system.time(replicate(1000, .Call("convolve2", a, b)))

## load shared library with wrapper code and callback class
dyn.load("convolve2_cpp.so")

## call the wrapper function provided in the shared library
v2 <- .Call("convolve2cpp", a, b)[[1]]
t2 <- system.time(replicate(1000, .Call("convolve2cpp", a, b)))

print(t1)
print(t2)
print(summary(v1))
print(summary(v2))
print(all.equal(v1, v2))








