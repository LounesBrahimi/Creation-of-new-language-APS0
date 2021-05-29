/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expressions Syntaxe C                                              == */
/* == Fichier: parser.y                                                    == */
/* ==  Grammaire et génération terme Prolog                                == */
/* == Nota: prend son entrée sur stdin                                     == */
/* ========================================================================+== */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prologTerm.h"

#include <math.h>

int yylex (void);
int yyerror (char *);

 Sexpr theExpr;
 def def_const;
 prog* prog_ = NULL;
 prog* cmds_ = NULL;
 prog* block2_ = NULL;
 prog* tmp_block = NULL;
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
%token       VAR
%token       PROC
%token       SET
%token       IFB
%token       WHILE
%token       CALL
%token       var
%token       ADR


%union {
  int num;
  char* str;
  Sexpr expr;
  Sexprs exprs;
  def def_;
  arg arg_;
  type type_;
  prog* block;
  stat stat_;
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
%type<stat_> stat
%type<block> Block
%type<block> cmdsBlock
%type<arg_> Argsp
%type<arg_> Argp
%type<exprs> Exprsp
%type<expr> Exprp

%start prog
%%

prog: LSQBR cmds RSQBR   { theExpr = $2; }
  ;

Block: LSQBR cmdsBlock RSQBR   { tmp_block = cmds_;
								cmds_ = NULL;   
								cmds_ = malloc(sizeof(prog));
								cmds_->size = 0; 
								$$ = tmp_block; }
  ;

cmdsBlock :
  stat						   { add_stat_prog(cmds_, $1); $$ = cmds_; }
| def SEMICOLON cmdsBlock      { add_def_block(cmds_, $1); $$ = cmds_;} 
| stat SEMICOLON cmdsBlock     { add_stat_prog(cmds_, $1); $$ = cmds_; }
  ;

cmds: 
  stat                    { add_stat_prog(prog_, $1); }
| def SEMICOLON cmds      { add_def_block(prog_ , $1); $$ = $3; } 
| stat SEMICOLON cmds     { add_stat_prog(prog_, $1); } 
  ;

stat: ECHO expr		   { $$ = newEcho($2); }
| SET IDENT expr       { $$ = newSet($2, $3); }
| IFB expr Block Block { $$ = newIfBlock($2, $3, $4); }
| WHILE expr Block     { $$ = newWhile($2, $3); }
| CALL IDENT Exprsp      { $$ = newCall($2, $3); }
  ;

def: 
  CONST IDENT type expr		{ $$ = newDefConst($2, $3, $4); }
| FUN IDENT type LSQBR args RSQBR expr { $$ = newDefFun($2, $3, $5, $7); }
| FUN REC IDENT type LSQBR args RSQBR expr { $$ = newDefRecFun($3, $4, $6, $8); }
| VAR IDENT type { $$ = newDefVar($2, $3); }
| PROC IDENT LSQBR Argsp RSQBR Block { $$ = newDefProc($2, $4, $6); }
| PROC REC IDENT LSQBR Argsp RSQBR Block { $$ = newDefRecProc($3, $5, $7); }
  ;

Argsp:
	Argp			{ $$ = $1; }
|	Argp COMMA Argsp { $$ = addArg($1, $3); }
  ;
  
Argp: 
	IDENT DDOTS type { $$ = newDefArg($1, $3); }
|	var IDENT DDOTS type { $$ = newDefArgVar($2, $4); }
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

Exprp:
	expr					{ $$ = $1; }
|	LPAR ADR IDENT RPAR		{ $$ = newASTIdAdr($3); }
  ;

Exprsp:
	Exprp	{ $$ = addSexpr($1,NULL); }
|	Exprp Exprsp	{ $$ = addSexpr($1,$2); }
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
  cmds_ = malloc(sizeof(prog));
  cmds_->size = 0;
  block2_ = malloc(sizeof(prog));
  block2_->size = 0;
  yyparse();
  fclose(infile);
  printf("prog([");
  print_prog(prog_);
  printf("])");
  printf(".\n");
  return 0;
}
