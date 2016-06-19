#ifndef aljabr_h
#define aljabr_h


#define MSG(x)   fprintf(stdout, "%s",x)
#define ERROR(x) fprintf(stderr, "%s: %i: error: %s", __FILE__ , __LINE__ , x)
#define FUNC_NUM 5
#define PI 3.14159

namespace FJ
{
enum { XQUADRAT = 0 , SINX , COSX , NATURAL_LOGX , FROM_FILE ,DLL_FUNCTION };

enum	{
	IntervalBegin = 0x0001,
	IntervalEnd   = 0x0002,
	InputFile     = 0x0004,
	NoGui         = 0x0008,
	PointNum      = 0x0010,
	FuncType      = 0x0020,
	LinearMethod  = 0x0040,
	ParabolicMethod=0x0080,
	Plugin	      = 0x0100
	};

enum	{
	Version = 0x0020,
	Help    = 0x0040,
	QtHelp  = 0x0080,
	Hint    = 0x0100
	};

template <class T>
T abs(T val1)
{
    return val1 >= 0 ? val1 : val1 * (T) -1;
};

}

#endif 
