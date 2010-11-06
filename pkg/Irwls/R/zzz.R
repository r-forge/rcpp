
# grab the namespace
.NAMESPACE <- environment()

# dummy module, will be replace later
mIrwls <- new( "Module" )

.onLoad <- function(pkgname, libname){
	# load the module and store it in our namespace
	unlockBinding( "mIrwls" , .NAMESPACE )
	assign( "mIrwls",  Module( "mIrwls" ), .NAMESPACE )
	lockBinding( "mIrwls", .NAMESPACE )
}

