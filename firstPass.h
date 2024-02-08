#include <stdio.h>
#include "fileInput.h"
#include "data.h"

#define FLAGS 5
enum flagsNames {
    ICnum, DCnum, enternal, external, err
};

/* Input: three FILE pointers  *Action: Closes all the file that it got and deletes the two last params file*/
void FileClosing(FILE *, FILE *, FILE *);
/* Input: FILE pointer to file with all the labels that have .entry declaration that  * in the source file and Symbol list pointer. * Output: Checks if all the labels with the .entry declaration that was written in the file are in */	
int checkLabel(FILE *, data *); 
/* Input: FILE pointer to file with all labels that 
* in the source file and Symbol list pointer.
* Output: Checks if all the labels in the file are in the Symbols list   */
int undefinedLabels(FILE *, data *);
/* Input : Symbol list pointer to the symbol list  , char * - the curr line in the code , int * DC - pointer to the Data conter , 
*FILE pointer to the file that has all the label with .entry direction , int -  the curr line in the input file , symbol list pointer where will be new instruction
*Action : taking care of the .entry directions
*Output: Checks if error occured through the procces*/
int entryInst(data *, char *, int *, FILE *, int , data *); 
/* Input : Symbol list pointer to the symbol list  , char * - the curr line in the code , int * DC - pointer to the Data conter , 
*FILE pointer to the file that has all the label with .extern direction , int -  the curr line in the input file , symbol list pointer where will be new instruction
*Action : taking care of the .extern directions
*Output: Checks if error occured through the procces*/
int externInst(data *, char *, int *, int , data *); 
/* Input : Symbol list pointer to the symbol list  , char * - the curr line in the code , int * DC - pointer to the Data conter , 
*FILE pointer to the file that has all the label with .string direction , int -  the curr line in the input file , symbol list pointer where will be new instruction.
*Action : Taking care of the .string directions
*Output: Checks if error occured through the procces*/
int stringInst(data *, char *, int *, int , data *);
/* Input : char * - command line string , int * - pointer to the IC  , FILE * - file to write al the label on and char[] - string that comntains the first operand.
*Action : Taking care of the first operand and checking if it appropriate operand  .
*Output: Checks if error occured through the procces*/
int dataInst(data *, char *, int *, int , data *); 
/* Input : char * - command line string , int * - pointer to the IC  , FILE * - file to write al the label on and char[] - string that comntains the first operand.
*Action : Taking care of the first operand and checking if it appropriate operand  .
*Output: Checks if error occured through the procces*/
int checkOneOp(char *, int *, FILE *, int , char []); 

/* Input : char * - command line string , int * - pointer to the IC  , FILE * - file to write al the label on and char[] - string that contains the two operands.
*Action : Taking care of the second operand and checking if it appropriate operand . 
*Output: Checks if error occured through the procces*/
int checksOps(char *, int *, FILE *, int , char []); 
/* Input : char * - command line string , int * - pointer to the IC  , FILE * - file to write al the label on and int - the num of the curent line in the source file
*Action : Taking care of the command sentence by checking if it contains an appropriate order.   
*Output: Checks if there are syntax errors (such as invailid order) through the procces */
int cmdStatement(char *, int *, FILE *, int ); 

/* Input : symbol list pointer , char * - the direction name , int * - pointer to the DC , FILE * - file with the label with the .entry direction 
*  int - the num of the curr line in the source file , symbol list pointer to the new symbol , int * - pointer to the flag of which label direction  (entry or extern)  
*Action : Taking care of the direction and checks if the direction is appropriate   
*Output: Checks if error occured through the procces*/
int instHandler(data *, char *, int *, FILE *, int , data *, int *);
/* Input : symbol list pointer , char * - the new label name , int  - the IC ,  
*  int - the num of the curr line in the source file , symbol list pointer to the new symbol , int * - pointer to the flag of which label direction  (entry or extern)  
*Action : Checking if the new label is appropriate label and add it to the symbol list   
*Output: Checks if error occured through the procces*/
int labelHandler(data *, char *, int , int , data *);
/*Input: char * - the name of the file we're testing , int * - pointer to the flag of which action had done
* Action: Passing on the Input file and checking if there are any Syntax Errors and building the symbol list 
* Output: Checks if through the all procces there are any error if there are no error returns the symbol list otherwise return NULL 
**/ 
data mainFirstPass(char *, int *); 

