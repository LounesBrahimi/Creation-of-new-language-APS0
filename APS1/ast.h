/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expressions Syntaxe C                                              == */
/* == Fichier: ast.h                                                       == */
/* ==  Arbre de syntaxe abstraite                                          == */
/* ========================================================================== */

#include <stdbool.h>

typedef struct _sexpr *Sexpr;
typedef struct _sexprs *Sexprs;
typedef struct _def *def;
typedef struct _arg *arg;
typedef struct _type *type;
typedef struct _prog prog;
typedef struct _cmd cmd;
typedef enum _tag Tag;
typedef enum _tag_def Tag_def;
typedef enum _tag_type tag_type;

enum _tag {
  ASTNum, ASTId, ASTApp, ASTBool, ASTAbs, ASTIf, ASTAnd, ASTOr, ASTAdd, ASTMul, ASTEq, ASTSub, ASTDiv,
  ASTLt, ASTNot, ASTSet, ASTIfBlock, ASTWhile, ASTCall
};

enum _tag_def {
  ASTConst, ASTFun, ASTRecFun, ASTVar, ASTProc, ASTRecProc
};

enum _tag_type {
  ASTTypePrim, ASTTypeFunc, ASTTypeMultyPar
};

struct _type{
	tag_type tag;
	union{
		char* typePrim;
		struct {
			struct _type* parm;
			struct _type* sortie;
		} typeFunc;
		struct {
			struct _type* actuel;
			struct _type* suivant;
		} typeMultyPar;
	} content;
};

struct _arg{
	char* id;
	char* type_;
	struct _arg* suivant;
};

struct _def{
	Tag_def tag;
	union{
		struct {
			char* id;
			char* type_;
			Sexpr expr;
		} defConst;
		struct {
			char* id;
			type type_;
			arg  arg_;
			Sexpr expr;
		} defFun;
		struct {
			char* id;
			char* type_;
			arg  arg_;
			Sexpr expr;
		} defRecFun;
		struct {
			char* id;
			char* type_;
		} defVar;
		struct {
			char* id;
			arg  arg_;
			prog* block;
		} defProc;
		struct {
			char* id;
			arg  arg_;
			prog* block;
		} def_rec_proc;
	} content;
};

struct _sexpr {
  Tag tag;
  union {
    int num;
    char* id;
    char* boolean;
    struct {
      Sexpr fun;
      Sexprs args;
    } app;
    struct {
	  arg  arg_;
	  Sexpr expr;
    } abstract;
    struct {
	  Sexpr cond;
	  Sexpr cons;
	  Sexpr alt;
    } if_;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } and_;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } or_;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } add;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } mul;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } eq;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } sub;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } div;
    struct {
	  Sexpr e1;
	  Sexpr e2;
    } lt;
    struct {
	  Sexpr e;
    } not_;
    struct {
	  char* id;
	  Sexpr e;
    } set;
    struct {
	  Sexpr condition;
	  prog* block1;
	  prog* block2;
    } if_block;
    struct {
	  Sexpr condition;
	  prog* block;
    } while_;
    struct {
	  char* id;
	  Sexpr expr;
    } call_;
  } content;
};

struct _sexprs {
  Sexpr head;
  Sexprs tail;
};

Sexpr newASTNum(int n);
Sexpr newASTId(char* x);
Sexpr newASTBool(char* b);
Sexpr newASTApp(Sexpr e, Sexprs es);
Sexpr newASTAbs(arg arg_, Sexpr expr);
Sexpr newASTIf(Sexpr cond, Sexpr cons, Sexpr alt);
Sexpr newASTAnd(Sexpr e1, Sexpr e2);
Sexpr newASTOr(Sexpr e1, Sexpr e2);
Sexpr newASTAdd(Sexpr e1, Sexpr e2);
Sexpr newASTMul(Sexpr e1, Sexpr e2);
Sexpr newASTEq(Sexpr e1, Sexpr e2);
Sexpr newASTSub(Sexpr e1, Sexpr e2);
Sexpr newASTDiv(Sexpr e1, Sexpr e2);
Sexpr newASTLt(Sexpr e1, Sexpr e2);
Sexpr newASTNot(Sexpr e);
Sexpr newSet(char* id, Sexpr e);
Sexpr newIfBlock(Sexpr condition, prog* block1, prog* block2);
Sexpr newWhile(Sexpr condition, prog* block);
Sexpr newCall(char* id, Sexpr expr);

def newDefConst(char* id, type type_, Sexpr expr);
def newDefFun(char* id, type type_, arg arg_,Sexpr expr);
def newDefRecFun(char* id, type type_, arg arg_,Sexpr expr);
def newDefVar(char* id, type type_);
def newDefProc(char* id, arg arg_, prog* block);
def newDefRecProc(char* id, arg arg_, prog* block);

arg newDefArg(char* id, type type_);
arg addArg(arg arg_1, arg arg_2);

type newTypePrim(char* type_);
type addTypeMulty(type type_1, type type_2);
type addTypeFun(type type_1, type type_2);

Sexprs addSexpr(Sexpr e, Sexprs es);

struct _prog {
	int size;
	cmd* cmds;
};

struct _cmd{
		int type_;
		Sexpr expr;
		def def_const;
		def def_fun;
		def def_rec;
		def def_var;
		def def_proc;
		def def_rec_proc;
		struct _cmd* block;
};

void add_expr_prog(prog* prog_ , Sexpr expr);
void add_def_const_prog(prog* prog_ , def def_);
void add_def_fun_prog(prog* prog_ , def def_);
void add_def_rec_prog(prog* prog_ , def def_);
void add_def_var_prog(prog* prog_ , def def_);
void add_def_proc_prog(prog* prog_ , def def_);
void add_def_rec_proc_prog(prog* prog_ , def def_);

void add_def_block(prog* prog_ , def def_);

#define mallocSexpr malloc(sizeof(struct _sexpr))
#define mallocSexprs malloc(sizeof(struct _sexprs))
#define mallocDef malloc(sizeof(struct _def))
#define mallocArg malloc(sizeof(struct _arg))
#define mallocType malloc(sizeof(struct _type))
#define tagOf(r) r->tag
#define getNum(r) r->content.num
#define getId(r) r->content.id
#define getBool(r) r->content.boolean
#define getFun(r) r->content.app.fun
#define getArgs(r) r->content.app.args
