
simpl (plus(X,0), X).
simpl (plus(0,X), X) :- cut.
simpl (times(X,0), 0) :- cut.
simpl (times(0,X), 0) :- cut.
simpl (times(X,1), X) :- cut.
simpl (times(1,X), X) :- cut.
simpl (plus(X,minus(X)), 0) :- cut.
simpl (plus(minus(X),X), 0) :- cut.
simpl (plus(X,Y), plus(X1,Y1)) :- simpl(X,X1), simpl(Y,Y1).
simpl (times(X,Y), times(X1,Y1)) :- simpl(X,X1), simpl(Y,Y1).
simpl (X, X).

simplif (X, X).
simplif (X, Z) :- simpl (X, Y), cut, simplif (Y, Z), cut.

goal :- simplif (times(a,plus(minus(b),b)), X),
	printexpr (X), printstring ("""\n""").

goal1 :- simpl (times(a,plus(minus(b),b)), X),
	simpl (X, Y),
	printexpr(Y), printstring ("""\n""").

goal2 :- simpl (plus(0,a), X),
	printexpr(X), printstring("""\n""").

