/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expressions Syntaxe C                                              == */
/* == Fichier: ast.c                                                       == */
/* ==  Arbre de syntaxe abstraite                                          == */
/* ========================================================================== */
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

Sexpr newASTNum(int v) {
  Sexpr r = mallocSexpr;
  r->tag = ASTNum;
  r->content.num = v;
  return r;
}

Sexpr newASTId(char* v) {
  Sexpr r = mallocSexpr;
  r->tag = ASTId;
  r->content.id = v;
  return r;
}

Sexpr newASTBool(char* v) {
  Sexpr r = mallocSexpr;
  r->tag = ASTBool;
  r->content.boolean = v;
  return r;
}

def newDefConst(char* id_, type type_, Sexpr expr_){
	def c = mallocDef;
	c->tag = ASTConst;
	c->content.defConst.id = id_;
	c->content.defConst.type_ = type_;
	c->content.defConst.expr = expr_;
	return c;
}

def newDefFun(char* id_, type type_, arg arg_, Sexpr expr_){
	def c = mallocDef;
	c->tag = ASTFun;
	c->content.defFun.id = id_;
	c->content.defFun.type_ = type_;
	c->content.defFun.arg_ = arg_;
	c->content.defFun.expr = expr_;
	return c;
}

def newDefRecFun(char* id_, type type_, arg arg_, Sexpr expr_){
	def c = mallocDef;
	c->tag = ASTRecFun;
	c->content.defFun.id = id_;
	c->content.defFun.type_ = type_;
	c->content.defFun.arg_ = arg_;
	c->content.defFun.expr = expr_;
	return c;
}

type newTypePrim(char* type_){
	type t = mallocType;
	t->tag = ASTTypePrim;
	t->content.typePrim = type_;
}

arg newDefArg(char* id_, type type_){
	arg a = mallocArg;
	a->id = id_;
	a->type_ = type_;
	a->suivant = NULL;
	return a;
}

type addTypeFun(type type_1, type type_2){
	type t = mallocType;
	t->tag = ASTTypeFunc;
	t->content.typeFunc.parm = type_1;
	t->content.typeFunc.sortie = type_2;
	return t;
}

type addTypeMulty(type type_1, type type_2){
	type t = mallocType;
	t->tag = ASTTypeMultyPar;
	t->content.typeMultyPar.actuel = type_1;
	t->content.typeMultyPar.suivant = type_2;
	return t;
}

arg addArg(arg arg_1, arg arg_2){
	
	 if (arg_1 == NULL) 
    { 
       arg_1 = arg_2; 
       return arg_1;
    } 
    
    arg tmp = arg_1;
    
    while (tmp->suivant != NULL) 
        tmp->suivant = tmp->suivant->suivant; 
        
	tmp->suivant = arg_2;
	
	return arg_1;
}

Sexpr newASTAbs(arg arg_, Sexpr expr){
  Sexpr r = mallocSexpr;
  r->tag = ASTAbs;
  r->content.abstract.arg_ = arg_;
  r->content.abstract.expr = expr;
  return r;
}

Sexpr newASTApp(Sexpr e, Sexprs es) {
  Sexpr r = mallocSexpr;
  r->tag = ASTApp;
  r->content.app.fun = e;
  r->content.app.args = es;
  return r;
}

void add_expr_prog(prog* prog_ , Sexpr expr){
	int i = prog_->size;
	prog_->size++;
	prog_->cmds = realloc(prog_->cmds, (prog_->size)*sizeof(cmd));
	prog_->cmds[i].type_ = 1;
	prog_->cmds[i].expr = expr;
}

void add_def_const_prog(prog* prog_ , def def_){
	int i = prog_->size;
	prog_->size++;
	prog_->cmds = realloc(prog_->cmds, (prog_->size)*sizeof(cmd));
	prog_->cmds[i].type_ = 2;
	prog_->cmds[i].def_const = def_;
}

void add_def_fun_prog(prog* prog_ , def def_){
	int i = prog_->size;
	prog_->size++;
	prog_->cmds = realloc(prog_->cmds, (prog_->size)*sizeof(cmd));
	prog_->cmds[i].type_ = 3;
	prog_->cmds[i].def_fun = def_;
}

void add_def_rec_prog(prog* prog_ , def def_){
	int i = prog_->size;
	prog_->size++;
	prog_->cmds = realloc(prog_->cmds, (prog_->size)*sizeof(cmd));
	prog_->cmds[i].type_ = 4;
	prog_->cmds[i].def_rec = def_;
}

Sexprs addSexpr(Sexpr e, Sexprs es) {
  Sexprs r = mallocSexprs;
  r->head = e;
  r->tail = es;
  return r;
}

Sexpr newASTIf(Sexpr cond, Sexpr cons, Sexpr alt){
  Sexpr r = mallocSexpr;
  r->tag = ASTIf;
  r->content.if_.cond = cond;	
  r->content.if_.cons = cons;
  r->content.if_.alt = alt;
  return r;
}

Sexpr newASTAnd(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTAnd; 
  r->content.and_.e1 = e1;
  r->content.and_.e2 = e2;
  return r;
}

Sexpr newASTOr(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTOr; 
  r->content.or_.e1 = e1;
  r->content.or_.e2 = e2;
  return r;
}

Sexpr newASTAdd(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTAdd; 
  r->content.add.e1 = e1;
  r->content.add.e2 = e2;
  return r;
}

Sexpr newASTMul(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTMul; 
  r->content.mul.e1 = e1;
  r->content.mul.e2 = e2;
  return r;
}

Sexpr newASTEq(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTEq; 
  r->content.eq.e1 = e1;
  r->content.eq.e2 = e2;
  return r;
}

Sexpr newASTSub(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTSub; 
  r->content.sub.e1 = e1;
  r->content.sub.e2 = e2;
  return r;
}

Sexpr newASTDiv(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTDiv; 
  r->content.div.e1 = e1;
  r->content.div.e2 = e2;
  return r;
}

Sexpr newASTLt(Sexpr e1, Sexpr e2){
  Sexpr r = mallocSexpr;	
  r->tag = ASTLt; 
  r->content.lt.e1 = e1;
  r->content.lt.e2 = e2;
  return r;
}

Sexpr newASTNot(Sexpr e){
  Sexpr r = mallocSexpr;
  r->tag = ASTNot;
  r->content.not_.e = e;
  return r;
}
