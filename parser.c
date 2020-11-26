/* Kevin Holz, s77179, 041/61/17 */
#include "lex.h"
#include "parser.h"
#include "stdio.h"
#include "stdlib.h"

extern tMorph Morph; // Wonky

tBog gExpression[] =
        {
/* 0  */ {BgSy, {(ul)'-'	        }, NULL, 1,		3},/* (0)----------'-'---------> expression	*/
/* 1  */ {BgGr, {(ul)gTerm              	}, NULL, 2,		0},/* '-'----------term--------> 2			*/
/* 2  */ {BgNl, {(ul)0		        }, NULL, 6,		0},/* (0)---------'odd'--------> expression	*/
/* 3  */ {BgGr, {(ul)gTerm	            }, NULL, 2,		0},/* (0)----------term--------> 2			*/
/* 4  */ {BgGr, {(ul)gTerm	            }, NULL, 2,		0},/* '+'----------term--------> 2			*/
/* 5  */ {BgGr, {(ul)gTerm	            }, NULL, 2,		0},/* '+'----------term--------> 2			*/ //Perhaps obsolete?
/* 6  */ {BgSy, {(ul)'+'	        }, NULL, 4,		7},/* NL-----------'+'---------> term		*/
/* 7  */ {BgSy, {(ul)'-'	        }, NULL, 5,		8},/* NL-----------'-'---------> term		*/
/* 8  */ {BgEn, {(ul)0		        }, NULL, 0,		0} /* NL-----------------------> Ende		*/
/* 9  */
/* 10 */
        };

tBog gStatement[] = {
/* 0  */ {BgMo, {(ul)mcIdent              }, &st1, 1,		3},
/* 1  */ {BgSy, {(ul)zERG	                }, NULL, 2,		0},
/* 2  */ {BgGr, {(ul)gExpression	        }, NULL,22,		0},
/* 3  */ {BgSy, {(ul)zIF	                }, NULL, 4,		7},
/* 4  */ {BgGr, {(ul)gCondition	        }, NULL, 5,		0},
/* 5  */ {BgSy, {(ul)zTHN	                }, NULL, 6,		0},
/* 6  */ {BgGr, {(ul)gStatement	        }, NULL,22,		0},
/* 7  */ {BgSy, {(ul)zWHL	                }, NULL, 8,	   11},
/* 8  */ {BgGr, {(ul)gCondition	        }, NULL, 9,		0},
/* 9  */ {BgSy, {(ul)zDO	                }, NULL,10,		0},
/* 10 */ {BgGr, {(ul)gStatement	        }, NULL,22,		0},
/* 11 */ {BgSy, {(ul)zBGN	                }, NULL,12,	   15},
/* 12 */ {BgGr, {(ul)gStatement	        }, NULL,13,	    0},
/* 13 */ {BgSy, {(ul)zEND	                }, NULL,22,	   14},
/* 14 */ {BgSy, {(ul)';'	        }, NULL,12,		0},
/* 15 */ {BgSy, {(ul)zCLL	                }, NULL,16,	   17},
/* 16 */ {BgMo, {(ul)mcIdent               }, &st2,22,		0},
/* 17 */ {BgSy, {(ul)'?'	       }, NULL,18,	   19},
/* 18 */ {BgMo, {(ul)mcIdent              }, %st3,22,		0},
/* 19 */ {BgSy, {(ul)'!'	       }, NULL,20,	   21},
/* 20 */ {BgGr, {(ul)gExpression	       }, NULL,22,	0},
/* 21 */ {BgNl, {(ul)0		       }, NULL,22,		0},
/* 22 */ {BgEn, {(ul)0		   }, NULL, 0,		0}
};

tBog gBlock[]=
        {
        /*0*/{BgSy,{(ul)zCST             }, NULL, 1 ,6},
        /*1*/{BgMo,{(ul)mcIdent          }, &bl1, 2 ,0},
        /*2*/{BgSy,{(ul)'='       }, NULL, 3 ,0},
        /*3*/{BgMo,{(ul)mcNum            }, &bl2, 4 ,0},
        /*4*/{BgSy,{(ul)','       }, NULL, 1 ,5},
        /*5*/{BgSy,{(ul)';'       }, NULL, 7 ,0},
        /*6*/{BgNl,{(ul)0         }, NULL, 7 ,0}, // nilbogen-> Zuletzt!
        /*7*/{BgSy,{(ul)zVAR             }, NULL, 8 ,11},
        /*8*/{BgMo,{(ul)mcIdent          }, &bl3, 9 ,0},
        /*9*/{BgSy,{(ul)','       }, NULL, 8 ,10},
        /*10*/{BgSy,{(ul)';'      }, NULL, 12 ,0},
        /*11*/{BgNl,{(ul)0        }, NULL, 12 ,0},
        /*12*/{BgSy,{(ul)zPRC            }, NULL, 13 ,17},
        /*13*/{BgMo,{(ul)mcIdent         }, &bl4, 14 ,0},
        /*14*/{BgSy,{(ul)';'      }, NULL, 15 ,0},
        /*15*/{BgGr,{(ul)gBlock          }, NULL, 16 ,0},
        /*16*/{BgSy,{(ul)';'      }, &bl5, 12 ,0},
        /*17*/{BgNl,{(ul)0        }, &bl6, 18 ,0},
        /*18*/{BgGr,{(ul)gStatement      }, NULL, 19 ,0},
        /*19*/{BgEn,{(ul)0        }, NULL, 0 ,0},
        };

tBog gProg[]=
        {
        /*0*/{BgGr,{(ul)gBlock           }, NULL, 1 ,0},
        /*1*/{BgSy,{(ul)'.'       }, &pr1, 2 ,0},
        /*2*/{BgEn,{(ul)0         }, NULL, 0 ,0},
        };

tBog gFact[]=
        {
        /*0*/{BgMo,{(ul)mcIdent         }, &fa1, 5 ,1},
        /*1*/{BgMo,{(ul)mcNum           }, %fa2, 5 ,2},
        /*2*/{BgSy,{(ul)'('      }, NULL, 3 ,0},
        /*3*/{BgGr,{(ul)gExpression     }, NULL, 4 ,0},
        /*4*/{BgSy,{(ul)')'      }, NULL, 5 ,0},
        /*5*/{BgEn,{(ul)0        }, NULL, 0 ,0},
        };


tBog gCondition[]=
        {
        /*0*/ {BgSy,{(ul)zODD           }, NULL, 1 ,2},
        /*1*/ {BgGr,{(ul)gExpression    }, NULL, 5 ,0},
        /*2*/ {BgGr,{(ul)gExpression    }, NULL, 3 ,0},
        /*3*/ {BgSy,{(ul)'='     }, NULL, 9 ,4},
        /*4*/ {BgSy,{(ul)'#'     }, NULL, 9 ,5},
        /*5*/ {BgSy,{(ul)'<'     }, NULL, 9 ,6},
        /*6*/ {BgSy,{(ul)zLE            }, NULL, 9 ,7},
        /*7*/ {BgSy,{(ul)'>'     }, NULL, 9 ,8},
        /*8*/ {BgSy,{(ul)zGE            }, NULL, 9 ,0},
        /*9*/ {BgGr,{(ul)gExpression    }, NULL, 10 ,0},
        /*10*/ {BgEn,{(ul)0      }, NULL, 0 ,0},
        };

tBog gTerm[]=
        {
        /*0*/ {BgGr,{(ul)gFact          }, NULL, 1 ,0},
        /*1*/ {BgNl,{(ul)0       }, NULL, 2 ,0},
        /*2*/ {BgSy,{(ul)'*'     }, NULL, 3 ,4},
        /*3*/ {BgGr,{(ul)gFact          }, NULL, 1 ,0},
        /*4*/ {BgSy,{(ul)'/'     }, NULL, 5 ,0},
        /*5*/ {BgGr,{(ul)gFact          }, NULL, 1 ,0},
        /*6*/ {BgEn,{(ul)0       }, NULL, 0 ,0},

        };

char* graphNames[] = {
        "Program",          "Program",          "Block",        "Block",
        "Statement",        "Statement",        "Expression",   "Expression",
        "Term",             "Term",             "Factor",       "Factor",
        "Condition",        "Condition"
};

tBog* endStates[] = {
        gProg,           &(gProg[2]),      gBlock,         &(gBlock[19]),
        gStatement,      &(gStatement[22]),gExpression,    &(gExpression[8]),
        gTerm,           &(gTerm[6]),      gFact,          &(gFact[5]),
        gCondition,      &(gCondition[10])
};

int depth = 0;

void LookupGraph(tBog* pBog){
    int i, j;

    for(i = 0; i < 13; i+=2){
        if(pBog >= endStates[i] && pBog <= endStates[i+1]){
            printf("==================================================\n");
            printf("Depth: %d Graph: %s.\n", depth, graphNames[i]);
            printf("==================================================\n\n");
            return;
        }
    }
}

int pars(tBog* pGraph) {
    tBog *pBog = pGraph;
    int succ = 0;
    if (Morph.MC == mcEmpty)Lex();
    while (1) {

//        if (pGraph == gProg) printf("Programm\n");
//        if (pGraph == gBlock) printf("Block\n");
//        if (pGraph == gExpression) printf("Expression\n");
//        if (pGraph == gTerm) printf("Term\n");
//        if (pGraph == gStatement) printf("Statement\n");
//        if (pGraph == gFact) printf("Faktor\n");
//        if (pGraph == gCondition) printf("Condition\n");

        //LookupGraph(pGraph);
        switch (pBog->BgD) {
            case BgNl:
                succ = 1;
                break;
            case BgSy:
                succ = (Morph.Val.Symb == pBog->BgX.S);
                break;
            case BgMo:
                succ = (Morph.MC == (tMC) pBog->BgX.M);
                break;
            case BgGr:
                succ = pars(pBog->BgX.G); // Nächster Graph

                break;
            case BgEn:
                return 1;
/* Ende erreichet - Erfolg */
        }
        if (succ && pBog->fx != NULL) succ = pBog->fx();

        if (!succ) /* Alternativbogen probieren */
            if (pBog->iAlt != 0)
                pBog = pGraph + pBog->iAlt;
            else return -1;
        else /* Morphem formal akzeptiert (eaten) */
        {
            if (pBog->BgD & BgSy || pBog->BgD & BgMo) Lex();
            pBog = pGraph + pBog->iNext;
        }
    }
}