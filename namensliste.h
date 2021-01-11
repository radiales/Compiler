/* Kevin Holz, s77179, 041/61/17 */
#define FAIL -1
#define OK 0
#define NOTFOUND 0
#ifndef FINALLEXER_NAMENSLISTE_H
#define FINALLEXER_NAMENSLISTE_H
#endif //FINALLEXER_NAMENSLISTE_H
#include "stdio.h"
#include "stdlib.h"
#include "lex.h"


typedef enum KZs{
    KzProc, KzConst, KzVar
} Kzs;

typedef int tKz;
typedef struct TBez tBez;


typedef struct TBez{
    tKz     Kz;
    tBez*   nxt;
    short   IdxProc;
    void*   pObj;
    int     Len;
    char*   pName;
} tBez;

typedef struct tVAR
{
    tKz Kz;
    int Dspl; // Relativadresse
}tVar;

typedef struct tCONST
{
    tKz Kz;
    int Val;
    int Idx;
}tConst;

typedef struct tPROC
{
    tKz Kz;
    short IdxProc;
    struct tPROC*pParent;
    tBez *pLBez;
    int SpzzVar;
}tProc;

tProc* root;
tProc* procList;
tVar* CreateVar(void);
tConst* createConst(int Val);
tBez* createBez(char* pBez);
tProc* createProc(tProc* pParent);
tBez* Search(char* name);
tBez* SearchGlobal(char* name);
tBez* SearchByVal(int val);
int NewVar();
int NewConstBez(char* name);
int NewConst();
int newProc();
void newProg();
int FreeDescriptions();