#include "aljabr.h"
#include "Al-Jabr-plugin.h"

#include <stdio.h>
#include <dlfcn.h>


int dll_function(float *x,
		 float *f,
		 float &interval_a,
		 float &interval_b,
		 int &n,
		 char *filename,
		 int bitmap,
		 char *pfunc_name,
		pf *pfunc	
		 )
{
	void *handle;
	pf temp;
	float step,x0;
	int i=0;

	handle = dlopen (filename, RTLD_NOW);
	if (handle == NULL) {
		
		printf ("%s\n", dlerror ());
		return -3;
	}

	*pfunc  = (float (*)(float) ) dlsym (handle, pfunc_name);
	if (*pfunc == NULL) {
		
		printf ("%s\n", dlerror ());
		dlclose (handle);
		return -4;
	}
	temp = *pfunc;

	if (! ((bitmap & FJ::IntervalBegin) || (bitmap & FJ::IntervalEnd)) ){interval_a=0;interval_b=15;}
	if (! (bitmap & FJ::IntervalEnd) )interval_b=interval_a+15;
	if (! (bitmap & FJ::IntervalBegin) )interval_a=interval_b-15;
	if(interval_a >= interval_b) {ERROR("Illogical interval limits \n");return -1;} 
	
	step = (interval_b - interval_a) / (n-1);

	printf("STEP= %f:\n",step);
	x0=interval_a;
	while(i<n)
		{
		f[i] = temp(x0);
		x[i] = x0;	
		x0 += step;
		i++;
		}	
	
	

	return 1;
};

