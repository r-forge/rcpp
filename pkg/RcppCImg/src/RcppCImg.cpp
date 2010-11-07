#include <RcppCImg.h>

namespace RcppCImg{
 
    class CImg_Rbyte {
    public:
        CImg_Rbyte() :image(){}
        CImg_Rbyte(const char* filename) : image( filename){}
        CImg_Rbyte(int w, int h) : image(w,h){}
       
        void fill( Rbyte value ){ image.fill( value ) ; }
        void noise( double sigma, int noise_type ){ image.noise(sigma, noise_type) ; }
        void save( std::string filename){ image.save( filename.c_str() ) ; }
        void resize( int w, int h){ image.resize(w,h) ; }
        
        private:
            cimg_library::CImg<Rbyte> image ;
    } ;
    
}

RCPP_MODULE(CIMG){
    using namespace Rcpp ;
    
    class_<RcppCImg::CImg_Rbyte>( "CImg_Rbyte" )
    
        .default_constructor( )
        .constructor( init_1<const char*>() )
        .constructor( init_2<int,int>() )
    
        .method( "fill" , &RcppCImg::CImg_Rbyte::fill )
        .method( "noise", &RcppCImg::CImg_Rbyte::noise )
        .method( "save", &RcppCImg::CImg_Rbyte::save )
        .method( "resize", &RcppCImg::CImg_Rbyte::resize )
    ;    
    
}



