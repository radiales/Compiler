/* Kevin Holz, s77179, 041/61/17 */

#ifndef FINALLEXER_LEX_H
#define FINALLEXER_LEX_H

#endif //FINALLEXER_LEX_H

#pragma once

typedef enum T_Fx { ifl , ifb, ifgl, ifsl, ifslb, ifel } tFx;

/* Morphemcodes */
typedef enum T_MC { mcEmpty, mcSymb, mcNum, mcIdent, mcStrng } tMC;

typedef enum T_ZS
{
    zNIL,
    zERG = 128, zLE, zGE,
    zBGN, zCLL, zCST, zDO, zEND, zIF, zODD, zPRC, zTHN, zVAR, zWHL, zELS
}tZS;
// Else hinzugefügt in ENUM

typedef struct
{
    tMC  MC;		/* Morphemcode */
    int  PosLine;   /* Zeile       */
    int  PosCol;	/* Spalte      */
    union VAL
    {
        long Num;
        char* pStr;
        int  Symb;
    }Val;
    int  MLen;      /* Morphemlänge*/
}tMorph;

typedef struct
{
    int NextState;  //nächster Zustand
    tFx Funk;       //zu ausführende Funktion
}tState;

int initLex(char* fname);

extern tMorph* Lex(void);
