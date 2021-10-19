#include "parameter.h"

int main(){
    printf("calculating u\n");
    double *u = malloc(sizeof(double)*N);
    double *v = malloc(sizeof(double)*N);
    double *A = malloc(sizeof(double)*N*N);
    int fd = open("A.data", O_RDONLY);
    if(read(fd, A, sizeof(double)*N*N)==-1){
	printf("Unable to read A.data\n");
	close(fd);
	return -1;
    }
    close(fd);
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
 
