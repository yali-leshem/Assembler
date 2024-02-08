

typedef struct linked *data;
typedef enum Type { /* type of different directives may be found in file */
    operate,
    DATA, 
    ENTRY,
    EXTERNAL
} Type;


/* Input: char * - the new symbol's name , int - the new symbol's address
   Output: Return the new symbol block . 
*/
data createLinked(char *, int );


/* Input:  the pointer to the symbols list ,  the new symbol block to add
   Action: Add the symbol to add to the end of the list . 
*/
void InsertLinked(data *, data );


/* Input: symbols list , char * - the name of the list we're looking for
   Output: Return the symbol we're looking for.
*/
data SearchLink(data , char *);


/* Input: symbols list , int - the pass we're in (first or second)
   Action: Prints all the symbols we did't use in the assembly code file.
*/
void allUsed(data , int );


/* Input: the pointer to the begining of the symbols list
   Action: releases the first block of the symbols list and promoting to the next 
*/
void moveHead(data *);


/* Input: a given list 
   Output: Return the name of the block in the list 
*/
char *getName(data );

/* Input: a given list 
   Output: Return the address of the block in the list 
*/
int getAddress(data );


/* Input: a given list 
   Output: Return the type of the block in the list 
*/
Type getType(data );

/* Input: a given list 
   Output: Return the next node in the list 
*/
data getNext(data );

/* Input: a given list , int - the address to add to the node 
   Action: set the address to the node . 
*/
void setAddress(data , int );
/* Input: a given list , int - flag that represent if the node was used.
   Action: set the node used field .  
*/
void setUsed(data , int );

/* Input: a given list , Type - type of the represent node.
   Action: set the node type field .  
*/
void setType(data , Type );

/* Input: a given list .
   Action: free the given block from the memory .  
*/
void freeLinked(data );

/* Input: a given list .
   Action: free all the given list from the memory .  
*/
void freelist(data );

/* Input: a given list pointer , int - the IC .
   Action: update the all the addresses by the IC value (only for the memory labels)  .  
*/
void update_IC_symbols(data *, int);



