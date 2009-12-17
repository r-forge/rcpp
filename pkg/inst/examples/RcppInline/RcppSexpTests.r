#!/usr/bin/r

suppressMessages(library(Rcpp))

cat("===Doubles\n")
foo <- '
        double d = RcppSexp(x).asDouble();
	std::cout << "Returning twice the value of " << d << " : ";
	return(RcppSexp( 2*d ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x=2.123), "\n")
cat(funx(x=2), "\n")
##funx(x='2')  ## throws as expected
##funx(x=2:3)  ## throws as expected


cat("\n===Int\n")
foo <- '
        int i = RcppSexp(x).asInt();
	std::cout << "Returning twice the value of " << i << " : ";
	return(RcppSexp( 2*i ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x=2), "\n")
cat(funx(x=2.2), "\n")


cat("\n===String\n")
foo <- '
        std::string s = RcppSexp(x).asStdString();
	std::cout << "Returning twice the value of " << s << " : ";
	return(RcppSexp( s+s ).asSexp());
        '
funx <- cfunction(signature(x="character"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x="abc"), "\n")

