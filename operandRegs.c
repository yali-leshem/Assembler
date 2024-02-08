#include "fileInput.h"
#include "operandRegs.h"

static const struct regTypes {
    const char *reg;
	int num;
/* These are all the possible registers can be found */
} regs[] = {{"r7", 7},
			{"r6", 6},
			{"r5", 5},
			{"r4", 4},
			{"r3", 3},
			{"r2", 2},
			{"r1", 1},
			{"r0", 0}
}; 

int isNameCorrect(char *NameCorrect)
{
    int index;
    for (index = 0; index <= MAXREGS; index++)
	{
        if (strcmp(NameCorrect, regs[index].reg) == 0)
            return TRUE;
    }
    return FALSE;
}

int valueRegister(char *Name) 
{
    int index;
    for (index = 0; index <= MAXREGS; index++)
	{
        if (strcmp(Name, regs[index].reg) == 0)
            return regs[index].num;
    }
    return FALSE;
}
