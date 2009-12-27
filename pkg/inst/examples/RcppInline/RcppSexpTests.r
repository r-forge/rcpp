#!/usr/bin/r -t
#
# Copyright (C) 2009 Dirk Eddelbuettel and Romain Francois
#
# This file is part of Rcpp.
#
# Rcpp is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

suppressMessages(library(Rcpp))
suppressMessages(library(inline))

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
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x=as.raw(2)), "\n")

cat("\n===String\n")
foo <- '
        std::string s = RcppSexp(x).asStdString();
	std::cout << "Returning twice the value of " << s << " : ";
	return(RcppSexp( s+s ).asSexp());
        '
funx <- cfunction(signature(x="character"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x="abc"), "\n")

cat("\n===Raw (bytes)\n")
foo <- '
        Rbyte i = RcppSexp(x).asRaw();
	std::cout << "Returning twice the value of " << (int)i << " : ";
	return(RcppSexp( (Rbyte)(2*i) ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat( funx(x=2), "\n")
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
cat( funx(x=2L), "\n")
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
cat( funx(x=as.raw(2)), "\n")

cat("\n=== logical \n")
foo <- '
bool b = RcppSexp(x).asBool();
std::cout << "flip  " << ( b ? "TRUE" : "FALSE" ) << " : ";
return(RcppSexp( !b ).asSexp());
'
funx <- cfunction(signature(x="logical"), foo, Rcpp=TRUE, verbose=FALSE)
cat( res <- funx(x=TRUE) , "\n")  ; stopifnot( !res )
cat( res <- funx(x=FALSE), "\n" ) ; stopifnot( res)
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat( res <- funx(x=2)  , "\n")   ; stopifnot( !res )
cat( res <- funx(x=0.0), "\n")   ; stopifnot( res)
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
cat( res <- funx(x=2L), "\n")    ; stopifnot( !res )
cat( res <- funx(x=0L), "\n")    ; stopifnot( res)
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
cat( res <- funx(x=as.raw(2)), "\n") ; stopifnot( !res )
cat( res <- funx(x=as.raw(0)), "\n") ; stopifnot( res)

### vectors

cat("\n===Int Vector via RcppResultSet.getSEXP\n")
foo <- '
        std::vector<int> iv = RcppSexp(x).asStdVectorInt();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
        RcppResultSet rs;
        rs.add("", iv);
        return(rs.getSEXP());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.raw(2:5)))

cat("\n===Int Vector\n")
foo <- '
        std::vector<int> iv = RcppSexp(x).asStdVectorInt();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5+.1))
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.raw(2:5)))


cat("\n===Double Vector\n")
foo <- '
        std::vector<double> iv = RcppSexp(x).asStdVectorDouble();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
 	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=0.1+2:5))
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.raw(2:5)))

cat("\n===Raw Vector\n")
foo <- '
        std::vector<Rbyte> iv = RcppSexp(x).asStdVectorRaw();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
 	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.raw(0:9)))
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=0:9))
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=0:9+.1))

cat("\n=== vector<bool>\n")
foo <- '
std::vector<bool> bv = RcppSexp(x).asStdVectorBool();
std::cout << "Flip the value of vector : ";
for (size_t i=0; i<bv.size(); i++) {
    bv[i].flip() ;
}
return(RcppSexp( bv ).asSexp());
'
funx <- cfunction(signature(x="logical"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=c(TRUE,FALSE)))
funx <- cfunction(signature(x="raw"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.raw(0:9)))
funx <- cfunction(signature(x="integer"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=0:9))
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=as.numeric(0:9)))


cat("\n===String Vector\n")
foo <- '
        std::vector<std::string> iv = RcppSexp(x).asStdVectorString();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = iv[i] + iv[i];
        }
 	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="character"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=c("foo", "bar")))

### using std::set
cat("\n=== set<int>\n")
foo <- '
std::set<int> iv ;
iv.insert( 0 ) ;
iv.insert( 1 ) ;
iv.insert( 0 ) ;
return RcppSexp( iv ).asSexp();'
funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>" )
print(res <- funx())
stopifnot( identical( res, 0:1 ) )

cat("\n=== set<double>\n")
foo <- '
std::set<double> ds;
ds.insert( 0.0 );
ds.insert( 1.0 );
ds.insert( 0.0 );
return(RcppSexp( iv ).asSexp()); '
funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>")
print( res <- funx() )
stopifnot( identical( res, as.numeric(0:1)))

cat("\n=== set<raw>\n")
foo <- '
std::set<Rbyte> bs ;
bs.insert( (Rbyte)0 ) ;
bs.insert( (Rbyte)1 ) ;
bs.insert( (Rbyte)0 ) ;
return(RcppSexp( bs ).asSexp()); '
funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, includes = "#include <set>")
print( res <- funx() )
stopifnot( identical( res, as.raw(0:1)))

cat("\n=== set<string> \n")
foo <- '
std::set<std::string> ss ;
ss.insert( "foo" ) ;
ss.insert( "bar" ) ;
ss.insert( "foo" ) ;
return(RcppSexp( ss ).asSexp()); '
funx <- cfunction(signature(), foo, Rcpp=TRUE, verbose=FALSE, include = "#include <set>" )
print( res <- funx() )
stopifnot( identical( res, c("bar","foo")) )


#========= attributes

funx <- cfunction(
	signature(x="data.frame"), '
std::vector<std::string> iv = RcppSexp(x).attributeNames();
return(RcppSexp( iv ).asSexp());
', Rcpp=TRUE, verbose=FALSE)
res <- funx( iris )
stopifnot( all( c("names", "row.names", "class" ) %in% res ) )

funx <- cfunction(signature(x="data.frame"), '
bool has_class = RcppSexp(x).hasAttribute( "class" ) ;
return RcppSexp( has_class ).asSexp() ;
', Rcpp=TRUE, verbose=FALSE)
res <- funx( iris )
stopifnot( res )

funx <- cfunction(signature(x="data.frame"), '
return RcppSexp(x).attr( "row.names" ).asSexp() ;
', Rcpp=TRUE, verbose=FALSE)
res <- funx( iris )
stopifnot( identical(res, 1:150) )

#============ NULL
funx <- cfunction(signature(x="ANY"), '
bool is_null = RcppSexp(x).isNULL() ;
return RcppSexp( is_null ).asSexp() ;
', Rcpp=TRUE, verbose=FALSE)
res <- funx( iris )
stopifnot( !res )
res <- funx( NULL )
stopifnot( res )



