#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#define For(i)\
    for(int i##x=0;i##x<Nx;i##x++){\
    double x##i=(i##x + .5) * dx;\
    for(int i##y=0;i##y<Nx;i##y++){\
    double y##i=(i##y + .5) * dx;\
    for(int i##z=0;i##z<Nx;i##z++){\
    double z##i=(i##z + .5) * dx;

#define End }}}
#define idx(a,b,c) u[((a)*Nx+(b))*Nx+(c)]
#define get(i) u[(i##x*Nx+i##y)*Nx+i##z+side*N]
#define set(i) u[(i##x*Nx+i##y)*Nx+i##z+(1-side)*N]
#define norm2(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))

void CTT(u,Nx,N_step,step,dx,M,J2,tol,alpha,verbose)
double *u,dx,M,J2,tol,alpha;
int Nx,N_step,*step;
_Bool verbose;
{
    int N = Nx*Nx*Nx;
    int side = 1;
    for(*step=0; *step < N_step; (*step)++){
        side = 1-side;
	double max_change = 0;
	For(i)
	    if(verbose){
                printf("step %d: %d/%d\r", *step, ix, Nx);
                fflush(stdout);
            }
	    double s = 0;
    	    For(j)
		double rj2  = norm2(xj,yj,zj);
		double rj   = sqrt(rj2);
		double sin2 = (xj*xj+yj*yj) / rj2;
		double x    = (get(j)+1)*rj+M;
		double x3   = x*x*x;
		double f    = dx*dx*dx*9*J2*rj*sin2/(16*M_PI*x3*x3*x);
		if(ix==jx&&iy==jy&&iz==jz)
		    s += alpha * f / dx;
		else
		    s += f / sqrt(norm2(xi-xj,yi-yj,zi-zj));
		s += f / sqrt(norm2(xi+xj,yi-yj,zi-zj));
		s += f / sqrt(norm2(xi-xj,yi+yj,zi-zj));
		s += f / sqrt(norm2(xi-xj,yi-yj,zi+zj));
		s += f / sqrt(norm2(xi+xj,yi+yj,zi-zj));
		s += f / sqrt(norm2(xi-xj,yi+yj,zi+zj));
		s += f / sqrt(norm2(xi+xj,yi-yj,zi+zj));
		s += f / sqrt(norm2(xi+xj,yi+yj,zi+zj));
	    End
	    double change = fabs(s-get(i));
	    if(change>max_change)
		max_change=change;
	    set(i) = s;
	End
	if(verbose)
            printf("step %d: max change %e\n", *step, max_change);
	if(max_change<tol) break;
    }
}
