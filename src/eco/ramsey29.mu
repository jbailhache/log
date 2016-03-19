read("c:\\jacques\\eco\\ramsey26in.txt");
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

@} */
eacne;
/* @O{
    @G28

@} */
eqcne;
/* @O{
    @G29

@} */
eqk;
/* @O{
    @G30

@} */
s=solve({eqcne,eqk},{c,k});
/* @O{
    @G31

@} */
s[1];
/* @O{
    @G32

@} */
s[0];
/* @O{
    @G33

@} */
s;
/* @O{
    @G34

@} */
s:=solve({eqcne,eqk},{c,k})
;
/* @O{
    @G35

@} */
s[0];
/* @O{
Error: Index out of range [piecewise::expression]

@} */
s[1];
/* @O{
    @G36

@} */
s[1][1];
/* @O{
    @G37

@} */
s[1][2];
/* @O{
    @G38

@} */
s[1][2][1];
/* @O{
    @G39

@} */
s[1][2][1][2];
/* @O{
    @G40

@} */
s[1,2,1,2];
/* @O{
    @G41

@} */
s[1;2;1;2];
/* @O{
s[1;
   ^ Error: ']' expected [line 1, col 4]

@} */
read("c:\\jacques\\eco\\ramsey2*in.txt");
/* @O{
Error: Could not open "c:\jacques\eco\ramsey2*in.txt" [read]

@} */
read("c:\\jacques\\eco\\ramsey28in.txt");
/* @O{
    @G42
    @G43
    @G44
    @G45
    @G46
    @G47
    @G48
    @G49
    @G50
    @G51
    @G52
    @G53
    @G54
    @G55
    @G56
    @G57
    @G58
    @G59
    @G60
    @G61
    @G62
    @G63
    @G64
    @G65
    @G66
    @G67
    @G68
    @G69
    @G70
    @G71

@} */
cstar;
/* @O{
    @G72

@} */
kstar;
/* @O{
    @G73

@} */
cstar+0.0;
/* @O{
    @G74

@} */
eval{kstar);
/* @O{
eval{
    ^ Error: Unexpected '{' [line 1, col 5]

@} */
eval(kstar);
/* @O{
    @G75

@} */
value(kstar);
/* @O{
    @G76

@} */
kstar;
/* @O{
    @G77

@} */
val(kstar);
/* @O{
    @G78

@} */
float(kstar);
/* @O{
    @G79

@} */
read("c:\\jacques\\eco\\ramsey28in.txt");
/* @O{
    @G80
    @G81
    @G82
    @G83
    @G84
    @G85
    @G86
    @G87
    @G88
    @G89
    @G90
    @G91
    @G92
    @G93
    @G94
    @G95
    @G96
    @G97
    @G98
    @G99
    @G100
    @G101
    @G102
    @G103
    @G104
    @G105
    @G106
    @G107
    @G108
    @G109

@} */
de1:=ode(y'(x)=y(x),y(x));
/* @O{
    @G110

@} */
solve(de1);
/* @O{
    @G111

@} */
solve;
/* @O{
    @G112

@} */
zith5DEtools-;
/* @O{
zith5DEtools-
             ^ Error: 'expression' expected [line 1, col 14]

@} */
with(DEtools);
/* @O{
    @G113

@} */
DEplot({edk,edcne},{k(t),c(t)},0..40,[[0,20,12.2],[0,20,12],[0,20,8]],stepsize=0.1,scene=[t,k],linecolor=sin(t));
/* @O{
    @G114

@} */

edk;
/* @O{
    @G115

@} */
edc;
/* @O{
    @G116

@} */
edcne;
/* @O{
    @G117

@} */
de:=ode({edk,edc=0},{k(t),c(t)});
/* @O{
    @G118

@} */
sde:=solve(de);
/* @O{
    @G119

@} */
numeric::odesolve(;
