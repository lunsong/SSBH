#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

const    int  N_r    = 100;
const    int  N_theta = 100;
const    int  N_step = 50;
const    int  N      = N_r * N_theta;
const double  dr     = .1;
const double  J      = 1;
const double  J2     = J*J;
const double  M      = 1;
const double  dtheta = M_PI / N_theta;

#define idx(ir,itheta) ((ir)*N_theta+(itheta))
