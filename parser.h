/* Kevin Holz, s77179, 041/61/17 */

#ifndef FINALLEXER_PARSER_H
#define FINALLEXER_PARSER_H
#endif //FINALLEXER_PARSER_H

typedef enum BOGEN_DESC
{   BgNl= 0,    // NIL
    BgSy= 1,    // Symbol
    BgMo= 2,    // Morphem
    BgGr= 4,    // Graph
    BgEn= 8,    // Graphende
}tBg;


typedef struct BOGEN
{
    tBg BgD;
    union BGX
    {
        unsigned long X;
        int
                S;
        tMC
                M;
        struct BOGEN* G;
    }
            BgX;
    int (*fx)(void);
    int iNext;
    int iAlt;
}tBog;

int pars(tBog* pGraph);

extern tBog gBlock[];
extern tBog gProg[];
extern tBog gFact[];
extern tBog gExpression[];
extern tBog gBlock[];
extern tBog gTerm[];
extern tBog gCondition[];
extern tBog gStatement[];
typedef unsigned long ul;