#!/bin/bash

export PKG_CPPFLAGS=`Rscript -e 'Rcpp:::CxxFlags()'`
export PKG_LIBS=`Rscript -e 'Rcpp:::LdFlags()'` 
R CMD SHLIB bm.cpp evaluate*.c evaluate*.cpp 

Rscript bm.r
