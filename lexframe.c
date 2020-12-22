/* Kevin Holz, s77179, 041/61/17 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

static FILE *pIF;
static tMorph MorphInit;            // alles mit 0 belegt
tMorph Morph;                       // globale Variable f. Akt. Token
static int X;                       // Eingabezeichen
static int Z;                       // Aktueller Zustand
static char vBuf[1024+1];           // Buffer zum Sammeln
static char* pBuf;                  // Pointer in den Buffer
static int line,col;                // Zeile und Spalte
static int Ende;                    // Entfällt, wenn Zustand 9 → Ende

const char* Keyw[] = {                                                      // Die Keywords von pl0
        ":=", "<=" , ">=","BEGIN","CALL","CONST","DO","END","IF","ODD",
        "PROCEDURE","THEN","VAR","WHILE"
};
int kn = sizeof(Keyw)/sizeof(char*);                                        // Berechnet die Größe des Arrays

/*---- Initialisierung der lexiaklischen Analyse ----*/

int initLex(char* fname)
{
/*
    pIF=fopen(fname,"r+t");
    if (pIF) X=fgetc(pIF);
    Morph.MC=mcEmpty;
    return (int)pIF;
*/

    char vName[128 + 1];
    strcpy(vName, fname);
    if (strstr(vName, ".pl0") == NULL) strcat(vName, ".pl0");
    pIF = fopen(vName, "r+t");
    if (pIF != NULL) { X = fgetc(pIF); return 0; }
    return -1;

}





/* Zeichenklassenvector */                                                     //Wandelt eine EIngabe in Ascii um und in dieser Tabelle ist dann eine Kategoriesierung dazu

//
static char vZKl[128] =
/*     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*---------------------------------------------------------*/
/* 0*/{7, 7, 7, 7, 7, 7, 7, 7, 7 ,7 ,7 ,7 ,7 ,7 ,7 ,7,/*15*/
/*16*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/*31*/
/*32*/ 7, 0, 0, 0, 0, 0, 0, 0, 8, 9,10, 0, 0, 0, 0, 0,/*47*/
/*48*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 4, 6, 0,/*63*/
/*64*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*79*/
/*80*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,/*95*/
/*96*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*111*/
/*112*/2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0 }/*127*/;



/* 0 == Sonderzeichen
 * 1 == Ziffern
 * 2 == Buchstaben
 * 3 == ':'
 * 4 == '='
 * 5 == '<'
 * 6 == '>'
 * 7 == Müll
 * 8 == '('
 * 9 == ')'
 * 10== '*'
 */

/* Automatentabelle */


////vSMatrix[Zustand][Gelesenes Zeichen] [nähster Zustand, Funktion]
//tState vSMatrix[9][8][2] ={
///*          So           Zi          Bu            ':'           '='           '<'          '>'           Space
///*-----------------------------------------------------------------------------------------------------------------*/
///* Z0 */ {{9 , ifslb}, {1 , ifsl},  {2 , ifgl},   {3, ifsl},   {9, ifslb },   {4, ifsl},    {5, ifsl},    {0, ifl}} ,
///* Z1 */ {{9 , ifb},   {1 , ifsl},  {9 , ifb},    {9, ifb},    {9, ifb},      {9, ifb},     {9, ifb},     {9, ifb}} ,
///* Z2 */ {{9 , ifb},   {2 , ifsl},  {2 , ifgl},   {9 , ifb},   {9 , ifb},     {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z3 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {6 , ifsl},    {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z4 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {7 , ifsl},    {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z5 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {8 , ifsl},    {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z6 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {9 , ifb},     {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z7 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {9 , ifb},     {9 , ifb},    {9 , ifb},    {9 , ifb}} ,
///* Z8 */ {{9 , ifb},   {9 , ifb},   {9 , ifb},    {9 , ifb},   {9 , ifb},     {9 , ifb},    {9 , ifb},    {9 , ifb}}
//};


//vSMatrix[Zustand][Gelesenes Zeichen] [nähster Zustand, Funktion]
tState vSMatrix[13][11][2] ={
/*          So           Zi          Bu            ':'           '='           '<'          '>'           Space      |    '('             ')'         *
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Z0 */ {{12 , ifslb}, {1 , ifsl},  {2 , ifgl},   {3, ifsl},   {12, ifslb },  {4, ifsl},   {5, ifsl},     {0, ifl}   ,{9, ifsl} ,   {12, ifslb},  {12, ifslb}}, // '('
/* Z1 */ {{12 , ifb},   {1 , ifsl},  {12 , ifb},   {12, ifb},   {12, ifb},     {12, ifb},   {12, ifb},     {12, ifb}  ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z2 */ {{12 , ifb},   {2 , ifsl},  {2 , ifgl},   {12 , ifb},  {12 , ifb},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z3 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {6 , ifsl},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z4 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {7 , ifsl},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z5 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {8 , ifsl},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z6 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {12 , ifb},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z7 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {12 , ifb},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z8 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {12 , ifb},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {12 , ifb}},
/* Z9 */ {{12 , ifb},   {12 , ifb},  {12 , ifb},   {12 , ifb},  {12 , ifb},    {12 , ifb},  {12 , ifb},    {12 , ifb} ,{12 , ifb},  {12 , ifb},   {10,ifel}},
/*Z10 */ {{10, ifl},    {10, ifl},   {10, ifl},    {10, ifl},   {10, ifl},     {10, ifl},   {10, ifl},     {10, ifl}  ,{10,ifl},    {10, ifl} ,   {11,ifl}},
/*Z11 */ {{10, ifl},    {10, ifl},   {10, ifl},    {10, ifl},   {10, ifl},     {10, ifl},   {10, ifl},     {10, ifl}  ,{10,ifl},    {0, ifl}  ,   {12,ifl}}

};

/*
Schreiben, lesen, beenden       void fslb(void);
Schreiben, lesen                void fsl(void);
Schreiben Großbuchstabe,lesen   void fgl(void);
Beenden                         void fb(void);
Lesen                           void fl(void);
*/


//Liefert Index, falls gefunden, sonst -1
int binary_search(const char** M, int n, const char* X) //(Array mit Schlüsselwörtern, Anzahl Schlüsselwörter, Gesuchter Wert)
{
    unsigned mitte;
    unsigned links = 0;                                 // Mann beginnt mit dem kleinsten Index
    unsigned rechts = n - 1;
    int ret = -1;
    int bool;
    do
    {
        if (n <= 0) break;
        mitte = links + ((rechts - links) / 2);         // Bereich halbieren
        if (rechts < links) break;                      // alles wurde durchsucht, X nicht gefunden
        bool = strcmp(M[mitte], X);
        if (bool == 0) ret = mitte; else                // Element X gefunden?
        if (bool > 0) rechts = mitte; else              // im linken Abschnitt weitersuchen
            links = mitte + 1;                          // im rechten Abschnitt weitersuchen
        n = (n) / 2;
    } while (bool != 0);
    return ret;
}

/* Ausgabefunktionen des Automaten */
static void fl(void)
{
    X = fgetc(pIF);
    if (X == '\n') line++, col = 0;
    else col++;
};
static void fb(void)
{
    int i, //Index in Keyw[]
    j;
    switch (Z)
    {
        /* Symbol */
        case 3: // :
        case 4: // <
        case 5: // >
        case 9: // (
        case 0:Morph.Val.Symb = vBuf[0];
            Morph.MC = mcSymb;
            break;
            /* Zahl */
        case 1:Morph.Val.Num = atol(vBuf);
            Morph.MC = mcNum;
            break;

        case 2: i = binary_search(Keyw, kn, vBuf);
            if (i >= 0) {
                Morph.MC = mcSymb;
                Morph.Val.Symb = i + 128;                   //mit 128 beginnt in tZS das zERG
            }
            else
            {
                Morph.Val.pStr = vBuf;
                Morph.MC = mcIdent;
            }
            break;
            /* Ergibtzeichen */
        case 6:Morph.Val.Symb = (long)zERG;
            Morph.MC = mcSymb;
            break;
            /* KleinerGleich */
        case 7:Morph.Val.Symb = (long)zLE;
            Morph.MC = mcSymb;
            break;
            /* GroesserGleich */
        case 8:Morph.Val.Symb = (long)zGE;
            Morph.MC = mcSymb;
            break;
        case 10: Morph.MC = mcEmpty; // *
        case 11: // )
            break;


    }
    Ende = 1; // entfällt bei Variante mit Zustand zEnd
};


static void fgl(void)
{
    *pBuf = (char)toupper(X);// oder *Buf=(char)X&0xdf;
    *(++pBuf) = 0;
    fl();
};
static void fsl(void)
{
    *pBuf = (char)X;
    *(++pBuf) = 0;
    fl();
};
static void fslb(void)
{
    fsl(); fb();
};

static void fel(void){ // Um das klammer zu zu flushen da es sonst zu fehlern kommmt
    pBuf--;
    *pBuf = 0;
    fl();
}


void startFunction(int f) {

    switch (f)
    {
        case ifl: fl(); break;
        case ifb: fb(); break;
        case ifgl: fgl(); break;
        case ifsl: fsl(); break;
        case ifslb: fslb(); break;
        case ifel: fel(); break;
    }

}

/*---- Lexikalische Analyse ----*/


tMorph* Lex(void)
{
    Z = 0; // Anfangszustand, wenn nicht aus Tabelle
    int zx;

    Morph = MorphInit;
    Morph.PosLine = line;
    Morph.PosCol = col;
    pBuf = vBuf;

    do
    {
        zx = vSMatrix[Z][vZKl[X & 0x7f]][0].NextState;
        startFunction(vSMatrix[Z][vZKl[X & 0x7f]][0].Funk);
        Z = zx;

    } while (Z != 12); // (Z!=zEnd) // zEnd:9
    return &Morph;
}