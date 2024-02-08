#include "DirectAddress.h"
#include "operandRegs.h"

static const struct directiveStatement {
    const char *name;
/*Names of all directive statements */
} directiveStatements[] = {
        {".data"},
        {".string"},
        {".entry"},
        {".extern"},
};

addressingMode addressHandler(char *op, FILE *labelsFile, int flag) 
{
    int index = 0;
    if (op != NULL && op[0] != '\0') {
        if (validNumber(op, FALSE))
            return addressZero;
        if (isNameCorrect(op))/* means the addressing is direct here */
            return addressThree;
        if (labelName(op) && (flag)) { /*found a label so redirecting is done */
                fprintf(labelsFile, "%s\n", op);
            return addressOne;
        }/* when the first operand begins with a '#' - it is immediate address (addressing zero) */
        if (op[index] == '#') {
            for (index = 0; index < strlen(op) - 1; ++index)
                op[index] = op[index + 1];
            op[strlen(op) - 1] = '\0';
            if (validNumber(op, FALSE))
                return addressZero;
        }
    }
    return NOP; /*if nothing was found, return NON  */
}


int instructionName(char *instr) {
    int index;
    for (index = 0; index <  MAXINST; index++) {
        if (strcmp(instr, directiveStatements[index].name) == 0) /* If an instruction has been found - return true */
            return TRUE;
    }
    return FALSE; /* Instruction wasn't found */
}
