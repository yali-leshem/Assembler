#include "action.h"
#include "fileInput.h"

static const struct operationConsts {
    const char *command;
    int opcode;
    int numOfOperation;
    int startOp;
    int endOp;
} 
actionsCons[] = {{"mov",  0,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"cmp",  1,  2, ALL_OPERATION,     ALL_OPERATION},
                 {"add",  2,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"sub",  3,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"not",  4,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"clr",  5,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"lea",  6,  2, ONE_TWO_OPERATION, WITHOUT_FIRST_OPERATION},
                 {"inc",  7,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"dec",  8,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"jmp",  9,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"bne",  10, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"red",  11, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"prn",  12, 1, 0,                 ALL_OPERATION},
                 {"jsr",  13, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"rts",  14, 0, 0, 0},
                 {"stop", 15, 0, 0, 0}}; /* actions conditions - represnting all legal actionames - legal parameters - and sources and destination adresses (check for errors by that) */

int isOpNameCorrect (char *cmd)
{
    int index;
    for (index = 0; index < MAXACTIONS; index++)
	{
        if (!strcmp(cmd, actionsCons[index].command))
            return TRUE;
    }
    return FALSE;
}

int codeOfAction(char *cmd)
{
    int index;
    for (index = 0; index < MAXACTIONS; index++)
	{
        if (!strcmp(cmd, actionsCons[index].command))
            return actionsCons[index].opcode;
    }
    return NOP;
}

int actionNum(char *cmd)
{
    int index;
    for (index = 0; index < MAXACTIONS; index++)
	{
        if (strcmp(cmd, actionsCons[index].command) == 0)
            return actionsCons[index].numOfOperation;
    }
    return NOP;
}
