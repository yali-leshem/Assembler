

#define MAXACTIONS 16
#define ALL_OPERATION 4
#define ONE_TWO_OPERATION 2
#define WITHOUT_FIRST_OPERATION 3

/* Input: char * - the order name .
   Output: Checks if the string that was given is an appropriate order .  
*/
int isOpNameCorrect(char *command);

/* Input: char * - the order name .
   Output: Retun the opcode of the order name .  
*/

int codeOfAction(char *command);

/* Input: char * - the order name .
   Output: Retun the number of the order name  (0-15).  
*/
int actionNum(char *command);



