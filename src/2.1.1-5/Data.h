
#ifndef DATA_H
#define DATA_H


struct Data
{
	float	begin;
	float	end;
	float	* x;
	float	* f;
	float	**a;
	float	* ksi;
	float  * virtx;
	  int	N;
	  int	bitmap;
	  int	(*interpolate)(float & ,float, float, int, float **,float *);	
	float	(* func)(float);
};

#endif

