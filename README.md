# CTT Initial Data

Following CTT, the initial data for a single spinning blackhole can be obtained by solving
$$
\Delta \psi+{1\over8}\psi^5K_{ij}K^{ij}=0
$$
The punctured version is
$$
\psi=1+{\mathcal{M}\over r}+u
$$
$$
\Delta u+{9J^2r\sin^2\theta\over4[(u+1)r+\mathcal{M}]^7}=0
$$
Solve it by
$$
u^{(k+1)}(r)=\int {9J^2r'\sin^2\theta\over16\pi[(u^{(k)}+1)r'+\mathcal{M}]^7|r-r'|}\mathrm{d}^3r'
$$
In spherical coordinate,
$$
u^{(k+1)}(r,\theta)=\int\limits_0^\pi\mathrm{d}\theta'\int\limits_0^{r_0}\mathrm{d}r'
{9J^2r^3\sin^3\theta 
f\left(
{2r'r\sin\theta\sin\theta'\over r^2+r'^2-2r'r\cos\theta\cos\theta' }
\right)
\over 4[(u^{(k)}+1)r'+\mathcal{M}]^7
\sqrt{r^2+r'^2-2r'r\cos\theta'\cos\theta}}
$$
where 
$$
f(x)=\int\limits_0^{2\pi}{\mathrm{d}\psi\over\sqrt{1-x\cos\psi}}
$$
