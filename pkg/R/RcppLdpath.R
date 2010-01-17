
## Use R's internal knowledge of path settings to find the lib/ directory
## plus optinally an arch-specific directory on system building multi-arch
RcppLdPath <- function() {
    if (nzchar(.Platform$r_arch)) {	## eg amd64, ia64, mips
        system.file("lib",.Platform$r_arch,package="Rcpp")
    } else {
        system.file("lib",package="Rcpp")
    }
}

## Provide linker flags -- i.e. -L/path/to/libRcpp -- as well as an
## optional rpath call needed to tell the Linux dynamic linker about the
## location.  This is not needed on OS X where we encode this as library
## built time (see src/Makevars) or Windows where we use a static library
RcppLdFlags <- function(static=FALSE) {
    rcppdir <- RcppLdPath()
    flags <- paste("-L", rcppdir, " -lRcpp", sep="")	## general default
    if (.Platform$OS.type == "unix") {
        if (length(grep("^linux",R.version$os))) {
            if (static==FALSE) {		## on Linux with dyn. linking, use rpath too
                flags <- paste(flags, " -Wl,-rpath,", rcppdir, sep="")
            }
        }
    }
    invisible(flags)
}

# indicates if Rcpp was compiled with GCC >= 4.3
canUseCXX0X <- function() .Call( "canUseCXX0X", PACKAGE = "Rcpp" )

## Provide compiler flags -- i.e. -I/path/to/Rcpp.h
RcppCxxFlags <- function(cxx0x=FALSE) {
    paste("-I", RcppLdPath(), if( cxx0x && canUseCXX0X() ) " -std=c++0x" else "", sep="")
}

## Shorter names, and call cat() directly
CxxFlags <- function(cxx0x=FALSE) cat(RcppCxxFlags(cxx0x=cxx0x))
LdFlags <- function(static=FALSE) cat(RcppLdFlags(static=static))

# capabilities
RcppCapabilities <- capabilities <- function() .Call("capabilities", PACKAGE = "Rcpp")

# compile, load and call the cxx0x.c script to identify whether
# the compiler is GCC >= 4.3
RcppCxx0xFlags <- function(){
	td <- tempfile()
	dir.create( td )
	here <- getwd()
	setwd(td)
	on.exit( { setwd(here) ; unlink( td, recursive = TRUE ) } )
	file.copy( system.file( "discovery", "cxx0x.c", package = "Rcpp" ), td )
        cmd <- paste(R.home(component="bin"), "/R CMD SHLIB cxx0x.c", sep="")
	system( cmd, intern = TRUE )
	dll <- sprintf( "cxx0x%s", .Platform$dynlib.ext )
	dyn.load( dll )
	res <- tryCatch( .Call( "cxx0x" ), error = "" )
	dyn.unload( dll )
	res
}

Cxx0xFlags <- function() cat( RcppCxx0xFlags() )

