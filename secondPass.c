#include "secondPass.h"
#include "action.h"
#include "operandRegs.h"
#include "fileInput.h"
#include <stdio.h>
#include <stdlib.h>

void uniqueBaseTwo(char *param, int tag, FILE *file, int *DC)
{
    int num;
    char *pt;
	int index;
    *DC = *DC + 1; /* increasing DC by one before being put onto file */
    fprintf(file, "0%d\t", *DC);  /*put to the file DC value */
    if (!tag)
        num = strtol(param, &pt, 10); /* string to decimal */
    else
        num = (int) (*param); /* num to ASCII value instead of integer */
    for (index = 13; index >= 0; index--) /* right encode data in 14 bits to the given file by defined unique base */
        (num & (1 << index)) ? fputc('/', file) : fputc('.', file);
    fprintf(file, "\n"); /* for the line ending in file */
}

void convertOpToUniqueBaseTwo(int num, int twoFirstBits, FILE *fileList, int *IC)
{
    int index;
    fprintf(fileList, "0%d\t", *IC); /*put to the file DC value */ 
    for (index = 11; index >= 0; index--) 
	{    /* right encode data in 12 bits to the given file by defined unique base */
       	if (num & (1 << index)) 
			fputc('/', fileList); 
		else
			fputc('.', fileList);
    }
    secondBits(twoFirstBits, fileList); /* handling the A,R,E bits */
    fprintf(fileList, "\n"); /* for the line ending in file */
}

void Opcode(int Opcode, FILE *listFile)
{
    int mask; /* mask for moving each time bits left, one for iteration */
    int bitPt = 3;
    while (bitPt >= 0) /* this loop is encoding the given opcode for the unique base two and writing it down to the given file */
	{
        mask = 1 << bitPt;
        mask = Opcode & mask;
        (mask != 0) ? fputc('/', listFile) : fputc('.', listFile);
        bitPt--;
    }
}

void secondBits(int dataEncoded, FILE *listFile)
{	/* handling the two first bits of A,R,E */
    if (dataEncoded == 3)
        fprintf(listFile, "//");/* means we have 11*/
	else if (dataEncoded == 2)
        fprintf(listFile, "/.");/*means we have 10*/
	else if (dataEncoded == 1)
        fprintf(listFile, "./");/*means we have 01*/
	else if (dataEncoded == 0)
        fprintf(listFile, "..");/*means we have 00*/
}


void oneWordTyped(int firstParam, int secondParam, int opcode, int src, int dest, int twoFirstBits, FILE *listFile, int *IC)
{
    *IC = *IC + 1; /* increasing IC By one so will be printed currently to listFile */
    fprintf(listFile, "0%d\t", *IC);
    secondBits(firstParam, listFile); /* encoding from binary all 14 bits - (2 parameters, source and destination, and two first bits which are represnting A,R,E) */
    secondBits(secondParam, listFile);
    Opcode(opcode, listFile);
    secondBits(src, listFile);
    secondBits(dest, listFile);
    secondBits(twoFirstBits, listFile);
    fprintf(listFile, "\n");
}

void dataEncode(char *line, int *DC, FILE *file)
{
    char *numNext = NULL;
    char theLine[MAX_LINE_LENGTH] = {0};
    copy(theLine, line);
    numNext = strtok(theLine, " \t\n\v\f\r,"); /* take number out of the given string which been copied to 'theLine' */
    while (numNext != NULL)
	{
        convertOpToUniqueBaseTwo((int)numNext, FALSE, file, DC); /* encode and write to the file in the unique base two */
        numNext = strtok(NULL, " \t\n\v\f\r,");/* cut the next number from the string */
    }
}

void stringEncode(char *line, int *DC, FILE *file)
{
    int index;
    char theLine[MAX_LINE_LENGTH] = {0};
    copy(theLine, line); /* copying input to the line array */
    for (index = 1; theLine[index] != '\"'; index++) 
	{
        convertOpToUniqueBaseTwo((int)&theLine[index], TRUE, file, DC); /* goes on input char by char and encoding to unique base two each iteration */
    }
    *DC = *DC + 1;
    fprintf(file, "0%d\t..............\n", *DC); /* encoding the zero sign as required in the end of the string */

}

void instInput(data *symbolList, char *instruction, int *DC, FILE *dataFile)
{
    if (!strcmp(".data", instruction))
        dataEncode(strtok(NULL, ""), DC, dataFile); /* encoding data directive to unique base two */
    if (!strcmp(".string", instruction))
        stringEncode(strtok(NULL, ""), DC, dataFile); /* encoding string directive to unique base two */
}

void singleOpEncode(data *ptSymbol, char *cmd, int *IC, FILE *listFile, FILE *externFile)
{
    int Opcode = 0; /* opcode of the command */
    char *op = NULL;
    addressingMode src = 0; /* destination and source address of input */
    addressingMode dest = 0;
    char *opFirst = NULL;
    char *opSecond = NULL;
    Opcode = codeOfAction(cmd);
    if ((strcmp(cmd, "jmp") == 0) || (strcmp(cmd, "bne") == 0) || (strcmp(cmd, "jsr") == 0)) /* If we have second addressing type (jump) */
	{ 
        op = strtok(NULL, "(\t\n\v\f\r ");
        opFirst = strtok(NULL, " \t\n\v\f\r,"); /* get only the first parameter in the line */
        if (opFirst != NULL)
		{	
            src = addressHandler(opFirst, NULL, FALSE);
            opSecond = strtok(NULL, " \t\n\v\f\r,)"); /* get only the second parameter in the line */
            dest = addressHandler(opSecond, NULL, FALSE); /* first word encoded to the unique base two */
            oneWordTyped(src, dest, Opcode, 0, addressTwo, 0, listFile, IC); /* label is being encoded to the unique base two */
            handleAddress(addressOne, ptSymbol, op, IC, listFile, externFile, FALSE); /* both parameters are being encoded to the unique base two */
            twoAddresses(ptSymbol, cmd, IC, src, dest, opFirst, opSecond, listFile,externFile);
        }
		else
		{	/* That means we have second addressing for sure (parameters' jump) */
            
            oneWordTyped(0, 0, Opcode, src, addressOne, 0, listFile, IC); /* first word encoded to the unique base two */
            handleAddress(addressOne, ptSymbol, op, IC, listFile, externFile, FALSE); /* label is being encoded to the unique base two */
        }
    }
	else
	{
        opFirst = strtok(NULL, " \t\n\v\f\r ");/* cut the first operand from the line */
        dest = addressHandler(op, NULL, FALSE);
        oneWordTyped(0, 0, Opcode, src, addressZero, 0, listFile, IC); /* first word encoded as a command to the unique base two */
        handleAddress(dest, ptSymbol, op, IC, listFile, externFile, FALSE); /* encoding the another line as required and adding it */
    }
}

void TwoOpsEncode(data *ptSymbol, char *cmd, int *IC, FILE *listFile, FILE *externFile)
{
    int Opcode = 0; /* the opcode */
    addressingMode src = 0;/* the destination and source addresses of the given input */
    addressingMode dest = 0;
    char *opFirst = NULL; /* represents first operand and second operand */
    char *opSecond = NULL;
    Opcode = codeOfAction(cmd);
    opFirst = strtok(NULL, " \t\n\v\f\r,");/* cut the first parameter from the line */
    src = addressHandler(opFirst, NULL, FALSE);
    opSecond = strtok(NULL, " \t\n\v\f\r,");/* cut the second parameter from the line */
    dest = addressHandler(opSecond, NULL, FALSE);
    /*encoding the first word for the command as a binary line from all relevant fields*/
    oneWordTyped(0, 0, Opcode, src, dest, 0, listFile, IC);
    /*encoding the extra line of the addressingMode as a binary line*/
    twoAddresses(ptSymbol, cmd, IC, src, dest, opFirst, opSecond, listFile, externFile);
}

void twoAddresses(data *ptSymbol, char *cmd, int *IC, addressingMode src, addressingMode dest, char *opFirst, char *opSecond, FILE *listFile, FILE *externFile)
{
    if (src == addressThree && dest == addressThree)
	{	
        *IC = *IC + 1; /* increasing IC as both destination and source addresses are as registers */
        convertOpToUniqueBaseTwo((valueRegister(opFirst) << 6) | (valueRegister(opSecond)), 0, listFile, IC);
    }
	else
	{	
        handleAddress(src, ptSymbol, opFirst, IC, listFile, externFile, TRUE); /* the next lines of addressing are being encoded to unique base two if not two 'r' */
        handleAddress(dest, ptSymbol, opFirst, IC, listFile, externFile, FALSE);
    }
}

void ImmediateAddress(char *op, int *IC, FILE *listFile)
{
    char * exitLine;
    int intOp = 0;
    intOp = strtol(op, &exitLine, 10); /* text to decimal number */
    convertOpToUniqueBaseTwo(intOp, 0, listFile, IC); /* operand is being encoded to the unique base two */
}

void directAddress(data *ptSymbol, char *op, int *IC, FILE *listFile, FILE *externFile)
{
	int twoFirstBits = 2;
    data temp = NULL;
    temp = SearchLink(*ptSymbol, op); /* symbol was found */
    if (getType(temp) == EXTERNAL) /* external so write to .ext file instead of the regular one */
{
        fprintf(externFile, "%s\t%d\n", getName(temp), *IC);
        twoFirstBits = 1;
    }
    convertOpToUniqueBaseTwo(getAddress(temp), twoFirstBits, listFile, IC); /* operand is being encoded to the unique base two */
}

void handleAddress(addressingMode ModeOp, data *ptSymbol, char *op, int *IC, FILE *listFile, FILE *externFile, int flag)
{
    *IC = *IC + 1;
    if (ModeOp == addressZero)
        ImmediateAddress(op, IC, listFile); /* more lines are required to be encoded so handling it with immediate address */
    else if (ModeOp == addressOne)
        directAddress(ptSymbol, op, IC, listFile, externFile); /* send it to be encoding the extra lines */
    else
	{
        if (flag)
            convertOpToUniqueBaseTwo((valueRegister(op) << 6), 0, listFile, IC); /*encoding the number of register to unique base two */
        else
            convertOpToUniqueBaseTwo((valueRegister(op)), 0, listFile, IC);
    }
}


void sentenceEncode(data *ptSymbol, char *cmd, int *IC, FILE *listFile, FILE *externFile)
{
    int NumOfOperand;
    NumOfOperand = actionNum(cmd);
    if (NumOfOperand == 0)/*encode a none operands command*/
        oneWordTyped(0, 0, codeOfAction(cmd), 0, 0, 0, listFile, IC);
    if (NumOfOperand == 1 || NumOfOperand == 2) /* encoding one or two operands given */
        TwoOpsEncode(ptSymbol, cmd, IC, listFile, externFile);
}

void secondPassExit(char *name, data *ptSymbol, FILE *file, FILE *enternalFile, FILE *externFile, FILE *listFile, FILE *dataFile)
{
    fclose(file);
    if (enternalFile != NULL) fclose(enternalFile);
    if (externFile != NULL) fclose(externFile); /* close all files which might have been created */
    fclose(listFile);
    fclose(dataFile);
    removeFile(name, "cmd"); /* deleting opened files */
    removeFile(name, "data");
}


void FileOutput(char *name, FILE *listFile, FILE *dataFile)
{
    char x;
    FILE *fileObj = openFile(name, "ob", "w");
    fseek(listFile, 0, SEEK_SET);
    fseek(dataFile, 0, SEEK_SET);
    while ((x = fgetc(listFile)) != EOF) /* copying all commands to the obj file */
        fputc(x, fileObj);
    while ((x = fgetc(dataFile)) != EOF) /* copying all data to the obj file */
        fputc(x, fileObj);
    fclose(fileObj);
}

void openFiles(char *name, FILE **I_File, FILE **listFile, FILE **dataFile, FILE **fileExternal, int *flags)
{
    if (flags[external] == ABLED) /* if needed to create external file */
        *fileExternal = openFile(name, "ext", "w+");
    *I_File = openFile(name, "am", "r"); 
    *listFile = openFile(name, "cmd", "w+");
    *dataFile = openFile(name, "data", "w+"); /* open files to read commands, and instructions & data */
    fprintf(*listFile, "\t%d\t%d\n", flags[ICnum] - 100, flags[DCnum]); /* put into file the counters' values */
}

void entryState(char *name, data *ptSymbol, FILE **enternFile, int *flags)
{
    data temp = *ptSymbol;
    if (flags[enternal] == ABLED)
	{	/* we will create entFile only if needed*/
        *enternFile = openFile(name, "ent", "w+");
        while (temp != NULL)
		{
            if (getType(temp) == ENTRY)/*we check all the symbolList, if the symbol is type of .entry we will write it on the entry file*/
                fprintf(*enternFile, "%s\t%d\n", getName(temp), getAddress(temp));
            temp = getNext(temp);
        }
    }
}

int secondPassMain(char *name, data *ptSymbol, int *flags)
{
	int DC = flags[ICnum] - 1;
    int IC = 99; /* assigning to counters */
	char *wordNow = NULL;
    FILE *I_File = NULL, *listFile = NULL, *dataFile = NULL, *fileEnt = NULL, *fileExt = NULL;
    char theLine[MAX_LINE_LENGTH] = {0}; /* for getting as an input lines from the files */
    int lineCounter = 0;
    printf("Started second iteration on the file: %s...\n", name);
    openFiles(name, &I_File, &listFile, &dataFile, &fileExt, flags); /* open all files */
    entryState(name, ptSymbol, &fileEnt, flags); /* If we have an entry in file */
    while (!feof(I_File)) /* going on file line by line */
	{	
        memset(theLine, 0, MAX_LINE_LENGTH);
        fgets(theLine, MAX_LINE_LENGTH, I_File); 
        lineCounter++; /* one more line saved */
        wordNow = strtok(theLine, " \t\n\v\f\r"); /* taking the first word in line */
        if (!wordNow || wordNow[0] == ';') /* line is empty - so continuing */
            continue;
        if (wordNow[strlen(wordNow) - 1] == ':') /* a label was found */
            wordNow = strtok(NULL, " \t\n\v\f\r,"); /* going to the next line */
        if (wordNow[0] == '.')  /* a directive was found */
            instInput(ptSymbol, wordNow, &DC, dataFile);
        else 
            sentenceEncode(ptSymbol, wordNow, &IC, listFile, fileExt); /* encoding the whole sentence to the unique base two */
    }
    printf("Second pass has been finished successfully.\n");
    FileOutput(name, listFile, dataFile); /* creating the obj file */
    secondPassExit(name, ptSymbol, I_File, fileEnt, fileExt, listFile, dataFile); /* closing and freeing all memory we have been using in second pass */
    return 0;
}
