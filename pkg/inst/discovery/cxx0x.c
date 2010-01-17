#include <R.h>
#include <Rdefines.h>

SEXP cxx0x(){

#ifdef __GNUC__
	#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#if GCC_VERSION >= 40300
	return mkString( "-std=c++0x" ) ;
	#endif
#endif
return mkString( "" ) ;
}

