#ifndef libCMD_H
#define libCMD_H


#define    NO_INDEX		-1
#define    UNKNOWN_ID		-2   
    
#define err(x) fprintf(stderr, "ERROR: %d: %s: %s\n", __LINE__, __FILE__,x)

typedef enum option_t {NO_ARGUMENT,REQUIRE_ARGUMENT,ALLOW_ARGUMENT,FREE_ARGUMENT} option_t;

typedef enum error_t  {UNKNOWN_OPTION=-10,NO_ARGUMENT_FOUND} error_t;



typedef struct option_pattern		// structure that is passed to Process_Cmd_Option_List
{
	int optId;
	const char *  short_option;
	const char *  long_option;
   	option_t option_type; 
} OptionPattern;



typedef struct option_list		// structure that is returned by Process_Cmd_Option_List
{
	int optId;
	int optIndex;
	int argIndex;
	char *argument;
	    
	struct option_list *next;
} OptionList;



typedef struct option_error		// structure that is passed to Process_Cmd_Option_List to get infomation about error
{
	int optId;
	int Index;
	error_t status;
} OptionError;



OptionList *Process_Cmd_Option_List(int argc, char *const argv[], OptionPattern * ,int, OptionError & );

void Free_Cmd_Option_List(OptionList *);

OptionList *MakeOpt( int ,int ,int , char * );

#endif  


