/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expression Syntaxe C                                               == */
/* == Fichier: prologTerm.h                                                == */
/* == Génération du terme prolog                                           == */
/* ========================================================================== */

void printSexpr(Sexpr);
void printSexprs(Sexprs);
void printDefConst(def);
void printDefFun(def);
void printDefRecFun(def);
void printDef(def);
void printType(type);
void print_prog(prog* prog_);
void printDefProc(def p);
void printDefRecProc(def p);
