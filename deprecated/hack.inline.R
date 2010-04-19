
hack_compileCode <- function(){
	inline <- asNamespace( "inline" )
	
	unlockBinding( "compileCode", inline )
	compileCode <- get("compileCode", 	inline )
	exprs <- append( 
		list( as.name("{"), parse(text="code <- Rcpp:::compileCode_hook(f, code)")[[1]] ) , 
		as.list( body( compileCode )[-1] )
		)
	body( compileCode ) <- as.call( exprs )
	assignInNamespace( "compileCode", compileCode, inline )
	unlockBinding( "compileCode", inline )
}

compileCode_hook <- function( f, code){
	# we only do something if we are called from cppfunction
	calls <- sys.calls()
	functions <- sapply( calls, function(.){ if( is.name(.[[1]]) ) as.character( .[[1]]) else "" } )
	if( "cppfunction" %in% functions ){
sprintf( 
'%s
#if defined(WIN32)
extern "C" void R_init_%s( DllInfo* info ){
	std::set_terminate( forward_uncaught_exceptions_to_r ) ;
}
#endif
', code, f )
	} else {
		code
	}
}

