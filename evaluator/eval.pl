assoc(X, [(X,V)|_], V).
assoc(X, [_|XVs], V) :- assoc(X, XVs, V).

expr(_, boolean(true), 1).
expr(_, boolean(false), 0).
expr(_, num(N), N) :- integer(N).

expr(Env, id(X), V) :- assoc(X, Env, V).

decs(Env, (id(X),_,EXPR), [(X, V)|Env]) :- expr(Env, EXPR, V).

stat(Env, O, EXPR, [N|O]) :- expr(Env, EXPR, N).

cmds(Env, O, [], Env, O).
cmds(Env, O, [defConst(id(X),type(T),EXPR)|LCMDS], Env2, O2):- decs(Env, (id(X),T,EXPR), Env1), cmds(Env1, O, LCMDS, Env2, O2). 
cmds(Env, O, [echo(EXPR)|LCMDS], Env1, O2) :- stat(Env, O, EXPR, O1), cmds(Env, O1, LCMDS, Env1, O2).

%%prog
prog(LCMDS) :- cmds([], [], LCMDS, _, O), print(O).
