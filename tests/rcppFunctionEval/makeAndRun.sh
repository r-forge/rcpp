#!/bin/bash

export PKG_CPPFLAGS=`r -e'Rcpp:::CxxFlags()'`
export PKG_LIBS=`r -e'Rcpp:::LdFlags()'` 
R CMD SHLIB bm.cpp evaluate*.c evaluate*.cpp 

./bm.r