
RcppLdPath <- function() {
    if (nzchar(.Platform$r_arch)) {	## eg amd64, ia64, mips
        system.file("lib",.Platform$r_arch,package="Rcpp")
    } else {
        system.file("lib",package="Rcpp")
    }
}

RcppLdFlags <- function(static=FALSE) {
    flags <- "-lRcpp"				## general default
    if (.Platform$OS.type == "unix") {
        if(length(grep("^linux",R.version$os))) {
            if (static==FALSE) {		## on Linux with dyn. linking, use rpath too
                rcppdir <- RcppLdPath()
                flags <- paste("-lRcpp -Wl,-rpath,", rcppdir, sep="")
            }
        }
    }
    invisible(flags)
}

