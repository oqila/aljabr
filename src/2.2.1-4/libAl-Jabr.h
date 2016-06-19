#ifndef libAl_H
#define libAl_H

void	sort		 (float *, float * , int );
int	read_from_file	 (float * , float * , float & ,float & ,int &, char *, int, char * );
int	cmd_args_support (int , char **, char *, float & , float & , int &, int&);
void	info		 (int);
int	default_XQuadrat (float *, float * ,float & ,float & , int & , char * ,int);
int	default_SinX	 (float *, float * ,float & ,float & , int & , char * , int );
int	default_CosX	 (float *, float * ,float & ,float & , int & , char * , int );
int	default_Natural_LogX(float *, float * ,float & ,float & , int & , char * , int );
char*	isNeg(char *);

extern float	XQuadrat(float);
extern float	CosX(float);
extern float	SinX(float);
extern float	LogX(float);

#endif
