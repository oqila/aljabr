

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "aljabr.h"
#include "libGauss.h"

//extern int gauss(float **matrix, int n, float *x,float *f);
// Function builts set of linear functions which are determined in [ x_i , x_i+1 ] intervals

int build_piecewise_linear_interpolation(int n,float *x,float *f ,float **a)
	{
	int i;
	if(n<1){fprintf(stderr,"Error: Can't build piecewise linear interpolation \n");return -1;}
	if(n>1)
		{
		for(i=0;i<n-1;i++)
			{
			a[i][0] = ( f[i+1] - f[i] )/( x[i+1] - x[i] );
			a[i][1] = f[i] - a[i][0]*x[i];
			fprintf(stdout,"a%i = %g , b%i =  %g\n",i+1, a[i][0],i+1,a[i][1]);
			}
		}
	else { a[0][0]=0; a[0][1]=f[0]; } // if only one point is given we assume that function is const,e.g. y(x) = const

	return 1;
	} 


// Function computes piecewise linear interpolating function's value on any arbitrary point in the interval

int find_piecewise_linear_interpolation_value(float & x0,float interval_a ,float interval_b ,int n,float **a,float *x)
	{
	float ai = 0 , bi = 0;
	int i;
	
	if ( (x0 < interval_a) || (x0 > interval_b) ){fprintf(stderr,"Warning: Incorrect point\n"); return -1; }

	if((interval_a<=x0) && (x0<=x[0])){ai=a[0][0];bi=a[0][1];}
	if ((x[n-1]<x0)&&(x0<=interval_b)) {ai=a[n-2][0];bi=a[n-2][1];}
	
		{
		for(i=0;i<n-1;i++) if ((x[i]<x0)&&(x0<=x[i+1])){ai=a[i][0];bi=a[i][1];break;}
		};

	x0 = ai * x0 + bi ;

	return 1;
	
	};




int build_parabolic_spline_interpolation(int n,float *x,float *f ,float **a,float *ksi) 
{
	float *v, **matrix,*Y,c1,c2,c3,c4,c5,c6;
	int i,j,nksi=n-1,nv=nksi,nrow=n-3,ncol=nv;

	if(n<3){ERROR("\
There are must be at least 3 point\n\
for parabolic spline interpolation.Since\n\
first and last points are taken as addition condition\n\
");return -1;}

	if((x[0]>=x[1]) || (x[n-2]>=x[n-1])){ERROR("Must be x0<x1 and xn<x(n+1).\
\nSince this program uses \"Additional knot at the \n\
limiting knots\" method.\n");return -1;}

	
	v   = (float *) malloc ( sizeof ( float)* (nv) );
	matrix = (float **) malloc (sizeof (float )* (nrow));
	Y   = (float *) malloc ( sizeof ( float)* (nrow) );
	for(i=0;i<nrow;++i) {
		matrix[i] = (float *) malloc (sizeof(float ) * ncol);
		for(j=0;j<ncol;j++) matrix[i][j] = 0.0;
				}
	//"Additional knot at the limiting knots method"
	// ksi1 = x0 and ksi(n+1) = x(n+1) -> 
	// v1 = f(ksi1) and v(n+1) = f(ksi(n+1))
	
	ksi[0] = x[0];
	ksi[nksi-1] = x[n-1];
	v[0] = f[0];
	v[nv-1] = f[n-1];

	// other ksis

	for(i=1;i<n-2;i++) ksi[i]= (x[i] + x[i+1])/2;

	// Debug
	printf("ksi : ");
	for(i=0;i<nksi;i++) printf("%f ",ksi[i]);
	printf("\n");

	// finding array v
	
	for(i=1;i<n-2;++i)
	{ 	
		j=i+1;
		
		c1=1/(x[j-1]-ksi[i-1]);
		c2=1/(ksi[i]-ksi[i-1]);
		c3=1/(ksi[i]-x[j-1]);
		c4=1/(x[j]-ksi[i]);
		c5=1/(ksi[i+1]-ksi[i]);
		c6=1/(ksi[i+1]-x[j]);
		
		matrix[i-1][i-1]=c1-c2;
		matrix[i-1][i]=c3+c2+c4+c5;
		matrix[i-1][i+1]=c6-c5;

		Y[i-1]=(c1+c3)*f[j-1]+(c4+c6)*f[j];
	}


	// Debug
	printf("Raw matrix : \n");
	for(i=0;i<nrow;++i)
	{
		for(j=0;j<ncol;j++) printf(" | %f ",matrix[i][j]);
		printf(" = %f \n",Y[i]);
	}



	Y[0]-=matrix[0][0]*v[0];
	Y[nrow-1]-=matrix[nrow-1][ncol-1]*v[nv-1];
	matrix[nrow-1][ncol-1] = matrix[0][0] = 0;

	for(i=0;i<nrow;++i) matrix[i] =  &matrix[i][1];

	// Debug
	printf("v1 vn added matrix : \n");
	for(i=0;i<nrow;++i)
	{
		for(j=0;j<ncol;j++) printf(" | %f ",matrix[i][j]);
		printf(" = %f \n",Y[i]);
	}


	if(FGauss(matrix,nrow,&v[1],Y)<0)return -1;
	
	// Debug
	printf("matrix after gauss method : \n");
	for(i=0;i<nrow;++i)
	{
		for(j=0;j<ncol;j++) printf(" | %f ",matrix[i][j]);
		printf(" = %f \n",Y[i]);
	}

	// Debug
	printf("v : ");	
	for(i=0;i<nv;++i)printf("%f ",v[i]);
	printf("\n");
	
	for(i=0;i<nv-1;++i)
	{
		c1 = v[i];
		c2 = (f[i+1] - v[i]) / (x[i+1] - ksi[i]) ;
		c3 = (1 / (ksi[i+1] - ksi[i])) * 
		( (v[i+1]- f[i+1]) / (ksi[i+1] - x[i+1]) - (f[i+1] - v[i]) / (x[i+1] - ksi[i]) );
	
	a[i][0] = c3; 
	a[i][1] = c2 - c3 * x[i+1] - c3 * ksi[i];
	a[i][2]	= c1 - c2 * ksi[i] + c3 * ksi[i] * x[i+1];
	}
	
	// Debug
	printf("a b c : \n");	
	for(i=0;i<nv-1;++i) printf("\n%f %f %f\n",a[i][0],a[i][1],a[i][2]);
	
	
	free(v);
	free(Y);
	free(matrix);			
	
	return 1;
}


int find_parabolic_spline_interpolation_value(float & x0,float interval_a ,float interval_b ,int n,float **a,float *ksi)
{
float ai = 0, bi = 0, ci = 0;
	int i;
	
	if ( (x0 < interval_a) || (x0 > interval_b) ){fprintf(stderr,"Warning: Incorrect point\n"); return -1; }

	for(i=0;i<n-2;i++) if ((ksi[i]<=x0)&&(x0<=ksi[i+1])){ai=a[i][0];bi=a[i][1];ci=a[i][2];break;}
		

	x0 = ai * x0 * x0 + bi * x0 + ci ;

	return 1;

}



float XQuadrat(float x0){return x0*x0-2*x0+1;};
float CosX(float x0){return 60 *cos((x0/180)*PI);};

float SinX(float x0){return 60 *sin((x0/180)*PI);};

float LogX(float x0){return log(x0);};

