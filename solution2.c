#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include<pthread.h>
#define N 4
pthread_mutex_t mtx;
void* Test(void* argument);
typedef struct setting{

	int id;
	int num_elements;
	int terms;
	double* x;
	double* result;
}Setting;

void sinx_talyor(int num_elements,int terms,double* x,double* result)
{
	Setting s;
	s.num_elements=num_elements;
	s.terms=terms;
	s.x=x;
	s.result=result;

	pthread_t threads[4];
	int i;

	for(int i=0;i<4;i++)
	{
		s.id=i;
		pthread_create(&threads[i],NULL,Test,(void*)&s);
	}

	for(int i=0;i<4;i++)
	{
		pthread_join(threads[i],NULL);
	}

}

void* Test(void* argument)
{
	Setting s=*((Setting*)argument);
	for(int i=0;i<s.num_elements;i++)
	{
		double value=s.x[i];
		double number=s.x[i]*s.x[i]*s.x[i];
		double denom=6;
		int sign=-1;
		if(s.id!=i) continue;

		for(int j=1;j<=s.terms;j++)
		{
			value+=(double)sign*number/denom;
			number*=s.x[i]*s.x[i];
			denom*=(2.*(double)j+2.)*(2.*(double)j+3.);
			sign*=-1;
		}

		pthread_mutex_lock(&mtx);
		s.result[i]=value;
		pthread_mutex_unlock(&mtx);
	}
}
int main()
{

	pthread_mutex_init(&mtx,NULL);
	double x[N]={0,M_PI/6.,M_PI/3.,0.134};
	double res[N];

	sinx_talyor(N,3,x,res);
	for(int i=0;i<N;i++)
	{

		printf("sin(%.2f) by Talyor serires=%f\n",x[i],res[i]);
		printf("sin(%.2f)= %f\n",x[i],sin(x[i]));
	}

	return 0;
}


