/***************************************************************************
*                        Al-Jabr 
*
*   File    : Al-Jabr-main.cpp
*   Purpose : 2D and 3G software for cross-platform use(Linux, Windows XP, Mac OS)
*   Author  : Fazliddin Jo'rayev
*   Date    : November 15, 2009
*
*
*   Copyright (C) 2009 by  Fazliddin Jo'rayev (fazliddin.uzbek@yahoo.com)
*
* 
*
* Al-Jabr is free software WITHOUT ANY WARRANTY as long as you do not remove 
* author's name from the file; you can redistribute it and/or modify it under the 
* afore-mentioned terms. Unless you have purchased license, DO NOT REMOVE author's 
* author's name from the file
*
***************************************************************************/






#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aljabr.h"
#include "Al-Jabr-main.h"
#include "Al-Jabr-Qt4.h"
#include "FGraph2D-Qt4.h"
#include "Data.h"
 
int main(int argc, char *argv[])
	{

	float interval_a,interval_b,*virx;	// begining  and ending point of the interval
	float *x,*f=x=NULL,**a=NULL,*ksi = NULL;// 2-dimensional array for saving interpolating
						// fuctions' coefficient
	float x0=1;
	int n=0,i,ftype=0;				 
	char * filename=NULL;
	int bitmap=0;				// holds info about user input as bits

	int (*interpolate)(float & ,float, float, int, float **,float *);	
	int (*FindXY[])(float *, float * , float & ,float & , int &, char *,int ) = {
	
	// fuctions to evaluate x and f arrays	

	default_XQuadrat,
	default_SinX,
	default_CosX,
	default_Natural_LogX,
	read_from_file
	};

	float (*func[])(float) = {
	
	// List of project's inner functions that will be used by default	

	XQuadrat,
	SinX,
	CosX,
	LogX
	};

	
	
	filename = (char * ) malloc (sizeof(char[20]));
	if( filename==NULL )
		{ ERROR("Can not allocate enough space for variables\n");exit(-1);}
	
	bitmap=cmd_args_support(argc , argv , filename , interval_a , interval_b ,n,ftype);
	if(bitmap<0)exit(-1);			//if there occured syntax error in cmd args format
	MSG("Processing command line arguments...\n");


//////////////////
//////////////////

	QApplication app(argc,argv);
	Aljabr * window = 0;
	Data * data = 0;
	QSplashScreen *splash = 0;

	if(! (bitmap & FJ::NoGui) )
	{
	
	app.setFont(QFont("sans",11));

	splash = new QSplashScreen;

	splash->setPixmap(QPixmap(":/images/al-jabr-logo.png"));
	splash->show();

	Qt::Alignment bottomLeft = Qt::AlignLeft | Qt::AlignBottom;
	splash->showMessage(QObject::tr("Loading modules..."),bottomLeft, Qt::black);

	}

//////////////////
//////////////////
	
	
	MSG("Loading variables...\n");
	
	if(! (bitmap & FJ::PointNum) )n=15;		//if n was not passed by cmd line
	
	x = (float *) malloc ( sizeof ( float)* (n+2) );
	f = (float *) malloc ( sizeof ( float)* (n+2) );
	if( x==NULL || f==NULL )
	{ ERROR("Can not allocate enough space for variables\n");exit(-1);}	
	
	a = (float **) malloc (sizeof (float )* (n+1));
	if(a==NULL){ERROR("Can not allocate enough space for variables\n");exit(-1);}
	
	for(i=0;i<n+1;++i) 
		{
		a[i] = (float *) malloc (sizeof (float [2]));
		if(a[i]==NULL)
			{ERROR("Can not allocate enough space for variables\n");exit(-1);};
		}

///////////////	Qt
///////////////	
///////////////
///////////////



	if(bitmap & FJ::InputFile)
	{
	MSG("Reading data from the input file...\n");

	if(FindXY[FJ::FROM_FILE](x,f,interval_a,interval_b,n,filename,bitmap)<1)
			{ERROR("Input file is corrupted or has incorrect format\n");exit(-1);}
	}
	else
	{
	MSG("Generating data from the default inner functions...\n");

	if(FindXY[ftype](x,f,interval_a,interval_b,n,NULL,bitmap)<0)exit(-1);
	}


	MSG("Building interpolation function...\n");

	if(bitmap & FJ::LinearMethod)
	{
		if(build_piecewise_linear_interpolation(n,x,f,a)<0)exit(-1);
		interpolate = find_piecewise_linear_interpolation_value;
		virx = x;	
	}
	else
	{
		ksi = (float *) malloc ( sizeof ( float)* (n) );
		if(build_parabolic_spline_interpolation(n,x,f,a,ksi)<0)exit(-1);	
		interpolate = find_parabolic_spline_interpolation_value;
		virx = ksi;
	}

	if(bitmap & FJ::NoGui)
	{
		while(1)
		{
			MSG("Enter any arbitrary point that lies in the interval: ");
			scanf("%f",&x0);
			if(x0 == 0)break;
			if(interpolate(x0,interval_a,interval_b,n,a,virx))fprintf(stdout,"%f\n",x0);
		}
	MSG("You pressed 0 that means end of loop\n");
	}
		

printf("intervals : [ %f , %f ]\n", interval_a , interval_b );	

	// Debug output
	printf("N = %i\n",n);
	for (i=0;i<n;i++) printf("x = %f : f(x) =  %f\n", x[i],f[i] );
	printf("intervals : [ %f , %f ]\n", interval_a , interval_b );	
	


	//free(x);
	//free(f);
	//free(a);
	//free(filename);
//////////////////
//////////////////

	
	 if(! (bitmap & FJ::NoGui) )
	{
		data = new Data;
		data->begin=interval_a;
		data->end=interval_b;
		data->x=x;
		data->f=f;
		data->N=n;
		data->a=a;
		data->ksi=ksi;
		data->virtx = virx;
		data->bitmap=bitmap;
		data->func=func[ftype];
		data->interpolate = interpolate;
	
		window = new Aljabr(data);
		window->show();
		splash->finish(window);
		delete splash;
		return app.exec();
	}
	
	
	return 0;
	}

