
RcppLdPath <- function() {
    if (nzchar(.Platform$r_arch)) {	## eg amd64, ia64, mips
        system.file("lib",.Platform$r_arch,package="Rcpp")
    } else {
        system.file("lib",package="Rcpp")
    }
}
