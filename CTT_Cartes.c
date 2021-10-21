#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

const int    Nx     = 20;
const int    N      = Nx*Nx*Nx;
const int    N_step = 100;
const double range  = 10;
const double dx     = range / Nx;
const double M      = 1;
const double J      = 1;
const double J2     = J*J;
const double tol    = 1e-6;
const double alpha  = 6 / (sqrt(3)+1);

#define grid_offset .5

#define For(i)\
    for(int i##x=0;i##x<Nx;i##x++){\
    double x##i=(i##x+grid_offset)*dx;\
    for(int i##y=0;i##y<Nx;i##y++){\
    double y##i=(i##y+grid_offset)*dx;\
    for(int i##z=0;i##z<Nx;i##z++){\
    double z##i=(i##z+grid_offset)*dx;

#define End }}}
#define idx(a,b,c) u[((a)*Nx+(b))*Nx+(c)]
#define get(i) u[(i##x*Nx+i##y)*Nx+i##z+side*N]
#define set(i) u[(i##x*Nx+i##y)*Nx+i##z+(1-side)*N]
#define norm2(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))

int main(){
    double*u = malloc(sizeof(double)*N*2);
    int side = 0;
    memset(u, 0, sizeof(double)*N*2);
    for(int step=0; step<N_step; step++){
	double max_change = 0;
	For(i)
	    printf("step %d: %d/%d\r", step, ix, Nx);fflush(stdout);
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
	printf("step %d: max change %e\n", step, max_change);
	if(max_change<tol) break;
	side = 1-side;
    }
    int fd = open("u.dat", O_WRONLY);
    u += N*(1-side);
    write(fd, u, sizeof(double)*N);
    close(fd);
    For(i)
	if(ix==0||iy==0||iz==0||ix==Nx-1||iy==Nx-1||iz==Nx-1)
	    continue;
	double r2 = norm2(xi,yi,zi);
	double r  = sqrt(r2);
	double s2 = (xi*xi+yi*yi)/r2;
	double x  = (idx(ix,iy,iz)+1)*r+M;
	double x3 = x*x*x;
	double err = ((idx(ix+1,iy,iz) + idx(ix-1,iy,iz)
		       +idx(ix,iy+1,iz) + idx(ix,iy-1,iz)
		       +idx(ix,iy,iz+1) + idx(ix,iy,iz-1)
		       -6*idx(ix,iy,iz)) / (dx*dx)
	    	     +(9*J2*r*s2)/(4*x3*x3*x)) / idx(ix,iy,iz);
	printf("%e @ (%lf,%lf,%lf)\n", fabs(err), xi,yi,zi);
    End
    return 0;
}


