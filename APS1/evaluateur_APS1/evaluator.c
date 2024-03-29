#include "evaluator.h"

void printIds(ids* ids_){
	int i;
	for(i=0; i<ids_->size; i++){
		printf("%s ", ids_->arg_[i]);
	}
	printf("\n");
}

void printValeurs(valeurs* valeurs_){
	int i;
	for(i=0; i<valeurs_->size; i++){
		printf("%d ", valeurs_->v[i]);
	}
	printf("\n");
}

ids* args_to_ids(arg arg_){
	arg p = arg_;
	ids* ids_ = malloc(sizeof(ids));
	ids_->arg_ = NULL;
	int size = 0;
	int i = 0;
	while(p != NULL){
		size++;
		ids_->arg_ = realloc(ids_->arg_, size*sizeof(char*));
		ids_->arg_[i] = p->id;
		i++;
		p = p->suivant;
	}
	ids_->size = size;
	return ids_;
}

int cherche_id_env(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if ((!(strcmp(p->id, id))) && (!(p->adresse))){
			return p->content.valeur;
		}
		else {
			p = p->suite;
		}
	}
	return -99999996;
}

int indice_id_env(env* env_, char* id){
	env* p = env_;
	int i = 0;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			return i;
		}
		else {
			p = p->suite;
		}
	}
	return -2;
}

closure_proc_rec* get_closure_proc_rec(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			if (p->tag == ASTRecProc)
				return p->content.def_proc_rec.closure_proc_rec_;
			else 
				p = p->suite;
		}
		else {
			p = p->suite;
		}
	}
	return NULL;
}

closure_proc* get_closure_proc(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			if (p->tag == ASTProc)
				return p->content.def_proc.closure_proc_;
			else 
				p = p->suite;
		}
		else {
			p = p->suite;
		}
	}
	return NULL;
}

closure* get_closure(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			if (p->tag == ASTFun)
				return p->content.def_fun.closure_;
			else 
				p = p->suite;
		}
		else {
			p = p->suite;
		}
	}
	return NULL;
}

closure_rec* get_closure_rec(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			if (p->tag == ASTRecFun)
				return p->content.def_rec.closure_rec_;
			else
				p = p->suite;
		}
		else {
			p = p->suite;
		}
	}
	return NULL;
}

valeurs* exprs_to_valeurs(env* env_, Sexprs es, mem* mem_){
	Sexprs p = es; 
	valeurs* valeurs_ = malloc(sizeof(valeurs));
	valeurs_->v = NULL;
	int size = 0;
	int i = 0;
	while(p != NULL){
		size++;
		valeurs_->v = (int*) realloc(valeurs_->v, size*sizeof(int));
		valeurs_->v[i] = eval_expr(env_, mem_, p->head);
		i++;
		p = p->tail;
	}
	valeurs_->size = size;
	return valeurs_;	
}

env* lier_args_vals_env(env* env_, ids* ids_, valeurs* valeurs_){
	env* new_env = NULL;
	env* p = NULL;
	int i;
	for(i=0; i<ids_->size; i++){
		if (i) {
			p = new_env;
		}
		new_env = malloc(sizeof(env));
		new_env->tag = ASTConst;
		new_env->adresse = false;
		new_env->id = ids_->arg_[i];
		new_env->content.valeur = valeurs_->v[i];
		if (i) {
			new_env->suite = p;
		} else {
			new_env->suite = env_;
		}	
	} 
	return new_env;
}

env* ajout_closure_rec_env_proc(env* env_, closure_proc_rec* closure_rec_proc){
	env* new_env = malloc(sizeof(env));
	new_env->id = closure_rec_proc->id;
	new_env->tag = ASTRecProc;
	new_env->adresse = false;
	new_env->content.def_proc_rec.closure_proc_rec_ = closure_rec_proc;
	new_env->suite = env_;
	return new_env;
}

env* ajout_closure_rec_env(env* env_, closure_rec* closure_rec_){
	env* new_env = malloc(sizeof(env));
	new_env->id = closure_rec_->id;
	new_env->tag = ASTRecFun;
	new_env->adresse = false;
	new_env->content.def_rec.closure_rec_ = closure_rec_;
	new_env->suite = env_;
	return new_env;
}

env* eval_def_rec(def def_rec, env* env_, mem* mem_){
	env* new_env = malloc(sizeof(env));
	new_env->id = def_rec->content.defRecFun.id;
	new_env->tag = ASTRecFun;
	new_env->adresse = false;
	new_env->content.def_rec.closure_rec_ = malloc(sizeof(closure_rec));
	new_env->content.def_rec.closure_rec_->corp = def_rec->content.defRecFun.expr;
	new_env->content.def_rec.closure_rec_->ids_ = malloc(sizeof(ids));
	new_env->content.def_rec.closure_rec_->ids_ = args_to_ids(def_rec->content.defRecFun.arg_);
	new_env->content.def_rec.closure_rec_->env_ = copy_env(env_);
	new_env->content.def_rec.closure_rec_->id = def_rec->content.defRecFun.id;
	new_env->suite = env_;
	return new_env;
}

env* eval_def_proc_rec(def def_rec_proc, env* env_, mem* mem_){
	env* new_env = malloc(sizeof(env));
	new_env->id = def_rec_proc->content.def_rec_proc.id;
	new_env->tag = ASTRecProc;
	new_env->adresse = false;
	new_env->content.def_proc_rec.closure_proc_rec_ = malloc(sizeof(closure_proc_rec));
	new_env->content.def_proc_rec.closure_proc_rec_->block = def_rec_proc->content.def_rec_proc.block;
	new_env->content.def_proc_rec.closure_proc_rec_->ids_ = malloc(sizeof(ids));
	new_env->content.def_proc_rec.closure_proc_rec_->ids_ = args_to_ids(def_rec_proc->content.def_rec_proc.arg_);
	new_env->content.def_proc_rec.closure_proc_rec_->env_ = copy_env(env_);
	new_env->content.def_proc_rec.closure_proc_rec_->id = def_rec_proc->content.def_rec_proc.id;
	new_env->suite = env_;
	return new_env;
}

env* eval_def_proc(def def_proc, env* env_, mem* mem_){
	env* new_env = malloc(sizeof(env));
	new_env->id = def_proc->content.defProc.id;
	new_env->tag = ASTProc;
	new_env->adresse = false;
	new_env->content.def_proc.closure_proc_ = malloc(sizeof(closure_proc));
	new_env->content.def_proc.closure_proc_->block = def_proc->content.defProc.block;
	new_env->content.def_proc.closure_proc_->ids_ = malloc(sizeof(ids));
	new_env->content.def_proc.closure_proc_->ids_ = args_to_ids(def_proc->content.defProc.arg_);
	new_env->content.def_proc.closure_proc_->env_ = copy_env(env_);
	new_env->suite = env_;
	return new_env;
}

env* eval_def_fun(def def_fun, env* env_, mem* mem_){
	env* new_env = malloc(sizeof(env));
	new_env->id = def_fun->content.defFun.id;
	new_env->tag = ASTFun;
	new_env->adresse = false;
	new_env->content.def_fun.closure_ = malloc(sizeof(closure));
	new_env->content.def_fun.closure_->corp = def_fun->content.defFun.expr;
	new_env->content.def_fun.closure_->ids_ = malloc(sizeof(ids));
	new_env->content.def_fun.closure_->ids_ = args_to_ids(def_fun->content.defFun.arg_);
	new_env->content.def_fun.closure_->env_ = copy_env(env_);
	new_env->suite = env_;
	return new_env;
}

void print_env(env* env_){
	env* p = env_;
	while(p != NULL){
		printf("=========\n");
		printf("id : %s\n", p->id);
		if (p->tag == ASTConst)
			printf("v : %d\n", p->content.valeur);
		else if (p->tag == ASTFun) {
			printf("closure : \n");
			printf("expr : ");printSexpr(p->content.def_fun.closure_->corp);printf("\n");
			printf("args : ");printIds(p->content.def_fun.closure_->ids_);printf("\n");
			printf("env : ");print_env(p->content.def_fun.closure_->env_);printf("\n");
		}
		else if (p->tag == ASTRecFun) {
			printf("closure_rec : \n");
			printf("id : %s\n", p->content.def_rec.closure_rec_->id);
			printf("expr : ");printSexpr(p->content.def_rec.closure_rec_->corp);printf("\n");
			printf("args : ");printIds(p->content.def_rec.closure_rec_->ids_);printf("\n");
			printf("env : ");print_env(p->content.def_rec.closure_rec_->env_);printf("\n");
		}
		else if (p->tag == ASTVar) {
			printf("#Var# : \n");
			printf("id : %s\n", p->id);
			printf("v : %d\n", p->content.valeur);
		}
		else if (p->tag == ASTProc) {
			printf("#Proc# : \n");
			printf("args : ");printIds(p->content.def_proc.closure_proc_->ids_);printf("\n");
			printf("env : ");print_env(p->content.def_proc.closure_proc_->env_);printf("\n");
			printf("block : \n");print_prog(p->content.def_proc.closure_proc_->block);printf("\n");
		}
		else if (p->tag == ASTRecProc) {
			printf("#Proc Rec# : \n");
			printf("args : ");printIds(p->content.def_proc_rec.closure_proc_rec_->ids_);printf("\n");
			printf("env : ");print_env(p->content.def_proc_rec.closure_proc_rec_->env_);printf("\n");
			printf("block : \n");print_prog(p->content.def_proc_rec.closure_proc_rec_->block);printf("\n");
			printf("id : %s\n", p->content.def_proc_rec.closure_proc_rec_->id);
		}
		p = p->suite;
	}
}

int cherche_id_adr_env(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if ((!(strcmp(p->id, id))) && (p->adresse)){
			return p->content.valeur;
		}
		else {
			p = p->suite;
		}
	}
	return -99999997;
}

mem* affectation_mem(mem* mem_, int adresse, int valeur){
	mem_->content[adresse].valeure = valeur;
	return mem_;
}

mem* stat_set(env* env_, mem* mem_, stat stat_){
	int adresse = cherche_id_adr_env(env_, stat_->content.set.id);
	int valeur = eval_expr(env_, mem_, stat_->content.set.e);
	return affectation_mem(mem_, adresse, valeur);
}

mem* stat_call(env* env_, mem** mem_, stat stat_){
	Sexprs es = stat_->content.call_.expr;
		if (get_closure_proc(env_, stat_->content.call_.id) != NULL) {
			closure_proc* closure_proc_ = get_closure_proc(env_, stat_->content.call_.id);
			valeurs* valeurs_ = exprs_to_valeurs(env_, es, *mem_);
			env* env_tmp = lier_args_vals_env(closure_proc_->env_, closure_proc_->ids_, valeurs_);
			eval_prog(env_tmp, *mem_, closure_proc_->block);
		} else if (get_closure_proc_rec(env_, stat_->content.call_.id) != NULL){
			closure_proc_rec* closure_proc_rec_ = get_closure_proc_rec(env_, stat_->content.call_.id);
			valeurs* valeurs_ = exprs_to_valeurs(env_, es, *mem_);
			env* env_tmp = lier_args_vals_env(closure_proc_rec_->env_, closure_proc_rec_->ids_, valeurs_);
		    env_tmp = ajout_closure_rec_env_proc(env_tmp, closure_proc_rec_);
			eval_prog(env_tmp, *mem_, closure_proc_rec_->block);
		}
	return *mem_;
}

mem* stat_while(env* env_, mem** mem_, stat stat_){
	int taille = -1;
	if (eval_expr(env_, *mem_,  stat_->content.while_.condition)){
		if (*mem_ == NULL){
			taille = 0;
		} else {
			taille = (*mem_)->last_adr;
		}
		eval_prog(env_, (*mem_), stat_->content.while_.block);
		if (taille) {
			mem_ = realloc(mem_, taille*sizeof(mem));
		}
		*mem_ = stat_while(env_, mem_, stat_);
	} else {
		return *mem_;
	}
}

mem* stat_IF(env* env_, mem** mem_, stat stat_){
	int taille = -1;
	if (eval_expr(env_, *mem_,  stat_->content.if_block.condition)){
		if (*mem_ == NULL){
			taille = 0;
		} else {
			taille = (*mem_)->last_adr;
		}
		eval_prog(env_, (*mem_), stat_->content.if_block.block1);
		if (taille) {
			mem_ = realloc(mem_, taille*sizeof(mem));
		}
		return (*mem_);
	} else {
		if (*mem_ == NULL){
			taille = 0;
		} else {
			taille = (*mem_)->last_adr;
		}
		eval_prog(env_, (*mem_), stat_->content.if_block.block2);
		if (taille) {
			mem_ = realloc(mem_, taille*sizeof(mem));
		}
		return (*mem_);
	}
}

void eval_prog(env* env__, mem* mem__, prog* prog_){
	env* env_ =  env__;
	mem* mem_ = mem__;
	int valeur = -404404404;
	int size = prog_->size;
	int i = 0;
	while (i < size) {
		switch (prog_->cmds[i].type_) {
			case 1: // expr
			{
					valeur = eval_expr(env_, mem_, prog_->cmds[i].expr);
					break;
				}
			case 2: // def_const
					env_ = eval_def_const(prog_->cmds[i].def_const, env_, mem_);
					break;
			case 3: // def_fun
					env_ = eval_def_fun(prog_->cmds[i].def_fun, env_, mem_);
					break;
			case 4: // def_rec
					env_ = eval_def_rec(prog_->cmds[i].def_rec, env_, mem_);
					break;
			case 5: // def_var
					env_ = eval_def_var(prog_->cmds[i].def_var, env_, &mem_);
					break;
			case 6: // def_proc
					env_ = eval_def_proc(prog_->cmds[i].def_proc, env_, mem_);
					break;
			case 7: // def_proc_rec
					env_ = eval_def_proc_rec(prog_->cmds[i].def_rec_proc, env_, mem_);
					break;
			case 8 : 
					if (prog_->cmds[i].stat_->tag == ASTEcho){
						valeur = eval_expr(env_, mem_, prog_->cmds[i].stat_->content.echo.expr);
					} else if (prog_->cmds[i].stat_->tag == ASTSet){
						mem_ = stat_set(env_, mem_, prog_->cmds[i].stat_);
					} else if (prog_->cmds[i].stat_->tag == ASTIfBlock){
						mem_ = stat_IF(env_, &mem_, prog_->cmds[i].stat_);
					} else if (prog_->cmds[i].stat_->tag == ASTWhile){
						mem_ = stat_while(env_, &mem_, prog_->cmds[i].stat_);
					} else if (prog_->cmds[i].stat_->tag == ASTCall){
						mem_ = stat_call(env_, &mem_, prog_->cmds[i].stat_);
					}
					break;
			default:
					printf("Erreur\n");
		}
		i++;
	}
	if (valeur != -404404404)
	printf("\nECHO => %d\n", valeur);
}

env* eval_def_const(def def_const, env* env_, mem* mem_){
	env* new_env = malloc(sizeof(env));
	new_env->tag = ASTConst;
	new_env->adresse = false;
	new_env->id = def_const->content.defConst.id;
	int valeur_expr;
	valeur_expr = eval_expr(env_, mem_, def_const->content.defConst.expr);
	new_env->content.valeur = valeur_expr;
	new_env->suite = env_;
	return new_env;
}

env* eval_def_var(def def_var, env* env_, mem** mem_){
	env* new_env = malloc(sizeof(env));
	new_env->tag = ASTVar;
	new_env->adresse = true;
	new_env->id = def_var->content.defVar.id;
	*mem_ = alloc(*mem_);
	new_env->content.valeur = (*mem_)->last_adr;
	new_env->suite = env_;
	return new_env;
}

mem* alloc(mem* mem_){
	if (mem_ == NULL){
		mem_ = malloc(sizeof(mem));
		mem_->last_adr = 0;
		mem_->content = malloc(sizeof(adr_val));
		mem_->content->adresse = 0;
		mem_->content->valeure = ANY;
		return mem_;
	} else {
		mem_->last_adr++;
		int adresse = mem_->last_adr;
		adr_val* tmp = malloc(sizeof(adr_val));;
		tmp->adresse = adresse;
		tmp->valeure = ANY;
		mem_->content = realloc(mem_->content, (adresse+1)*sizeof(adr_val));
		mem_->content[adresse] = *tmp;
		tmp = NULL;
		return mem_;
	}
}

env* copy_env(env* env_){
	env* p = env_;
	env* copy = malloc(sizeof(env));
	env* p_copy = copy;
	while (p != NULL) { 
		p_copy->id = p->id;
		p_copy->tag = p->tag;
		p_copy->adresse = p->adresse;
		if (p_copy->tag == ASTConst){
			p_copy->content.valeur = p->content.valeur;
		} else if (p_copy->tag == ASTFun){
			p_copy->content.def_fun.closure_ = p->content.def_fun.closure_;
		} else if (p_copy->tag == ASTProc){
			p_copy->content.def_proc.closure_proc_ = p->content.def_proc.closure_proc_;
		} else if (p_copy->tag == ASTVar){
			p_copy->content.valeur = p->content.valeur;
		} else {
			p_copy = NULL;
		}
		p = p->suite;
		if(p != NULL){
			p_copy->suite = NULL;
			p_copy->suite = malloc(sizeof(env));
			p_copy = p_copy->suite;
		}		
	}
	if (env_ == NULL) copy = NULL;
	return copy;
}

int eval_expr(env* env_, mem* mem_, Sexpr expr){
	int indice = -9999;
	int valeur = -9999;
	switch (expr->tag) {
		case ASTNum:
				return expr->content.num;
				break;
		case ASTBool:
				if (!(strcmp(expr->content.boolean, "true"))) return 1;
					else return 0;
				break;
		case ASTId:
				valeur = cherche_id_env(env_, expr->content.id);
				if (!(valeur == -99999996)){
					return valeur;
				} else {
					valeur = cherche_id_adr_env(env_, expr->content.id);
					if (!(valeur == -99999997)){
						return mem_->content[valeur].valeure;
					}
				}
				break;
		case ASTNot:
				if (eval_expr(env_, mem_, expr->content.not_.e))
					return 0;
				else return 1;
				break;
		case ASTAdd:
				return eval_expr(env_, mem_, expr->content.add.e1)
							+ eval_expr(env_, mem_, expr->content.add.e2);
				break;
		case ASTMul:
				return eval_expr(env_, mem_, expr->content.mul.e1)
							* eval_expr(env_, mem_, expr->content.mul.e2);
				break;
		case ASTSub:
				return eval_expr(env_, mem_, expr->content.sub.e1)
							- eval_expr(env_, mem_, expr->content.sub.e2);
				break;
		case ASTDiv:
				return eval_expr(env_, mem_, expr->content.div.e1)
							/ eval_expr(env_, mem_, expr->content.div.e2);
				break;
		case ASTAnd:
				if (!(eval_expr(env_, mem_, expr->content.and_.e1))) return 0;
					else return eval_expr(env_, mem_, expr->content.and_.e2);
				break;
		case ASTOr:
				if (eval_expr(env_, mem_, expr->content.or_.e1)) return 1;
					else return eval_expr(env_, mem_, expr->content.or_.e2);
				break;
		case ASTEq: 
				if (eval_expr(env_, mem_, expr->content.eq.e1) == eval_expr(env_, mem_, expr->content.eq.e2)) 
					return 1;
					else return 0;
				break;
		case ASTLt:
				if (eval_expr(env_, mem_, expr->content.eq.e1) < eval_expr(env_, mem_, expr->content.eq.e2)) 
					return 1;
					else return 0;
				break;
		case ASTIf:
				if (eval_expr(env_, mem_, expr->content.if_.cond)) 
					return eval_expr(env_, mem_, expr->content.if_.cons);
					else return eval_expr(env_, mem_, expr->content.if_.alt);
				break;
		case ASTApp:{
				Sexpr e = expr->content.app.fun;
				Sexprs es = expr->content.app.args;
				if ((get_closure(env_, getId(e))) != NULL) {
					closure* closure_ = get_closure(env_, e->content.id);
					valeurs* valeurs_ = exprs_to_valeurs(env_, es, mem_);
					env* env_tmp = lier_args_vals_env(closure_->env_, closure_->ids_, valeurs_);
					return eval_expr(env_tmp, mem_, closure_->corp);
				} else if ((get_closure_rec(env_, getId(e))) != NULL) {
					closure_rec* closure_rec_ = get_closure_rec(env_, e->content.id);
					valeurs* valeurs_ = exprs_to_valeurs(env_, es, mem_);
					env* env_tmp = lier_args_vals_env(closure_rec_->env_, closure_rec_->ids_, valeurs_);				
					env_tmp = ajout_closure_rec_env(env_tmp, closure_rec_);
					return eval_expr(env_tmp, mem_, closure_rec_->corp);
				} else if (tagOf(e) == ASTAbs){
					closure* closure_ = malloc(sizeof(closure));
					closure_->corp = e->content.abstract.expr;
					closure_->ids_ = malloc(sizeof(ids));
					closure_->ids_ = args_to_ids(e->content.abstract.arg_);
					closure_->env_ = copy_env(env_);
					
					valeurs* valeurs_ = exprs_to_valeurs(env_, es, mem_);
					env* env_tmp = lier_args_vals_env(closure_->env_, closure_->ids_, valeurs_);
					return eval_expr(env_tmp, mem_, closure_->corp);	
				} else {
					return -5;
				} 
				break;
				}
		case ASTAbs:{
				return -10;
				break;
			}
		default:
				return -3;
    }	
}
