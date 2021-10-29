from ctypes import * 
from numpy.ctypeslib import ndpointer 
import numpy as np
ctt = cdll.LoadLibrary("./CTT.so").CTT
ctt.restype = None
ctt.argtypes = [ndpointer(np.float64),
                c_int,
                c_int,
                POINTER(c_int),
                c_double,
                c_double,
                c_double,
                c_double,
                c_double,
                c_bool]
class CTT:
    def __init__(self, x_span, Nx, M, J):
        self.Nx = Nx
        self.dx = x_span / Nx
        self.M  = M
        self.J  = J
        self.u  = np.zeros(shape=(2,Nx,Nx,Nx))
        self.tol = None
        self.step = 0
    def solve(self, step=100, tol=1e-6, verbose=True, alpha=None):
        if alpha==None:
            alpha=(np.sqrt(3)+1)/2
        if step%2==1:
            step += 1
        step_run = c_int(0)
        ctt(self.u,self.Nx,step,step_run,self.dx,self.M,
                self.J*self.J,tol,alpha,verbose)
        step_run = step_run.value + 1
        if step_run < step:
            self.tol = tol
        self.step += step_run

if __name__ == '__main__':
    sim = CTT(x_span=10, Nx=20, M=1., J=1.)
    sim.solve()
