# -*- tab-width: 8 -*- 
#
# Makefile for Rcpp 
#
# Copyright (C) 2008    Dirk Eddelbuettel <edd@debian.org>

RHOME		= $(shell R RHOME)
RLIBDIR		= $(RHOME)/$(shell R CMD config LIBnn)

RCPPFLAGS	= $(shell R CMD config --cppflags)
RLDFLAGS	= $(shell R CMD config --ldflags)

EXMPLLIB 	= Rcpp.so
EXMPLOBJ 	= RcppExample.o Rcpp.o
#EXMPLLDFLAGS 	= -s -L. -lRcpp $(RLDFLAGS) 
EXMPLLDFLAGS 	= -s $(RLDFLAGS) 

PKGLIB		= ../inst/lib/libRcpp.so
PKGOBJ		= Rcpp.o

#PKGLDFLAGS	= gcc -shared -Wl,-soname,librcpp.so.5 \
#                                -o ../librcpp.so.5.0 Rcpp.o

CXXFLAGS 	= -I. $(RCPPFLAGS) -Wall -O2 

RM 		= rm -f

.PHONY: 	all clean

all: 		$(PKGLIB) $(EXMPLLIB)

clean:
		${RM} $(EXMPLOBJ) $(EXMPLLIB)

$(EXMPLLIB): 	$(EXMPLOBJ)
		$(CXX) -shared -o $(EXMPLLIB) $(EXMPLOBJ) $(EXMPLLDFLAGS)

$(PKGLIB):	$(PKGOBJ)
		$(CXX) -shared -o $(PKGLIB) $(PKGOBJ) 
		cp -vax Rcpp.h ../inst/lib
		@echo "**** You may need to execute the following command as root"
		@echo "**** so that the libRcpp library will get found by R at runtime"
		@echo "     cp -vax $(EXMPLLIB) $(RLIBDIR)"

#RcppExample.o: 	RcppExample.cpp
#		$(CXX) -c RcppExample.cpp -o RcppExample.o $(CXXFLAGS)
