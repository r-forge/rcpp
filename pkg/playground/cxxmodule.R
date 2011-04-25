require( inline )
require( Rcpp )

cxxmodule <- function( code = paste( readLines( file ), collapse = "\n" ), 
    ..., 
    plugin = "Rcpp", file ){
    
    fx <- cxxfunction( , '', includes = code, ..., plugin = plugin )
    
    lines <- strsplit( code, "\\n")[[1]]
    modline <- grep( "RCPP_MODULE[(].*[)]", lines, value = TRUE )
    modname   <- sub( "^.*RCPP_MODULE[(](.*?)[)].*$", "\\1", modline )
    mod <- Module( modname, getDynLib( fx ), mustStart = TRUE )
    mod
}

mod <- cxxmodule( '
double norm( double x, double y ){
    return sqrt( x*x + y*y) ;
}
RCPP_MODULE(foo){
    function( "norm", &norm ) ;
}
')

