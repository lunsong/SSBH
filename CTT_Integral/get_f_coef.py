import numpy as np
from config import polyfit, N_t, N_int

t = np.linspace(0,.99999999,N_t)
y = (np.sum(1/np.sqrt(1-t.reshape((N_t,1))*np.cos(
    np.linspace(0,np.pi*2,N_int)
    )), axis=1) / N_int * 2. * np.pi)

np.set_printoptions(floatmode="unique")
f_coef = str(polyfit(2*t-1,y,40))
f_coef = f_coef.strip('[] ').split()
f_coef = ',\n'.join(','.join(f_coef[i:i+3])
                        for i in range(0,len(f_coef),3))
print(f"const double f_coef[]={{\n{f_coef}}};")
