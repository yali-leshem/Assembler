#include "fileInput.h"
#include "action.h"
#include "DirectAddress.h"
#include "operandRegs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *openFile(char *fileName, char *fileExten, char *mode)
{
    char fileIncExten[MAX_FILE_LEN] = {0};
    FILE *openFile;
    FileWithEnding(fileName, fileExten, fileIncExten); /* adding the full file name */
    /*Opening the file with matching extension according to the received mode*/
    openFile = fopen(fileIncExten, mode); /* opening given file & returning it */
    return openFile;
}

void removeFile(char *fileOp, char *fileIncExten)
{
    char file_Inc_Exten[MAX_FILE_LEN] = {0};
    FileWithEnding(fileOp, fileIncExten, file_Inc_Exten);
    remove(file_Inc_Exten); /* removing given file */
}

void LeftSpaces(char the_Line[MAX_LINE_LENGTH])
{
    int index1 = 0, index2 = 0;
    if (the_Line != NULL)
    {
        /*increasing both indexes while only white spaces have been found */
        while(index1 < strlen(the_Line) && (the_Line[index1] == ' ' || the_Line[index1] == '\t' || the_Line[index1] == EOF || the_Line[index1] == '\n' || the_Line[index1] == '\r'))
	{
		index1++;
            	index2++;
	}
	index1 = 0;
	/* when there are no characters to the left of the string - delete them all */
        while(index2 <= strlen(the_Line))
	{
            the_Line[index1] = the_Line[index2];
	    index1++;
            index2++;
	}
        the_Line[index2] = '\0';
    }
}


void FileWithEnding(char *fileOp, char *exten, char *fileName)
{
    strcat(fileName, fileOp);  /*Add the full name to the file name*/
    fileName[strlen(fileOp)] = '.';
    strcat(fileName + (strlen(fileOp)) + 1, exten); /*requires to adding the extension to the file given*/
}

int copy(char the_Line[MAX_LINE_LENGTH], char *GivenLine)
{
    int index;
    if (GivenLine != NULL && (MAX_LINE_LENGTH >= strlen(GivenLine) - 1)) 
    {
	index = 0;
        while (index < strlen(GivenLine))
	{
            the_Line[index] = GivenLine[index]; /* taking the string to the array 'GivenLine' */
	    index++;
	}
        return TRUE; /* string has been copied */
    }
    return FALSE;/*If the string is null or exceeds the input limits*/
}

void RightSpaces(char line[MAX_LINE_LENGTH]) 
{
    int index;
    if (line != NULL) {
        index = strlen(line) - 1;
        for (;index >= 0 && (line[index] == EOF || line[index] == '\r' || line[index] == ' ' || line[index] == '\t' || line[index] == '\n') ;index--) 
	{
            line[index] = '\0'; /* when there are spaces to the right of the string, put '\0' instead */
        }
    }
}

void findError(char *Error_Message, int line_Counter) /* If an error was found */
{
    printf("Error! occurred at line %d: %s\n", line_Counter, Error_Message);
}

void findWarning(char *Warning_Message, int line_Counter) /* If a warning was found */
{
    printf("Warning! occurred at line %d: %s\n", line_Counter, Warning_Message);
}

int savedWord(char *word)
{
    return (isNameCorrect(word) || instructionName(word) || isOpNameCorrect(word)); /* return true if the word is a command or an instruction or an operand */
}

int validNumber(char num[MAX_LINE_LENGTH], int *tag) 
{
    char *temp = NULL;
    long decimalNum = 0;
    if (num[0] != '\0') /* if num isn't empty */
    {
        decimalNum = strtol(num, &temp, 10);
        if (temp[0] == '\n'|| !temp || temp[0] == '\0' || temp[0] == '\t' || temp[0] == '\r') 
	{
            if ((!num && decimalNum<= MAX12B && decimalNum>= MIN12B) || (num && decimalNum<= MAX14B && decimalNum>= MIN14B))
                return TRUE; /* This flags shows the encoding numbers that might be in the text (which means 12 bits or 14 either) */
        }
        return FALSE; /* impossible for encoding - number isn't valid so returning false */
    }
    return FALSE;
}

int labelName(char *name)
{
    int index;
    if (isalpha(name[0]) && strlen(name) <= MAX_LABEL_LENGTH && name != NULL)
	{
        for (index = 0; index < strlen(name); index++)
		{
            if (!isalnum(name[index]))
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

int spaceJump(char *file, char line[MAX_LINE_LENGTH * 2]) 
{
    int index1 = 0;
    int index2 = 0;
    if (file != NULL) 
	{
        for (index1 = 0; index1 < strlen(file); index1++) 
		{
            if (isspace(file[index1]) == 0) 
			{
                line[index2] = file[index1];
                if (file[index1] == ',') 
				{
                    index2++; /* adding right after the comma a white space */
                    line[index2] = ' ';
                }
                index2++;
            }
        }
        line[index2] = '\0';
        return TRUE;
    }
    return FALSE;
}
