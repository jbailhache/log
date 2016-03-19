/* slc */

domains
 /* proof (node, level, name, sd1, sd2) */
 /* proof = proof (symbol, integer, symbol, proof, proof) */
 dem = n ; dem(symbol,integer,symbol,dem,dem)
 /*
 dem = symb(symbol);
  var(integer);
  axiom(dem,dem);
  ap(dem,dem) ; 
  ltransym(dem,dem);
  rtransym(dem,dem);
  lambda(dem);
  subst(dem,dem);
  left(dem);
  right(dem)
  */

predicates
 shift (integer, integer, dem, dem)
 subst (integer, dem, dem, dem)
 dbname (integer, dem, dem, dem)
 lr (symbol, dem,dem)
 sym (dem, dem)
 trans (dem, dem, dem)
 loopright (dem, dem)
 red (dem, dem)
 red1 (dem, integer, dem)
 red2 (dem, integer, dem)
 red3 (dem, integer, dem)
 red3ap (dem, dem, integer, dem)
 red3ap1 (dem, dem, integer, dem)
 red3ap2 (dem, integer, dem)
 red3ap3 (dem, dem, dem)
 reduc (dem, integer, dem)
     
clauses
/*
 shift(M,N,var(N1),var(N2)) if N1 >= M and N2=N+N1.
 shift(M,_,var(N1),var(N1)) if N1 < M.
 shift(M,N,lambda(D1),lambda(D2)) 
  if M1=M+1 and shift(M1,N,D1,D2).
*/
 shift(_,_,n,n) :- !.
 shift(M,N,dem(var,N1,n,n,n),dem(var,N2,n,n,n))
  if N1>=M and ! and N2=N+N1.
 shift(M,_,dem(var,N1,n,n,n),dem(var,N1,n,n,n))
  if ! and N1<M.
 shift(M,N,dem(lambda,0,n,D1,n),dem(lambda,0,n,D2,n))
  if ! and M1=M+1 and shift(M1,N,D1,D2).
 shift(_,_,dem(symb,0,S,n,n),dem(symb,0,S,n,n)) if !.
 shift(M,N,dem(A,B,C,D,E),dem(A,B,C,F,G))
  if shift(M,N,D,F) and shift(M,N,E,G).

 subst(_,n,_,n) :- !.
 subst(N,dem(var,N,n,n,n),Z,Z) :- ! .
 subst(N,dem(var,P,n,n,n),_,dem(var,Q,n,n,n)) :- P>N, !, Q=P-1.
 subst(N,dem(ap,0,n,LY,RY),Z,dem(ap,0,n,LW,RW)) :-
  !, subst(N,LY,Z,LW), subst(N,RY,Z,RW). 
 subst(N,dem(lambda,0,n,Y,n),Z,dem(lambda,0,n,W,n)) :-
  !, N1=N+1, shift(0,1,Z,V), subst(N1,Y,V,W).
 subst(_,dem(symb,0,S,n,n),_,dem(symb,0,S,n,n)) :- !.
 subst(N,dem(K,P,_,L,R),Z,dem(K,P,n,L1,R1)) :-
  subst(N,L,Z,L1), subst(N,R,Z,R1).
  
 dbname(_,_,n,n) :- !.
 dbname(N,X,X,dem(var,N,n,n,n)) :- !.
 dbname(N,X,dem(ap,0,n,L,R),dem(ap,0,n,L1,R1)) :-
  !, dbname(N,X,L,L1), dbname(N,X,R,R1).
 dbname(N,X,dem(lambda,0,n,Y,n),dem(lambda,0,n,Z,n)) :-
  !, N1=N+1, dbname(N1,X,Y,Z).
 dbname(N,X,dem(K,P,S,L,R),dem(K,P,S,L1,R1)) :-
  dbname(N,X,L,L1), dbname(N,X,R,R1).

 lr(M,dem(ap,0,n,L,R),dem(ap,0,n,L1,R1)) :- !,
  lr(M,L,L1), lr(M,R,R1).
  
 lr(l,dem(ltransym,0,n,X,Y),W) :- !,
  lr(l,X,Z), lr(l,Y,Z), lr(r,X,W).
  
 lr(r,dem(rtransym,0,n,X,Y),W) :- !,
  lr(l,X,Z), lr(l,Y,Z), lr(r,Y,W).
  
 lr(l,dem(rtransym,0,n,X,Y),W) :- !,
  lr(r,X,Z), lr(r,Y,Z), lr(l,X,W).
  
 lr(r,dem(rtransym,0,n,X,Y),W) :- !,
  lr(r,X,Z), lr(r,Y,Z), lr(l,Y,W).
  
 lr(_,dem(symb,0,S,n,n),dem(symb,0,S,n,n)) :- !.
 
 lr(_,dem(var,N,n,n,n),dem(var,N,n,n,n)) :- !.
 
 lr(M,dem(lambda,0,n,X,n),dem(lambda,0,n,Y,n)) :- !,
  lr(M,X,Y).
  
 lr(l,dem(axiom,0,n,X,_),X) :- !.
 lr(r,dem(axiom,0,n,_,X),X) :- !.
 
 lr(l,dem(left,0,n,X,n),dem(left,0,n,Y,n)) :- !, lr(l,X,Y).
 lr(r,dem(left,0,n,X,n),Y) :- !, lr(r,X,Z), lr(l,Z,Y).
 
 lr(l,dem(right,0,n,X,n),dem(right,0,n,Y,n)) :- !, lr(l,X,Y).
 lr(r,dem(right,0,n,X,n),Y) :- !, lr(r,X,Z), lr(r,Z,Y).
 
 lr(l,dem(apsubst,0,n,X,Y),dem(ap,0,n,S0,S1)) :- !,
  lr(l,X,S0), lr(l,Y,S1).
 
 lr(r,dem(apsubst,0,n,X,Y),W) :-
  lr(r,X,dem(lambda,0,n,Y1,n)), !,
  lr(r,Y,A),
  subst(0,Y1,A,W).
 
 lr(r,dem(apsubst,0,n,X,Y),dem(ap,0,n,S0,S1)) :- !,
  lr(r,X,S0), lr(r,Y,S1).

 lr(_,X,X).

   
 sym(AB,BA) :- lr(l,AB,AA), BA=dem(ltransym,0,n,AB,AA).    
   
 trans(AB,BC,AC) :-
  sym(AB,BA),
  AC=dem(ltransym,0,n,BA,BC).
   
 loopright(X,X) :- lr(r,X,X), !. 
 loopright(X,Y) :- lr(r,X,Z), loopright(Z,Y).   
 
 red(X,Y) :- red1(X,1,Y).
 red1(X,A,Y) :- red2(X,A,Y).
 
 red2(_,_,_).
 
 red3(dem(lambda,0,n,X,n),RA,dem(lambda,0,n,Y,n)) :- !, red1(X,RA,Y).
 red3(dem(ap,0,n,F,A),RA,R) :- !, red3ap(F,A,RA,R).
 red3(X,_,X).
 
 red3ap(dem(lambda,0,n,X,n),A,_,dem(subst,0,n,X,A)) :- !.
 red3ap(dem(ap,0,n,F1,A1),A,RA,R) :- !, red3ap1 (dem(ap,0,n,F1,A1),A,RA,R).
 red3ap(F,A,1,dem(ap,0,n,F,A2)) :- red1(A,1,A2).
 red3ap(F,A,0,dem(ap,0,n,F,A)).

 red3ap1(F,A,1,R) :-
  red1(F,1,F1),
  red1(A,1,A1),
  T3=dem(ap,0,n,F1,A1),
  red3ap2(T3,1,R).
  
 red3ap1(F,A,0,R) :-
  red1(F,1,F1),
  T3=dem(ap,0,n,F1,A),
  red3ap2(T3,0,R).
  
 red3ap2(T3,_,T3) :- lr(l,T3,T3LR), lr(r,T3,T3LR), !. 
 red3ap2(T3,RA,R) :- lr(r,T3,T3R), red1(T3R,RA,T4), red3ap3(T3,T4,R).
 
 red3ap3(T3,T4,T3) :- lr(l,T4,T4LR), lr(r,T4,T4LR), !.
 red3ap3(T3,T4,T) :- trans(T3,T4,T). 
   

  
   

 reduc(AB,RA,CD) :-
  lr(l,AB,A),
  lr(r,AB,B),
  red1(A,RA,AC),
  red1(B,RA,BD),
  CB=dem(ltransym,0,n,AC,AB),
  trans(CB,BD,CD).
  
   

