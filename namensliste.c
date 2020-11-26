//
// Created by radiales on 26.11.20.
//

#include "namensliste.h"
#include "string.h"

extern tBez* currentList;
extern tMorph Morph;

tBez* suche()
{
    tBez* tmp = currentList;
    while (tmp->nxt != NULL)
    {
        if (strcmp(Morph.Val.pStr,tmp->pName) == 0)
        {
            return tmp;
        }
        tmp=tmp->nxt;
    }

    return NULL;
}