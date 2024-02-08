

#include "fileInput.h"

#define MAXINST 4

/*Gave numerical values to given addressing methods*/
typedef enum addressing {
    addressZero = 0,
    addressOne = 1,
    addressTwo = 2,
    addressThree = 3
} addressingMode;


/* Input: char * - the operand name , FILE * - the file to write all the label in the soure file , int - flag the represent if the operand is one of the label
*  Output: Checks the operand addressing and return the addressing type if it is an appropriate one otherwise return invailid operand addressing. 
*/ 
int instructionName(char *);


/* Input: char * - the operand name , FILE * - the file to write all the label in the soure file , int - flag the represent if the operand is one of the label
*  Output: Checks the operand addressing and return the addressing type if it is an appropriate one otherwise return invailid operand addressing. 
*/
addressingMode addressHandler(char *, FILE *, int);


