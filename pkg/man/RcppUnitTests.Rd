\name{RcppUnitTests}
\alias{RcppUnitTests}

\title{Rcpp : unit tests results}
\description{
Unit tests results for package Rcpp. 

Unit tests are run automatically at build time and reports are included in the
\samp{doc} directory as html or text. 
}

\seealso{
\ifelse{html}{\url{../doc/Rcpp-unitTests.html}: html formatted unit test report}{\samp{../doc/Rcpp-unitTests.html}: html formatted unit test report}
}

\details{
\ifelse{html}{\out{<pre>}}{\ifelse{latex}{\out{\begin{verbatim}}}{}}
\Sexpr[echo=FALSE,results=text]{paste(readLines(system.file("doc", "Rcpp-unitTests.txt", package = "Rcpp" ) ), collapse="\n\n")}
\ifelse{html}{\out{</pre>}}{\ifelse{latex}{\out{\end{verbatim}}}{}}
}

\examples{
# unit tests are in the unitTests directory of the package
list.files( system.file("unitTests", package = "Rcpp" ), 
	pattern = "^runit", full = TRUE )

# trigger the unit tests, follow printed instructions
\dontrun{
source( system.file("unitTests", "runTests.R", package = "Rcpp" ) )
}
}

\keyword{programming}
