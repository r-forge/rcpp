
hijack <- function(name, value, namespace = "base" ){
	namespace <- asNamespace( namespace )
	unlockBinding( name, namespace )
	assign( name, value, envir = namespace )
	lockBinding(name, namespace )
}


.onLoad <- function(libname, pkgname){
	m <- Module( "patches" )
	hijack( "sequence", m$sequence )
}    


