#!/usr/bin/r -t
#
# Copyright (C) 2009 Romain Francois
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

require(Rcpp)
require(inline)
funx <- cfunction(signature(), '
throw std::range_error("boom") ;
return R_NilValue ;
', Rcpp=TRUE, verbose=FALSE)
tryCatch(  funx(), "C++Error" = function(e){
	print( "gotcha" )
} )
print( rnorm(10) )


