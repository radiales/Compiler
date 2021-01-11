/* Kevin Holz, s77179, 041/61/17 */

#ifndef FINALLEXER_CODEGEN_H
#define FINALLEXER_CODEGEN_H

#endif //FINALLEXER_CODEGEN_H

#define MAX_LEN_OF_CODE 81
#define ENoMem -2

typedef int tKz;
extern char* pCode;
extern char* vCode;
extern int LenCode;;

typedef struct tLABL{
    tKz Kz;
    long iJmp;
    struct tLABL* nxt;
}tLabl;

typedef struct fixlist{
    long instance;
    struct fixlist* prv;
}fxlist;

extern tLabl* LabelList;

int st1();
int st2();
int st3();

int st4();
int st5();
int st6();
int st7();
int st8();
int st9();
int st10();
int st11();
int st12();

int ex1();
int ex2();
int ex3();

int te1();
int te2();

int co1();
int co2();
int co3();
int co4();
int co5();
int co6();
int co7();
int co8();

int bl1();
int bl2();
int bl3();
int bl4();
int bl5();
int bl6();

int fa1();
int fa2();

int pr1();
