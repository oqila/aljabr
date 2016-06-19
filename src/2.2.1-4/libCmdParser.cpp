/***************************************************************************
*                       Library functions for Al-Jabr project
*
*   File    : libCmdParser.c
*   Purpose : Parsing command line options for cross-platform use(Linux, Windows XP, Mac OS)
*   Author  : Fazliddin Jo'rayev
*   Date    : November 15, 2009
*
*
*   Copyright (C) 2009 by  Fazliddin Jo'rayev (fazliddin.uzbek@yahoo.com)
*
* 
*
* libCmdParser.c is free software WITHOUT ANY WARRANTY as long as you do not remove 
* author's name from the file; you can redistribute it and/or modify it under the 
* afore-mentioned terms. Unless you have purchased license, DO NOT REMOVE author's 
* author's name from the file
*
***************************************************************************/













/***************************************************************************
*                             PREPROCESSORS
***************************************************************************/
#include "libCmdParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>















/****************************************************************************
*   Function   : Process_Cmd_Option_List
*   Description: This function is similar to the POSIX function getopt.  All
*                options and their corresponding arguments are returned in a
*                linked list.  This function should only be called once per
*                an option list and it does not modify argv or argc.
*   Parameters : argc - the number of command line arguments (including the
*                       name of the executable)
*                argv - pointer to the open binary file to write encoded
*                       output
*                OptPattern - An array of OptionPattern type items that
*			      hold option short and long name and other 
*			      requirements
*		 size - number of options, that is, size of OptPattern array
* 		 error_opt - stucture through which error infomation is passed		
*   Effects    : Creates a link list of command line options and their
*                arguments.
*   Returned   : OptionList type pointer value where the option and arguement fields
*                contain the next option symbol and its argument (if any).
*                The argument field will be set to NULL if the option is
*                specified as having no arguments or no arguments are found.
*                Null pointer is returned if there occured any error. For example,
*		 no argument is given to the option that is said to argument, or unknown
*		 option is encountered.		
*
*   NOTE: The caller is responsible for freeing up the option list when it
*         is no longer needed.
****************************************************************************/
OptionList *Process_Cmd_Option_List(const int argc, char *const argv[], OptionPattern  *  OptPattern, int size, OptionError & error_opt)
{
	int argvIndex;
	OptionList *head, *tail;
	
	int OptPatternIndex=0;
	int OptPatternSize=size;
	int found = 0; 	
	char * argument_addr; const char *  emp="";
	int argument_index,optId,option_index = 0,isFreeArg=0;
  

	argvIndex = 1;
	head = NULL;
	tail = NULL;

	/*we look for item that have FREE_ARGUMENT field */
	while (OptPatternIndex < OptPatternSize) 
        {
		if( !( strcmp(OptPattern[OptPatternIndex].short_option,emp) ) &&
		    !(strcmp(OptPattern[OptPatternIndex].long_option,emp))  &&
		    (OptPattern[OptPatternIndex].option_type==FREE_ARGUMENT) )	
		{
			/*if there are any we mark it and remove the item*/
			isFreeArg=OptPattern[OptPatternIndex].optId;
			break;
		}
		OptPatternIndex++;
	}



	/* loop through all of the command line arguments */
	while (argvIndex < argc)
	{
	
	
        	/* possible option */
		OptPatternIndex = 0;
         
		/* attempt to find a matching option */
		while (OptPatternIndex < OptPatternSize) 
		{
			if( (!strcmp(OptPattern[OptPatternIndex].short_option,argv[argvIndex])) ||
			    (!strcmp(OptPattern[OptPatternIndex].long_option,argv[argvIndex])) ){  found=1; break;  }
		
			 OptPatternIndex++;
                
            	}
		

		if (found)
		{/* we found the matching option */

			switch(OptPattern[OptPatternIndex].option_type)	
			{
				
				case NO_ARGUMENT: argument_addr=NULL; argument_index=NO_INDEX;option_index=argvIndex; break;
				
				case REQUIRE_ARGUMENT: 
					 if ((argvIndex+1<argc)&&(argv[argvIndex+1][0]!='-'))
						{ argument_addr=*(argv+argvIndex+1);argument_index=argvIndex+1;option_index=argvIndex;argvIndex++;}
					 else {
						if (head!=NULL)Free_Cmd_Option_List(head);
						error_opt.optId=OptPattern[OptPatternIndex].optId;
						error_opt.status=NO_ARGUMENT_FOUND;
						error_opt.Index=argvIndex;
						return NULL;
					      }
							break;

				case ALLOW_ARGUMENT:
					if ((argvIndex+1<argc)&&(argv[argvIndex+1][0]!='-')&& (!isFreeArg))
		{ argument_addr=*(argv+argvIndex+1);argument_index=argvIndex+1;option_index=NO_INDEX;argvIndex++;}
					else { argument_addr=NULL;argument_index=NO_INDEX;option_index=argvIndex;}
					
							break;

				default : argument_addr=NULL;argument_index=NO_INDEX;break;
				
			}
		optId=OptPattern[OptPatternIndex].optId;
		

		}
		else
		{	/* we didn't find the matching option */
			if(isFreeArg && (argv[argvIndex][0]!='-') )
		{	argument_addr=*(argv+argvIndex);argument_index=argvIndex;optId=isFreeArg;option_index=NO_INDEX;isFreeArg=0; }
			else
			{
				if(argv[argvIndex][0]=='-')
				{
					if (head!=NULL)Free_Cmd_Option_List(head);
					error_opt.optId=UNKNOWN_ID;
					error_opt.Index=argvIndex;
					error_opt.status=UNKNOWN_OPTION;
					return NULL;
				}
				else
				{
				argument_addr=*(argv+argvIndex);argument_index=argvIndex;optId=UNKNOWN_ID;option_index=NO_INDEX;
				}
			}

		
		
		}



		if (NULL == head)
		{
			head = MakeOpt( optId, option_index, argument_index, argument_addr );
			tail = head;
		}
		else
		{
			tail->next =MakeOpt( optId, option_index, argument_index, argument_addr );
			tail = tail->next;
		}

			
                 
	
	argvIndex++;
	found=0;
	}

return head;
}
















/****************************************************************************
*   Function   : MakeOpt
*   Description: This function uses malloc to allocate space for an option_t
*                type structure and initailizes the structure with the
*                values passed as a parameter.
*   Parameters : option - this option character
*                argument - pointer string containg the argument for option.
*                           Use NULL for no argument
*                index - argv[index] contains argument us OL_NOINDEX for
*                        no argument
*   Effects    : A new option_t type variable is created on the heap.
*   Returned   : Pointer to newly created and initialized option_t type
*                structure.  NULL if space for structure can't be allocated.
****************************************************************************/
OptionList *MakeOpt(int optId, int optIndex, int argIndex, char * argument)
{
    OptionList *opt;

    opt =(OptionList *) malloc(sizeof(OptionList));

    if (opt != NULL)
    {
        opt->optId = optId;
	opt->optIndex = optIndex;
        opt->argIndex = argIndex;
	opt->argument = argument;
       
        opt->next = NULL;
    }
    else
    {
        err("Failed to Allocate OptionList");
    }

    return opt;
}

/****************************************************************************
*   Function   : Free_Cmd_Option_List
*   Description: This function will free all the elements in an option_t
*                type linked list starting from the node passed as a
*                parameter.
*   Parameters : list - head of linked list to be freed
*   Effects    : All elements of the linked list pointed to by list will
*                be freed and list will be set to NULL.
*   Returned   : None
****************************************************************************/
void Free_Cmd_Option_List(OptionList *list)
{
    OptionList *head, *next;

    head = list;
    list = NULL;

    while (head != NULL)
    {
        next = head->next;
        free(head);
        head = next;
    }

    return;
}

