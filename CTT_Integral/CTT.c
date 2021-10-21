#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

const    int  N_r    = 100;
const    int  N_theta = 100;
const    int  N_step = 200;
const    int  N      = N_r * N_theta;
const double  dr     = .1;
const double  J      = 1;
const double  J2     = J*J;
const double  M      = 1;
const double  dtheta = M_PI / N_theta;
const double  tol    = 1e-4;

#define idx(u,i) u[i##r*N_theta+i##theta]
#define For(i) \
	for(int i##r=0;i##r<N_r;i##r++)\
	for(int i##theta=0;i##theta<N_theta;i##theta++){\
		r##i=(i##r+.5)*dr;\
		theta##i=(i##theta)*dtheta;

#define cube(x) ((x)*(x)*(x))

#include "f_coef.h"

int main(int argc, char *argv[]){
  double *u = malloc(sizeof(double)*N);
  double *v = malloc(sizeof(double)*N);
  memset(u, 0, sizeof(double)*N);
  memset(v, 0, sizeof(double)*N);
  double alpha = 9 * dr * dtheta * J2/ (16*M_PI);
  for(int step=0; step<N_step; step++){
    for(int ir=0; ir<N_r; ir++)
      for(int itheta=0; itheta<N_theta; itheta++){
	double foo = (u[idx(ir,itheta)]+1)*(ir+.5)*dr+M;
	double bar = foo*foo*foo;
	idx(v,i) = 1/(bar*bar*foo);
	idx(u,i) = 0;
	for(int jr=0; jr<N_r; jr++)
	  for(int jtheta=0; jtheta<N_theta; jtheta++){
	    if(ir==jr && itheta==jtheta)
	      continue;
	    double ri = (ir+.5)*dr;
	    double rj = (jr+.5)*dr;
	    double thetai = itheta*dtheta;
	    double thetaj = jtheta*dtheta;
	    double denominator = ri*ri+rj*rj-2*cos(thetai)*cos(thetaj)*ri*rj;
	    if (denominator < 1e-8)
	      continue;
	    double sin_thetaj = sin(thetaj);
	    double lambda = 2*(2*ri*rj*sin(thetai)*sin_thetaj / denominator) - 1;
	    double f_lambda=0;
	    for(int i=40; i>=0; i--)
	      f_lambda = f_lambda*lambda+f_coef[i];
	    u[idx(ir,itheta)] += v[idx(jr,
	      alpha*cube(rj)*cube(sin_thetaj)*f_lambda / sqrt(denominator);
	  }
      }
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



