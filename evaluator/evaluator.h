#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

typedef struct _env env;

struct _env {
	char* id;
	int valeur;
	env* suite;
};

void print_env(env* env_);
int cherche_id_env(env* env_, char* id);
int eval_expr(env* env_, Sexpr expr);
env* eval_def_const(def def_const, env* env_);
void eval_prog(prog* prog_);
