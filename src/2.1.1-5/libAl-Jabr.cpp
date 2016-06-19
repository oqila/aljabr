

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "aljabr.h"
#include "libCmdParser.h"
#include "libAl-Jabr.h"


// Function as its first two args takes argc and argv which were sent by stack to the
// main programme and analyses cmd options. FORMAT: [ -f FILENAME ] , [ --from num ], [ --to num ]
// [ -n POINT_AMOUNT ] | [ --number-of-points POINT_AMOUNT ] . After the analyse it assigns
// properly converted args to the provided parameters as result.
// 3 - input file name
// 4 - begining of the interval
// 5 - ending of the interval
// 6 - number of points
//
// Function returns -1 if there occured syntax error in cmd args format
// or returns bitmap representing read argments as 1 bit

int cmd_args_support(int argc, char **argv, char *filename, float &a , float &b , int &n,int & type )
	{
	int bitmap=0;
	
	if(argc<2)
	{bitmap|= FJ::LinearMethod;return bitmap;}

	char * ftype = NULL;
	OptionList * optlist, * temp=NULL;
	OptionError  opterror;
	OptionPattern  optpattern[]={
					{1, "-f",	"",		REQUIRE_ARGUMENT},
					{2,  "",	"--from",	REQUIRE_ARGUMENT},
					{3,  "",	"--to",		REQUIRE_ARGUMENT},
					{4,  "-n",	"--number-of-points",	REQUIRE_ARGUMENT},
					{5,  "",	"--no-gui",	NO_ARGUMENT},
					{6,  "-h",	"--help",	NO_ARGUMENT},
					{7,  "",	"--help-qt",	NO_ARGUMENT},
					{8,  "-v",	"",		NO_ARGUMENT},
					{9,  "",	"",		FREE_ARGUMENT},
					{10, "-t",	"--func-type",	REQUIRE_ARGUMENT},
					{11, "-l",	"--linear",	ALLOW_ARGUMENT},
					{12, "-p",	"--parabolic",	ALLOW_ARGUMENT},
					{13, "",	"-style",	REQUIRE_ARGUMENT}
				      };
	

	optlist=Process_Cmd_Option_List(argc, argv, optpattern,13, opterror);

	if (optlist==NULL)
	{
		ERROR("");
		if(opterror.status == NO_ARGUMENT_FOUND)
		{
			printf("Option `%s' requires argument\n",argv[opterror.Index]);
		}
		else
		{
			if(opterror.status == UNKNOWN_OPTION)printf("Unknown option `%s'\n",argv[opterror.Index]);
		}
		info(FJ::Hint);
		return -1;
	}

	temp = optlist;
	
	

	while (temp)
		{
		
			switch(temp->optId)
			{
			case 1: bitmap|= FJ::InputFile; strcpy(filename,temp->argument); break;
			case 2: bitmap|= FJ::IntervalBegin; a=atof(isNeg(temp->argument));break;
			case 3: bitmap|= FJ::IntervalEnd; b=atof(isNeg(temp->argument));break;
			case 4: bitmap|= FJ::PointNum; n=atoi(temp->argument);break;
			case 5: bitmap|= FJ::NoGui;break;
			case 6: info(FJ::Help); Free_Cmd_Option_List(temp);return -1;break;
			case 7: info(FJ::QtHelp); Free_Cmd_Option_List(temp);return -1;break;
			case 8: info(FJ::Version); Free_Cmd_Option_List(temp);return -1;break;
			case 9: bitmap|= FJ::InputFile; strcpy(filename,temp->argument);break;
			case 10: bitmap|= FJ::FuncType; ftype=temp->argument;break;
			case 11: bitmap|= FJ::LinearMethod;break;
			case 12: bitmap|= FJ::ParabolicMethod;break;
			default :break;
			}
			temp=temp->next; 
		}
	
	if( (bitmap & FJ::LinearMethod) && (bitmap & FJ::ParabolicMethod) )
	{
	ERROR("Options -l,--linear and \n-p,--parabolic conflict with each other\n");
	info(FJ::Hint);
	return -1;
	}	
	
	if(!(bitmap & FJ::LinearMethod) && !(bitmap & FJ::ParabolicMethod) )bitmap|= FJ::LinearMethod; // by default


	if( (bitmap & FJ::InputFile) && (bitmap & FJ::FuncType) )
	{
	ERROR("Options -t,--func-type and -f or filename conflict with each other\n");
	info(FJ::Hint);
	return -1;
	}	
	


	if(bitmap & FJ::FuncType)
	{
		if(!strcmp(ftype,"cos"))type=FJ::COSX;
		else if	(!strcmp(ftype,"sin"))type=FJ::SINX;
		else if	(!strcmp(ftype,"log"))type=FJ::NATURAL_LOGX;
		else if	(!strcmp(ftype,"quad"))type=FJ::XQUADRAT;
		else {ERROR("");printf("Incorrect value `%s'\n",ftype);info(FJ::Hint);return -1;}
	}
	else type=FJ::XQUADRAT;  // by default





	Free_Cmd_Option_List(optlist);
	return bitmap;
	}




void info(int i)
{
	switch(i)
		{
	case FJ::Version : 
MSG("Al-Jabr 2-2.5 \n\
Copyright (C) 2009 Fazliddin Jurayev, FAZLI Corp LTD.\n\
License Non-free <http://fnok.ueuo.com>\n\
This software uses Qt4 as GUI. You can use free version\n\
with NO WARRANTY, to the extent permitted by law.\n") ;break;
	case FJ::Help    :
MSG("\nNAME\
\n\n    aljabr - Al-Jabr\
\n\nSYNOPSIS\
\n    aljabr [Qt-options] [option] [filename]\
\n\n    aljabr [ -f filename ] [ --from interval_a ] [ --to interval_b ]\
\n           [ -n point_num] | [ --number-of-points point_num ] [--no-gui]\
\n	     [ -t FUNCTION_NAME ] [ --func-type FUNCTION_NAME ] \
\n	     [ -p ] [ --parabolic ] [ -l ] [ --linear ]\
\n           [--help] [--qt-help] [-v]    [filename]\
\n\nDESCRIPTION\
\n     Al-Jabr is cross-platform sortware for graphical analyses of the\
\n     function by piecewise linear and parabolic spline interpolation method.\
\n\nQt options:\
\n\n	-style [windows | motif | plastique | cde]\n\
        	Specifies main window style. By default\n\
		your sistem's style is used.\
\n\nOPTIONS\
\n\n	-h,--help	Show help about options.\
\n\n	--help-qt	Show Qt specific options.\
\n\n	-v		Prints version\
\n\n	--no-gui	Run in comman line mode . Don’t load GUI.\
\n\n	-n point_num,\
\n	--number-of-points point_num\
\n			Number of the points in the interval. This option has less\
\n			affect, if you give input file. Because in this case point_num\
\n			argument will be changed to the number of really read points.\
\n\n	--from interval_a\
\n			Beginning a value of the interval [a,b]. If this option is not\
\n			given this argument is set to x1.\
\n\n	--to interval_b\
\n			Ending b value of the interval [a,b]. If this option is not\
\n			given this argument is set to xn.\
\n\n	-t,--func-type [cos | sin | quad | log]\
\n			Default function name. By defaull it is quad(parabola). If no\
\n			input file name is given inner function will be used.\
\n			Conflicts with -f option\
\n\n	-f filename\
\n			Input file name from where points are read. If this option is\
\n			not given default inner function will be used.\
\n			Conflicts with -t,--func-type option\
\n\n	-p,--parabolic\
\n			Use parabolic spline method.\
\n			Conflicts with -l,--linear option\
\n\n	-l,--linear\
\n			Use linear method.\
\n			Conflicts with -p,--parabolic option\
\n\n\
AUTHOR\
\n       Al-Jabr is being written and maintained by Jo\'rayev Fazliddin <fazliddin.uzbek@yahoo.com>.\
\n\n");break;
	case FJ::QtHelp  :
MSG("\nAl-Jabr\n\n\
Qt options:\
	-style [windows | motif | plastique | cde]\n\
        	Specifies main window style. By default\n\
		your sistem's style is used.\n\n");break;
	case FJ::Hint    :MSG("Use `aljabr --help' for a complete list of options.\n");break;
		};
};



// Function reads data (x1,x2... y1,y2...) from the file and sorts them in increasing order, 1- and 2-
// args are pointers to the memory where data will be written to . 3 arg donates file name as string. 
// if interval values are illogical then x1 and xn will be set as interval limits. Function returns 
// number of points read. if the file is empty 0 is returned and if file doesnʻt exist -1 is returned. 

int read_from_file(float * x, float * f, float &interval_a ,float &interval_b ,int &n, char * filename, int bitmap)
	{
	
	FILE *fp;
	fp=fopen(filename,"r");
	if(!fp)
		{ERROR("Can't open thd file for reading \n");return -1;}
	int i=-1;
	
	fscanf(fp,"%i",&n);

	while(!feof(fp))
		{
		i++;
		fscanf(fp,"%f %f",(x+i),(f+i));

		}	
	n=i;

	sort(x,f,n);
	if (! (bitmap & FJ::IntervalBegin)){interval_a=x[0];}
	if (! (bitmap & FJ::IntervalEnd))  {interval_b=x[n-1];}

	if ( x[0] < interval_a || interval_b < x[n-1]) {ERROR("Illogical interval limits \n");return -1;}
	if(!(interval_a < interval_b)) {ERROR("Illogical interval limits \n");return -1;} 
	fclose(fp);

	return (n>0) ?  n  : 0 ; //if file is empty
	}


char * isNeg(char * str)
{
if (str[0] == '&')return &str[1];
return str;
}


// sorts x array and applies all replacements to f array too

void sort(float * x, float * f , int n)
	{
	int i,j;
	float temp;
	for(i=1;i<n;i++)
		for(j=0;j<i;j++)
			{
			if (x[j]>x[i])
				{
				temp=x[j];x[j]=x[i];x[i]=temp;
				temp=f[j];f[j]=f[i];f[i]=temp;
				}
			}
	}




// If no input file is given we find x and f array values by default functions
// Here are some of those

int default_XQuadrat(float * x, float * f, float &interval_a ,float &interval_b ,int &n, char * filename, int bitmap)
	{
	float step,x0;
	int i=0;
	
	if (! ((bitmap & FJ::IntervalBegin) || (bitmap & FJ::IntervalEnd)) ){interval_a=-1;interval_b=4;}
	if (! (bitmap & FJ::IntervalEnd) )interval_b=interval_a+5;
	if (! (bitmap & FJ::IntervalBegin) )interval_a=interval_b-5;
	if(interval_a >= interval_b) {ERROR("Illogical interval limits \n");return -1;} 
	
	step = (interval_b - interval_a) / (n-1);

	printf("STEP= %f:\n",step);
	x0=interval_a;
	while(i<n)
		{
		f[i]=XQuadrat(x0);//x0*x0-2*x0+1;
		x[i]=x0;	
		x0+=step;
		i++;
		}	
	
	return 1;
	}


int default_SinX(float * x, float * f, float &interval_a ,float &interval_b ,int &n, char * filename, int bitmap )
{
	float step,x0;
	int i=0;
	
	if (! ((bitmap & FJ::IntervalBegin) || (bitmap & FJ::IntervalEnd)) ){interval_a=-90;interval_b=90;}
	if (! (bitmap & FJ::IntervalEnd) )interval_b=interval_a+180;
	if (! (bitmap & FJ::IntervalBegin) )interval_a=interval_b-180;
	if(interval_a >= interval_b) {ERROR("Illogical interval limits \n");return -1;} 
	
	step = (interval_b - interval_a) / (n-1);

	printf("STEP= %f:\n",step);
	x0=interval_a;
	while(i<n)
		{
		f[i]=SinX(x0);
		x[i]=x0;	
		x0+=step;
		i++;
		}	
	
	return 1;
};

int default_CosX(float * x, float * f, float &interval_a ,float &interval_b ,int &n, char * filename, int bitmap)
{
	float step,x0;
	int i=0;
	
	if (! ((bitmap & FJ::IntervalBegin) || (bitmap & FJ::IntervalEnd)) ){interval_a=-90;interval_b=90;}
	if (! (bitmap & FJ::IntervalEnd) )interval_b=interval_a+180;
	if (! (bitmap & FJ::IntervalBegin) )interval_a=interval_b-180;
	if(interval_a >= interval_b) {ERROR("Illogical interval limits \n");return -1;} 
	
	step = (interval_b - interval_a) / (n-1);

	printf("STEP= %f:\n",step);
	x0=interval_a;
	while(i<n)
		{
		f[i]=CosX(x0);
		x[i]=x0;	
		x0+=step;
		i++;
		}	
	
	return 1;
};

int default_Natural_LogX(float * x, float * f, float &interval_a ,float &interval_b ,int &n, char * filename, int bitmap)
{
	float step,x0;
	int i=0;
	
	if (! ((bitmap & FJ::IntervalBegin) || (bitmap & FJ::IntervalEnd)) ){interval_a=0.1;interval_b=16;}
	if (! (bitmap & FJ::IntervalEnd) )interval_b=interval_a+16;
	if (! (bitmap & FJ::IntervalBegin) )interval_a=interval_b-16;
	if(interval_a >= interval_b) {ERROR("Illogical interval limits \n");return -1;} 
	
	step = (interval_b - interval_a) / (n-1);

	printf("STEP= %f:\n",step);
	x0=interval_a;
	while(i<n)
		{
		f[i]= LogX(x0);
		x[i]=x0;	
		x0+=step;
		i++;
		}	
	
	

	return 1;
};





