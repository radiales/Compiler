/* Kevin Holz, s77179, 041/61/17 */

#include "codegen.h"
#include "lex.h"
#include "namensliste.h"
#include "codegen.h"
#include <string.h>


extern tMorph Morph;
extern tBez* currentList;
extern int constCounter;
extern long* constblock;
extern FILE* pOFile;

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

    return 1;
}

int st2(){
    tBez* tmp = SearchGlobal(Morph.Val.pStr);

    if(tmp == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(tmp->Kz != KzProc){
        printf("Identifier %s is a variable or constant, needed: procedure (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    return 1;
}

int st3(){

    tBez* tmp = SearchGlobal(Morph.Val.pStr);

    if(tmp == NOTFOUND){
        printf("Identifier %s not found (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    if(tmp->Kz != KzVar){
        printf("Identifier %s is a procedure or constant, needed: variable (line: %d)\n", Morph.Val.pStr, Morph.PosLine);
        exit(FAIL);
    }

    return 1;
}

int bl1()
        {
        NewConstBez(NULL);
        return 1;
        };

int bl2()
{
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
    FreeDescriptions();
    return 1;
}

int bl6(){

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

    return 1;
}

int pr1(){

    fseek(pOFile, 0, SEEK_END);
    fwrite(constblock, sizeof(long), constCounter, pOFile);

    return 1;
}