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
	printf("defConst(");
	printId(c->content.defConst.id);
	if (c->content.defConst.type_->tag == ASTTypePrim){
		printf(", type(%s), ", c->content.defConst.type_->content.typePrim);
	} else {
		printf(", ");
		printType(c->content.defConst.type_);
		printf(", ");
	}
	switch(c->content.defConst.expr->tag) {
		case ASTNum :    printNum(c->content.defConst.expr->content.num);  printf(")"); break;
		case ASTBool :   printBoolean(c->content.defConst.expr->content.boolean); printf(")"); break;
		default :    printSexpr(c->content.defConst.expr); printf(")"); break;
	}
}

void printTypeOuntF(type t){
	switch(t->tag){
		case ASTTypeFunc : printf("arrow(");
							printType(t->content.typeFunc.parm); 
							printf(", "); 
							printType(t->content.typeFunc.sortie); 
							printf(")"); 
							break;
		case ASTTypePrim : printf("%s", t->content.typePrim); break;
		case ASTTypeMultyPar : printType(t->content.typeMultyPar.actuel); printf("*"); printType(t->content.typeMultyPar.suivant); break;
	}
}

void printType(type t){
	switch(t->tag){
		case ASTTypeFunc : printf("arrow([");
							printTypeOuntF(t->content.typeFunc.parm); 
							printf("], "); 
							printTypeOuntF(t->content.typeFunc.sortie); 
							printf(")"); 
							break;
		case ASTTypePrim : printf("type(%s)", t->content.typePrim); break;
		case ASTTypeMultyPar : printType(t->content.typeMultyPar.actuel); printf("*"); printType(t->content.typeMultyPar.suivant); break;
	}
}

void printArgs(arg a){
	arg tmp = a;
	if (tmp->type_->tag == ASTTypePrim){
		printf("args([(%s,%s)", tmp->id, tmp->type_->content.typePrim);
	} else {
		printf("args([(%s,", tmp->id);
		printType(tmp->type_);
		printf(")");
	}
	
	while (tmp->suivant != NULL){ 
        tmp = tmp->suivant;
        if (tmp->type_->tag == ASTTypePrim){
			printf(", (%s,%s)", tmp->id, tmp->type_->content.typePrim);
		} else {
			printf(", (%s,", tmp->id);
			printType(tmp->type_);
			printf(")");
		}
	}
	printf("])");
}

void printDefFun(def c){
	printf("defFun("); printId(c->content.defFun.id);printf(", "); printType(c->content.defFun.type_); printf(", "); printArgs(c->content.defFun.arg_); printf(", "); printSexpr(c->content.defFun.expr); printf(")");
}

void printDefRecFun(def c){
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
