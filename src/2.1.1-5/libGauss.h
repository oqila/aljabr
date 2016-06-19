#ifndef LIB_GAUSS_H
#define LIB_GAUSS_H

//-----------PREPROCESSOR MACROS------------------
#include <stdio.h>
#include <stdlib.h>


//----------SUBROUTINE PROTYPES-------------------
void current(float ** , float *,int);
void teka_result(float **,float *,float *,int);
int straighten(float ** ,float * ,int ,int );
int zerofy(float ** ,float *,int ,int );
float iSUM(float ** ,float *,int ,int );

int FGauss(float **,int ,float *,float *);

#endif

