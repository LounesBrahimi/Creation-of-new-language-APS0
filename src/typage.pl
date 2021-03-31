assoc(X, [(X,V)|_], V).
assoc(X, [_|XVs], V) :- assoc(X, XVs, V).

num(N) :- integer(N).
boolean(true).
boolean(false).
%%id(X).

expr(_, int, num(N)) :- num(N).
expr(_ , bool, boolean(B)) :- boolean(B).
%%expr(G, T, id(X)) :- id(X), assoc(X, G, T).
expr(G, T, id(X)) :- assoc(X, G, T).
%%
expr(G, int, add(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, sub(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, mul(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, int, div(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, bool, and(E1, E2)) :- expr(G, bool, E1), expr(G, bool, E2).
expr(G, bool, or(E1, E2)) :- expr(G, bool, E1), expr(G, bool, E2).
expr(G, bool, not(E)) :- expr(G, bool, E).
expr(G, bool, eq(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
expr(G, bool, lt(E1, E2)) :- expr(G, int, E1), expr(G, int, E2).
%%
expr(G, int, if(COND, CONS, ALT)) :- expr(G, bool, COND), expr(G, int, CONS), expr(G, int, ALT).
expr(G, bool, if(COND, CONS, ALT)) :- expr(G, bool, COND), expr(G, bool, CONS), expr(G, bool, ALT).
%%
dec(G, (def(id(X), T, E)), [(X,T)|G]) :- expr(G, T, E).

%%stat(G, ech(num(N)), void) :- expr(G, int,num(N)).%% int echo
%%stat(G, ech(boolean(B)), void) :- expr(G, bool,boolean(B)).%% bool echo
%%stat(G, ech(id(X)), void) :- expr(G, id(X), int).%% bool id

defConst(_, type(T), E) :- expr(_, T, E).
%%
add(E1, E2) :- expr(_, int, add(E1, E2)).
sub(E1, E2) :- expr(_, int, sub(E1, E2)).
mul(E1, E2) :- expr(_, int, mul(E1, E2)).
div(E1, E2) :- expr(_, int, div(E1, E2)).
and(E1, E2) :- expr(_, bool, and(E1, E2)).
or(E1, E2) :- expr(_, bool, or(E1, E2)).
not(E) :- expr(_, bool, not(E)).
eq(E1, E2) :- expr(_, bool, eq(E1, E2)).
lt(E1, E2) :- expr(_, bool, lt(E1, E2)).
%%
if(COND, CONS, ALT) :- expr(_, int, if(COND, CONS, ALT)).
if(COND, CONS, ALT) :- expr(_, bool, if(COND, CONS, ALT)).
%%
%%echo(E) :- stat([], ech(E), void).

%%decs
decs(G, (id(X),TYPE,EXPR), [(X,TYPE)|G]):- expr(G, TYPE, EXPR).

%%stat
stat(G, EXPR, void) :- expr(G, int, EXPR).

%%cmds
cmds(_, [], void).
cmds(G, [defConst(id(X),type(T),EXPR)|LCMDS], void) :- decs(G,(id(X),T,EXPR),NEWG) ,cmds(NEWG, LCMDS, void).
cmds(G, [echo(EXPR)|LCMDS], void) :- stat(G, EXPR, void), cmds(G, LCMDS, void).

%%prog
prog(LCMDS) :- cmds([], LCMDS, void).

%%typeCheck(P,ok) :- typeProg(P).
typeCheck(num(N),ok) :- num(N).
typeCheck(defConst(X,Y,Z),ok) :- defConst(X,Y,Z).
typeCheck(_,ko).

exitCode(ok) :- halt(0).
exitCode(_) :- halt(1).

main_stdin :-
	read(user_input,T),
	typeCheck(T,R),
	print(R),
	nl,
	exitCode(R).
