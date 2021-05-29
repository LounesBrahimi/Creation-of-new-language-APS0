assoc(X, [(X,V)|_], V).
assoc(X, [_|XVs], V) :- assoc(X, XVs, V).

%% constantes numeriques
num(N) :- integer(N).

%% constantes booleennes
boolean(true).
boolean(false).

%% les types primaires
type(int).
type(bool).

%% expressions atomiques
expr(_, int, num(N)) :- num(N).
expr(_ , bool, boolean(B)) :- boolean(B).
expr(G, T, id(X)) :- assoc(X, G, T).
expr(G, T, id(X)) :- assoc(X, G, ref(T)).

%% fonctions primitives
expr(G, int, add(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, sub(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, mul(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, div(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, bool, and(E1, E2)) :- expr(G, bool, E1), expr(G, bool, E2).
expr(G, bool, or(E1, E2)) :- expr(G, bool, E1), expr(G, bool, E2).
expr(G, bool, not(E)) :- expr(G, bool, E).
expr(G, bool, eq(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, bool, lt(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).

%% if
expr(G, int, if(COND, CONS, ALT)) :- expr(G, bool, COND), expr(G, int, CONS), expr(G, int, ALT).
expr(G, bool, if(COND, CONS, ALT)) :- expr(G, bool, COND), expr(G, bool, CONS), expr(G, bool, ALT).

%% appication
expr(G, TYPE, app(EXPR,AEXPR)) :- typeaexprs(G,AEXPR,TYPESAE), expr(G,arrow(TYPESAE,TYPE),EXPR).

%% abstraction
expr(G,arrow(TYPES,TYPE),abst(args(ARGS),EXPR)) :- typeargs(ARGS,TYPES),append(ARGS,G,NEWG),expr(NEWG,TYPE,EXPR).

%%recupere le type des expressions pour l'application
typeaexprs(G,[E|[]],[TYPE]) :- exprp(G,TYPE, E).
typeaexprs(G,[E|ES],[TYPE|TYPES]) :- exprp(G,TYPE,E) , typeaexprs(G,ES,TYPES).

dec(G, (def(id(X), T, E)), [(X,T)|G]) :- expr(G, T, E).

defConst(_, type(T), E) :- expr(_, T, E).

add(E1, E2) :- expr(_, int, add(E1, E2)).
sub(E1, E2) :- expr(_, int, sub(E1, E2)).
mul(E1, E2) :- expr(_, int, mul(E1, E2)).
div(E1, E2) :- expr(_, int, div(E1, E2)).
and(E1, E2) :- expr(_, bool, and(E1, E2)).
or(E1, E2) :- expr(_, bool, or(E1, E2)).
not(E) :- expr(_, bool, not(E)).
eq(E1, E2) :- expr(_, bool, eq(E1, E2)).
lt(E1, E2) :- expr(_, bool, lt(E1, E2)).

if(COND, CONS, ALT) :- expr(_, int, if(COND, CONS, ALT)).
if(COND, CONS, ALT) :- expr(_, bool, if(COND, CONS, ALT)).

%%stat
stat(G,EXPR,void) :- expr(G,int,EXPR).

%%recupere le type des arguments
typeargs([(_,T)],[T]).
typeargs([(_,T)|AA], [T|TT]) :- typeargs(AA,TT).

%%fun
fun(G,(TYPE,[],EXPR)) :- expr(G,TYPE,EXPR).
fun(G,(TYPE,[ARG|ARGS],EXPR)) :- fun([ARG|G],(TYPE,ARGS,EXPR)).

%%decs
decsRecProc(G, (id(X), ARGS, block(CMDS)), [(X, arrow(TYPES, void))|G]) :-
	typeargs(ARGS, TYPES), append(ARGS, G, ARGSG),
		cmds([(X, arrow(TYPES, void))|ARGSG], CMDS, void).
decsProc(G, (id(X), ARGS, block(CMDS)), [(X, arrow(TYPES, void))|G]) :-
	typeargs(ARGS, TYPES), append(ARGS, G, ARGSG),
		cmds(ARGSG, CMDS, void).
decs(G, (id(X),TYPE,EXPR), [(X,TYPE)|G]):- expr(G, TYPE, EXPR).
decfun(G,(id(X),TYPE,ARGS,EXPR),[(X,arrow(TYPEARGS,TYPE))|G]) :- fun(G,(TYPE,ARGS,EXPR)), typeargs(ARGS, TYPEARGS).
decfunrec(G,(id(X),TYPE,ARGS,EXPR),[(X,arrow(TYPEARGS,TYPE))|G]) :- fun(G,(TYPE,[(X,arrow(TYPEARGS,TYPE))|ARGS],EXPR)), typeargs(ARGS, TYPEARGS).
decsVar(G,(id(X),T),[(X,ref(T))|G]) :- type(T).

%%ref
exprp(G, ref(T), adr(X)) :- assoc(X, G, ref(T)).
exprp(G, T, E) :- expr(G, T, E).

%%cmds
cmds(G, [defRecProc(id(X),args(ARGS),block(CMDS))|LCMDS], void) :- decsRecProc(G,(id(X),ARGS, block(CMDS)),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defProc(id(X),args(ARGS),block(CMDS))|LCMDS], void) :- decsProc(G,(id(X),ARGS, block(CMDS)),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(_, [], void).
cmds(G, [defConst(id(X),type(T),EXPR)|LCMDS], void) :- decs(G,(id(X),T,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defConst(id(X),arrow([T],Z),EXPR)|LCMDS], void) :- decs(G,(id(X),arrow([T],Z),EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defFun(id(X),type(T), args(ARGS), EXPR)|LCMDS], void) :- decfun(G,(id(X),T,ARGS,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defFun(id(X), arrow([T],Z), args(ARGS), EXPR)|LCMDS], void) :- decfun(G,(id(X),arrow([T],Z),ARGS,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defFunRec(id(X),type(T), args(ARGS), EXPR)|LCMDS], void) :- decfunrec(G,(id(X),T,ARGS,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defFunRec(id(X),arrow([T],Z), args(ARGS), EXPR)|LCMDS], void) :- decfunrec(G,(id(X),arrow([T],Z),ARGS,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [defVar(id(X),ref(type(T)))|LCMDS], void) :- decsVar(G,(id(X),T),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [echo(EXPR)|LCMDS], void) :- stat(G, EXPR, void), cmds(G, LCMDS, void).
cmds(G, [set(id(X), EXPR)|LCMDS], void) :- assoc(X, G, ref(TYPE)), expr(G, TYPE, EXPR), cmds(G, LCMDS, void).
cmds(G, [ifBlock(COND, block(CMDS1), block(CMDS2))|LCMDS],void) :- expr(G, bool, COND), cmds(G, CMDS1, void),
		cmds(G, CMDS2, void), cmds(G, LCMDS, void).
cmds(G, [while(COND, block(CMDS))|LCMDS],void) :- expr(G, bool, COND), 
		cmds(G, CMDS, void), cmds(G, LCMDS, void).
cmds(G,[call(id(X), EXPRS)|LCMDS],void) :- 
	typeaexprs(G, EXPRS, TYPES), assoc(X, G, arrow(TYPES, void)),
	cmds(G, LCMDS, void).

%%prog
prog(LCMDS) :- cmds([], LCMDS, void).

typeCheck(P,ok) :- P.
typeCheck(_,ko).

exitCode(ok) :- halt(0).
exitCode(_) :- halt(1).

main_stdin :-
	read(user_input,T),
	typeCheck(T,R),
	print(R),
	nl,
	exitCode(R).
