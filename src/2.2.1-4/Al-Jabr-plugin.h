#ifndef PLUGIN
#define PLUGIN

typedef float (*pf) (float);

int dll_function(float *, float *, float &, float &,int &, char *, int, char *,pf *);

#endif

