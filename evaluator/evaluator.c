#include "evaluator.h"

int cherche_id_env(env* env_, char* id){
	env* p = env_;
	while(p != NULL){
		if (!(strcmp(p->id, id))){
			return p->valeur;
		}
		else {
			p = p->suite;
		}
	}
	return -2;
}

int eval_expr(env* env_, Sexpr expr){
	switch (expr->tag) {
		case ASTNum:
				return expr->content.num;
				break;
		case ASTBool:
				if (!(strcmp(expr->content.boolean, "true"))) return 1;
					else return 0;
				break;
		case ASTId:
				return cherche_id_env(env_, expr->content.id);
				break;
		case ASTNot:
				if (eval_expr(env_, expr->content.not_.e))
					return 0;
				else return 1;
				break;
		case ASTAdd:
				return eval_expr(env_, expr->content.add.e1)
							+ eval_expr(env_, expr->content.add.e2);
				break;
		case ASTMul:
				return eval_expr(env_, expr->content.mul.e1)
							* eval_expr(env_, expr->content.mul.e2);
				break;
		case ASTSub:
				return eval_expr(env_, expr->content.sub.e1)
							- eval_expr(env_, expr->content.sub.e2);
				break;
		case ASTDiv:
				return eval_expr(env_, expr->content.div.e1)
							/ eval_expr(env_, expr->content.div.e2);
				break;
		case ASTAnd:
				if (!(eval_expr(env_, expr->content.and_.e1))) return 0;
					else return eval_expr(env_, expr->content.and_.e2);
				break;
		case ASTOr:
				if (eval_expr(env_, expr->content.or_.e1)) return 1;
					else return eval_expr(env_, expr->content.or_.e2);
				break;
		case ASTEq:
				if (eval_expr(env_, expr->content.eq.e1) == eval_expr(env_, expr->content.eq.e2)) 
					return 1;
					else return 0;
				break;
		case ASTLt:
				if (eval_expr(env_, expr->content.eq.e1) < eval_expr(env_, expr->content.eq.e2)) 
					return 1;
					else return 0;
				break;
		case ASTIf:
				if (eval_expr(env_, expr->content.if_.cond)) 
					return eval_expr(env_, expr->content.if_.cons);
					else return eval_expr(env_, expr->content.if_.alt);
				break;
		default:
				return -3;
    }	
}

env* eval_def_const(def def_const, env* env_){
	env* new_env = malloc(sizeof(env));
	new_env->id = def_const->content.defConst.id;
	int valeur_expr;
	valeur_expr = eval_expr(env_, def_const->content.defConst.expr);
	new_env->valeur = valeur_expr;
	new_env->suite = env_;
	return new_env;
}

void print_env(env* env_){
	env* p = env_;
	while(p != NULL){
		printf("=========\n");
		printf("id : %s\n", p->id);
		printf("v : %d\n", p->valeur);
		p = p->suite;
	}
}

void eval_prog(prog* prog_){
	env* env_ = NULL;
	int valeur = -404;
	int size = prog_->size;
	int i = 0;
	printf("###prog####\n");
	printf("#1 : expr\n");
	printf("#2 : def_const\n");
	printf("size : %d\n\n", size);
	//printf("type cmds => %d \n", prog_->cmds[i].type_);
	
	while (i < size) {
		switch (prog_->cmds[i].type_) {
			case 1: // expr
					valeur = eval_expr(env_, prog_->cmds[i].expr);
					break;
			case 2: // def_const
					env_ = eval_def_const(prog_->cmds[i].def_const, env_);
					break;
			default:
					printf("Erreur\n");
		}
		i++;
	}
	print_env(env_);
	printf("Resultat => %d\n", valeur);
}
