#include "parameter.h"

#include "f_coef.h"

#define cube(x) ((x)*(x)*(x))
#define Idx(a,b,c,d) (idx(a,b)*N+idx(c,d))

int main(int argc, char *argv[]){
    double *A = malloc(sizeof(double)*N*N);
    double alpha = 9 * dr * dtheta * J2/ (16*M_PI);
    printf("calculating A\n");
    for(int ir=0; ir<N_r; ir++)
    {printf("%d / %d done\n",ir,N_r);
    for(int jr=0; jr<N_r; jr++)
    for(int itheta=0; itheta<N_theta; itheta++)
    for(int jtheta=0; jtheta<N_theta; jtheta++){
	if(ir==jr && itheta==jtheta)
	    continue;
	double ri = (ir+.5)*dr;
	double rj = (jr+.5)*dr;
	double thetai = itheta*dtheta;
	double thetaj = jtheta*dtheta;
	double denominator = ri*ri+rj*rj-2*cos(thetai)*cos(thetaj)*ri*rj;
	if (denominator < 1e-8){
	    A[Idx(ir,itheta,jr,jtheta)] = 0;
#if 0
	    printf("escaping: ri=%lf,rj=%lf,thetai=%lf,thetaj=%lf\n",
			    ri,rj,thetai,thetaj);
#endif
	    continue;
	}
	double sin_thetaj = sin(thetaj);
	double lambda = 2*(2*ri*rj*sin(thetai)*sin_thetaj / denominator) - 1;
	double f_lambda=0;
	for(int i=40; i>=0; i--)
	    f_lambda = f_lambda*lambda+f_coef[i];
	A[Idx(ir,itheta,jr,jtheta)]=
	    alpha*cube(rj)*cube(sin_thetaj)*f_lambda / sqrt(denominator);
    }
    }
    int fd = open("A.data", O_WRONLY);
    if(write(fd, A, sizeof(double)*N*N)==-1){
	printf("Unable to write A.data\n");
	close(fd);
	return -1;
    }
    close(fd);
    printf("calculating u\n");
    double *u = malloc(sizeof(double)*N);
    double *v = malloc(sizeof(double)*N);
    for(int i=0; i<N; i++) u[i] = 0;
    for(int step=0; step<N_step; step++){
    	double max_change = 0;
	for(int ir=0; ir<N_r; ir++)
	for(int itheta=0; itheta<N_theta; itheta++){
	    double foo = (u[idx(ir,itheta)]+1)*(ir+.5)*dr+M;
	    double bar = foo*foo*foo;
	    v[idx(ir,itheta)] = 1/(bar*bar*foo);
	}
	for(int i=0; i<N; i++){
	    double new = 0;
	    for(int j=0; j<N; j++)
		new += A[i*N+j]*v[j];
	    double change = fabs(u[i] - new);
	    if(change>max_change)
		max_change = change;
	    u[i] = new;
	}
	if(max_change<tol)
	    break;
	printf("step %d/%d: max_change %lf\n", step, N_step, max_change);
    }
    fd = open("U.data", O_WRONLY);
    if(write(fd, u, sizeof(double)*N)==-1){
	printf("Unable to write U.data\n");
	close(fd);
	return -1;
    }
    close(fd);
    return 0;
}



