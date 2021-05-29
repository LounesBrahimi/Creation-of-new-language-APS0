/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expressions Syntaxe C                                              == */
/* == Fichier: parser.y                                                    == */
/* ==  Grammaire et génération terme Prolog                                == */
/* == Nota: prend son entrée sur stdin                                     == */
/* ========================================================================== */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "evaluator.h"
#include "prologTerm.h"

int yylex (void);
int yyerror (char *);

 Sexpr theExpr;
 def def_const;
 prog* prog_ = NULL;
%}

%token<str>  CONST
%token<str>  FUN
%token<str>  REC
%token<num>  NUM
%token<str>  BOOL
%token<str>  IDENT
%token<str>  INT
%token<str>  TBOOL
%token<str>  ECHO
%token       LSQBR RSQBR
%token       LPAR RPAR
%token       STAR
%token       ARROW
%token       DDOTS
%token       COMMA
%token       SEMICOLON
%token       IF
%token       AND
%token       OR
%token       ADD
%token       MUL
%token       EQ
%token       SUB
%token       DIV
%token       LT
%token       NOT

%union {
  int num;
  char* str;
  Sexpr expr;
  Sexprs exprs;
  def def_;
  arg arg_;
  type type_;
}

%type<expr> expr
%type<exprs> exprs
%type<expr> prog
%type<def_> def
%type<type_> tprim
%type<type_> type
%type<type_> types
%type<arg_> arg
%type<arg_> args
%type<expr> cmds
%type<expr> stat

%start prog
%%

prog: LSQBR cmds RSQBR   { theExpr = $2; }
  ;

cmds: 
  stat
| def SEMICOLON cmds      { $$ = $3; }  
  ;

stat: ECHO expr		   { add_expr_prog(prog_, $2); $$ = $2; }
  ;

def: 
  CONST IDENT type expr		{ add_def_const_prog(prog_, newDefConst($2, $3, $4)); $$ = newDefConst($2, $3, $4); }
| FUN IDENT type LSQBR args RSQBR expr { add_def_fun_prog(prog_, newDefFun($2, $3, $5, $7)); $$ = newDefFun($2, $3, $5, $7); }
| FUN REC IDENT type LSQBR args RSQBR expr { add_def_rec_prog(prog_, newDefRecFun($3, $4, $6, $8)); $$ = newDefRecFun($3, $4, $6, $8); }
  ;

args:
  arg				{ $$ = $1; }
| arg COMMA args	{ $$ = addArg($1, $3); }
  ;
  
arg: IDENT DDOTS type { $$ = newDefArg($1, $3); }
  ;

type:
  tprim						{ $$ = $1; }
| LPAR types ARROW type RPAR { $$ = addTypeFun($2, $4); }
  ;
  
tprim: 
  INT						{ $$ = newTypePrim($1); }
| TBOOL						{ $$ = newTypePrim($1); }
  ;

types:
  type						{ $$ = $1; }
| type STAR types			{ $$ = addTypeMulty($1, $3); }
  ;

expr:
  NUM                         { $$ = newASTNum($1); }
| IDENT                       { $$ = newASTId($1); }
| LPAR expr exprs RPAR        { $$ = newASTApp($2,$3); }
| LSQBR args RSQBR expr       { $$ = newASTAbs($2, $4);}
| BOOL						  { $$ = newASTBool($1); }
| LPAR IF expr expr expr RPAR { $$ = newASTIf($3, $4, $5); }
| LPAR AND expr expr RPAR     { $$ = newASTAnd($3, $4); }
| LPAR OR expr expr RPAR     { $$ = newASTOr($3, $4); }
| LPAR ADD expr expr RPAR     { $$ = newASTAdd($3, $4); }
| LPAR MUL expr expr RPAR     { $$ = newASTMul($3, $4); }
| LPAR EQ expr expr RPAR     { $$ = newASTEq($3, $4); }
| LPAR SUB expr expr RPAR     { $$ = newASTSub($3, $4); }
| LPAR DIV expr expr RPAR     { $$ = newASTDiv($3, $4); }
| LPAR LT expr expr RPAR     { $$ = newASTLt($3, $4); }
| LPAR NOT expr RPAR     { $$ = newASTNot($3); }

;
exprs:
  expr       { $$ = addSexpr($1,NULL); }
| expr exprs { $$ = addSexpr($1,$2); }
%%

int yyerror(char *s) {
  printf("error: %s\n",s);
  return 1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: ./prologTerm <fichier.aps>\n");
    return 1;
  }
  FILE* infile = freopen(argv[1], "r", stdin);
  prog_ = malloc(sizeof(prog));
  prog_->size = 0;
  yyparse();
  fclose(infile);
  printf("prog([");
  //printSexpr(theExpr);
  print_prog(prog_);
  printf("])");
  //printDef(def_const);
  printf(".\n");
  eval_prog(prog_);
  return 0;
}
