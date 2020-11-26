//
// Created by radiales on 26.11.20.
//

#ifndef FINALLEXER_NAMENSLISTE_H
#define FINALLEXER_NAMENSLISTE_H

#endif //FINALLEXER_NAMENSLISTE_H
#include "stdio.h"
#include "stdlib.h"
#include "lex.h"
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
    long Val;
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

typedef struct
{
    tKz Kz;
    short IdxProc;
    void* pObj;
    int Len;
    char* pName;
}tBez;
tBez* suche();