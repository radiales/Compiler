/* Kevin Holz, s77179, 041/61/17 */
#include "namensliste.h"
#include "string.h"

int constCounter = 0;
extern tBez* currentList;
extern tMorph Morph;
int procCounter = 0;
extern long* constblock;
tProc* procList;


tVar* CreateVar(void)
{
    tVar* newVar 		= malloc(sizeof(tVar));

    newVar->Kz 			= KzVar;
    newVar->Dspl 		= procList->SpzzVar;

    procList->SpzzVar	+=4;

    return newVar;
}

tConst* createConst(long Val)
{
    tConst* newConst = (tConst*)malloc(sizeof(tConst));

    newConst->Kz = KzConst;
    newConst->Val = Val;
    newConst->Idx = constCounter++;

    //Not sure if correct
    void* ret = realloc(constblock, sizeof(long) * constCounter);
    if(ret == NULL) printf("Realloc in const-Creation failed\n");
    constblock = ret;
    constblock[constCounter-1] = Val;

    return newConst;
}

tBez* createBez(char* pBez)
{

    tBez* newBez = malloc(sizeof(tBez));

    newBez->nxt 	= procList->pLBez;
    newBez->IdxProc = procCounter-1;
    newBez->Len 	= strlen(pBez);
    newBez->pName 	= (char*)malloc(newBez->Len + 1);
    strcpy(newBez->pName, pBez);
    newBez->pObj	= NULL;

    procList->pLBez = newBez;

    return newBez;
}

tProc* createProc(tProc* pParent)
{
    tProc* newProc = (tProc*)malloc(sizeof(tProc));

    ((tProc*)(newProc))->Kz 		= KzProc;
    ((tProc*)(newProc))->IdxProc 	= procCounter++;
    ((tProc*)(newProc))->pParent 	= pParent;
    ((tProc*)(newProc))->pLBez 		= NULL;
    ((tProc*)(newProc))->SpzzVar 	= 0;

    return newProc;
}

tBez* Search(char* name)
{

    if(procList == NULL) return NOTFOUND;


    tBez* start = procList->pLBez;
    tBez* tmp 	= start;
    tBez* tmp2  = tmp;

    while(tmp != NULL){
        if(strcmp(tmp->pName, name) == 0) return tmp;
        tmp = tmp->nxt;
    }

    return NOTFOUND;
}

tBez* SearchGlobal(char* name)
{
    tBez* tmp = NOTFOUND;
    tProc* procListCopy = procList;
    int isRoot = 0;

    do{
        tmp = Search(name);
        if(isRoot) break;
        procList = procList->pParent;
        isRoot = (procList == root);
    }while(tmp == NOTFOUND);

    procList = procListCopy;

    return tmp;
}

tBez* SearchByVal(int val)
{
    tBez* start = procList->pLBez;
    tBez* tmp 	= start;

    while(tmp != NULL){
        if(((tConst*)(tmp->pObj))->Val == val && tmp->Kz == KzConst) return tmp;
        tmp = tmp->nxt;
    }

    return NOTFOUND;
}

int NewVar(){

    if(Search(Morph.Val.pStr) != NOTFOUND){
        printf("Variable identifier '%s' already exists (Error in line %d)\n", Morph.Val.pStr, Morph.PosLine + 1);
        exit(FAIL);
    }

    tBez* newBezeichner = createBez(Morph.Val.pStr);
    newBezeichner->Kz = KzVar;



    procList->pLBez->pObj = CreateVar();


    return 1;
}

int NewConstBez(char* name)
{

    if(name == NULL && Search(Morph.Val.pStr) != NOTFOUND){
        printf("Constant identifier '%s' already exists (Error in line %d)\n", Morph.Val.pStr, Morph.PosLine + 1);
        exit(FAIL);
    }

    tBez* newBezeichner = NULL;
    if(name == NULL)
        newBezeichner = createBez(Morph.Val.pStr);
    else
        newBezeichner = createBez(name);
    newBezeichner->Kz = KzConst;



    return 1;
}

int NewConst(){
    procList->pLBez->pObj = createConst(Morph.Val.Num);
    return 1;
}

int newProc()
{

    //procList = procList->pParent;
    if(Search(Morph.Val.pStr) != NOTFOUND)
    {
        printf("Procedure identifier '%s' already exists (Error in line %d)\n", Morph.Val.pStr, Morph.PosLine + 1);
        exit(FAIL);
    }

    tBez* newBezeichner = createBez(Morph.Val.pStr);
    newBezeichner->Kz = KzProc;
    tProc* newProcedure = createProc(procList);
    newBezeichner->pObj = newProcedure;
    procList = newProcedure;
    return 1;
}

void newProg()
{
    tProc* newProcedure = root = createProc(NULL);
    procList = newProcedure;
}

int FreeDescriptions()
{
    tBez *start = procList->pLBez;
    tBez *tmp = start;
    tBez *tmpOld = start;

    while (tmp != NULL) {
        tmp = tmp->nxt;
        free(tmpOld->pObj);
        free(tmpOld);
        tmpOld = tmp;
    }

    tProc *oldProc = procList;
    procList = procList->pParent;
}


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