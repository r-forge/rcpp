#include "patches.h"

using namespace Rcpp ;

IntegerVector sequence(IntegerVector nvec){
BEGIN_RCPP
    
    IntegerVector x(nvec ) ;
    R_len_t n = x.size(), s = 0 ;
    IntegerVector::iterator px = x.begin() ;
    int x_i ;
    
    /* error checking */
    for( int i=0; i<n; i++){
        x_i = px[i] ;
        if( x_i == NA_INTEGER || x_i <= 0 ) 
            throw std::range_error( "argument must be coercible to non-negative integer" ) ;
        s += x_i ;
    }
        
    IntegerVector out( s ) ;
    int end ;
    for( int i=0, start=0; i<n; i++){
        x_i = x[i] ;
        end = start + x_i - 1;
        out[ Range( start, end ) ] = seq_len( x_i ) ;
        start = end + 1;
    } 
    return out ;
    
END_RCPP
}                  

