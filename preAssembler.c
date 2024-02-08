#include "preAssembler.h"
/****************************************************
It is recommended to avoid global variables. (-3)
*****************************************************/
short flagEOF;
char  FIRSTMCRFIELD[]  = "mcr" , LASTMCRFIELD[] = "endmcr";


void preAssemblerPass(char * fullFileName)
{
	char * currCodeLine = NULL ;  /* buffer for the curr code line */
	strNodeP finalCode = NULL; /* list of strings that each string reflect a line in the code after the macros spread set to NULL in order to prevent segmentation fault*/ 
	mcrTableP mcrTablePointer = NULL , mcrWasCalled = NULL; /* Symbal table pointer sets to NULL in ordewr to prevent segmentation fault */
	short flagInMcr = 0; /* flag that presenting if we are in a mcr block */ 
	FILE * file; /* FILE pointer to our assembly file */ 
	isDynamicAlocationError(fullFileName);
	file = fopen(fullFileName, "r");
	if(file == NULL)
	{ /* checks if there is such file */ 
		printf("~PreAssembler : There is no such file (file name : %s)" , fullFileName);
		exit(0);
	}
	getCodeLine(&currCodeLine, file);
	while(!flagEOF)
	{
		
		if(!flagInMcr && isWordInLine(currCodeLine,FIRSTMCRFIELD))
		{
			flagInMcr = 1;
			
			addToMcrTable(&mcrTablePointer, getMcrName(currCodeLine)); /* adding new Macro name to the Symbals Table */
			
		}else if( flagInMcr && !isWordInLine(currCodeLine,LASTMCRFIELD)){
			
			addLineInMcrToTheMcrTable(mcrTablePointer, currCodeLine);

		}else if(flagInMcr && isWordInLine(currCodeLine,LASTMCRFIELD) ){
			
			flagInMcr = 0;	
		}else {
			if((mcrWasCalled = whatMcrWasCalled(mcrTablePointer ,currCodeLine)) == NULL){
				addLineToFinalCode(&finalCode,currCodeLine);/* add a regular line to the final code */
			}else{
				addMcrLinesToFinalCode(&finalCode, mcrWasCalled->data);/* add the macro lines to the final code lines */
			}

		}
		
		
		getCodeLine(&currCodeLine, file); /* getting the new line from the code */
	}
	free(currCodeLine);
	fclose(file);/* close the assembly code file */ 
	
	
	/* writting the new code in the file after the spreading of the macros */ 
	writeFinalCode(finalCode ,fullFileName);
	
	free(mcrTablePointer);

}

void getCodeLine(char ** codeLineP , FILE * file) /* method that puts the current code line in the 'codeLine' */  
{
	char c;
	int lettersInLine = 1;
	if(!(*codeLineP))
		free(*codeLineP);
	while ( (c = getc(file)) != EOF && c != '\n') /* we'll pass til the new line or the end of the file */ 
	{
		if(!(*codeLineP)){
			(*codeLineP) = (char *) malloc(sizeof(char) * lettersInLine);
			isDynamicAlocationError(*codeLineP);
			(*codeLineP)[0] = c;
		}else{
			(*codeLineP) = (char *) realloc((*codeLineP),sizeof(char) * lettersInLine);
			isDynamicAlocationError(*codeLineP);
			(*codeLineP)[lettersInLine - 1]  = c; 
		}
		lettersInLine++;
	}
	if(*codeLineP == NULL){
		(*codeLineP) = (char *) malloc(sizeof(char) * (lettersInLine +1));
		isDynamicAlocationError(*codeLineP);
		(*codeLineP)[0] = '\0';
	}else if(c  == '\n'){
		(*codeLineP) = (char *) realloc((*codeLineP),sizeof(char) * lettersInLine );
		isDynamicAlocationError(*codeLineP);
		(*codeLineP)[lettersInLine-1]  = '\0'; 
	}

	if( c == EOF)
		flagEOF = 1;/* we reached to the end of file */ 	
}




void isDynamicAlocationError(void * pointer)
{
	if(!pointer){
		printf("Error ,  Dynamic memory alocation eror");
		exit(0);
	}
}

int isWordInLine(char * currCodeLine, char word[])
{/* method that checks if we are at the beggining of word */ 
	char * mcrStartP;
	mcrStartP = strstr(currCodeLine,word);
	if(mcrStartP){
		
		return (isspace(*(mcrStartP + strlen(word) )) || *(mcrStartP+strlen(word)) == EOF ||  *(mcrStartP+strlen(word)) == '\0') 
			&& (( mcrStartP == currCodeLine) || isspace(*(mcrStartP-1))); /* check if the word stands for itself */
	}
	return 0;
}

char * getMcrName(char * codeLine){
	char * bMcrName , * mcrName = NULL; /* pointer before the begginning of the macro name in the string line presentation
						, pointer to the macro name */
	int nameSize = 0; /* the size of the macro name */
	bMcrName = strstr(codeLine , FIRSTMCRFIELD);  
	 bMcrName+=3;/* getting where the macro first field ends */ 
	for(; *bMcrName == ' ' || *bMcrName == '\t' ; bMcrName++); /* getting where the macro name starts */ 
	for ( nameSize = 1;!isspace(*bMcrName) && *bMcrName != '\0'; bMcrName++ , nameSize++){/** passing on the whole macro 																																		name's chars */
		if(!mcrName)
		{
			mcrName = (char * )malloc(nameSize * sizeof(char));
			isDynamicAlocationError(mcrName);
			mcrName[nameSize-1] = *bMcrName;
		}else { 
			mcrName = (char * )realloc(mcrName,nameSize * sizeof(char));
			isDynamicAlocationError(mcrName);
			mcrName[nameSize-1] = *bMcrName;
		}
		
	}
	mcrName = (char * )realloc(mcrName,nameSize * sizeof(char));
	isDynamicAlocationError(mcrName);
	mcrName[nameSize-1] = '\0';
	return mcrName; /* returns the macro name */
	
}

void addToMcrTable(mcrTableP * mcrTablePTP, char * mcrName){/* method that adds the mcro name to the macro table */ 

	
	if((*mcrTablePTP) == NULL){ /* checks if is the first macro that we are going to insert the table */ 
		(*mcrTablePTP) = (mcrTableP) malloc(sizeof(mcrTable));
		isDynamicAlocationError((*mcrTablePTP));
		(*mcrTablePTP)-> name = mcrName;/* check dynamic alocation */
		(*mcrTablePTP)-> next = NULL;/* preventing segmentation fault */
		(*mcrTablePTP)-> data = NULL;/* preventing segmentation fault */
	}else{
		  for(; (*mcrTablePTP) -> next != NULL ; mcrTablePTP = &((*mcrTablePTP)->next)); /* passing to the last macro in the list */ 
		(*mcrTablePTP)->next = (mcrTableP) malloc(sizeof(mcrTable));
		isDynamicAlocationError((*mcrTablePTP)->next); /* check dynamic alocation */
		(*mcrTablePTP)->next->name = mcrName;
		(*mcrTablePTP)-> next -> next = NULL;/* preventing segmentation fault */
		(*mcrTablePTP)-> next -> data = NULL;/* preventing segmentation fault */
	}
	
}

/* method that adds the lines in the macro to the macro data lines in the table */ 
void addLineInMcrToTheMcrTable(mcrTableP  mcrTablePointer, char * lineInMcr){
	
	for(; mcrTablePointer -> next != NULL; mcrTablePointer = mcrTablePointer->next);/* passing to the last macro in the list */ 
	if(mcrTablePointer -> data == NULL){
		mcrTablePointer -> data = (strNodeP) malloc (sizeof(strNode)); /* creating new memory storage to save the lines in the macro */
		mcrTablePointer -> data -> data = (char *) malloc(sizeof(char) * (strlen(lineInMcr))) ; /* creating new place to the new line */  
		strcpy(mcrTablePointer -> data -> data,lineInMcr ); /* adding the new line by preventing false pointing */ 
		mcrTablePointer -> data -> next = NULL; /* preventing segmentation fault */
		
	}else{
		/* adding new block of memory to the new line */
		strNodeP dataP = mcrTablePointer -> data; /* curr line in the macro */ 
		for(; dataP -> next != NULL ; dataP = dataP -> next);
		
	 	dataP -> next = (strNodeP) malloc (sizeof(strNode)); /* adding the new line data struct */ 
		dataP -> next -> data = (char *) malloc (sizeof(char)*(strlen(lineInMcr)));
		strcpy(dataP -> next -> data , lineInMcr); /* adding the new line */ 
		dataP -> next -> next = NULL; /* preventing segmentation fault */ 
		
	}
	
}

char * getFirstLineField(char * currCodeLine){
	int firstFieldSize = 0;
	char * firstLineField;
	firstLineField = NULL;
	if(currCodeLine == NULL)
		return NULL;
	
	for(; *currCodeLine == ' ' ||  *currCodeLine == '\t' ; currCodeLine++);
	
	for(firstFieldSize = 1; *currCodeLine != EOF && *currCodeLine != '\0' && !isspace(*currCodeLine)  ; currCodeLine++ , firstFieldSize++){
		
		if(firstLineField == NULL){ /* adding the first letter */ 
			
			firstLineField = (char *) malloc(firstFieldSize);
			isDynamicAlocationError(firstLineField);
			firstLineField[0] = currCodeLine[0];
		}else{ /* adding other letter */ 
			
			firstLineField = (char *) realloc(firstLineField, firstFieldSize * sizeof(char));
			isDynamicAlocationError(firstLineField);
			firstLineField[firstFieldSize - 1] = *currCodeLine;
		}
	}
	return firstLineField;
}

/* method that returns the macro pointer if the macro was called in line in the code else returns NULL */ 
mcrTableP whatMcrWasCalled(mcrTableP  mcrTablePointer ,char * currCodeLine){
	char *	firstLineField;
	firstLineField = getFirstLineField(currCodeLine);
	for (; mcrTablePointer != NULL ; mcrTablePointer = mcrTablePointer -> next){/* passing on the macro table looking for a macro name with the val of macroName */ 
		if(firstLineField != NULL && strcmp(mcrTablePointer -> name , firstLineField) == 0){
			free(firstLineField);
			return mcrTablePointer; /* returns the macro pointer if the macro was found in the line */ 
		}
	} 
	return NULL; /* return NULL if the macro wasn't found */
}

/* method that adds new line to the final code lines */ 
void addLineToFinalCode(strNodeP * finalCodeP , char * newLine){
	if((*finalCodeP) == NULL){ /* if the final code lines is empty than create the first data  block */ 
		(*finalCodeP) = (strNodeP) malloc(sizeof(strNode)); 
		isDynamicAlocationError(*finalCodeP);
		(*finalCodeP) -> data = (char *) malloc (sizeof(char)*(strlen(newLine)+1));
		isDynamicAlocationError((*finalCodeP) -> data);
		strcpy((*finalCodeP) -> data , newLine);
		(*finalCodeP) -> next = NULL;/* preventing segmentation fault */
	}else{/* if there are lines in the final code struct */ 
		strNodeP finalCode;
		finalCode = (*finalCodeP);
		for(; finalCode  -> next != NULL ; finalCode = finalCode->next); /* passing on the final code lines and adding to the last line block */ 
		finalCode  -> next = (strNodeP) malloc(sizeof(strNode)); 
		isDynamicAlocationError (finalCode  -> next);
		finalCode -> next -> data = (char *) malloc(sizeof(char)*(strlen(newLine)+1));
		isDynamicAlocationError(finalCode -> next -> data);
		strcpy(finalCode -> next -> data  , newLine);
		finalCode -> next -> next = NULL;/* preventing segmentation fault */
	}
		
}
/* method that adds (spreads) macro line where it was called to the final code line list */ 
void addMcrLinesToFinalCode(strNodeP * finalCodeP,strNodeP mcrLines){
	
	strNodeP finalCode; /* final code*/ 
	
	if((*finalCodeP) == NULL){ /* if the final code lines is empty than create the first data  block */ 
		(*finalCodeP) = (strNodeP) malloc(sizeof(strNode)); 
		(*finalCodeP) -> data =(char *) malloc(sizeof(char)*(strlen(mcrLines -> data)));
		isDynamicAlocationError((*finalCodeP) -> data);
		strcpy((*finalCodeP) -> data, mcrLines -> data);
		(*finalCodeP) -> next = NULL;/* preventing segmentation fault */
		mcrLines = mcrLines -> next;
	}
	finalCode = *finalCodeP;
	for(; finalCode -> next != NULL ; finalCode = finalCode -> next); /* passing to the last final code block. */
	for(; mcrLines  != NULL ; mcrLines = mcrLines->next){ /* passing on the macro lines and adding it to the final code lines */ 
		finalCode -> next = (strNodeP) malloc(sizeof(strNode));
		isDynamicAlocationError(finalCode -> next);
		finalCode = finalCode->next;
		finalCode->data = (char *) malloc (sizeof(char)*(strlen( mcrLines-> data)));
		isDynamicAlocationError(finalCode -> data);
		strcpy(finalCode->data , mcrLines-> data);
		finalCode -> next = NULL; /* preventing segmentation fault */
	}

}

void writeFinalCode(strNodeP finalCode, char * fileName){/* method that writes the new file (.am) after the macro spreading */ 
	FILE * file; /* FILE pointer to our assembly file */
	fileName = getFileNameWithNewEnding(fileName);
	file = fopen(fileName, "w");
	if(file == NULL){ /* checks if there is such file */ 
		printf("There is no file with the name : %s" , fileName);
		exit(0);
	}
	for ( ; finalCode != NULL ; finalCode = finalCode->next){
		fputs(finalCode -> data, file);	/* print list of strings that each string reflect a line in the code after the macros spread in th file */ 
		fputs("\n",file);
	}
	free(fileName);
	fclose(file);
}
char * getFileNameWithNewEnding(char * fileName) /* method that change the file name to a proper one with the ending (.am) */
{
	char * newName;
	int i;
	newName = (char *) malloc((strlen(fileName)+1) * sizeof(char));
	isDynamicAlocationError(newName);
	for(i = 0 ; i < strlen(fileName) -1 ; i++)
		newName[i] = *(fileName+i);
	newName[i] = 'm';
	newName[i+1] = '\0';
	 
	return newName;
}




