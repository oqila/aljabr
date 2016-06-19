/***************************************************************************
*                       Library functions for Al-Jabr project
*
*   File    : libGauss.cpp
*   Purpose : Standard funtions for solving linear equations by Gauss method and analisys.
*   Author  : Fazliddin Jo'rayev
*   Date    : November 15, 2009
*
*
*   Copyright (C) 2009 by  Fazliddin Jo'rayev (fazliddin.uzbek@yahoo.com)
*
* 
*
* libGauss.cpp is free software WITHOUT ANY WARRANTY as long as you do not remove 
* author's name from the file; you can redistribute it and/or modify it under the 
* afore-mentioned terms. Unless you have purchased license, DO NOT REMOVE author's 
* author's name from the file
*
***************************************************************************/



#include "libGauss.h"


//----------GAUSS METHOD------------------------
int FGauss(float **matrix, int n, float *x,float *f)
{

//---------VARIABLE DECLARATION AND INITIALIZING--
	int i,j;
	
        float **teka,*teka_f;
	
	

	teka=(float **)malloc(sizeof(float) *n);
	for(i=0;i<n;i++)
	teka[i]=(float *)malloc(sizeof(float)*n);
	teka_f=(float *)malloc(sizeof(float)*n);
	
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	teka[i][j]=matrix[i][j];
	
	i=j=0;
	for(i=0;i<n;i++)
	teka_f[i]=f[i];

//--------CONVERTING MATRIX INTO UPPER TRANGLE FORM--	
	current(matrix,f,n);
	putchar('\n');
	for (i=0;i<n;i++)
	{
	if(!straighten(matrix,f,n,i)) exit(0);
	current(matrix,f,n);
	zerofy(matrix,f, n,i);
	current(matrix,f,n);
	}

//--------COMPUTING UNKOWN XS' VALUES--------------- 	
	x[n-1]=f[n-1];
	for(i=n-2;i>=0;i--)
	x[i]=f[i]-iSUM(matrix,x,n,i);

//--------PRINTING RESULTS--------------------------	
	printf("\nResult----------------------------\n|");
	for(i=0;i<n;i++)
	printf(" x%i=%g |",i+1,x[i]);
	putchar('\n');

//--------CHECKING ANSWERS with GIVEN LINEAR FUNCTION VALUES----
	teka_result(teka,x,teka_f,n);
	//1-array pointer,2-x1.. array,3-func. given values,4-array size
	//prints far example:
	//   7*0.2+8*6.1=50.2   diff=0.005

//--------FLUSHING MEMORY----------------------------------
	free(teka);
	free(teka_f);
		
	return 0;
	}


int straighten(float **mat ,float *f,int n,int i)
{///mat - matrix,f - func. values,n - dimension,i - current line number
	int isZero,t,j;
	float *nonzero,h;

	if(mat[i][i]==0)
	{
	isZero = 1;
	for(t=i+1;t<n;t++)
		if (mat[t][i]!=0)
		{	
			if (mat[t][i]==1){j=t;isZero = 0;break;}
			else{j = t;isZero = 0;break;}
		}
	
	if (isZero)
	{
		fprintf(stderr,"Error:det|A| mustn't be zero\n ");
		return -1;
	}

	nonzero=mat[i];
	mat[i]=mat[j];
	mat[j]=nonzero;
	
	h=f[i];
	f[i]=f[j];
	f[j]=h;
	
	}
	
	if (mat[i][i]==1) return 1;
	else
	{
		float factor=mat[i][i];
		for(t=i;t<n;t++)
		mat[i][t]/=factor;  
		f[i]/=factor;
	}
		
        return 1;
}


int zerofy(float **mat ,float *f,int n,int cur)
{
	int j,i;
	float multi;
	for(i=cur+1;i<n;i++)
	{
	multi=mat[i][cur];
	if(multi==0)continue;
	for(j=0;j<n;j++) mat[i][j] -= mat[cur][j]*multi;
	f[i] -= f[cur]*multi;
	}
	
	return 0;
}


float iSUM(float **mat,float *x,int n,int i)
{
	int j;
	float sumk=0 ;
	for(j=i+1;j<n;j++)
	sumk+=mat[i][j]*x[j];
	return sumk;
}
	

void current(float **mat ,float *f,int n)
{
	int i,j;
	printf("----------Current position----------------\n");
	for(i=0;i<n;i++)
	{
	printf("          ");
	//if (mat[i][0])
	for(j=0;j<n;j++)
	{
	if ((j>0)&&(mat[i][j]>0))
	printf("+");
	if(mat[i][j]==1)
	printf("x%i",j+1);
	else if (mat[i][j]!=0)
	printf("%gx%i",mat[i][j],j+1);
	else printf("   ");
	}
	printf(" = %g\n",f[i]);
	}
}


void teka_result(float **mat ,float *x,float *f,int n)
{
	int i,j;
	float sum=0,modul;
	printf("----------CHEKING ANSWERS----------------\n");
	for(i=0;i<n;i++)
	{sum=0;
	printf("          ");
	for(j=0;j<n;j++)
	{
	if ((j>0)&&(mat[i][j]>=0))
	printf("+");
	printf("%g*%g",mat[i][j],x[j]);
	sum+=mat[i][j]*x[j];
	}
	modul=abs(f[i]-sum);
	printf(" = %g     diff = %g\n",sum,modul);
	}
}




