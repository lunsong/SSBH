from numpy.polynomial.chebyshev import chebfit, cheb2poly
polyfit = lambda x,y,n: cheb2poly(chebfit(x,y,n))
N_t     = 100
N_int   = 1000

