\name{wls-package}
\alias{wls-package}
\alias{wls}
\docType{package}
\title{
Weighted least squares using C++, repeated with varying weights.
}
\description{
As a demonstration of C++ classes exposed through the Rcpp package,
this package creates a reference class to generate repeated
least-squares fits.  The class is created with a response variable and
a fully-specified model matrix; fits are then generated with varying
weight vectors.  The design is that this package can be used by any
form of model-fitting that eventually uses re-weighted least squares.

This (nearly trivial) package is intended to demonstrate subclassing
of the C++ class
with reference classes that apply weighted least squares for a variety
of purposes.  The C++ under-layer takes over the numerical fitting with
lower overhead in time and memory.
}
\author{
John Chambers <jmc at r-project.org>
}
\keyword{ package }

