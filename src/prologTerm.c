/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expression Syntaxe C                                               == */
/* == Fichier: prologTerm.c                                                == */
/* == Génération du terme prolog                                           == */
/* ========================================================================== */

#include <stdio.h>
#include "ast.h"
#include "prologTerm.h"

void printNum(int n) {
  printf("num(%d)",n);
}

void printId(char* x) {
  printf("id(%s)",x);
}

void printBoolean(char* x) {
  printf("boolean(%s)",x);
}

void printDefConst(def c){
	switch(c->content.defConst.expr->tag) {
		case ASTNum : printf("defConst("); printId(c->content.defConst.id); printf(", type(%s), ", c->content.defConst.type_); printNum(c->content.defConst.expr->content.num);  printf(")"); break;
/*supr*/case ASTId :  printf("Const %s %s %s", c->content.defConst.id, c->content.defConst.type_, c->content.defConst.expr->content.id); break;
		case ASTBool :printf("defConst("); printId(c->content.defConst.id); printf(", type(%s), ", c->content.defConst.type_); printBoolean(c->content.defConst.expr->content.boolean); printf(")"); break;
	}
}

void printType(type t){
	switch(t->tag){
		case ASTTypeFunc : printf("type(");printType(t->content.typeFunc.parm); printf("->"); printType(t->content.typeFunc.sortie); printf(")"); break;
		case ASTTypePrim : printf("type(%s)", t->content.typePrim); break;
		case ASTTypeMultyPar : printType(t->content.typeMultyPar.actuel); printf("*"); printType(t->content.typeMultyPar.suivant); break;
	}
}

void printArgs(arg a){
	arg tmp = a;
	
	printf("args([%s:%s", tmp->id, tmp->type_);
	while (tmp->suivant != NULL){ 
        tmp = tmp->suivant;
        printf(", %s:%s", tmp->id, tmp->type_);
	}
	printf("])");
}

void printDefFun(def c){
	/*switch(c->content.defFun.expr->tag) {
		case ASTNum : printf("Fun %s ", c->content.defFun.id); printType(c->content.defFun.type_); printArgs(c->content.defFun.arg_); printf(" %d", c->content.defFun.expr->content.num); break;
		case ASTId :  printf("Fun %s ",c->content.defFun.id); printType(c->content.defFun.type_); printArgs(c->content.defFun.arg_);	printf("%s",c->content.defFun.expr->content.id); break;
		case ASTBool :printf("Fun %s ", c->content.defFun.id); printType(c->content.defFun.type_); printArgs(c->content.defFun.arg_); printf(" %s", c->content.defFun.expr->content.boolean); break;
	}*/
	printf("defFun("); printId(c->content.defFun.id);printf(", "); printType(c->content.defFun.type_); printf(", "); printArgs(c->content.defFun.arg_); printf(", "); printSexpr(c->content.defFun.expr); printf(")");
}

void printDefRecFun(def c){
	/*switch(c->content.defRecFun.expr->tag) {
		case ASTNum : printf("Fun Rec %s ", c->content.defRecFun.id); printType(c->content.defFun.type_); printArgs(c->content.defRecFun.arg_); printf(" %d", c->content.defRecFun.expr->content.num); break;
		case ASTId :  printf("Fun Rec %s ",c->content.defRecFun.id); printType(c->content.defFun.type_); printArgs(c->content.defRecFun.arg_);	printf("%s",c->content.defRecFun.expr->content.id); break;
		case ASTBool :printf("Fun Rec %s ", c->content.defRecFun.id); printType(c->content.defFun.type_); printArgs(c->content.defRecFun.arg_); printf(" %s", c->content.defRecFun.expr->content.boolean); break;
	}*/
	printf("defFunRec(");printId(c->content.defRecFun.id); printf(", "); printType(c->content.defFun.type_); printf(", "); printArgs(c->content.defRecFun.arg_); printf(", "); printSexpr(c->content.defRecFun.expr); printf(")");
}

void printDef(def c){
	switch(c->tag) {
		case ASTConst : printDefConst(c); break;
		case ASTFun :  printDefFun(c); break;
		case ASTRecFun : printDefRecFun(c); break;
	}
}

void printSexpr(Sexpr e) {
  switch(tagOf(e)) {
  case ASTNum : printNum(getNum(e)); break;
  case ASTId : printId(getId(e)); break;
  case ASTBool : printBoolean(getBool(e)); break;
  case ASTApp : {
    printf("app(");
    printSexpr(getFun(e));
    printf(", [");
    printSexprs(getArgs(e));
    printf("])");
    break;
  }
  case ASTAbs : {
	printf("abst(");
	printArgs(e->content.abstract.arg_);
	printf(", ");
	printSexpr(e->content.abstract.expr);
	printf(")");	
	break;
  }
  case ASTIf : {
	printf("if(");
	printSexpr(e->content.if_.cond);	
	printf(", ");
	printSexpr(e->content.if_.cons);	
	printf(", ");
	printSexpr(e->content.if_.alt);	
	printf(")");	
	break;
  }
  case ASTAnd : {
	printf("and(");
	printSexpr(e->content.and_.e1);	
	printf(", ");
	printSexpr(e->content.and_.e2);			
	printf(")");	
	break;
  }
  case ASTOr : {
	printf("or(");
	printSexpr(e->content.or_.e1);	
	printf(", ");
	printSexpr(e->content.or_.e2);			
	printf(")");	
	break;
  }
  case ASTAdd : {
	printf("add(");
	printSexpr(e->content.add.e1);	
	printf(", ");
	printSexpr(e->content.add.e2);			
	printf(")");	
	break;
  }
  case ASTMul : {
	printf("mul(");
	printSexpr(e->content.mul.e1);	
	printf(", ");
	printSexpr(e->content.mul.e2);			
	printf(")");	
	break;
  }
  case ASTEq : {
	printf("eq(");
	printSexpr(e->content.eq.e1);	
	printf(", ");
	printSexpr(e->content.eq.e2);			
	printf(")");	
	break;
  }
  case ASTSub : {
	printf("sub(");
	printSexpr(e->content.sub.e1);	
	printf(", ");
	printSexpr(e->content.sub.e2);			
	printf(")");	
	break;
  }
  case ASTDiv : {
	printf("div(");
	printSexpr(e->content.div.e1);	
	printf(", ");
	printSexpr(e->content.div.e2);			
	printf(")");	
	break;
  }
  case ASTLt : {
	printf("lt(");
	printSexpr(e->content.lt.e1);	
	printf(", ");
	printSexpr(e->content.lt.e2);			
	printf(")");	
	break;
  }
  case ASTNot : {
	printf("not(");
	printSexpr(e->content.not_.e);			
	printf(")");	
	break;
  }
  }
}

void printSexprs(Sexprs es) {
  if (es) {
    while (es->tail) {
      printSexpr(es->head);
      printf(",");
      es = es->tail;
    };
    printSexpr(es->head);
  }
}

void print_prog(prog* prog_){
	int i;
	for(i=0; i<prog_->size; i++){
		if(i) printf(", ");
		if(prog_->cmds[i].type_ == 1){
			if(i == (prog_->size-1)){
				printf("echo(");
				printSexpr(prog_->cmds[i].expr);
				printf(")");
			} else {
				printSexpr(prog_->cmds[i].expr);
			}
		} else if(prog_->cmds[i].type_ == 2){
			printDefConst(prog_->cmds[i].def_const);
		} else if(prog_->cmds[i].type_ == 3){
			printDefFun(prog_->cmds[i].def_fun);
		} else if(prog_->cmds[i].type_ == 4){
			printDefRecFun(prog_->cmds[i].def_rec);
		}
	}
}
