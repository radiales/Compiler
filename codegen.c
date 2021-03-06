/* Kevin Holz, s77179, 041/61/17 */

#include "codegen.h"
#include "lex.h"
#include "namensliste.h"
#include "code.h"
#include <stdarg.h>
#include <string.h>

#define VCODE_DEFAULT_LEN 1024

long lastjnot;
long lastjnotparameter;
fxlist* lastjnotstack;
fxlist* lastjnotparameterstack;
FILE* pOFile;
char* pCode;
char* vCode;
int LenCode;
tLabl* LabelList;
int cmpSymb;
extern tMorph Morph;
extern tBez* currentList;
extern int constCounter;
extern int* constblock;
extern int procCounter;
extern FILE* pOFile;

void wr2ToCode(short x)
{
    *pCode++=(unsigned char)(x & 0xff);
    *pCode++=(unsigned char)(x >> 8);
}
void wr2ToCodeAtP(short x,char*pD)
{
    *pD    =(unsigned char)(x & 0xff);
    *(pD+1)=(unsigned char)(x >> 8);
}

int CodeOut(void)
{
    unsigned short Len=(unsigned short)(pCode-vCode);
    wr2ToCodeAtP((short)Len,vCode+1);
    wr2ToCodeAtP((short)procList->SpzzVar,vCode+5);
    if (Len==fwrite(vCode,sizeof(char),Len,pOFile)) return OK;
    else                                            return FAIL;
}

int code(tCode Code,...)
{
    va_list ap;
    short sarg;

    if (pCode-vCode+MAX_LEN_OF_CODE>=LenCode)
    {
        char* xCode=realloc(vCode,(LenCode+=1024));
        if (xCode==NULL) exit(ENoMem);
        pCode=xCode+(pCode-vCode);
        vCode=xCode;
    }

    *pCode++=(char)Code;
    va_start(ap,Code);

    switch (Code)
    {
        /* Befehle mit 3 Parametern */
        case entryProc:
            sarg=va_arg(ap,int);
            wr2ToCode(sarg);
            /* Befehle mit 2 Parametern */
        case puValVrGlob:
        case puAdrVrGlob:
            sarg=va_arg(ap,int);
            wr2ToCode(sarg);
            /* Befehle mit 1 Parameter */
        case puValVrMain:
        case puAdrVrMain:
        case puValVrLocl:
        case puAdrVrLocl:
        case puConst:
        case jmp :
        case jnot:
        case call:
            sarg=va_arg(ap,int); /* Prozedurnummer               */
            wr2ToCode(sarg);
            break;
            /* keine Parameter */
        default     : break;
    }
    va_end  (ap);
    return OK;
}

int pr1(){
    fseek(pOFile, 0, SEEK_SET);
    fwrite(&procCounter,sizeof(int),1,pOFile);
    fseek(pOFile, 0, SEEK_END);
    fwrite(constblock, sizeof(int), constCounter, pOFile);
    return 1;
}

int bl1(){
    NewConstBez(NULL);
    return 1;
}

int bl2(){
    NewConst();
    return 1;
}

int bl3(){
    NewVar();
    return 1;
}

int bl4(){
    newProc();
    return 1;
}

int bl5(){
    code(retProc);
    CodeOut();
    FreeDescriptions();
    return 1;
}

int bl6(){
    vCode = (char*)malloc(VCODE_DEFAULT_LEN);

    if(vCode == NULL){
        printf("Code buffer memory could not be allocated.\n");
        exit(ENoMem);
    }

    pCode = vCode;
    code(entryProc, 0, procList->IdxProc, procList->SpzzVar);
    return 1;
}

int st1(){
    tBez* tmp = SearchGlobal(Morph.Val.pStr);

    if(tmp == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(tmp->Kz != KzVar){
        printf("Identifier %s is a procedure or constant, needed: variable (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(procList == root)
        code(puAdrVrMain, ((tVar*)(tmp->pObj))->Dspl);
    else if(procList->IdxProc == tmp->IdxProc)
        code(puAdrVrLocl, ((tVar*)(tmp->pObj))->Dspl);
    else
        code(puAdrVrGlob, ((tVar*)(tmp->pObj))->Dspl, tmp->IdxProc);

    return 1;
}

int st2(){
    code(storeVal);
    return 1;
}

int st3(){
    tLabl* newLabel = (tLabl*)malloc(sizeof(tLabl));

    newLabel->nxt = LabelList;
    newLabel->iJmp = (long)((void*)(pCode) + 1);
    LabelList = newLabel;

    code(jnot, 0);

    return 1;
}

int st4(){
    tLabl* nxt = LabelList->nxt;
    long target = (long)(LabelList->iJmp);

    free(LabelList);
    LabelList = nxt;
    wr2ToCodeAtP((short)(pCode - target - 2), (char*)target);


    lastjnot = target;
    lastjnotparameter = (short)(pCode - target - 2);


    if(lastjnotstack == NULL) {
        lastjnotstack = malloc(sizeof(fxlist));
        lastjnotstack->instance = target;
    } else {
        fxlist* newEntry = malloc(sizeof(fxlist));
        fxlist* tmp = lastjnotstack;
        lastjnotstack = newEntry;
        lastjnotstack->prv = tmp;
        lastjnotstack->instance = target;
    }

    if(lastjnotparameterstack == NULL) {
        lastjnotparameterstack = malloc(sizeof(fxlist));
        lastjnotparameterstack->instance = target;
    } else {
        fxlist* newEntry = malloc(sizeof(fxlist));
        fxlist* tmp = lastjnotparameterstack;
        lastjnotparameterstack = newEntry;
        lastjnotparameterstack->prv = tmp;
        lastjnotparameterstack->instance = target;
    }

    lastjnotstack->instance = target;
    lastjnotparameterstack->instance = (short)(pCode - target - 2);



    return 1;
}

int st5(){
    tLabl* newLabel = (tLabl*)malloc(sizeof(tLabl));

    newLabel->nxt = LabelList;
    newLabel->iJmp = (long)((void*)(pCode) + 1);
    LabelList = newLabel;

    return 1;
}

int st6(){
    tLabl* newLabel = (tLabl*)malloc(sizeof(tLabl));

    newLabel->nxt = LabelList;
    newLabel->iJmp = (long)((void*)(pCode) + 1);
    LabelList = newLabel;

    code(jnot, 0);

    return 1;
}

int st7(){

    tLabl* nxt = LabelList->nxt;
    long target1 = (long)(LabelList->iJmp);

    free(LabelList);
    LabelList = nxt;

    wr2ToCodeAtP((short)(pCode - target1 + 1), (char*)target1);

    nxt = LabelList->nxt;
    long target2 = (long)(LabelList->iJmp);

    free(LabelList);
    LabelList = nxt;

    code(jmp, ~((long)pCode - target2) - 3);

    return 1;
}

int st8(){
    tBez* tmp = SearchGlobal(Morph.Val.pStr);

    if(tmp == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(tmp->Kz != KzProc){
        printf("Identifier %s is a variable or constant, needed: procedure (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    code(call, ((tProc*)(tmp->pObj))->IdxProc);

    return 1;
}

int st9(){

    tBez* tmp = SearchGlobal(Morph.Val.pStr);

    if(tmp == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(tmp->Kz != KzVar){
        printf("Identifier %s is a procedure or constant, needed: variable (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(procList == root)
        code(puAdrVrMain, ((tVar*)(tmp->pObj))->Dspl);
    else if(tmp->IdxProc == procList->IdxProc)
        code(puAdrVrLocl, ((tVar*)(tmp->pObj))->Dspl);
    else
        code(puAdrVrGlob, ((tVar*)(tmp->pObj))->Dspl, tmp->IdxProc);

    code(getVal);

    return 1;
}

int st10(){
    code(putVal);

    return 1;
}

int st11(){ // Neue fkt für else
    code(jmp,0);

    tLabl* newLabel = (tLabl*)malloc(sizeof(tLabl));

    newLabel->nxt = LabelList;
    newLabel->iJmp = (long)((void*)(pCode) - 2  );
    LabelList = newLabel;

    return 1;
}

int st12(){

    tLabl* nxt = LabelList->nxt;
    long target = (long)(LabelList->iJmp);

    free(LabelList);
    LabelList = nxt;

    wr2ToCodeAtP((short)(pCode - target - 2), (char*)target);
    //wr2ToCodeAtP((short)(lastjnotparameter + 3), (char*)lastjnot);

    wr2ToCodeAtP((short)((lastjnotparameterstack->instance) + 3), (char*)lastjnotstack->instance);

    fxlist* oldljnps = lastjnotparameterstack;
    fxlist* oldljns = lastjnotstack;

    lastjnotparameterstack = lastjnotparameterstack->prv;
    lastjnotstack = lastjnotstack->prv;

    free(oldljnps);
    free(oldljns);


    return 1;
}

int ex1(){
    code(vzMinus);
    return 1;
}

int ex2(){
    code(OpAdd);
    return 1;
}

int ex3(){
    code(OpSub);
    return 1;
}

int te1(){
    code(OpMult);
    return 1;
}

int te2(){
    code(OpDiv);
    return 1;
}

int fa1(){
    tBez* tmp = SearchByVal(Morph.Val.Num);

    char name[1024];
    sprintf(name, "c%ld", Morph.Val.Num);

    if(tmp == NOTFOUND){
        NewConstBez(name);
        NewConst();
        tmp = procList->pLBez;
    }

    code(puConst, ((tConst*)(tmp->pObj))->Idx);
    return 1;
}

int fa2(){
    tBez* bez = SearchGlobal(Morph.Val.pStr);

    if(bez == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine + 1);
        exit(FAIL);
    } else if(bez->Kz == KzProc){
        printf("Identifier %s is a procedure, needed: variable or constant (line: %d)\n", Morph.Val.pStr, Morph.PosLine + 1);
        exit(FAIL);
    }

    if(bez->Kz == KzConst){
        code(puConst, ((tConst*)(bez->pObj))->Idx);
    } else{
        if(procList == root){
            code(puValVrMain, ((tVar*)(bez->pObj))->Dspl);
        } else if(procList->IdxProc == bez->IdxProc){
            code(puValVrLocl, ((tVar*)(bez->pObj))->Dspl);
        } else{
            code(puValVrGlob, ((tVar*)(bez->pObj))->Dspl, bez->IdxProc);
        }
    }

    return 1;
}

int co1(){
    code(odd);
    return 1;
}

int co2(){
    cmpSymb = '=';

    return 1;
}

int co3(){
    cmpSymb = '#';

    return 1;
}

int co4(){
    cmpSymb = '<';

    return 1;
}

int co5(){
    cmpSymb = zLE;

    return 1;
}

int co6(){
    cmpSymb = '>';

    return 1;
}

int co7(){
    cmpSymb = zGE;
}

int co8(){
    switch(cmpSymb){
        case '=': code(cmpEQ); break;
        case '#': code(cmpNE); break;
        case '<': code(cmpLT); break;
        case zLE: code(cmpLE); break;
        case '>': code(cmpGT); break;
        case zGE: code(cmpGE); break;
    }

    return 1;
}
