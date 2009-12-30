all: Rcpp-unitTests.html Rcpp-unitTests.pdf

Rcpp-unitTests.html: Rcpp-unitTests.R
	Rscript --default-packages="Rcpp,RUnit,utils,tools" Rcpp-unitTests.R

Rcpp-unitTests.pdf: Rcpp-unitTests.Rnw
	Rscript --default-packages="Rcpp,RUnit,utils,tools" -e "Sweave('Rcpp-unitTests.Rnw'); texi2dvi( 'Rcpp-unitTests.tex', pdf = TRUE, clean = TRUE ); unlink('RcppUnitTests.tex') "
