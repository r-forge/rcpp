
## From: Douglas Bates <bates@stat.wisc.edu>
## To: xian@unm.edu
## Cc: William Dunlap <wdunlap@tibco.com>, rcpp-devel@r-forge.wu-wien.ac.at,
##         Kevin Ummel <kevinummel@gmail.com>
## Subject: Re: [Rcpp-devel] [R] Find number of elements less than some number:
##  Elegant/fastsolution needed
## Date: Sun, 17 Apr 2011 13:00:39 -0500

## I enclose some comparisons, admittedly on only one size of example (10
## million in the reference distribution and 100,000 in the test sample).

## The versions using Rcpp and the std algorithms came out about 3 times
## as fast as the version using R's findInterval.  What surprises me is
## that the sequential comparisons in C++ (version k) is slightly faster
## than the binary search versions (h and j).  The unassisted binary
## search requires 30 probes for each element of ans and the sequential
## comparisons show take an average of 100 comparisons.  I suppose the
## difference is sequential access versus random access to the elements
## of y.

## [ ... lots of lines from email deleted, final results below ... ]

## > benchmark(f(x,y), g(x,y), h(x,y), j(x,y), k(x,y),
## +           order="relative", replications=10,
## +           columns=c("test", "replications", "elapsed", "relative"))
##      test replications elapsed relative
## 5 k(x, y)           10  19.477 1.000000
## 4 j(x, y)           10  19.984 1.026031
## 3 h(x, y)           10  20.107 1.032346
## 2 g(x, y)           10  57.691 2.962006
## 1 f(x, y)           10  60.563 3.109462
## >
## >
## > proc.time()
##    user  system elapsed
## 213.060  15.880 233.067

## Code slighly edited

suppressMessage(require(inline))
suppressMessage(require(Rcpp))
suppressMessage(require(rbenchmark))

## I'm changing the rules a bit here because Sunduz wanted the
## counts in the original order of x

f <- function(x, y) length(y) - findInterval(-x, rev(-sort(y)))

## findInterval can take advantage of x being ordered

g <- function(x, y) {
    ord <- order(x)
    ans <- integer(length(x))
    ans[ord] <- length(y) - findInterval(-x[ord], rev(-sort(y)))
    ans
}

## version using std::upper_bound for unordered x
h <- cxxfunction(signature(x_="numeric", y_="numeric"), '
{
    Rcpp::NumericVector x(x_),
        y = clone(Rcpp::NumericVector(y_));
    int n = x.size();
    Rcpp::IntegerVector ans(n);
    const Rcpp::NumericVector::iterator bb = y.begin(), ee = y.end();
               // sort (a copy of) the y values for binary search
    std::sort(bb, ee);

    for (int i = 0; i < n; i++) {
	ans[i] = std::upper_bound(bb, ee, x[i]) - bb;
    }
    return ans;
}
', plugin="Rcpp")

## version using std::upper_bound when x is non-decreasing
j1 <- cxxfunction(signature(x_="numeric", y_="numeric"), '
{
    Rcpp::NumericVector x(x_),
        y = clone(Rcpp::NumericVector(y_));
    int n = x.size();
    Rcpp::IntegerVector ans(n);
    const Rcpp::NumericVector::iterator bb = y.begin(), ee = y.end();
    Rcpp::NumericVector::iterator mm = y.begin();
               // sort (a copy of) the y values for binary search
    std::sort(bb, ee);

    for (int i = 0; i < n; i++) {
        mm = std::upper_bound(mm, ee, x[i]);
        ans[i] = mm - bb;
    }
    return ans;
}
', plugin="Rcpp")

j <- function(x, y) {
    ord <- order(x)
    ans <- integer(length(x))
    ans[ord] <- j1(x[ord], y)
    ans
}

## version using sequential search
k1 <- cxxfunction(signature(xs_="numeric", y_="numeric", ord_="integer"), '
{
    Rcpp::NumericVector xs(xs_),
        y = clone(Rcpp::NumericVector(y_));
    int n = xs.size();
    Rcpp::IntegerVector ord(ord_), ans(n);
    const Rcpp::NumericVector::iterator bb = y.begin(), ee = y.end();
    Rcpp::NumericVector::iterator yy = y.begin();
    double *xp = xs.begin();
               // sort (a copy of) the y values for binary search
    std::sort(bb, ee);

    for (int i = 0; i < n && yy < ee; i++, xp++) {
	while (*xp >= *yy && yy < ee) yy++;
	ans[ord[i] - 1] = yy - bb;
    }
    return ans;
}
', plugin="Rcpp")

k <- function(x, y) {
    ord <- order(x)
    k1(x[ord], y, ord)
}

x <- rnorm(1e5)
yy <- y <- rnorm(1e7)
yy[1] <- y[1]                           # ensure y and yy are distinct

aa <- f(x, y)
all.equal(aa, g(x, y))
all.equal(aa, h(x, y))
all.equal(aa, j(x, y))
all.equal(aa, k(x, y))
all.equal(y, yy)                        # check that nothing has changed in y

res <- benchmark(f(x,y), g(x,y), h(x,y), j(x,y), k(x,y),
                 order="relative", replications=10,
                 columns=c("test", "replications", "elapsed", "relative"))
print(res)
