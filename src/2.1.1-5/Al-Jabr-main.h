#ifndef __AL_JABR_MAIN
#define __AL_JABR_MAIN






// Function that compute or read from given file x_i and f_i values

extern int  read_from_file	(float *, float * ,float & ,float & , int & , char * ,int);
extern int  default_XQuadrat	(float *, float * ,float & ,float & , int & , char * ,int);
extern int  default_SinX	(float *, float * ,float & ,float & , int & , char * ,int);
extern int  default_CosX	(float *, float * ,float & ,float & , int & , char * ,int);
extern int  default_Natural_LogX(float *, float * ,float & ,float & , int & , char * ,int);



extern int cmd_args_support(int, char **, char *, float & , float & , int & ,int &);

extern int build_piecewise_linear_interpolation(int ,float *,float * ,float **);

extern int find_piecewise_linear_interpolation_value(float & ,float ,float  ,int ,float **,float * );

extern int build_parabolic_spline_interpolation(int ,float *,float *,float **,float *); 

extern int find_parabolic_spline_interpolation_value(float &,float ,float ,int ,float **,float *);






#endif

