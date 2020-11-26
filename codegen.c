//
// Created by radiales on 26.11.20.
//
#include "lex.h"
#include "namensliste.h"
#include <string.h>


extern tMorph Morph;
extern tBez* currentList;
extern int constCounter;

int st1()
{

    tBez* tmp;
    if (suche(Morph) != NULL)
    {
        printf("ST1 Bezeichner schon Vorhanden\n");
        exit(1);
    }

    currentList->nxt = malloc(sizeof(tBez));    // iRGENDWAS MIT HEAP
    currentList->nxt->pName = Morph.Val.pStr;
    currentList->nxt->IdxProc = currentList->IdxProc;
    currentList->nxt->Kz = currentList->Kz+1;
    currentList->nxt->Len = strlen(currentList->pName);
    tConst* ttmp = malloc(sizeof(struct tCONST));
    currentList->nxt->pObj = ttmp;
    ttmp->Kz = 0;
    ttmp->Idx = constCounter++;

}

int st2()
{

}

int st3()
{

}