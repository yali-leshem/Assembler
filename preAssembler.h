#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct strNode * strNodeP;
typedef struct strNode{ /* data struct that presents a linek-list that holds two field that one hold tha data (char pointer) and the sec hold the next node */
	char * data;
	strNodeP next;
	} strNode;

typedef struct mcrTable * mcrTableP;
typedef struct mcrTable {
	char * name ;
	strNodeP data;
	mcrTableP next;
	} mcrTable;

/* methods declaration */ 
void preAssemblerPass(char * );

void getCodeLine(char **  , FILE * );

void isDynamicAlocationError(void * );

int isWordInLine(char * , char []);

char * getMcrName(char * codeLine);

void addToMcrTable(mcrTableP * , char * );

void addLineInMcrToTheMcrTable(mcrTableP  , char * );

char * getFirstLineField(char * );

mcrTableP whatMcrWasCalled(mcrTableP   ,char * );

void addLineToFinalCode(strNodeP * , char *);

void addMcrLinesToFinalCode(strNodeP * ,strNodeP ); 

void writeFinalCode(strNodeP , char * );

char * getFileNameWithNewEnding(char * fileName);

