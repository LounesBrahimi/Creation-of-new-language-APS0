#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "prologTerm.h"

typedef struct _env env;
typedef struct _closure closure;

struct _env {
	Tag_def tag;
	char* id;	
	env* suite;
	union{
		int valeur;
		struct {
			closure* closure_;
		} def_fun;
	} content;
};

struct _closure {
	Sexpr corp;
	arg   arg_;
	env*  env_;
};

env* eval_def_fun(def def_fun, env* env_);
void print_env(env* env_);
int cherche_id_env(env* env_, char* id);
int eval_expr(env* env_, Sexpr expr);
env* eval_def_const(def def_const, env* env_);
void eval_prog(prog* prog_);
