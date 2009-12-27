# Copyright (C)        2009 Romain Francois
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

uncaught_cpp_exception <- function( message = "uncaught C++ exception" ){
	callstack <- sys.calls()
	ncalls <- length(callstack)
	call <- if( ncalls > 1L) callstack[[ ncalls - 1L ]] else match.call()
	classes <- c( "C++Error", "error", "condition" )
	condition <- structure( 
		list( message = message, call = call ), 
		class = classes )
	stop( condition )
}

