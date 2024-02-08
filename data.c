#include "data.h"
#include "fileInput.h"
#include <stdlib.h>

struct linked{
    char *name;
    int startId;
    int endId; 
    int used;
    int address; /* Address of the symbol */
    Type type;/* Symbol type */
    data next;  /* Next linked */
};

data createLinked(char *name, int address)
{
    data new_general_linked = (data) calloc(1, sizeof(struct linked));
    new_general_linked->name = (char *) calloc(1, (strlen(name) + 1) * sizeof(char));
    strcpy(new_general_linked->name, name);
    new_general_linked->startId = 0;
    new_general_linked->endId = 0;
    new_general_linked->used = 0;
    new_general_linked->address = address;
    new_general_linked->next = NULL;
    new_general_linked->type = operate;
    return new_general_linked;
}

void InsertLinked(data *head, data new)
{
    if (!(*head)) /* if the list is empty */
        *head = new;
    else {
        /*Make next of new node as head*/
        new->next = (*head);
        /*move the head to point to the new node*/
        (*head) = new;
    }
}

data SearchLink(data linked, char *theName)
{
    data temp;
    if (linked != NULL) {
        temp = linked;
        if (linked != NULL) {
            if ((theName != NULL) && strcmp(linked->name, theName) == 0)
                /*if exist, return it*/
                return temp;
            return SearchLink(linked->next, theName);/*recursive call to check next node*/
        }
    }
    return NULL;
}

void allUsed(data linked, int phase)
{
    int flag = TRUE;
    while (linked != NULL)
	{
        if (linked->used != TRUE)
		{ 	/* found an unused link */
            if (flag) 
			{
                if (phase == 0) /* in pre assembler currently */
                    printf("Warning occurred at the process, macros we didn't use: '%s'", linked->name);
                else/*message belonging to the phase one*/
                    printf("Warning occurred at the process, symbols we didn't use: '%s'", linked->name);
                flag = FALSE;
            } else
                printf(",'%s'", linked->name);/*Printing another unused name*/
        }
        linked = linked->next;/* move to check the next node*/
    }
    if (!flag)
        printf("\n");
}

void moveHead(data *head)
{
    data temp = *head;
    *head = (*head)->next;
    freeLinked(temp);
}

char *getName(data linked)
{
    return linked->name;
}

int getAddress(data linked)
{
    return linked->address;
}

Type getType(data linked)
{
    return linked->type;
}

data getNext(data linked)
{
    return linked->next;
}

void setUsed(data linked, int used)
{
    linked->used = used;
}

void setAddress(data linked, int address)
{
    linked->address = address;
}

void setType(data linked, Type type)
{
    linked->type = type;
}

void freeLinked(data linked)
{
    free(linked->name);
    free(linked);
}

void freelist(data head)
{
    data prev, current = head;
    while (current != NULL)
	{
        prev = current;
        current = current->next;
        freeLinked(prev);
    }
}

void update_IC_symbols(data *linked, int IC)
{
    data temp = *linked;
    while (temp != NULL)
	{
        if (temp->type == DATA)/* if it's a data instruction */
            temp->address += IC;/* updates the IC counter */
        temp = temp->next;
    }
}

