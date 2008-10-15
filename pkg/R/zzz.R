# Init file for package Rcpp
.onLoad <- function(lib, pkg) {
    library.dynam("Rcpp", pkg, lib)
}

# Runs Rcpp demos interactively.
RcppDemo <- function() {
    demoCode <- demo(package="Rcpp")$results[,3]
    if(length(demoCode) == 0) {
        return('No demos found in package RcppTemplate')
    }
    while(TRUE) {
        for(i in seq(1,length(demoCode))) {
            cat(i, ': ', demoCode[i], '\n')
        }
        suppressWarnings(demoNum <- as.integer(readline(prompt="Enter demo number to run [0 to view descriptions, RETURN to exit]: ")))

        if(is.na(demoNum)) {
            cat("Didn't get a number, exiting.\n")
            break
        }
        else if(demoNum == 0) {
            print(demo(package="Rcpp"))
        }
        else {
            if(demoNum < 0 || demoNum > length(demoCode)) {
                cat('Number out of range, try again...\n')
                next
            }
            demo(demoCode[demoNum], character.only=TRUE)
        }
    }
}

