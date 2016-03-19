read("c:\\jacques\\eco\\ramsey32in.txt");
/* @O{
    @G1
    @G2
    @G3
    @G4
    @G5
    @G6
    @G7
    @G8
    @G9
    @G10
    @G11
    @G12
    @G13
    @G14
    @G15
    @G16
    @G17
    @G18
    @G19
    @G20
    @G21
    @G22
    @G23
    @G24
    @G25
    @G26
    @G27
    @G28

@} */
s;
/* @O{
    @G29

@} */
cstar:=float(s[1][1][2]);

/* @O{
Error: Invalid index [list]

@} */
s;
/* @O{
    @G30

@} */
cstar:=float(s[1][2][1][2]);

/* @O{
    @G31

@} */
kstar:=float(s[1][2][2][2]);
/* @O{
    @G32

@} */
edc;
/* @O{
    @G33

@} */
edc+diff(c(t),t);
/* @O{
    @G34

@} */
dct=edc+diff(c(t),t);
/* @O{
    @G35

@} */
edk;
/* @O{
    @G36

@} */
dkt:=edk[2];
/* @O{
    @G37

@} */
plotfunc2d(x*sin(1/x),x=0.001..10);
/* @O{
 @G38
    @G39

@} */
dct1:=subs(dct,k(t)=kc[1],c(t)=kc[2]);

/* @O{
    @G40

@} */
dct;
/* @O{
    @G41

@} */
edc;
/* @O{
    @G42

@} */
dct=edc+diff(c(t),t);

/* @O{
    @G43

@} */
dct1:=subs(dct,k(t)=kc[1],c(t)=kc[2]);

/* @O{
    @G44

@} */
dct:=edc+diff(c(t),t);

/* @O{
    @G45

@} */
dct1:=subs(dct,k(t)=kc[1],c(t)=kc[2]);

/* @O{
    @G46

@} */
dkt:=edk[2];
/* @O{
    @G47

@} */
dkt1:=subs(dkt,k(t)=kt[1],c(t)=kc[2]);

/* @O{
    @G48

@} */
f:=(t,kc)->[edk1,edc1];
/* @O{
    @G49

@} */
edk!;
/* @O{
Error: argument must be of 'Type::Arithmetical' [fact]

@} */
f:=(t,kc)->[dkt1,dct1];

/* @O{
    @G50

@} */
dkt!;
/* @O{
    @G51

@} */
dct!;
/* @O{
    @G52

@} */
dkt1;
/* @O{
    @G53

@} */
dct1;
/* @O{
    @G54

@} */
sol:=T->numeric::odesolve(0..T,f,[kstar,0.999*cstar]);
/* @O{
    @G55

@} */
plotfunc2d(sol(T),T=0..50);
/* @O{
Error: Can't evaluate to boolean [_leequal];
during evaluation of 'numeric::odesolve'

@} */
sol(10);
/* @O{
Error: Can't evaluate to boolean [_leequal];
during evaluation of 'startingstepsize'

@} */
numeric::odesolve(0..10,f,[kstar,0,999*cstar]);
/* @O{
Error: size of vector field and initial vector do not match [numeric::odesolve]

@} */
numeric::odesolve(0..10,f,[kstar,0.999*cstar]);
/* @O{
Error: Can't evaluate to boolean [_leequal];
during evaluation of 'startingstepsize'

@} */
f1:=(t,kc)->[-0.1*kc[1]-kc[2]+8.0*kc[1]^0.3,-0.15*kc[2]+2.4*kc[2]/kc[1]^(7/10)];
/* @O{
    @G56

@} */
sol!:=numeric::odesolve(0..!0,f,[kstar,0.999*cstar]);
/* @O{
sol!:=
     ^ Error: Invalid left-hand side in assignment [line 1, col 6]

@} */
sol1:=numeric::odesolve(0..50,f,[kstar,0.999*cstar]);
/* @O{
Error: Can't evaluate to boolean [_leequal];
during evaluation of 'startingstepsize'

@} */
kstar;
/* @O{
    @G57

@} */
cstar;
/* @O{
    @G58

@} */

