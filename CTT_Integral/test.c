#include "parameter.h"

int main(){
    double *u = malloc(sizeof(double)*N);
    int fd = open("U.data", O_RDONLY);
    printf("read: %d\n", read(fd, u, sizeof(double)*N));
    close(fd);
    double max_err = 0;
    double max_err_r, max_err_theta;
    for(int ir=1; ir<N_r-1; ir++)
    for(int itheta=1; itheta<N_theta-1; itheta++){
	double r = (ir+.5)*dr;
	double theta = (itheta+.5)*dtheta;
	double err = 
         (u[idx(ir+1,itheta)]+u[idx(ir-1,itheta)]-2*u[idx(ir,itheta)])/(dr*dr);
	err += (u[idx(ir+1,itheta)]-u[idx(ir-1,itheta)])/(dr*r);
	err +=
	 (u[idx(ir,itheta+1)]+u[idx(ir,itheta-1)]-2*u[idx(ir,itheta)])/(r*r*dtheta*dtheta);
	err += (u[idx(ir,itheta+1)]-u[idx(ir,itheta-1)])*tan(M_PI/2 - theta)/(2*dtheta*r*r);
	double s = sin(theta);
	s *= s;
	double foo = (u[idx(ir,itheta)]+1)*r+M;
	double bar = foo*foo*foo;
	foo =  2.25*J2*r*s / (bar*bar*foo);
	printf("r=%lf,theta=%lf,u=%lf,D2u=%lf,F=%lf,err=%lf\n",
			r,theta,u[idx(ir,itheta)],err,foo,err+foo);
	err += foo;
	err = fabs(err);
	if(err > max_err){
	    max_err = err;
	    max_err_r = r;
	    max_err_theta = theta;
	}
    }
    printf("max error %lf @ r = %lf , theta = %lf\n", max_err, max_err_r, max_err_theta);
    return 0;
}



