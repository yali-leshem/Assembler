
#include "operandRegs.h"
#include "DirectAddress.h"
#include "firstPass.h"
#include <stdio.h>



/* Input: char * - the value to convert , int - flag that represent if it's a string or number , FILE * - the pointer to the file where we'll write the data 
          int * - the DC pointer.
   Action: Convert the given string to binary 14-bits word.  
*/
void uniqueBaseTwo(char *, int , FILE *, int *);


/* Input: int - the value to convert , int - the AER field , FILE * - the pointer to the file where we'll write the data 
          int * - the IC pointer.
   Action: Convert the given int to binary 14-bits word.  
*/
void convertOpToUniqueBaseTwo(int , int , FILE *, int *);


/* Input: int - the opcode of a given command , FILE * - pointer to the file wher ewe'll right data we want to save to usr later
   Action: Writing the opcode to the file.   
*/
void Opcode(int , FILE *);

/* Input: int - the type of the encode , FILE * - pointer to the file wher ewe'll right data we want to save to usr later
   Action: Writing the encode to the file.   
*/
void secondBits(int , FILE *);

/* Input: int - the first oprand that being encoded,int - the sec oprand that being encoded , int - the order encode, int - the addres of the first  operand  ,int - the addres of the sec  operand , int - the type of encodeeing (A/E/R field),FILE * - pointer to the file wher ewe'll right data we want to save to usr later
   Action: Writing the codeing word to the file.   
*/
void oneWordTyped(int , int , int , int , int , int , FILE *,int *IC);


/* Input: char * - the data that we are encoding , int * - DC pointer, FILE * - pointer to the file wher ewe'll right data we want to save to usr later
   Action: Writing the encode to the file.   
*/
void dataEncode(char *, int *, FILE *);


/* Input: char * - the string that we are encoding , int * - DC pointer, FILE * - pointer to the file where we'll right data we want to save to use later
   Action: Writing the encode to the file.   
*/
void stringEncode(char *, int *, FILE *);

/* Input: symbol list pointer , char * - name of order , int * - DC pointer , FILE * - pointer to the file where we'll right data we want to save to use later.  
   Action: Marking the corresponding what encode method we're using.  
*/
void instInput(data *, char *, int *, FILE *);

/* Input: symbol list pointer , char * - name of order , int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later ,
	  FILE * - pointer to the file where we write the extern labels.  
   Action: Encoding all the orders that works on one operand.  
*/
void singleOpEncode(data *, char *, int *, FILE *, FILE *);

/* Input: symbol list pointer , char * - name of order , int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later ,
	  FILE * - pointer to the file where we write the extern labels.  
   Action: Encoding all the orders that works on two operand.  
*/
void TwoOpsEncode(data *, char *, int *, FILE *, FILE *);

/* Input: symbol list pointer , char * - name of order , int * - IC pointer , the first operand addreseing mode , the sec operand addreseing mode
char * the fist operand name , char * - the sec operand name , 
FILE * - pointer to the file where we'll right data we want to save to use later , FILE * - pointer to the file where we write the extern labels.  
   Action: Taking Care of order that are being done with the 2 addresings.  
*/
void twoAddresses(data *, char *, int *, addressingMode , addressingMode ,char *, char *, FILE *, FILE *);

/* Input: char * - operand value , int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later/ 
   Action: Taking Care of mode 0 addressing method (immediate) operand.  
*/
void ImmediateAddress(char *, int *, FILE *);

/* Input: symbol list pointer, int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later/ 
   Action: Taking Care of mode 3 addressing method (direct) operand.  
*/

void directAddress(data *, char *, int *, FILE *, FILE *);


/* Input: the operand addressing type,symbol list pointer, char * - operand name,int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later , FILE * - pointer to the file where we'll write the external labels , int - flag to which operand (sec/first)  
   Action: Taking Care of address and the matching method .  
*/
void handleAddress(addressingMode , data *, char *, int *, FILE *, FILE *, int );

/* Input: tsymbol list pointer, char * - order to encode ,int * - IC pointer , FILE * - pointer to the file where we'll right data we want to save to use later , FILE * - pointer to the file where we'll write the external labels .  
   Action: Taking Care of everyline where there is an order by using the method that checks how many operands the order use.  
*/
void sentenceEncode(data *, char *, int * , FILE *, FILE *);

/* Input: char * - the file name , symbol list pointer , FILE * - the source file , FILE  * - the pointer to the file where we 
	  will write the entry labels ,   FILE  * - the pointer to the file where we will write the extern labels , 
	  FILE  * - the pointer to the file where we will write the srtuctions data  ,FILE  * - the pointer to the file where we will write the data.      
   Action: Taking Care of the finals of the sec assembler pass by closing files free all the used memo and delete the additinal files.  
*/
void secondPassExit(char *, data *, FILE *, FILE *, FILE *,FILE *, FILE *);


/* Input: char * - the file name  ,  FILE  * - the pointer to the file where we will write the srtuctions data  
	  ,FILE  * - the pointer to the file where we will write the data.      
   Action: Aranging all the data in the two file into one file (ob file)   
*/
void FileOutput(char *, FILE *, FILE *);

/* Input: char * - the file name  , FILE ** - pointer to the pointer of the source file, FILE  ** - the pointer to the pointer of file where we will write the 		          srtuctions data  ,FILE  ** - pointer to the pointer to the file where we will write the data.      
   Action: Open all the file that used in the sec pass.   
*/
void openFiles(char *, FILE **, FILE **, FILE **, FILE **,int *);

/* Input: char * - the file name  ,  symbol list pointer , FILE ** - the pointer to the pointer of the entry file 
	, int * - flag of what type    
   Action: Taking care of the entry file writing   
*/
void entryState(char *, data *, FILE **, int *);

/* Input: char * - the file name  , symbol list pointer , int * - the flag.     
   Action: Writing all the assembly in the second pass/ 
*/
int secondPassMain(char *, data *, int *);






