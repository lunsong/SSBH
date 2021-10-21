#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

const    int  N_r    = 100;
const    int  N_th   = 100;
const    int  N_step = 200;
const    int  N      = N_r * N_th;
const double  dr     = .1;
const double  J      = 1;
const double  J2     = J*J;
const double  M      = 1;
const double  dth    = M_PI / N_th;
const double  tol    = 1e-4;

#define get(u,i) u[i##r*N_th+i##th]
#define For(i)\
	for(int i##r=0;i##r<N_r;i##r++)\
	for(int i##th=0;i##th<N_th;i##th++){\
		double r##i=(i##r+.5)*dr;\
		double th##i=(i##th)*dth;
#define End }

#define cube(x) x*x*x

#include "f_coef.h"

int main(){
    double *u = malloc(sizeof(double)*N);
    double *v = malloc(sizeof(double)*N);
    memset(u, 0, sizeof(double)*N);
    memset(v, 0, sizeof(double)*N);
    double a = 9 * dr * dth * J2/ (16*M_PI);
    for(int step=0; step<N_step; step++){
	printf("%d ", step);
	For(i)
	    double foo = (get(u,i)+1)*ri+M;
    	    double bar = foo*foo*foo;
	    get(v,i) = 1/(bar*bar*foo);
	End
	int converged = 1;
	For(i) 
	    double new = 0;
	    For(j)
	   	 if(ir==jr&&ith==jth) continue;
	   	 double d  = ri*ri + rj*rj - 2*cos(thi)*cos(thj)*ri*rj;
		 if(d<1e-8)           continue;
	         double sj = sin(thj);
	         double x  = 4*ri*rj*sin(thi)*sj/d - 1;
	         double f  = 0;
	         for(int i=40; i>=0; i--) f = f*x+f_coef[i];
	         new += get(v,j)*a*cube(rj)*cube(sj)*f/sqrt(d);
	    End
	    if(fabs(new-get(u,i))>tol) converged = 0;
	End
	if(converged) break;
    }
    int fd = open("u.dat",O_WRONLY);
    write(fd, u, sizeof(double)*N);
    close(fd);
    return 0;
}



	    
 
