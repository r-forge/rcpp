RcppVersion <- function() {

    version <- installed.packages()["Rcpp","Version"]

    ## Print version of this package
    cat('\nRcpp package Version ', version, '\n\n')

    ## Print version of Rcpp/RcppTemplate used to build this package
    licenseFile <- file(system.file(".", "LICENSE-Rcpp.txt", package="Rcpp"),"r")
    writeLines(readLines(licenseFile)[1:5])
    invisible(NULL)
}
