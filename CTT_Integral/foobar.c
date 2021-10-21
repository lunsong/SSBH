#define f(x) x##a

f(bb)

int main(){
	double*x=malloc(sizeof(double)*20);
	memset(x,0,sizeof(double)*20);
	for(int i=0; i<20; i++)
		printf("%lf,", x[i]);
	return 0;
}
