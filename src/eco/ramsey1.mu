3+44;
/* @O{
    @G1

@} */
#\$;
/* @O{
#
^ Error: Illegal character '#' [line 1, col 1]

@} */
3*4;
/* @O{
    @G2

@} */
3*4;
/* @O{
    @G3

@} */
1;
/* @O{
    @G4

@} */
diff(x^3,x);

H:=U(c)*exp(-T*t)+p*(f(k)-n*k-c);
/* @O{
    @G5

@} */
ec.=diff5Hmc-=);
/* @O{
ec.=d
    ^ Error: 'expression' expected [line 1, col 5]

@} */
ec:=diff(H,c)=0;
/* @O{
    @G6

@} */
solve(ec,p);
/* @O{
    @G7

@} */
p1:=solve(ec,p);
/* @O{
    @G8

@} */
p1[0];
/* @O{
    @G9

@} */
p1+0;
/* @O{
    @G10

@} */
p1[1];
/* @O{
    @G11

@} */
p1=p1[1];
/* @O{
    @G12

@} */
p1;
/* @O{
    @G13

@} */
p1:=p1[1];
/* @O{
    @G14

@} */
p1;
/* @O{
    @G15

@} */

ek:=diff(P(t),t)=subs(-diff(H,k),p=P(t));
/* @O{
    @G16

@} */
p2:=subs(p1,c=C(t));
/* @O{
    @G17

@} */
D(sin);
/* @O{
    @G18

@} */
p3:=subs(p1,diff(U(c),c)=D(U)(c));
/* @O{
    @G19

@} */
p4:=subs(p3,c=C(t));
/* @O{
    @G20

@} */
fff:=x->x*x*x;
/* @O{
    @G21

@} */
fff5!)-;
/* @O{
fff5!)
     ^ Error: Unexpected ')' [line 1, col 6]

@} */
fff(10);
/* @O{
    @G22

@} */
P:=t->p4;
/* @O{
    @G23

@} */
ek;
/* @O{
    @G24

@} */
ek2:=simplify(ek/(U'(C(t))*exp(-T*t)));
/* @O{
    @G25

@} */
ek3:=normal(ek2);
/* @O{
    @G26

@} */
ek3:=expand(ek2);
/* @O{
    @G27

@} */
ek4:=simplify(ek3+T);
/* @O{
    @G28

@} */

