#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "prologTerm.h"

#define ANY -999999;

typedef struct _env env;
typedef struct _closure closure;
typedef struct _closure_rec closure_rec;
typedef struct _ids ids;
typedef struct _valeurs valeurs;
typedef struct _mem mem;
typedef struct _adr_val adr_val;
typedef struct _closure_proc closure_proc;
typedef struct _closure_proc_rec closure_proc_rec;

struct _mem {
	int last_adr;
	adr_val* content;
};

struct _adr_val {
	int adresse;
	int valeure;
};

struct _env {
	Tag_def tag;
	bool adresse;
	char* id;	
	env* suite;
	union{
		int valeur;
		struct {
			closure* closure_;
		} def_fun;
		struct {
			closure_rec* closure_rec_;
		} def_rec;
		struct {
			closure_proc* closure_proc_;
		} def_proc;
		struct {
			closure_proc_rec* closure_proc_rec_;
		} def_proc_rec;
	} content;
};

struct _closure_rec {
	char* id;
	Sexpr corp;
	ids* ids_;
	env*  env_;
};

struct _closure {
	Sexpr corp;
	ids* ids_;
	env*  env_;
};

struct _closure_proc {
	prog* block;
	ids* ids_;
	env*  env_;
};

struct _closure_proc_rec {
	char* id;
	prog* block;
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

env* copy_env(env* env_);
env* ajout_closure_rec_env(env* env_, closure_rec* closure_rec_);
closure_rec* get_closure_rec(env* env_, char* id);
env* lier_args_vals_env(env* env_, ids* ids_, valeurs* valeurs_);
void printValeurs(valeurs* valeurs_);
valeurs* exprs_to_valeurs(env* env_, Sexprs es, mem* mem_);
closure* get_closure(env* env_, char* id);
void printIds(ids* ids_);
ids* args_to_ids(arg arg_);
env* eval_def_fun(def def_fun, env* env_, mem* mem_);
env* eval_def_rec(def def_rec, env* env_, mem* mem_);
env* eval_def_var(def def_var, env* env_, mem** mem_);
env* eval_def_proc(def def_proc, env* env_, mem* mem_);
void print_env(env* env_);
int cherche_id_env(env* env_, char* id);
int eval_expr(env* env_, mem* mem_, Sexpr expr);
env* eval_def_const(def def_const, env* env_, mem* mem_);
void eval_prog(env* env_, mem* mem_, prog* prog_);
// retourne l'indice de l'id dans l'environnement
int indice_id_env(env* env_, char* id);
mem* alloc(mem* mem_);
// retourne l'adresse de l'id dans l'environnement si il existe
int cherche_id_adr_env(env* env_, char* id);
mem* stat_set(env* env_, mem* mem_, stat stat_);
// affecte la valeur à l'adresse indiqué dans la memoire
mem* affectation_mem(mem* mem_, int adresse, int valeur);
mem* stat_IF(env* env_, mem** mem_, stat stat_);
mem* stat_while(env* env_, mem** mem_, stat stat_);
env* eval_def_proc_rec(def def_rec_proc, env* env_, mem* mem_);
mem* stat_call(env* env_, mem** mem_, stat stat_);
closure_proc_rec* get_closure_proc_rec(env* env_, char* id);
env* ajout_closure_rec_env_proc(env* env_, closure_proc_rec* closure_rec_proc);
