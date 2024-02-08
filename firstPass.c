#include "action.h"
#include "firstPass.h"
#include "DirectAddress.h"
#include "fileInput.h"
#include <stdio.h>

int checksOps(char *command, int *IC, FILE *labelsFile, int lineCounter, char theLine[])
{
    char secondOp[MAX_LINE_LENGTH] = {0};
	char firstOp[MAX_LINE_LENGTH] = {0};
	addressingMode destAddress;
    addressingMode scrAddress;
    copy(firstOp, strtok(theLine, ",")); /* get first operand */
	copy(secondOp, strtok(NULL, "")); /* get second operand */
    LeftSpaces(secondOp);
    RightSpaces(firstOp);
    /* here we can check what labels are requiring a check */
	destAddress = addressHandler(secondOp, labelsFile, TRUE);
    scrAddress = addressHandler(firstOp, labelsFile, TRUE);
    if (scrAddress != NOP && destAddress != NOP) 
	{
        /* check if was negative or another alternatives have been typed */
        if (!((scrAddress == addressZero || scrAddress == addressThree) && (strcmp(command, "lea") == 0)) && (!(destAddress == addressZero && (strcmp(command, "cmp") != 0))))
			{
            if (scrAddress == addressThree && destAddress == addressThree)
                (*IC)++; /* an operand like a register was found - then increase  IC pointer by two */
            else
                (*IC) = (*IC) + 2; /* increase IC counter for 2 because two parameters were found*/
            return TRUE;
        }
		else 
		{
            if ((scrAddress == addressZero || scrAddress == addressThree) && (strcmp(command, "lea") == 0)) /* syntax error for source */
                findError("wrong source address by syntax!", lineCounter);
            else
                findError("wrong destination address by syntax!", lineCounter);
            return FALSE;
        }
    }
    if (scrAddress == NOP)
        findError("first operand's name was undefined", lineCounter); /* first operand was invalid by syntax */
    else
        findError("second operand's name was undefined", lineCounter);
    return FALSE;
}

int checkLabel(FILE * check, data *ptSymbol) 
{
    char check_Line[MAX_LINE_LENGTH] = {0};
    data temp = NULL;
    char * labelNow;
    fseek(check, 0, SEEK_SET);
    while (!feof(check)) {
        labelNow = NULL;
        memset(check_Line, 0, MAX_LINE_LENGTH);
        fgets(check_Line, MAX_LINE_LENGTH, check); /* going on each line in the file */
        labelNow = strtok(check_Line, " \t\n\v\f\r");
        if (!labelNow) /* if line is empty - return true */
            return TRUE;
        temp = SearchLink(*ptSymbol, labelNow);/* temp is if label is defined or either undefined */
        if ((temp) != NULL && (getType(temp) != EXTERNAL)) 
		{
            setType(temp, ENTRY); /* a valid label - then it's enternal of course */
            continue;
        }
        if (!(temp) || getType(temp) == EXTERNAL)  /* error as label was external */
		{
            if (!(temp))
                printf("an undefined label was found\n");
            else
                printf("A label can't be both enternal and external \n");
            return FALSE; /*if wrong label syntax - return false immediately */
        }
    }
    return TRUE;
}

int undefinedLabels(FILE *fileLabel, data *ptSymbol) 
{
    int boolean = TRUE;
    char checking_Line[MAX_LINE_LENGTH] = {0};
    data temp = NULL;
    char *labelNow = NULL;
    fseek(fileLabel, 0, SEEK_SET);
    while (!feof(fileLabel)) /* going on all file */
	{
        memset(checking_Line, 0, MAX_LINE_LENGTH);
        fgets(checking_Line, MAX_LINE_LENGTH, fileLabel); /* running line by line from given file */
        labelNow = strtok(checking_Line, " \t\n\v\f\r");
        if (!labelNow) /* If an empty line has been found */
		{
            printf("\n");
            return boolean;
        }
        temp = SearchLink(*ptSymbol, labelNow); /*If label is defined or either undefined by it's exsistence in symbols list */
        if (temp != NULL) 
		{
            setUsed(temp, TRUE);
            continue;
        } 
		else 
		{
            if (boolean) /* When label is undefined */
                printf("undefined label was found: '%s'", labelNow);
            else
                printf(",'%s'", labelNow);
            boolean = FALSE;
        }
    }
    return TRUE;
}

int entryInst(data *ptSymbol, char *mark, int *DC, FILE *entryFile, int lineCounter, data *temp) 
{
    char theLine[MAX_LINE_LENGTH];
    if (*temp != NULL) 
	{   /* When .entry was found before - label isn't required */
        findWarning("entry with a symbol decleartion - so symbol is removed from line", lineCounter);
        moveHead(ptSymbol);
    }
    if (mark != NULL) 
	{
        mark[strlen(mark) - 1] = '\0'; /* as we don't need to enter sign - we remove it from string */
        memset(theLine, 0, MAX_LINE_LENGTH);
        copy(theLine, mark); /* copying the rest of the line here */
        LeftSpaces(theLine);
        mark[strlen(mark) - 1] = '\0'; /* removing colon as it's not needed */
        if (theLine[0] != '\0' && labelName(theLine)) 
		{   /* if it .entry declaration and all valid, write it in entry file*/
            fprintf(entryFile, "%s\n", theLine);
            return TRUE;
        }
        if (theLine[0] != '\0') /* label name was undefined */
            printf("Error was found at line %d: undefined label name: %s\n", lineCounter, mark);
        else
            findError("no label found next to an entry instruction - error!", lineCounter);
        return FALSE;
    }
    findError("no label found next to an entry instruction - error!", lineCounter);
    return FALSE;
}

int externInst (data *ptSymbol, char *mark, int *DC, int lineCounter, data *temp)
{
    if (*temp != NULL) /* label was found already */
	{ 
        findWarning("found an extern instruction with a symbol declared to it. new symbol got deleted.", lineCounter);
        moveHead(ptSymbol);
    }
    if (mark != NULL && labelHandler(ptSymbol, mark, *DC, lineCounter, temp)) 
	{
        setType(*temp, EXTERNAL); /* When label is legal, add it's type and extern */
        setAddress(*temp, 0);
        return TRUE;
    }
    if (!mark) /* extern without label - illegal syntax */
        findError("Found an extern instruction without any label!", lineCounter);
    return FALSE;
}

int stringInst (data *ptSymbol, char *lineCheck, int *DC, int lineCounter, data *temp)
{
    int index = 0;
    int len = 0;
    int strChecked = 0;
    char theLine[MAX_LINE_LENGTH] = {0};
    if (*temp != NULL) 
	{
        setAddress(*temp, *DC); /* storing data address and type */
        setType(*temp, DATA);
    }
    if (copy(theLine, lineCheck)) 
	{
        RightSpaces(theLine);
        LeftSpaces(theLine); /* put no-white spaces in string from both sides */
        if (theLine[index] != '\0') 
		{
            if (theLine[index] == '\"') /* because string always begins with an '"' */
			{ 
                for (index = 1; index < strlen(theLine) && theLine[index] != '\n' && theLine[index] != '\0' && !strChecked; index++)
				{
                    len++;
                    if (theLine[index] == '\"') /* string is valid - so returning true */
                        strChecked = TRUE; 
                }
                if (strChecked && len != 1) 
				{
                    *DC = *DC + len; /* adding chars' amount to DC */
                    if (index == strlen(theLine))
                        return TRUE;
                    else 
					{   /* unexpected text was found after the string */
                        findError("Found invalid text after string", lineCounter);
                        return FALSE;
                    }
                } 
		else 
		{
                    if (!strChecked)
                        findError("valid string must end with \"", lineCounter);
                    else /* not all directive was typed correctly */
                        findError("Missing chars in .string instruction", lineCounter);
                    return FALSE;
                }
            }
            findError("valid string must start with \"", lineCounter);
            return FALSE;
        }
        findError("only white spaces its not allow in string instruction", lineCounter);
        return FALSE;
    }
    findError("Missing string in .string instruction!", lineCounter);
    return FALSE;
}

int dataInst(data *ptSymbol, char *lineCheck, int *DC, int lineCounter, data *temp)
{
    char theLine[MAX_LINE_LENGTH * 2] = {0};
    char numTemp[MAX_LINE_LENGTH] = {0};
	char *nextNumber = NULL;
	int numSearch = 0;
	int * boolean = NULL;;
    if (*temp != NULL)
	{
        setAddress(*temp, *DC);
        setType(*temp, DATA);
    }
    if (spaceJump(lineCheck, theLine)) /* after the command, it's required to add a space */
	{ 
        if ((theLine[0] != '\0') && (theLine[0] != ',') && (theLine[strlen(theLine) - 2] != ','))
		{
            nextNumber = strtok(theLine, ",");
            while (nextNumber != NULL) 
			{
                memset(numTemp, 0, MAX_LINE_LENGTH);
                numSearch = 1;
				boolean = &numSearch;
                copy(numTemp, nextNumber);
                LeftSpaces(numTemp); /* no white spaces from the left of the string that way */
                if (validNumber(numTemp, boolean) && numTemp[0] != '\0') 
				{ /* represents the 14 bits */
                    (*DC)++; /*increase DC according to the amount of numbers were found */
                    nextNumber = strtok(NULL, ",");
                    continue;
                } else {
                    if (numTemp[0] == '\0')
                        findError("between two commas must be number", lineCounter);
                    else /* data instruction was typed incorrectly */
                        printf("Error occurred at line %d: number '%s' invalid in .data instruction! \n", lineCounter,numTemp);
                    return FALSE;
                }
            }
            if (!numSearch)
                findError("found a .data instruction without any numbers", lineCounter);
            return numSearch;
        }
    }
    if (!lineCheck || theLine[0] == '\0')
        findError("Missing numbers in .data instruction", lineCounter);
    else if (theLine[0] == ',')
        findError("Invalid ',' at the beginning of .data instruction", lineCounter);
    else
        findError("Invalid ',' at the end of .data instruction", lineCounter);
    return FALSE;
}

int checkOneOp(char *cmd, int *IC, FILE *labelsFile, int lineCounter, char theLine[]) 
{
	char secondOp[MAX_LINE_LENGTH] = {0};
    char firstOp[MAX_LINE_LENGTH] = {0};
    addressingMode dest;
	addressingMode src;
    char *mark;
    /* handling here special commands of addressing 2 */
    if ((strcmp(cmd, "jmp") == 0) || (strcmp(cmd, "bne") == 0) || (strcmp(cmd, "jsr") == 0)) 
	{
        /* found an '(' so we got here addressing number two */
        if (theLine[strlen(theLine) - 1] == ')') 
		{
            mark = strtok(theLine, "(");
            fprintf(labelsFile, "%s\n", mark);
            copy(firstOp, strtok(NULL, ",")); /* copying strings of second addressing without ')' */
            copy(secondOp, strtok(NULL, ")"));
            src = addressHandler(firstOp, labelsFile, TRUE); /* saving labels from file as needed in a mark */
            dest = addressHandler(secondOp, labelsFile, TRUE);
            if (src != NOP && dest != NOP && labelName(mark))
			{
                if (src == addressThree && dest == addressThree)
                    (*IC) = (*IC) + 2; /*When both operands are registers*/
                else
                    (*IC) = (*IC) + 3; /*When one of the operands isn't a register*/
                return TRUE;
            }
            if (src == NOP) /* first operand is undefined here by syntax */
                findError("Invalid definition of firstOperands name!", lineCounter);
            else if (dest == NOP) /* second operand is undefined here by syntax */
                findError("Invalid definition of secondOperand name!", lineCounter);
            else
                findError("Invalid label name!", lineCounter);
            return FALSE;
        } 
	 else { /* If we have second addressing with a single operand */
            mark = strtok(theLine, "");
            if (labelName(mark)) 
			{
                (*IC)++; /* increase IC by one to the next compartment */
                fprintf(labelsFile, "%s\n", mark);
                return TRUE;
            }
            findError("Invalid Operand that should be label !", lineCounter); /* incorrect as operand was found as a label */
            return FALSE;
        }
    } 
	else { /* If we have other addressings with a single operand */
        dest = addressHandler(theLine, labelsFile, TRUE); /* save labels for checking if they're valid in given file */
        if (!(dest == addressZero && (strcmp(cmd, "prn") != 0)) &&
            (dest != NOP))
		{
            (*IC)++; /* increase IC by one to the next compartment - then return true*/
            return TRUE;
        }
        if (dest == NOP)
            findError("Invalid definition of a Operand!", lineCounter);
        else	/* destination address was incorrect */
            findError("Invalid destination address!", lineCounter);
        return FALSE;
    }
}

int cmdStatement(char *cmd, int *IC, FILE *labelsFile, int lineCounter)
{
    int numOperand;
    int mark;
    char theLine[MAX_LINE_LENGTH] = {0};
    numOperand = actionNum(cmd);
    (*IC)++; /* needed one more word so increasing IC */
    if (numOperand == 0)
	{  /* if there are no operands found */
        if (!copy(theLine, strtok(NULL, " \t\n\v\f\r")))
            return TRUE;
        findError("for this command no Operand allowed or any characters", lineCounter); /* mistake operands given to action */
        return FALSE;
    }
    mark = copy(theLine, strtok(NULL, ""));
    if (mark && theLine[0] != ',') 
	{
        LeftSpaces(theLine);
        RightSpaces(theLine); /* removing all white spaces from both left and right of the string */
        if (numOperand == 1)
			 /* When a single operand is required */
            return checkOneOp(cmd, IC, labelsFile, lineCounter, theLine);
        else /* When two operands are required */
            return checksOps(cmd, IC, labelsFile, lineCounter, theLine);
    }
    if (!mark) /* no operands - missing */
        findError("Missing operand", lineCounter);
    else  /* an illegal comma was found by syntax */
        findError("illegal commas found right after the command name", lineCounter);
    return FALSE;
}

int instHandler(data *ptSymbol, char *wordNow, int *DC, FILE *entryFile, int lineCounter, data *temp, int *tags)
{
    if (!strcmp(".data", wordNow)) /* tags for data label */
        return dataInst(ptSymbol, strtok(NULL, ""), DC, lineCounter, temp);
    if (!strcmp(".string", wordNow)) /* tags for string label */
        return stringInst(ptSymbol, strtok(NULL, ""), DC, lineCounter, temp);
    if (!strcmp(".entry", wordNow)) {
        tags[enternal] = ABLED; /* tags for entry label */
        return entryInst(ptSymbol, strtok(NULL, ""), DC, entryFile, lineCounter, temp);
    }
    if (!strcmp(".extern", wordNow)) {
        tags[external] = ABLED;/* tags for extern label  */
        return externInst(ptSymbol, strtok(NULL, ""), DC, lineCounter, temp);
    }
    findError("found an illegal instruction", lineCounter);
    return FALSE;
}

int labelHandler(data *ptSymbol, char *label, int IC, int lineCounter, data *temp)
{
    char theLine[MAX_LINE_LENGTH];
    memset(theLine, 0, MAX_LINE_LENGTH);
    label[strlen(label) - 1] = '\0'; /* removing enter sign as it's not needed */
    copy(theLine, label);
    LeftSpaces(theLine);
    if (labelName(theLine)) 
	{
        *temp = SearchLink(*ptSymbol, theLine);
        if (!(*temp) && (!savedWord(theLine))) 
		{ /* checks if label is valid and also exists in symbols' list*/
            *temp = createLinked(theLine, IC);
            InsertLinked(ptSymbol, *temp); /* because the label is valid, we need to add it to the list */
            return TRUE;
        }
        if ((*temp) != NULL) /* label was found again - error */
            findError("Found multiple definition of the same label", lineCounter);
        else
            findError("label can't be a reserved word", lineCounter);
        return FALSE;
    }
    if (theLine[0] != '\0') 
        printf("Error occurred at line %d: Invalid label name: %s\n", lineCounter, theLine);
    else	/* syntax error for no label */
        findError("the label cannot be empty", lineCounter);
    return FALSE;
}

data mainFirstPass(char *fileName, int *marks) 
{
    FILE *input_File, *labelsFile, *entryFile; /* a file pointer and one for entry file, and another one for label file */
    char line[MAX_LINE_LENGTH] = {0};
	int DC = 0;
	int IC = 100;
    data ptSymbol = NULL, temp = NULL;
    char *wordNow = NULL; /* updating each time for the word we're passing */
    int lineCounter = 0; 
    input_File = openFile(fileName, "am", "r"); /* open given file for a check */
    /* writing down all of the labels we'll be needing to check */
    labelsFile = openFile("LabelFile", "am", "w+");
    entryFile = openFile("EntryFile", "am", "w+"); /* writing down all labels for the entry file */
    if (!input_File) 
	{ /* file couldn't be opened */
        findError("Error because input file couldn't be opened!", lineCounter);
        marks[err] = ABLED;
        return NULL;
    }
    printf("Started first iteration on the file: %s...\n", fileName);
    while (!feof(input_File)) /* saving each line by iteration */
	{ 
        temp = NULL;
        memset(line, 0, MAX_LINE_LENGTH); /* assigns zero to all array's compartment */
        fgets(line, MAX_LINE_LENGTH, input_File); 
        lineCounter++;
        wordNow = strtok(line, " \t\n\v\f\r"); /* taking out of the line the first word */
        if (!wordNow || wordNow[0] == ';')
            continue; /* line is empty - so keep passing for next line */
        if (wordNow[strlen(wordNow) - 1] == ':') /* a label was found */
		{
            if (!labelHandler(&ptSymbol, wordNow, IC, lineCounter, &temp)) 
			{ /* if label's syntax valid */
                marks[err] = ABLED; /* error - so continuing */
                continue;
            }
			else
			{
                wordNow = strtok(NULL, " \t\n\v\f\r"); /* pass next line as word doesn't exist */
                if (!wordNow) 
				{
                    findError("Found an empty label declaration", lineCounter);
                    marks[err] = ABLED;/* if a occurred an error */
                    continue;
                }
            }
        }
        if (wordNow[0] == '.') /* an instruction was found */
		{
            if (!instHandler(&ptSymbol, wordNow, &DC, entryFile, lineCounter, &temp, marks))
			{
                marks[err] = ABLED;
            }
            continue;
        } 
        if (isOpNameCorrect(wordNow)) /* a command was found */
		{
            if (!cmdStatement(wordNow, &IC, labelsFile, lineCounter))
			{
                marks[err] = ABLED;
                continue;
            }
        }
		else 
		{ /* command is incorrect by syntax */
            printf("Error occurred at line %d: Invalid command: '%s'\n", lineCounter, wordNow);
            marks[err] = ABLED;/* if a occurred an error */
        }
	}
    update_IC_symbols(&ptSymbol, IC); /* updating all of the data */
    if (!checkLabel(entryFile, &ptSymbol))
        marks[err] = ABLED; /* en error of entry label was found */
    if (!undefinedLabels(labelsFile, &ptSymbol))
        marks[err] = ABLED; /* undefined label in file was found */
    marks[ICnum] = IC; 
    marks[DCnum] = DC; /* save DC & IC for the second pass */
    FileClosing(input_File, labelsFile, entryFile);  /* free memory */
    if (ptSymbol != NULL)
        allUsed(ptSymbol, 1); /* If an unused symbol was found */
    if (marks[err] == ABLED) 
	{ /* an error on first iteration by file */
        printf("First iteration on file: %s.am failed!\n", fileName);
        removeFile(fileName, "am");
        freelist(ptSymbol);/* free the linked-lists */
        return NULL;
    }
    printf("Finished first iteration on file: %s.am!\n", fileName);
    return ptSymbol;
}

void FileClosing(FILE *I_File, FILE *labelsFile, FILE *entryFile) 
{
    fclose(I_File);
    fclose(labelsFile);
    fclose(entryFile);
    removeFile("EntryFile", "am");
    removeFile("LabelFile", "am");
}

