.NameSpace <- environment()
.onLoad <- function(libname, pkgname) {
    ## load the module and store it in our namespace
    mod <- Module("RcppModels")
    populate(mod, .NameSpace)
}
