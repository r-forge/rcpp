#include "patches.h"

using namespace Rcpp ;

RCPP_MODULE(patches){
    function( "sequence", &sequence, List::create( _["nvec"] ) ) ;   
}

