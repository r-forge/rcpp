# -*- tab-width: 8 -*- 
#
# Makefile for Rcpp 
#
# Copyright (C) 2008    Dirk Eddelbuettel <edd@debian.org>

RHOME		= $(shell R RHOME)
RLIBDIR		= $(RHOME)/$(shell R CMD config LIBnn)

RCPPFLAGS	= $(shell R CMD config --cppflags)
RLDFLAGS	= $(shell R CMD config --ldflags)

## the 'package' library contains both Rcpp.{cpp,h} and the RcppExample used to demonstrate the package
## it is loaded via library(Rcpp) but is not used for compiling / linking against
PKGLIB 		= Rcpp.so
PKGOBJ 		= RcppExample.o Rcpp.o
PKGLDFLAGS 	= -s $(RLDFLAGS) 

## the 'pure' library contains only Rcpp.{cpp,h} and is was users need for their projects
PURELIB		= ../inst/lib/libRcpp.so
PUREOBJ		= Rcpp.o

## more correct version of shared library build
#		= gcc -shared -Wl,-soname,librcpp.so.5 \
#                                -o ../librcpp.so.5.0 Rcpp.o
## where you'd still need the softlink from librcpp.so.5.0 to librcpp.so

CXXFLAGS 	= -I. $(RCPPFLAGS) -Wall -O2 
RM 		= rm -f

.PHONY: 	all clean

all: 		$(PURELIB) $(PKGLIB)

clean:
		${RM} $(PKGOBJ) $(PKGLIB)

$(PKGLIB): 	$(PKGOBJ)
		$(CXX) -shared -o $(PKGLIB) $(PKGOBJ) $(PKGLDFLAGS)

$(PURELIB):	$(PUREOBJ)
		$(CXX) -shared -o $(PURELIB) $(PUREOBJ) 
		cp -vax Rcpp.h ../inst/lib
		@echo "****"
		@echo "**** You may need to execute the following command as root"
		@echo "**** so that the libRcpp library will get found by R at runtime"
		@echo "     cp -vax $(PURELIB) $(RLIBDIR)"
		@echo "**** with a similar copy of Rcpp.h to R's include directory"
		@echo "****"

