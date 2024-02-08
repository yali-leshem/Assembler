

#include <stdio.h>
#include <string.h>
#define MAX12B 4095
#define MIN12B -4095
#define NOP 0 /*does not existence*/
#define ABLED 1
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30
#define TRUE 1
#define FALSE 0
#define MAX14B 16383
#define MIN14B -16383
#define MAX_FILE_LEN 255


/* Input: char * - the file original name , char * - the file's ending (.am , .as , .ob ,...) , char * - the full file name string .
*  Action: Adding the ending to the file original name to the full file name.
*/
void FileWithEnding(char *, char *, char *);


/* Input: char * - the file original name , char * - the file's ending , char * - word that represent the way to open the file.
*  Output: Return FILE pointer of the file to open
*/

FILE * openFile(char *, char *, char *);

/* Input: char * - the file original name , char * - the file's ending 
   Action: Delete the file that the method get from the folder*/

void removeFile(char *, char *);

/* Input: char [] - a given char array (string) . 
   Action: Removing all the white spaces from the char array ending.
*/
void LeftSpaces(char [MAX_LINE_LENGTH]);


/* Input: char [] - a given char array (string)/
   Action: Removing all the white spaces from the very start of the array 
*/
void RightSpaces(char [MAX_LINE_LENGTH]);

/* Input: char [] - char array to copy to , char * - char array to copy from
   Output: Check if its possible to copy the sec string to the other.
*/
int copy(char [MAX_LINE_LENGTH], char *);

/* Input: char * - the error that was found , int - the line in the file were the error was discovered.
   Action: Prints if necessary the error that was found and it's line. 
*/
void findError(char *, int );

/* Input: char * - the warning that was found , int - the line in the file were the warning was discovered.
   Action: Prints if necessary the warining that was found and it's line. 
*/
void findWarning(char *, int );

/* Input: char * - the string to chek if its a valid (action,register,direction...)
   Output: Checks if the string that was given is valid (action,register,direction...)
*/
int savedWord(char *);

/* Input: char [] - the char array to exam , int - the flag 
   Output: Checks if the string that was given is an appropriate number in the assembler settings*/
int validNumber(char [MAX_LINE_LENGTH], int *);

/* Input: char * - label name to check
*  Output: Checks if the label name that was given is an appropriate name in the assmebler settings.
*/
int labelName(char *);

/* Input: char * - a given str , char [] - the string we put the data in after adding the spaces.
   Action: Copying string to other string without with spaces but, adding a space in the appropriate place after the order.
   Output: Check if the new string wasn't NULL
*/
int spaceJump(char *, char [MAX_LINE_LENGTH * 2]);


