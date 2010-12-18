#!/bin/sh
#
# Rebuilds all vignettes
# Builds tarball
# Runs checks based on tarball
# Copies tarball in place for Debian package builds from SVN dir
# Copies tarball to web directory too

set -e 

if [ ! -d Rcpp ]; then
    echo "Not above Rcpp/"
    exit -1
fi

version=$(r -e'cat(as.character(read.dcf("Rcpp/DESCRIPTION")[,"Version"]))')
encodedversion=$(grep RCPP_VERSION Rcpp/inst/include/Rcpp/config.h | awk '{print $3}')
echo "Working on:  $encodedversion -- $version"

#export RCPP_CXX0X="yes"
export RCPP_CXX0X="no"

## remove old pdf vignettes and make fresh ones
cd Rcpp/inst/doc && make pdfclean && make && cd - 

R CMD build --force Rcpp

R CMD check Rcpp_${version}.tar.gz

test -d tarballs   && cp -vax Rcpp_${version}.tar.gz tarballs/rcpp_${version}.orig.tar.gz
test -d build-area && cp -vax Rcpp_${version}.tar.gz build-area/rcpp_${version}.orig.tar.gz
test -d ~/www/code/rcpp && cp -vax Rcpp_${version}.tar.gz ~/www/code/rcpp

