read("c:\\jacques\\eco\\ramsey31in.txt");
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
Error: Invalid index;
during evaluation of 'read';
while reading file 'c:\jacques\eco\ramsey31in.txt'

@} */
s;
/* @O{
    @G28

@} */
cstar:=float(s[1][1][2]);

/* @O{
    @G29

@} */
kstar:=float(s[1][2][2]);

/* @O{
    @G30

@} */
edk;
/* @O{
    @G31

@} */
edk[2];
/* @O{
    @G32

@} */
edc;
/* @O{
    @G33

@} */
solve(edc,diff(c(t),t));
/* @O{
Error: Illegal variable(s) to solve for: diff(c(t), t) [solve]

@} */
edcs:=subs(edc,diff(c(t),t)=x);
/* @O{
    @G34

@} */
dc:=solve(edcs,x);
/* @O{
Error: Computation aborted;
during evaluation of 'piecewise::flattenBranch'

@} */
edcsn:=normal(edcs);
/* @O{
    @G35

@} */
edc;
/* @O{
    @G36

@} */
normal(edc);
/* @O{
    @G37

@} */
simplify(edc);
/* @O{
    @G38

@} */
ratnormal(edc);
/* @O{
    @G39

@} */
radsimpl(edc);
/* @O{
    @G40

@} */
radsimp(edc);
/* @O{
    @G41

@} */
combine(edc);
/* @O{
    @G42

@} */
Simplify(edc);
/* @O{
    @G43

@} */
nor;ql5Si;plify5edc--;
/* @O{
    @G44
    @G45
nor;ql5Si;plify5edc--
                     ^ Error: 'expression' expected [line 1, col 22]

@} */
normal(simplify(edc));
/* @O{
    @G46

@} */
normal(Simplify(edc));
/* @O{
    @G47

@} */
edc:=Simplify(edc);
/* @O{
    @G48

@} */
edc+diff(c(t),t);
/* @O{
    @G49

@} */
edk;
/* @O{
    @G50

@} */
edc;
edc;
/* @O{
    @G51

@} */
f:=(t,kc)->[-0.1*kc[1]-kc[2]+8.0*kc[1]^0.3,-0.15*kc[2]+2.4*kc[2]/kc[1]^(7/10)];
/* @O{
    @G52

@} */
sol:=numeric::odesolve(0..50,f,[kstar,0.999*cstar]);
/* @O{
    @G53

@} */
f1:=(x,Y)->[Y[1]+2*Y[2],Y[1]];
/* @O{
    @G54

@} */
numeric::odesolve(0..1,f,[1,1]);
/* @O{
    @G55

@} */

