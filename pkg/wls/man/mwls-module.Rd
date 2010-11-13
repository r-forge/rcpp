\name{mwls}
\alias{mwls}
\title{
	Rcpp module: mwls
}
\description{
	Rcpp module for weighted least-squares repeated with varying
        weights.
}
\details{
	The module contains the following items: 
	
	 
	
	classes: \describe{
        \item{cppWls}{ A simple class for fitting weighted least
            squares repeatedly with varying weights.  The initializer
            establishes the x matrix and y vector for this problem.
            Each invocation of method \code{fit(weights)} does a
            least-squares fit and returns the coefficients. }
		}
}

\keyword{datasets}
