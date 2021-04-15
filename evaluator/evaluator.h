#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "prologTerm.h"

typedef struct _env env;
typedef struct _closure closure;
typedef struct _ids ids;
typedef struct _valeurs valeurs;

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
	ids* ids_;
	env*  env_;
};

struct _ids {
	char** arg_;
	int size;
};

struct _valeurs {
	int* v;
	int size;
};

env* lier_args_vals_env(env* env_, ids* ids_, valeurs* valeurs_);
void printValeurs(valeurs* valeurs_);
valeurs* exprs_to_valeurs(env* env_, Sexprs es);
closure* get_closure(env* env_, char* id);
void printIds(ids* ids_);
ids* args_to_ids(arg arg_);
env* eval_def_fun(def def_fun, env* env_);
void print_env(env* env_);
int cherche_id_env(env* env_, char* id);
int eval_expr(env* env_, Sexpr expr);
env* eval_def_const(def def_const, env* env_);
void eval_prog(prog* prog_);
