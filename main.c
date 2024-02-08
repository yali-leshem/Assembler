
/* This Project is belong to Itay Mizrachi and Yali Leshem. The date: 31.3.2023 */
#include "secondPass.h"
#include "preAssembler.h"

int main (int argc, char **argv) 
{
    data ptSymbol = NULL; /* This pointer is for saving the list of all symbols */
    int index;
	int flags[FLAGS] = {NOP, NOP, NOP, NOP, NOP};
	/* The 'flags' is for 5 different flags (errors, *IC, *DC, external & enternal) */	
    if (argc < 2) 
	{ /* If needed more arguements to be typed in terminal */
        printf("Error! too little arguements have been typed in the command line \n");
        return FALSE;
    }
    for (index = 1; index < argc; index++) /* continuing on all given arguments in the command line */
    {
	char * fullFileName; /* the name of the file with thwe ending as that being sent to the preAssembler */
	int size;
	fullFileName = (char *) malloc(sizeof(char)* strlen(argv[index]));
	strcpy(fullFileName,argv[index]);
	size = strlen(fullFileName);
	fullFileName = (char * ) realloc(fullFileName,sizeof(char) * (strlen(fullFileName)+4) );
	fullFileName[size ] = '.'; /* adding the ending */
	fullFileName[size +1] = 'a';
	fullFileName[size +2] = 's';
	fullFileName[size +3] = '\0';
    	preAssemblerPass(fullFileName); /* sending the full name to the preAssembler */
		/* all macros are out of file as pre-Assembler is done here */
    	ptSymbol = mainFirstPass(argv[index], flags); /* checking the validation of the file and create a list storaging symbols*/
    	if (flags[err] == ABLED)
    		continue;
    	secondPassMain(argv[index], &ptSymbol,flags); /* when the file is valid, encoding it in a unique binary base */
    	freelist(ptSymbol);
    }
    return FALSE;
}
