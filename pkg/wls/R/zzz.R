
# grab the namespace
.NAMESPACE <- environment()

# dummy module, will be replace later
mwls <- new( "Module" )

.onLoad <- function(pkgname, libname){
	# load the module and store it in our namespace
	unlockBinding( "mwls" , .NAMESPACE )
	assign( "mwls",  Module( "mwls" ), .NAMESPACE )
	lockBinding( "mwls", .NAMESPACE )
}

