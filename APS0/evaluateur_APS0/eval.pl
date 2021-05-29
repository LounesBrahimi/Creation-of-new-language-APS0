assoc(X, [(X,V)|_], V).
assoc(X, [_|XVs], V) :- assoc(X, XVs, V).

expr(_, boolean(true), 1).
expr(_, boolean(false), 0).
expr(_, num(N), N) :- integer(N).
expr(Env, id(X), V) :- assoc(X, Env, V).

%not
expr(Env, not(EXPR), 0) :- expr(Env, EXPR, 1).
expr(Env, not(EXPR), 1) :- expr(Env, EXPR, 0).
%op calcul
expr(Env, add(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, V1), expr(Env, EXPR2, V2), RES is V1+V2.
expr(Env, sub(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, V1), expr(Env, EXPR2, V2), RES is V1-V2.
expr(Env, mul(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, V1), expr(Env, EXPR2, V2), RES is V1*V2.
expr(Env, div(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, V1), expr(Env, EXPR2, V2), RES is V1//V2.
%op logic
expr(Env, and(EXPR1, _), 0) :- expr(Env, EXPR1, 0).
expr(Env, and(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, 1), expr(Env, EXPR2, RES).
expr(Env, or(EXPR1, _), 1) :- expr(Env, EXPR1, 1).
expr(Env, or(EXPR1, EXPR2), RES) :- expr(Env, EXPR1, 0), expr(Env, EXPR2, RES).
expr(Env, eq(EXPR1, EXPR2), 1) :- expr(Env, EXPR1, VAL), expr(Env, EXPR2, VAL).
expr(Env, eq(EXPR1, EXPR2), 0) :- expr(Env, EXPR1, VAL1), expr(Env, EXPR2, VAL2), not(VAL1 = VAL2).
expr(Env, lt(EXPR1, EXPR2), 1) :- expr(Env, EXPR1, VAL1), expr(Env, EXPR2, VAL2), VAL1 < VAL2.
expr(Env, lt(EXPR1, EXPR2), 0) :- expr(Env, EXPR1, VAL1), expr(Env, EXPR2, VAL2), VAL1 >= VAL2.

%if
expr(Env, if(COND, CONS, _), RES) :- expr(Env, COND, 1), expr(Env, CONS, RES).
expr(Env, if(COND, _, ALT), RES) :- expr(Env, COND, 0), expr(Env, ALT, RES).

expr(Env, abst(args(XsTs), E), closure(E, Xs, Env)) :- listeArgs(XsTs, Xs).

expr(Env, app(F, Es), V) :-
	expr(Env, F, closure(Ef, Xs, Envf)),
	valeurs(Env, Es, Vs),
	lier(Xs, Vs, XsVs),
	append(XsVs, Envf, EnvfXsVs),
	expr(EnvfXsVs, Ef, V).

expr(Env, app(F, Es), V) :-
	expr(Env, F, closure_rec(Xf, Ef, Xs, Envf)),	
	valeurs(Env, Es, Vs),	
	lier(Xs, Vs, XsVs),
	append(XsVs, Envf, EnvfXsVs),
	expr([(Xf, closure_rec(Xf, Ef, Xs, Envf))|EnvfXsVs], Ef, V).

lier([], [], []).
lier([X1|Xs], [V1|Vs], [(X1, V1)|XsVs]) :- lier(Xs, Vs, XsVs).

valeurs(_, [], []).
valeurs(Env, [E1|Es], [V1|Vs]) :- expr(Env, E1, V1), valeurs(Env, Es, Vs).

listeArgs([], []).
listeArgs([(X1, _)|XsTs], [X1|Xs]) :- listeArgs(XsTs, Xs).

decs(Env, (id(X),_,EXPR), [(X, V)|Env]) :- expr(Env, EXPR, V).
decfun(Env,(id(X),_,ARGS,EXPR),[(X, closure(EXPR, Xs, Env))|Env]) :- listeArgs(ARGS, Xs).
decfunrec(Env,(id(X),_,ARGS,EXPR),[(X, closure_rec(X, EXPR, Xs, Env))|Env]) :- listeArgs(ARGS, Xs).

stat(Env, O, EXPR, [N|O]) :- expr(Env, EXPR, N).

cmds(Env, O, [], Env, O).
cmds(Env, O, [defConst(id(X),type(T),EXPR)|LCMDS], Env2, O2):- decs(Env, (id(X),T,EXPR), Env1), cmds(Env1, O, LCMDS, Env2, O2).
cmds(Env, O, [defFun(id(X),type(T), args(ARGS), EXPR)|LCMDS], Env2, O2):-  decfun(Env, (id(X),T,ARGS,EXPR), Env1), cmds(Env1, O, LCMDS, Env2, O2).
cmds(Env, O, [defFunRec(id(X),type(T), args(ARGS), EXPR)|LCMDS], Env2, O2):-  decfunrec(Env, (id(X),T,ARGS,EXPR), Env1), cmds(Env1, O, LCMDS, Env2, O2).

cmds(Env, O, [echo(EXPR)|LCMDS], Env1, O2) :- stat(Env, O, EXPR, O1), cmds(Env, O1, LCMDS, Env1, O2).

%%prog
prog(LCMDS) :- cmds([], [], LCMDS, _, O), print(O).
