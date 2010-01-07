#ifndef Rcpp_Rinternals_h
#define Rcpp_Rinternals_h

/* all of this comes from Rinternals.h so that we can get access to 
   DATA_PTR since R forbids to use VECTOR_PTR, we need to be able to call
   VECTOR_PTR to have pointer arithmetics on generic vectors, character
   vectors, expression vectors, etc... 
   
   below we define RCPP_VECTOR_PTR we can use where we would use
   VECTOR_PTR
   
   We cannot just define USE_RINTERNALS because it clashes with our use
   of R_NOREMAP
   
   There might be a possibility to not have to use pointer arithmztics
   with VECTOR_PTR but we would need to implement a custom iterator
   for Rcpp::GenericVector, ..., if we manage this then we will remove 
   these
   
   */

/* Flags */
struct sxpinfo_struct {
    SEXPTYPE type      :  5;/* ==> (FUNSXP == 99) %% 2^5 == 3 == CLOSXP
			     * -> warning: `type' is narrower than values
			     *              of its type
			     * when SEXPTYPE was an enum */
    unsigned int obj   :  1;
    unsigned int named :  2;
    unsigned int gp    : 16;
    unsigned int mark  :  1;
    unsigned int debug :  1;
    unsigned int trace :  1;  /* functions and memory tracing */
    unsigned int spare :  1;  /* currently unused */
    unsigned int gcgen :  1;  /* old generation number */
    unsigned int gccls :  3;  /* node class */
}; /*		    Tot: 32 */

struct vecsxp_struct {
    R_len_t	length;
    R_len_t	truelength;
};

struct primsxp_struct {
    int offset;
};

struct symsxp_struct {
    struct SEXPREC *pname;
    struct SEXPREC *value;
    struct SEXPREC *internal;
};

struct listsxp_struct {
    struct SEXPREC *carval;
    struct SEXPREC *cdrval;
    struct SEXPREC *tagval;
};

struct envsxp_struct {
    struct SEXPREC *frame;
    struct SEXPREC *enclos;
    struct SEXPREC *hashtab;
};

struct closxp_struct {
    struct SEXPREC *formals;
    struct SEXPREC *body;
    struct SEXPREC *env;
};

struct promsxp_struct {
    struct SEXPREC *value;
    struct SEXPREC *expr;
    struct SEXPREC *env;
};

#define SEXPREC_HEADER \
    struct sxpinfo_struct sxpinfo; \
    struct SEXPREC *attrib; \
    struct SEXPREC *gengc_next_node, *gengc_prev_node

/* The standard node structure consists of a header followed by the
   node data. */
typedef struct SEXPREC {
    SEXPREC_HEADER;
    union {
	struct primsxp_struct primsxp;
	struct symsxp_struct symsxp;
	struct listsxp_struct listsxp;
	struct envsxp_struct envsxp;
	struct closxp_struct closxp;
	struct promsxp_struct promsxp;
    } u;
} SEXPREC, *SEXP;

typedef struct VECTOR_SEXPREC {
    SEXPREC_HEADER;
    struct vecsxp_struct vecsxp;
} VECTOR_SEXPREC, *VECSEXP;
typedef union { VECTOR_SEXPREC s; double align; } SEXPREC_ALIGN;
#define RCPP_DATAPTR(x)	(((SEXPREC_ALIGN *) (x)) + 1)
#define RCPP_VECTOR_PTR(x)	((SEXP *) RCPP_DATAPTR(x))

#endif

