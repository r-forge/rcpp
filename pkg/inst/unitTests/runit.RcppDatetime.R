#!/usr/bin/r -t
#
# Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
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

.setUp <- function(){
    suppressMessages( require( inline ) )
}

test.RcppDatetime.get.functions <- function() {
    src <- '//RcppDatetime dt = RcppDatetime(946774923.123456); // 2001-02-03 01:02:03.123456
            RcppDatetime dt = RcppDatetime(1152338523.456789); // as.POSIXct("2006-07-08 01:02:03.456789")
            RcppResultSet rs;
            //std::cout << dt << std::endl;
            rs.add("year",     dt.getYear());
            rs.add("month",    dt.getMonth());
            rs.add("day",      dt.getDay());
            rs.add("wday",     dt.getWeekday());
            rs.add("hour",     dt.getHour());
            rs.add("minute",   dt.getMinute());
            rs.add("second",   dt.getSecond());
            rs.add("microsec", dt.getMicroSec());
            //rs.add("dt",    dt);
	        return rs.getReturnList();';
    funx <- cfunction(signature(), src, Rcpp=TRUE)
    checkEquals(funx(),
                list(year=2006, month=7, day=8, wday=6,
                     hour=1, minute=2, second=3, microsec=456789),
                msg = "RcppDate.get.functions")
}

test.RcppDatetime.operators <- function() {
     src <- 'RcppDatetime d1 = RcppDatetime(946774923.123456);
             //RcppDatetime d1 = RcppDatetime(1152338523.456789); // as.POSIXct("2006-07-08 01:02:03.456789")
             RcppDatetime d2 = d1 + 60*60;
             RcppResultSet rs;
             rs.add("diff",    d2 - d1);
             rs.add("bigger",  d2 > d1);
             rs.add("smaller", d2 < d1);
             rs.add("equal",   d2 == d1);
             rs.add("ge",      d2 >= d1);
             rs.add("le",      d2 <= d1);
   	         return rs.getReturnList();';
     funx <- cfunction(signature(), src, Rcpp=TRUE)
     checkEquals(funx(), list(diff=3600, bigger=1, smaller=0, equal=0, ge=1, le=0), msg = "RcppDatetime.operators")
}














