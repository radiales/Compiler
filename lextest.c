/* Kevin Holz, s77179, 041/61/17 */
#include <stdio.h>
#include <ctype.h>
#include "code.h"
#include "lex.h"
#include "parser.h"
#include "namensliste.h"

tMorph Morph = { (tMC)0 };
tBez* currentList;
int* constblock = NULL;
FILE* pOFile;
int placeholder = 0;


int main(int argc, char* argv[])
{

    if (argc < 2 ){
        printf("Bitte eine pl0 Datei mit angeben\n");
        return 1;
    }

    tBez initNameListEntry = {0,NULL,0,NULL,0,NULL};
    currentList = &initNameListEntry;
    tProc rootproc = {0,0,NULL,NULL,0};
    pOFile = fopen("output.cl0","w+");
    fwrite(&placeholder,sizeof(int),1,pOFile);
    newProg();





    initLex( (char*) argv[1] ); //(f)lexing und parsing
        Lex();
        if (pars(gProg)==1)puts("OK");
        else printf("Syntaxerror near line %d col %d\n",Morph.PosLine+1,Morph.PosCol+1);
        switch (Morph.MC)
        {
            case mcSymb:
                if (Morph.Val.Symb == zERG)   printf("Symbol,:=\n");    else
                if (Morph.Val.Symb == zLE)   printf("Symbol,<=\n");    else
                if (Morph.Val.Symb == zGE)   printf("Symbol,>=\n");    else
                if (Morph.Val.Symb == zBGN)   printf("Symbol,_BEGIN\n"); else
                if (Morph.Val.Symb == zCLL)   printf("Symbol,_CALL\n"); else
                if (Morph.Val.Symb == zCST)   printf("Symbol,_CONST\n"); else
                if (Morph.Val.Symb == zDO)   printf("Symbol,_DO\n");   else
                if (Morph.Val.Symb == zEND)   printf("Symbol,_END\n");  else
                if (Morph.Val.Symb == zIF)   printf("Symbol,_IF\n");   else
                if (Morph.Val.Symb == zODD)   printf("Symbol,_ODD\n");  else
                if (Morph.Val.Symb == zPRC)   printf("Symbol,_PROCEDURE\n"); else
                if (Morph.Val.Symb == zTHN)   printf("Symbol,_THEN\n"); else
                if (Morph.Val.Symb == zVAR)   printf("Symbol,_VAR\n");  else
                if (Morph.Val.Symb == zWHL)   printf("Symbol,_WHILE\n"); else
                if (Morph.Val.Symb == zELS)   printf("Symbol,_ELS\n");

                if (isprint(Morph.Val.Symb))printf("Symbol,%c\n", (char)Morph.Val.Symb);
                break;
            case mcNum:
                printf("Zahl  | %ld\n", Morph.Val.Num);
                break;
            case mcIdent:
                printf("Ident | %s\n", (char*)Morph.Val.pStr);
                break;
        }
    puts("");
    return 0;
}

